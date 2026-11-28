# Semana 13 — Consolidação para P2

**Período:** 02/11 a 08/11/2026. **Unidade no plano:** —.

**Situação documental:** reconstruída a partir do calendário e do histórico disponível.

## Agenda da disciplina

| Encontro | Atividade prevista |
| --- | --- |
| Segunda, 02/11 | Feriado de 2 de novembro |
| Quarta, 04/11 | Compiladores e LLM; formulário P2 até 23h59 |

## Foco e relação com o projeto

Consolidar semântica, código gerado e testes para P2, explicitando a arquitetura efetivamente entregue.

### Issues relacionadas ao tema

Os responsáveis e estados abaixo são da consulta em 27/11/2026; não representam uma distribuição histórica de sprint.

| Issue | Atividade | Responsáveis | Estado consultado |
| --- | --- | --- | --- |
| [#9](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/9) | Issue: Preparar e Ensaiar P2 | Não atribuído no GitHub | Fechada |
| [#26](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/26) | Implementar tabela de símbolos com escopos | [@PVictorMSI](https://github.com/PVictorMSI), [@ChickRicks](https://github.com/ChickRicks) | Fechada |
| [#27](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/27) | Tratamento de variáveis não declaradas | [@Camile0318](https://github.com/Camile0318), [@Joaolramos](https://github.com/Joaolramos) | Fechada |
| [#28](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/28) | Reportar erros semânticos | [@Camile0318](https://github.com/Camile0318), [@Giih-martins](https://github.com/Giih-martins) | Fechada |
| [#30](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/30) | Gerar código para laços de repetição | [@Camile0318](https://github.com/Camile0318), [@ChickRicks](https://github.com/ChickRicks) | Fechada |
| [#39](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/39) | Tratar retorno múltiplo de valores | [@Camile0318](https://github.com/Camile0318), [@PVictorMSI](https://github.com/PVictorMSI) | Fechada |
| [#41](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/41) | Reindexar tables (base 1 para base 0) | [@Camile0318](https://github.com/Camile0318), [@Joaolramos](https://github.com/Joaolramos) | Fechada |
| [#54](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/54) | Validar saída .py com py_compile | [@Camile0318](https://github.com/Camile0318), [@PVictorMSI](https://github.com/PVictorMSI) | Fechada |

## Evidências do período

| Data registrada no Git | Commit | Autor | Entrega registrada |
| --- | --- | --- | --- |
| 02/11/2026 | [4b9a822](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/4b9a822b434d03dfc0aee75a2608c81df4ba08a5) | Paulo Victor Mendes Silva | tabela de simbolos + melhorias em testes |
| 02/11/2026 | [39cfd37](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/39cfd37e5495b0972fc0b82e669087319520665c) | João Ramos | chore: adiciona pasta build ao gitignore |
| 02/11/2026 | [a1cb6e5](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/a1cb6e582091104c93a93b46d7dfb3e99bf02aca) | João Ramos | feat(semantica): tratamento de variaveis nao declaradas (issue #27) |
| 02/11/2026 | [45a0c49](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/45a0c49fda80428e9c0a0cc8b1e307f44718147d) | João Ramos | Merge branch 'main' into feat/construcao-ast-parser - resolve conflitos |
| 02/11/2026 | [8ee016c](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/8ee016cbabebc8745bda3139834fcdadbce78f8f) | João Ramos | Merge pull request #60 from PVictorMSI/feat/construcao-ast-parser |
| 03/11/2026 | [1626ced](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/1626ced5577e8f5ee3257e8704aa083aa8063846) | Giovana Martins de Brito | feat(semantica): implementa reporte de erros semanticos (#28) |
| 03/11/2026 | [d8491a5](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/d8491a5615fecf23ca0f63ab40f2047ce801a3b8) | Giovana Martins de Brito | test(semantica): alinha linhas esperadas nos testes semanticos |
| 03/11/2026 | [3ad440e](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/3ad440eebb08ae52eba4a665e135e5112b721b79) | Camile0318 | Merge feat/tratamento-variaveis (#27) |
| 03/11/2026 | [56febfb](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/56febfbabd274b52b919e035a57deb82044a1366) | Camile0318 | Merge feat/Issue#28-reportar-erros-semanticos (#28) |
| 03/11/2026 | [bc5242f](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/bc5242fccc76fd42035e2859948b38d3f63d626d) | Camile0318 | fix(semantica): integra #27 e #28 sem conflitos |
| 03/11/2026 | [9d98b06](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/9d98b06e568b5993f6ab17677fa9b359c890ac30) | Camile0318 | fix(semantica): variável não declarada só quando nunca recebe valor (#27) |
| 03/11/2026 | [44c0eca](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/44c0eca98041abd8b91278b42a3fdc6ecf1db906) | Camile | test: find_test procura 'TesteN(' para não confundir Teste1 com Teste10 em locale pt_BR |
| 03/11/2026 | [54a9120](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/54a912077ab7217cde4cafe250113f434a2dc8eb) | Camile0318 | feat(front-end): ajustes necessários para a geração de código |
| 03/11/2026 | [5030ff5](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/5030ff5267a79d1fee7a8467b30c9e8721d96713) | Camile0318 | feat(codegen): gerador de código Python a partir da AST |
| 03/11/2026 | [39dfa59](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/39dfa59101596607d70eeb4569deb0a21a569fd5) | Camile0318 | test(codegen): py_compile e comparação com o Lua real no make check (#54) |
| 03/11/2026 | [7f99735](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/7f9973575059e3cb19ff022105df826c7d243267) | Camile0318 | Merge pull request #61 from PVictorMSI/feat/gerador-python |

### Fechamentos de issues registrados

| Data (Bahia) | Issue | Título |
| --- | --- | --- |
| 03/11/2026 | [#25](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/25) | Construção da AST durante o parsing |
| 03/11/2026 | [#26](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/26) | Implementar tabela de símbolos com escopos |
| 03/11/2026 | [#27](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/27) | Tratamento de variáveis não declaradas |
| 03/11/2026 | [#54](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/54) | Validar saída .py com py_compile |

Fechamento é um evento administrativo; não prova a data da implementação ou a aprovação do professor.

## Resultado documental e continuidade

O histórico registra integração de AST, símbolos, semântica e geração no começo de novembro. O prazo do formulário P2 foi 04/11 às 23h59; seu envio não foi comprovado nas fontes. [Dossiê P2](../entregas/p2.md).

## Fontes

[Plano de ensino, seção 6](../fontes/plano-de-ensino.pdf) · [Snapshot e metodologia](../fontes/index.md) · [Cronograma completo](../gestao/cronograma.md).
