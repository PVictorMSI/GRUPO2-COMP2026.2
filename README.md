# GRUPO2-COMP2026.2

# Compilador Lua para Python

**Disciplina:** Compiladores 1  
**Instituição:** FCTE  
**Semestre/Ano:** 2026.2  

## Sobre o Projeto

Este projeto consiste no desenvolvimento do *front-end* de um compilador (Análise Léxica e Sintática) que traduz código escrito na linguagem **Lua** para a linguagem **Python**. O objetivo principal é aplicar na prática os conceitos estudados na disciplina de Compiladores 1, utilizando ferramentas geradoras de analisadores.

O compilador lê um arquivo de origem `.lua`, analisa sua estrutura gramatical e gera um arquivo `.py` funcional e semanticamente equivalente (dentro do subconjunto da linguagem suportado).

## Tecnologias e Ferramentas Utilizadas

*   **FLEX (Fast Lexical Analyzer Generator):** Utilizado para a Análise Léxica. Responsável por ler o código fonte em Lua e convertê-lo em uma sequência de *tokens*.
*   **BISON (GNU Parser Generator):** Utilizado para a Análise Sintática. Trabalha em conjunto com o FLEX para validar a estrutura dos *tokens* com base na gramática da linguagem Lua e construir as regras de tradução para Python.
*   **Linguagem Base (C/C++):** Utilizada para a lógica interna de integração entre o FLEX, BISON e a geração de código.

## Funcionalidades Suportadas

*Este compilador traduz um subconjunto específico da linguagem Lua. Atualmente, ele prevê suportar:*

*   [ ] Declaração e atribuição de variáveis.
*   [ ] Operações aritméticas e lógicas básicas.
*   [X] Estruturas condicionais (`if`, `elseif`, `else`).
*   [X] Estruturas de repetição (`while`, `for`).
*   [X] Declaração e chamada de funções (`function nome(args) ... end` → `def nome(args):`).
*   [X] Tabelas: usadas como array viram `list`; usadas como mapa viram `dict`.
*   [X] Subconjunto da biblioteca padrão (`print`, `math`, `string`, `table`, `io.write`...); funções fora dele geram erro de "não suportado".
