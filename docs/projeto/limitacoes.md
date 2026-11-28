# Limitações e pendências

Esta página descreve limitações do commit `1c5e2e3` e inconsistências entre o planejamento e a implementação. Ela permite avaliar o produto sem interpretar o encerramento de issues como prova suficiente de atendimento.

## Representação intermediária

As issues [#33](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/33), [#34](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/34) e [#35](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/35) estão fechadas, porém o gerador recebe `Block` e `SemanticInfo`, consulta os nós da AST e gera Python diretamente. Não foi encontrado um módulo separado de código de três endereços. O critério de #35, que exige uma geração sem acesso direto à AST, não está demonstrado. A mesma ressalva vale para memória de uma RI autônoma em [#47](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/47).

## Valores e semântica

- O PR #65, integrado à `main` em 27/11, passou a armazenar literais inteiros em `long long` e emitir `intVal`, preservando os casos grandes cobertos por `literais_37.lua`. Isso corrige o truncamento anterior em 32 bits; não demonstra compatibilidade completa com literais fora da faixa representável. A issue [#37](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/37) ainda aparece aberta na consulta.
- A aritmética gerada não implementa o overflow de inteiros de 64 bits de Lua.
- Comparar uma variável booleana com um literal numérico pode herdar a igualdade de Python (`True == 1`).
- Há diferença no sinal de zero para alguns módulos com operandos float.
- Uma função sem `return`, invocada por um callback desconhecido e expandida, pode produzir `nil` em vez de nenhum valor.
- `pcall`/`error` não acrescentam a localização original `arquivo:linha:` à mensagem.
- A fidelidade de todas as funções e casos de fronteira da biblioteca padrão não é demonstrada apenas pela suíte atual.

## Gramática e escopo histórico

A BNF original excluía varargs e descrevia comparações como não associativas; o parser atual possui `VARARG` e `%left` para comparações. A [BNF foi alinhada](../BNF.md) à implementação. `...` fora de função variádica é rejeitado pela análise semântica.

## Evidências e gestão

- Não foram localizados comprovantes de envio dos formulários P1/P2, notas ou atas das entrevistas.
- A periodicidade de rotação de papéis não está registrada nas fontes disponíveis.
- O Projects tem datas preenchidas apenas para gerenciamento e especificação; não há base para inventar início/fim das demais atividades.
- A imagem do Valgrind não informa commit ou data da execução e não mostra o relatório bruto completo.
- A verificação local desta documentação pulou Valgrind por ausência do executável. O resultado de 144 verificações não é um novo laudo de memória.

## Issues abertas na consulta

| Issue | Título | Responsáveis |
| --- | --- | --- |
| [#2](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/2) | Gerenciamento | [@PVictorMSI](https://github.com/PVictorMSI) |
| [#4](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/4) | Implementação | Não atribuído no GitHub |
| [#5](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/5) | Testes e Documentação | [@ChickRicks](https://github.com/ChickRicks) |
| [#11](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/11) | Issue: Preparar e Ensaiar T | Não atribuído no GitHub |
| [#19](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/19) | Frente E — Infraestrutura e Memória | Não atribuído no GitHub |
| [#20](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/20) | Issue: Escrever relatório técnico / manual do compilador | [@PVictorMSI](https://github.com/PVictorMSI) |
| [#29](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/29) | Gerar código para estruturas condicionais | [@ChickRicks](https://github.com/ChickRicks) |
| [#32](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/32) | Frente D — Geração Final | Não atribuído no GitHub |
| [#37](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/37) | Traduzir literais | [@ChickRicks](https://github.com/ChickRicks), [@Joaolramos](https://github.com/Joaolramos) |
| [#48](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/48) | Organizar código em módulos desacoplados | Não atribuído no GitHub |
| [#49](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/49) | Garantir robustez a entradas malformadas | Não atribuído no GitHub |
| [#50](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/50) | Padronizar mensagens de erro | [@ChickRicks](https://github.com/ChickRicks) |
| [#53](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/53) | Testes de robustez com entradas malformadas | [@PVictorMSI](https://github.com/PVictorMSI), [@ChickRicks](https://github.com/ChickRicks) |

## Encaminhamentos para a entrega

1. Conciliar os critérios de RI com a arquitetura entregue.
2. Ampliar a revisão dos limites numéricos além dos literais já corrigidos pelo PR #65.
3. Reexecutar a suíte com Lua 5.4 e Valgrind instalados, preservando logs e commit.
4. Atualizar os estados das issues após a revisão dos critérios.
5. Registrar comprovantes acadêmicos, apresentação e periodicidade da rotação quando disponíveis.

Fonte técnica complementar: [limitações do gerador](../geracao_codigo.md#limitacoes-conhecidas).
