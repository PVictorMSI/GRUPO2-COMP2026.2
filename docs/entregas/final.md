# T — Entrevista de entrega final

**Datas do calendário:** 30/11 e 02/12/2026. **Peso:** 30% da nota final. Todos os membros devem estar presentes. O horário específico do Grupo 2 não foi localizado nas fontes.

Em 27/11/2026, a entrevista ainda é futura. A issue [#11](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/11) está aberta e sem assignee. Esta página é um roteiro de preparação e um inventário da entrega disponível, não um relato de apresentação realizada.

## Produto a demonstrar

O executável traduz Lua para Python após análise léxica, sintática e semântica. Demonstre funções, laços, tabelas, erros e os helpers necessários para preservar a semântica Lua. Apresente também as [limitações](../projeto/limitacoes.md).

## Sequência de demonstração

```bash
make
./build/lua2py Testes/geracao/algoritmos.lua -o /tmp/algoritmos.py
python3 -W error -m py_compile /tmp/algoritmos.py
python3 /tmp/algoritmos.py > /tmp/algoritmos-saida.txt
diff -u Testes/geracao/algoritmos.esperado /tmp/algoritmos-saida.txt
make test-simbolos
make check
```

Em seguida, compile um caso de erro semântico e explique o diagnóstico. Para demonstrar memória, use o [procedimento Valgrind](../qualidade/valgrind.md) em ambiente com a ferramenta instalada.

## Tópicos para a entrevista

| Tema | Evidência para consulta |
| --- | --- |
| Organização, papéis e evolução | [Equipe](../projeto/equipe.md), [semanas](../gestao/cronograma.md) |
| Tokens e gramática | [Léxico](../tecnico/lexico.md), [BNF](../BNF.md) |
| AST e memória | [AST](../ast_design.md), [Valgrind](../qualidade/valgrind.md) |
| Escopos e erros | [Semântica](../tecnico/semantica.md) |
| Decisões Lua/Python | [Geração](../geracao_codigo.md) |
| RI planejada versus implementação | [Análise da RI](../tecnico/representacao-intermediaria.md) |
| Evidências de correção | [Testes](../qualidade/testes.md), [requisitos](../projeto/requisitos.md) |

## Checklist de preparação

- [ ] Confirmar horário da equipe e presença de todos.
- [ ] Identificar o commit final e ensaiar a instalação em ambiente adequado.
- [ ] Executar a suíte com Python, Lua 5.4 e Valgrind disponíveis.
- [ ] Salvar logs, saída dos exemplos e evidências de memória.
- [ ] Conciliar issues abertas e critérios da RI com o produto entregue.
- [ ] Verificar que a documentação publicada corresponde ao commit apresentado.
- [ ] Registrar perguntas, feedback e encaminhamentos após a entrevista.

## Inventário documental disponível

Há manual de instalação, gramática, arquitetura, AST, semântica, geração, testes, imagem de memória, catálogo de issues, responsáveis, cronograma e dossiês dos pontos de controle. O envio dos formulários e os resultados acadêmicos dependem dos registros da equipe.

Fonte: [plano de ensino](../fontes/plano-de-ensino.pdf). Continuidade: [semana 17](../semanas/semana-17.md) e [avaliação C](compilador.md).
