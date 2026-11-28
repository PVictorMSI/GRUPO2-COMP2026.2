# Planejamento e EAP

O [GitHub Projects](https://github.com/users/PVictorMSI/projects/1/views/6) e o [TSV exportado](../fontes/cronograma.tsv) contêm quatro épicos. A página pública consultada também informa quatro itens e não apresenta outra página de resultados nessa visualização. As tarefas detalhadas são recuperadas pelo [catálogo de issues](issues.md).

## Cronograma exportado

| Épico | Início | Meta | Responsável no épico | Progresso de subissues | Status |
| --- | --- | --- | --- | --- | --- |
| [Gerenciamento](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/2) | Aug 12, 2026 | Nov 30, 2026 | PVictorMSI | 5 / 6 (83%) | In progress |
| [Especificação](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/3) | Aug 19, 2026 | Sep 23, 2026 | Não atribuído | 4 / 4 (100%) | Done |
| [Implementação](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/4) | Não informado | Não informado | Não atribuído | 3 / 5 (60%) | In progress |
| [Testes e Documentação](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/5) | Não informado | Não informado | ChickRicks | 3 / 5 (60%) | In progress |

O progresso de subissues se refere aos filhos diretos de cada épico. Não deve ser somado para calcular a porcentagem de conclusão do compilador. Campos vazios, como Sprint e datas dos dois últimos épicos, foram preservados como ausentes.

## Estrutura analítica do projeto

```text
Transpilador
├── Gerenciamento
│   ├── Cronograma
│   ├── Papéis
│   ├── Ambiente
│   └── Ensaios
├── Especificação
│   ├── Requisitos funcionais e não funcionais
│   ├── Escopo
│   ├── Gramática suportada
│   └── Estrutura AST
├── Implementação
│   ├── Front-end
│   ├── Semântica
│   ├── Código intermediário (planejado)
│   ├── Geração final
│   └── Infraestrutura e memória
└── Testes e documentação
    ├── Suítes de teste
    ├── Validação
    ├── Correções
    └── Documentação
```

[EAP original em PDF](../EAP.pdf).

## Gestão por marcos

As milestones distinguem P1 (Documentação/Apresentação), P2 (Documentação/Apresentação) e T (Apresentação). Os prazos acadêmicos e horários vêm do plano de ensino; os campos de data do GitHub não substituem o limite de 23h59 dos formulários.

## Organização da evidência

O catálogo conserva responsáveis e critérios; as [semanas](cronograma.md) relacionam a agenda acadêmica aos commits e fechamentos efetivamente registrados. A associação temática de issues a uma semana é uma organização documental, não uma data de execução presumida.

## Manutenção do acompanhamento

Ao encerrar uma tarefa, vincule o PR/commit, o teste ou a evidência ao critério correspondente. Ao mudar o escopo, atualize gramática, manual e testes. A [lista de divergências](../projeto/limitacoes.md) reúne os pontos que exigem conciliação antes da entrega final.
