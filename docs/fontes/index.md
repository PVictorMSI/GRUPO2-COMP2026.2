# Fontes e método de registro

## Referência da documentação

- Data de referência: **27/11/2026**, fuso **America/Bahia (UTC−03)**.
- Código analisado: [`1c5e2e3`](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/commit/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43), alcançável pela `main` local.
- Issues e comentários: consulta à API pública do GitHub, incluindo issues abertas e fechadas.
- Projects: visualização pública Cronograma e exportação TSV fornecida pela equipe.

## Arquivos preservados

| Fonte | Conteúdo |
| --- | --- |
| [Plano de ensino](plano-de-ensino.pdf) | Contexto da disciplina, avaliação e calendário integral |
| [Cronograma TSV](cronograma.tsv) | Quatro épicos com status, responsáveis e datas disponíveis |
| [Snapshot GitHub](github-snapshot.json) | 55 issues, critérios, estados, responsáveis, comentários e histórico Git |
| [Log da verificação local](verificacao-local.txt) | Resultado de `make check` e etapas puladas |
| [EAP](../EAP.pdf) | Decomposição do trabalho já existente no repositório |
| [Imagem Valgrind](../assets/valgrind.jpeg) | Captura histórica fornecida pela equipe |

## Critérios de leitura

1. **Agenda prevista:** vem do plano de ensino; não comprova realização pela equipe.
2. **Responsabilidade:** vem dos assignees; campos vazios não são preenchidos por inferência.
3. **Implementação:** verificada no código e nos testes existentes.
4. **Histórico:** usa commits alcançáveis pelo hash de referência; branches paralelas e stash não são tratados como entrega da `main`.
5. **Estado de issue:** reproduz a consulta atual; fechamento não comprova critério ou data de implementação.
6. **Resultados:** diferenciam execução local, comentários históricos e imagem fornecida.

As semanas usam datas de autoria registradas no Git. Fechamentos de issues são convertidos de UTC para America/Bahia; por exemplo, um fechamento UTC de 28/11 à 01h corresponde a 27/11 às 22h neste fuso. As datas sem horário do Projects são tratadas como datas de planejamento, sem conversão que altere seu dia.

Não foram encontradas atas semanais, comprovantes de formulário, notas ou periodicidade documentada de rotação. A ausência desses registros é declarada nas páginas pertinentes, sem criar narrativas retroativas.

## Referências técnicas

- [Manual Lua 5.4](https://www.lua.org/manual/5.4/manual.html).
- [Manual GNU Bison](https://www.gnu.org/software/bison/manual/).
- [Manual Flex](https://westes.github.io/flex/manual/).
- [Documentação Python: py_compile](https://docs.python.org/3/library/py_compile.html).
- [Publicação com MkDocs](https://www.mkdocs.org/user-guide/deploying-your-docs/).
- [Workflows de GitHub Pages](https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages).

## Atualizações futuras

Ao atualizar os registros, identifique nova data e commit; confira as issues na origem; mantenha datas de evidência separadas das datas planejadas; execute `mkdocs build --strict`. Um resultado posterior deve complementar o histórico, sem alterar retroativamente o que foi comprovado em P1 ou P2.
