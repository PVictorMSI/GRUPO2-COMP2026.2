# Análise sintática

O Bison gera `build/parser.tab.c` e o cabeçalho de tokens a partir de `parser/parser.y`. O símbolo inicial `chunk` recebe um `block` e transfere a raiz para `root`.

## Construção durante as reduções

Cada produção cria um nó da AST ou agrega elementos a uma lista. Por exemplo, uma produção `IF exp THEN block ... END` cria `If`, adiciona `IfClause` e preserva os blocos de cada ramo. Expressões binárias usam `bin`, unárias usam `un`, e `at` registra a linha aproximada.

## Precedência

A gramática declara `or`, `and`, comparações, concatenação, soma/subtração, multiplicação/divisão, unários e potência, nessa ordem da menor para a maior precedência. Concatenação e potência associam à direita. Consulte a [BNF](../BNF.md).

## Conflitos conhecidos

O arquivo declara `%expect 2`. O comentário da gramática registra a ambiguidade entre chamada e novo comando quando aparece `f (x)` após uma expressão. A resolução adotada é shift. O número esperado de conflitos não deve ser apresentado como ausência total de ambiguidades.

## Erros e memória

`%define parse.error verbose` habilita mensagens detalhadas. `yyerror` inclui a linha aproximada. `%destructor` libera valores na pilha descartada durante falhas. Alvos de atribuição são limitados a identificador ou indexação; expressões usadas como comando precisam ser chamadas de função.

Quando `chunk` é aceito, a posse da árvore passa para `root`; o valor da produção é anulado para evitar liberação duplicada. O `main` libera a raiz depois da validação ou geração.

Rastreabilidade: [#23](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/23), [#24](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/24), [#25](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/25), [#45](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/45). Fonte: [parser/parser.y](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/parser/parser.y). Testes: `Teste6` a `Teste9` e `Testes/unitarios/erro_sintatico.lua`.
