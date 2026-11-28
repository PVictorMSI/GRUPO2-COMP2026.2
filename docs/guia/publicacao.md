# MkDocs e publicação

O site é construído com **MkDocs 1.6.1** e **Material 9.6.19**, fixados em `requirements-docs.txt`. O conteúdo Markdown está em `docs/`; a navegação está em `mkdocs.yml`; o tema usa pesquisa em português, modo claro/escuro, navegação lateral e cópia de código.

## Executar localmente

```bash
python3 -m venv .venv-docs
source .venv-docs/bin/activate
python -m pip install -r requirements-docs.txt
python -m mkdocs serve
```

Abra `http://127.0.0.1:8000`. Para validar todos os arquivos:

```bash
python -m mkdocs build --strict
```

O resultado é gerado em `site/`, ignorado pelo Git. Corrija avisos antes da publicação. Fotos e a imagem do Valgrind são servidas de `docs/assets/`, sem depender de avatares externos.

## GitHub Pages

O arquivo `.github/workflows/docs.yml` constrói a documentação em pull requests e publica após alterações na `main` ou execução manual. O deploy usa o artefato estático do MkDocs, `pages: write`, `id-token: write` e o ambiente `github-pages`.

Configuração inicial, por alguém com permissão administrativa:

1. Abrir **Settings → Pages** no repositório.
2. Selecionar **GitHub Actions** como fonte de publicação.
3. Enviar as alterações de documentação à `main` ou executar **Actions → Documentação → Run workflow**.
4. Conferir a conclusão do job `deploy` e o endereço retornado pelo GitHub.

**Endereço esperado:** [pvictormsi.github.io/GRUPO2-COMP2026.2/](https://pvictormsi.github.io/GRUPO2-COMP2026.2/). Esse endereço configurado não é, sozinho, prova de publicação. Na consulta inicial, o repositório informava `has_pages: false`.

## Atualizar o conteúdo

- Adicione a página Markdown em uma seção de `docs/`.
- Inclua-a em `nav` no `mkdocs.yml`.
- Use links relativos para páginas e imagens internas.
- Atualize o registro de fontes, datas e commit quando mudar evidências.
- Rode o build estrito e revise as páginas afetadas.
- Publique pelo workflow após a integração das alterações.

## Falhas comuns

| Falha | Verificação |
| --- | --- |
| Pages não configurado | Selecionar GitHub Actions em Settings → Pages |
| Deploy sem permissão | Conferir permissões do job e políticas do repositório |
| Build estrito falha | Corrigir links, páginas na navegação ou configuração |
| Imagem ausente | Conferir nome exato e maiúsculas/minúsculas em `docs/assets/` |
| Site antigo | Conferir branch, commit e execução do workflow |

Fontes: [MkDocs](https://www.mkdocs.org/user-guide/deploying-your-docs/) e [GitHub Pages](https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages).
