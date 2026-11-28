# C — Avaliação do compilador

O componente C representa **40% da nota final**. O plano considera correção, funcionalidades entregues, organização do código e documentação. Não há uma rubrica numérica detalhada ou uma data independente de C no material fornecido.

## Evidências por dimensão

| Dimensão | Material | Limite da evidência |
| --- | --- | --- |
| Correção | [Testes](../qualidade/testes.md), comparação de `.esperado` e `py_compile` | 144 verificações locais; não é prova de equivalência para qualquer programa |
| Funcionalidades | [Escopo](../projeto/escopo.md) e [requisitos](../projeto/requisitos.md) | Subconjunto Lua, com limitações explícitas |
| Organização | [Arquitetura](../tecnico/arquitetura.md) e módulos do código | RI separada planejada não evidenciada |
| Memória | [Valgrind](../qualidade/valgrind.md) | Captura histórica sem commit/data; ferramenta ausente no teste local |
| Documentação | Manual, semanas, checkpoints, issues e fontes | Comprovantes acadêmicos e rotação ainda não documentados nas fontes |

## Reprodutibilidade da entrega

Registre o hash do commit, versões das ferramentas, comando de build, log de teste e versão publicada da documentação. Execute exemplos em ambiente preparado e preserve as saídas. Não altere arquivos `.esperado` apenas para fazer uma falha desaparecer; compare o comportamento desejado com Lua 5.4.

## Situação acadêmica

Esta documentação não atribui nota nem presume aprovação. A avaliação pertence à disciplina. O produto e suas limitações estão organizados para permitir a análise dos critérios definidos no [plano de ensino](../fontes/plano-de-ensino.pdf).
