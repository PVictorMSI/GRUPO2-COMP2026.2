# Símbolos, escopos e análise semântica

## Resolução de nomes

`analyze(root, info)` percorre a AST e associa cada identificador ao símbolo correto. `SymbolTable` mantém uma pilha de escopos e uma coleção proprietária de símbolos que continua viva após a saída de um bloco.

| Campo | Uso |
| --- | --- |
| `id` / `name` | Identidade única e nome em Lua |
| `kind` | Global, Local ou Param |
| `scopeDepth` / `functionId` | Escopo e função de declaração |
| `declLine` | Linha aproximada da declaração |
| `captured` | Uso por outra função, necessário para closures |
| `reads` / `writes` | Leituras e atribuições |
| `codegenName` | Nome Python sem colisões |

`SemanticInfo.uses` associa `Identifier` a `Symbol`; `declared` relaciona nós às declarações; `funcName` identifica os nomes declarados por funções.

## Variáveis não declaradas

O resolvedor considera as atribuições globais e funções antes de rejeitar usos sem declaração. Isso permite chamadas de funções definidas posteriormente e recursão mútua. Uma global nunca atribuída e que não é reconhecida como biblioteca/função permitida gera diagnóstico. A validação não é uma prova completa de definição antes do uso em todo caminho de execução.

## Verificações estruturais

- `break` precisa estar dentro de um laço da função corrente.
- `...` precisa ocorrer em função variádica.
- Parâmetros duplicados são rejeitados.
- Entrar em outra função reinicia o contexto de laços; um `break` dentro de função aninhada não pode escapar do laço externo.

`reportSemanticError` centraliza mensagens e incrementa `sem_errors`. A checagem estrutural roda mesmo quando o resolvedor já encontrou problemas, para reunir diagnósticos antes de interromper a geração.

## Biblioteca padrão

`checkStdlibSubset` consulta a lista explícita em `stdlib_subset.cpp`. Usos conhecidos de funções não suportadas são rejeitados na compilação. Acessos só conhecidos em execução podem ser rejeitados pelo runtime. Sombreamentos e campos definidos pelo próprio programa são tratados separadamente das bibliotecas originais.

## Verificação

```bash
make test-simbolos
make check
```

Os testes verificam globais/locais homônimas, sombreamento, capturas, recursão, métodos com `self` e colisões com palavras reservadas Python. Testes negativos cobrem `break`, varargs, parâmetros duplicados e variáveis não declaradas.

Issues: [#26](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/26), [#27](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/27), [#28](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/28), [#42](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/42). Fontes: [src/resolver.cpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/resolver.cpp), [src/symbol_table.cpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/symbol_table.cpp), [src/semantic_checker.cpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/semantic_checker.cpp).
