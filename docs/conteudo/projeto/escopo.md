# Escopo da linguagem

O `lua2py` traduz um subconjunto de Lua para Python 3.8+. A referência operacional é o parser e o runtime do commit `1c5e2e3`. O escopo original da issue [#13](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/13) excluía varargs; a implementação evoluiu e passou a aceitar `...` em funções variádicas.

## Construções implementadas

| Grupo | Construções | Evidência |
| --- | --- | --- |
| Valores | nil, booleanos, inteiros, floats, strings curtas e [[longas]] | [lexer/lexer.l](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/lexer/lexer.l) |
| Variáveis | local, globais, atribuição múltipla, escopos e sombreamento | [src/resolver.cpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/resolver.cpp) |
| Operadores | + - * / // % ^ .. #, comparações, and/or/not | [src/codegen.cpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/codegen.cpp) |
| Controle | if/elseif/else, while, repeat/until, for numérico e genérico, break | [parser/parser.y](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/parser/parser.y) |
| Funções | globais e locais, anônimas, métodos, recursão, closures, varargs, múltiplos retornos | [src/ast.hpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/ast.hpp) |
| Tabelas | construtores, acesso por campo/chave, arrays e mapas | [src/runtime_py.cpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/runtime_py.cpp) |
| Bibliotecas | funções globais e subconjunto de math/string/table/io | [Lista detalhada](../geracao_codigo.md#42-subconjunto-da-biblioteca-padrao) |


## Exclusões

- `goto` e rótulos.
- Operadores bit a bit (`&`, `|`, `~`, `<<`, `>>`).
- Atributos de variáveis `<const>` e `<close>`.
- Metatabelas, corrotinas, carregamento de módulos e `require`.
- Bibliotecas `os`, `package`, `debug`, `utf8` e funções não enumeradas em `stdlib_subset.cpp`.
- Padrões Lua em `string.find`, `gsub` e `gmatch`.
- Compatibilidade integral com overflow, limites numéricos e todos os detalhes de Lua 5.4.

O lexer implementa strings e comentários longos com `[[...]]`; não se deve presumir suporte a todos os níveis de delimitador `[=[...]=]` do Lua completo.

## Diferenças semânticas tratadas

Em Lua, apenas `nil` e `false` são falsos. Por isso `0` e `""` exigem tratamento próprio nas condições. Tabelas Lua começam em 1 quando usadas como sequência; listas Python começam em 0. O runtime adapta acessos e devolve `None` quando o acesso Lua resulta em `nil`. Retornos múltiplos usam tuplas e funções de ajuste para os contextos de expansão.

## Contrato de execução

O compilador usa saída padrão exclusivamente para Python gerado e saída de erro para diagnósticos. O retorno 0 indica compilação concluída; 1 indica erro de análise ou arquivo. A geração ocorre somente após a validação do front-end. Uma compilação bem-sucedida não elimina erros de execução presentes no programa de entrada.

Consulte [limitações](limitacoes.md), [gramática atualizada](../BNF.md) e [exemplos](../guia/exemplos.md).
