# Visão geral

O Grupo 2 desenvolve um compilador fonte a fonte de Lua para Python. A linguagem de implementação é C++17; Flex reconhece os tokens e Bison organiza esses tokens em uma árvore de sintaxe abstrata. O produto permite estudar todas as etapas do processamento de uma linguagem até uma saída executável.

## Contexto acadêmico

| Campo | Informação |
| --- | --- |
| Disciplina | FGA0003 — Compiladores 1 |
| Instituição | Universidade de Brasília, FCTE |
| Turma e semestre | T01, 2026/2 |
| Carga horária | 60 horas |
| Professor | Dr. Sergio Antônio Andrade de Freitas |
| Encontros | Segundas e quartas, 14h às 15h50, salas S2/I9 |
| Método | Aulas teóricas e Problem Based Learning (PBL) |

O plano de ensino propõe aplicar, analisar e criar soluções de compilação. A teoria aborda autômatos, expressões regulares, gramáticas, estruturas internas, semântica e geração/otimização de código. As práticas desenvolvem o compilador incrementalmente.

## Objetivo do produto

Receber programas do subconjunto Lua documentado, diagnosticar entradas inválidas e produzir Python que preserve o comportamento nos casos suportados. A equivalência é avaliada por programas de teste e saídas de referência, sem prometer compatibilidade integral com Lua.

## Entregáveis

1. Especificação do subconjunto e gramática.
2. Front-end Flex/Bison e AST.
3. Resolução de nomes e verificações semânticas.
4. Geração Python e runtime de compatibilidade.
5. Suítes de testes e evidências de memória.
6. Documentação técnica, acompanhamento semanal e materiais para P1, P2 e T.

## Organização do trabalho

A [EAP](../EAP.pdf) divide o projeto em gerenciamento, especificação, implementação e testes/documentação. A implementação é organizada em cinco frentes: A (léxico e sintático), B (semântica), C (representação intermediária), D (geração final) e E (infraestrutura e memória). A frente C é mantida como parte do planejamento, com a [diferença em relação ao código atual](../tecnico/representacao-intermediaria.md) documentada.

Fontes: [plano de ensino](../fontes/plano-de-ensino.pdf), [Projects](https://github.com/users/PVictorMSI/projects/1/views/6), [issues](../gestao/issues.md).
