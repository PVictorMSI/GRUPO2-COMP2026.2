# Estratégia de testes

## Camadas de verificação

| Camada | Local | O que verifica |
| --- | --- | --- |
| Entrada e CLI | `Testes/unitarios/` | stdin, argumento de arquivo, códigos de saída e arquivo ausente |
| Símbolos | `test_symbols.cpp` e `teste_simbolos_*.lua` | Escopos, sombreamento, capturas e nomes Python |
| Casos válidos | `Testes/Testes certos/` | Tradução das construções aceitas |
| Casos inválidos | `Testes/Testes errados/` | Rejeição léxica, sintática e semântica com diagnóstico |
| Geração | `Testes/geracao/` | `py_compile` e comparação com arquivos `.esperado` |
| Diferencial | `fuzz_operadores.sh` | Comparação de expressões geradas com Lua 5.4 |
| Memória | Etapa Valgrind em `rodar_checks.sh` | Perdas definitivas/indiretas e acessos inválidos |

## Executar a suíte

```bash
make check
```

O alvo chama `Testes/unitarios/rodar_checks.sh`. As etapas de Python e Valgrind dependem dos executáveis instalados; a suíte pode pular uma etapa e ainda terminar com código zero. Sempre confira o log, não apenas a linha final.

## Resultado local registrado

Na preparação desta documentação, em 27/11/2026, o código `1c5e2e3` produziu:

```text
== Memória (Valgrind) ==
  (valgrind não instalado — pulando; instale com: sudo apt install valgrind)
Resultado: 144 passaram, 0 falharam.
```

O resultado conta verificações, não 144 arquivos ou 144 requisitos. Os arquivos `.esperado` existentes foram usados na comparação; Lua 5.4 não está instalado neste ambiente e suas saídas não foram regeneradas nesta execução. [Log completo](../fontes/verificacao-local.txt).

## Comparação diferencial

```bash
bash Testes/geracao/fuzz_operadores.sh 100
```

Esse comando requer Lua 5.4. Para adicionar um caso, crie um `.lua` representativo e obtenha a saída de referência com Lua. O script abaixo atualiza os `.esperado`; revise o diff antes de aceitar novas referências:

```bash
bash Testes/geracao/gerar_esperados.sh
```

## Indentação e sintaxe Python

Para cada entrada válida selecionada, a suíte chama `python3 -W error -m py_compile`. Blocos vazios, funções anônimas e estruturas aninhadas são exercitados por `indentacao_31.lua` e `indentacao_31b.lua`. A aprovação da sintaxe não prova equivalência de execução; os testes com `.esperado` complementam essa validação.

## Cobertura e limites

A [matriz de requisitos](../projeto/requisitos.md) liga funcionalidades aos arquivos disponíveis. Não há relatório quantitativo de cobertura de linhas/branches nas fontes consultadas, nem demonstração formal de cobertura completa RF01–RF21. Casos aprovados não eliminam as [limitações conhecidas](../projeto/limitacoes.md).

Issues: [#52](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/52), [#53](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/53), [#54](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/54), [#55](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/55). Fonte: [Testes/unitarios/rodar_checks.sh](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/Testes/unitarios/rodar_checks.sh).
