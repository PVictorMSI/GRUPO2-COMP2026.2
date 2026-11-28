# Arquitetura

## Pipeline implementado

```text
arquivo.lua / stdin
        │
        ▼
lexer/lexer.l ────────────── tokens e valores
        │
        ▼
parser/parser.y ──────────── AST (src/ast.hpp)
        │
        ▼
analyze ──────────────────── SymbolTable + SemanticInfo
        │
        ▼
checkSemantics + checkStdlibSubset
        │
        ▼
generatePython ──────────── programa Python + helpers usados
        │
        ▼
stdout / arquivo.py
```

`src/main.cpp` coordena as etapas. Um erro léxico, sintático ou semântico impede a emissão do programa. A geração é feita em memória com `std::ostringstream` e depois enviada ao destino.

## Módulos e contratos

| Módulo | Responsabilidade | Entrada → saída |
| --- | --- | --- |
| lexer/lexer.l | Reconhecimento léxico, strings, contagem de linha e diagnósticos | caracteres → tokens/yylval |
| parser/parser.y | Gramática, precedência e construção de nós | tokens → Block raiz |
| src/ast.hpp | Tipos de nós e propriedade das subárvores | nós Expr/Stmt com unique_ptr |
| src/symbol_table.* | Escopos, símbolos e nomes Python únicos | declarações/usos → Symbol |
| src/resolver.* | Vincular identificadores aos símbolos | AST → SemanticInfo |
| src/semantic_checker.* | Validar break, varargs e parâmetros | AST → diagnósticos |
| src/stdlib_subset.* | Validar o subconjunto da biblioteca | AST + símbolos → diagnósticos |
| src/codegen.* | Percorrer a AST e gerar Python | Block + SemanticInfo → ostream |
| src/runtime_py.* | Fornecer compatibilidade semântica | helpers selecionados → código Python |
| src/main.cpp | CLI e coordenação | arquivo/stdin → arquivo/stdout |


## Decisões de projeto

### AST com propriedade exclusiva

`std::unique_ptr` modela a posse das subárvores. O parser usa ponteiros nos valores semânticos do Bison e transfere a posse aos nós. Destrutores Bison liberam valores descartados quando a análise aborta. Isso reduz o risco de vazamento, mas a correção precisa ser verificada em caminhos de erro, conforme o [registro de memória](../qualidade/valgrind.md).

### Informação semântica em tabelas laterais

`SemanticInfo` mantém os mapas `uses`, `declared` e `funcName`. Assim a resolução associa nós aos símbolos sem exigir que cada nó armazene todo o estado da análise. Os símbolos guardam escopo, função, capturas, leituras e escritas.

### Nomes únicos na saída

`assignCodegenNames()` evita colisões entre variáveis de mesmo nome em escopos distintos e palavras reservadas do Python. A resolução distingue globais, locais e parâmetros, permitindo decidir `global` e `nonlocal` durante a geração.

### Runtime incorporado

Python direto é usado onde a equivalência é conhecida. Helpers tratam verdade Lua, indexação, múltiplos retornos, coerções e biblioteca padrão. O programa gerado incorpora as funções necessárias, sem depender de um pacote Python do projeto.

### Geração direta da AST

A implementação não tem uma RI de três endereços separada. Essa é uma descrição do código observado, e não uma justificativa histórica atribuída à equipe. A motivação original para a mudança não foi localizada. Veja [RI e diferenças do planejamento](representacao-intermediaria.md).

Fontes: [src/main.cpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/main.cpp), [src/resolver.hpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/resolver.hpp), [src/codegen.hpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/codegen.hpp).
