# MkDocs e publicação no GitHub Pages

O MkDocs transforma os arquivos Markdown em um site HTML. O GitHub Actions executa esse build e o GitHub Pages hospeda o resultado. As versões utilizadas são **MkDocs 1.6.1** e **Material 9.6.19**, fixadas em `docs/requirements.txt`.

## Onde ficam os arquivos

| Caminho | Finalidade |
| --- | --- |
| `docs/conteudo/` | Páginas Markdown, fotos, fontes e evidências |
| `docs/mkdocs.yml` | Tema, navegação e caminhos de entrada/saída |
| `docs/requirements.txt` | Dependências Python da documentação |
| `docs/README.md` | Instruções de uso local |
| `docs/site/` | Site HTML gerado automaticamente; não é versionado |
| `docs/.venv/` | Ambiente Python local opcional; não é versionado |
| `.github/workflows/docs.yml` | Workflow de build e deploy, no diretório exigido pelo GitHub |

O README de apresentação permanece na raiz do repositório. Configuração, dependências, conteúdo e saída gerada do MkDocs estão concentrados em `docs/`. A saída `site/` fica separada de `conteudo/` para não ser incorporada novamente à entrada do build.

## Executar localmente

Execute na raiz do repositório:

```bash
python3 -m venv docs/.venv
source docs/.venv/bin/activate
python -m pip install -r docs/requirements.txt
python -m mkdocs serve -f docs/mkdocs.yml
```

Abra o endereço exibido pelo servidor. Para validar e gerar os arquivos HTML:

```bash
python -m mkdocs build --strict -f docs/mkdocs.yml
```

O resultado fica em `docs/site/`. Não edite esses arquivos manualmente: o próximo build os recria. Edite os Markdown de `docs/conteudo/`. O arquivo `docs/.gitignore` exclui o site gerado e o ambiente virtual do Git.

## Passo 1 — Ativar GitHub Pages

Uma pessoa com acesso de administrador ou mantenedor precisa configurar a origem de publicação:

1. Abra [Settings → Pages do repositório](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/settings/pages).
2. Em **Build and deployment**, localize **Source**.
3. Selecione **GitHub Actions**.
4. O workflow já foi preparado em `.github/workflows/docs.yml`; não é necessário criar outro a partir dos modelos sugeridos.

Este projeto usa um build MkDocs pelo Actions. A opção **Deploy from a branch → /docs** não executa esse build e não é o modo configurado aqui.

## Passo 2 — Enviar a branch de documentação

As alterações foram salvas na branch `docs/documentacao-mkdocs`. Em um terminal com autenticação GitHub configurada, execute na raiz:

```bash
git switch docs/documentacao-mkdocs
git push -u origin docs/documentacao-mkdocs
```

Se o Git pedir autenticação, conclua o login com a conta que tem acesso de escrita ao repositório. O deploy usa o token automático do GitHub Actions; não é necessário criar um secret personalizado para o workflow fornecido.

## Passo 3 — Integrar a documentação à main

1. No GitHub, abra a aba **Pull requests**.
2. Clique em **New pull request**.
3. Selecione **base: main** e **compare: docs/documentacao-mkdocs**.
4. Revise os arquivos e crie o pull request.
5. Aguarde a verificação **Documentação / build** terminar com sucesso.
6. Faça o merge na `main`, respeitando as regras do repositório.

O pull request valida o site. A publicação acontece quando a alteração chega à `main`.

## Passo 4 — Conferir o deploy

1. Abra [Actions](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/actions).
2. Selecione o workflow **Documentação**.
3. Abra a execução causada pelo merge na `main`.
4. Confirme que os jobs **build** e **deploy** ficaram verdes.
5. Abra o endereço apresentado no job de deploy ou em **Settings → Pages**.

Se precisar executar manualmente depois que o workflow estiver na `main`, use **Actions → Documentação → Run workflow**, selecione **main** e confirme a execução.

**Endereço esperado após o deploy:** [pvictormsi.github.io/GRUPO2-COMP2026.2/](https://pvictormsi.github.io/GRUPO2-COMP2026.2/).

Um endereço configurado não comprova publicação. Na preparação local, não foi possível enviar alterações porque não havia autenticação GitHub utilizável no ambiente. A ativação e o primeiro deploy devem ser confirmados pelos jobs do Actions.

## Atualizações seguintes

Edite as páginas em `docs/conteudo/`, ajuste a navegação em `docs/mkdocs.yml`, valide o build e integre as mudanças à `main`. O workflow publica automaticamente alterações em `docs/**` ou no próprio workflow. O diretório `docs/site/` não precisa ser enviado.

## Resolver problemas

| Sintoma | Como conferir |
| --- | --- |
| Sem acesso a Settings → Pages | Peça a um administrador ou mantenedor para selecionar GitHub Actions |
| Workflow não aparece | Confirme que `.github/workflows/docs.yml` chegou à `main` e que Actions está habilitado no repositório |
| Build vermelho | Abra o job `build`; corrija o aviso/erro apontado pelo MkDocs e envie a correção |
| Pages não encontrado ou erro em configure-pages | Selecione GitHub Actions em Settings → Pages e execute novamente na `main` |
| Permissão negada no deploy | Confira políticas do repositório e do ambiente `github-pages`; o YAML já declara `pages: write` e `id-token: write` |
| Aprovação de ambiente pendente | Um responsável definido nas regras do ambiente precisa aprovar o deploy |
| Site retorna 404 | Confirme se o job `deploy` terminou e use o endereço exibido pelo Pages |

Fontes: [configuração do MkDocs](https://www.mkdocs.org/user-guide/configuration/), [origem de publicação do Pages](https://docs.github.com/en/pages/getting-started-with-github-pages/configuring-a-publishing-source-for-your-github-pages-site) e [workflow personalizado](https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages).
