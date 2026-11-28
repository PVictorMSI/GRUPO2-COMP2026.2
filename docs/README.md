# Documentação do projeto

Todos os arquivos do MkDocs ficam neste diretório. O README principal permanece na raiz para a apresentação do repositório; o workflow fica em `.github/workflows/docs.yml`, localização exigida pelo GitHub Actions.

```text
docs/
├── mkdocs.yml          Configuração e navegação
├── requirements.txt    Dependências da documentação
├── README.md           Instruções locais
├── .gitignore          Exclusões da documentação
├── conteudo/           Markdown, fotos, fontes e evidências
├── site/               HTML gerado, ignorado pelo Git
└── .venv/              Ambiente Python opcional, ignorado pelo Git
```

## Rodar localmente

Na raiz do repositório:

```bash
python3 -m venv docs/.venv
source docs/.venv/bin/activate
python -m pip install -r docs/requirements.txt
python -m mkdocs serve -f docs/mkdocs.yml
```

Abra o endereço exibido no terminal. Para gerar e validar a documentação:

```bash
python -m mkdocs build --strict -f docs/mkdocs.yml
```

A pasta `docs/site/` é criada automaticamente. Edite o conteúdo em `docs/conteudo/`, não os arquivos HTML gerados.

## Publicar

Siga o [passo a passo de GitHub Pages](conteudo/guia/publicacao.md). O workflow instala as dependências, gera o site e publica o resultado quando as alterações chegam à `main`. Não é necessário enviar `docs/site/` ao GitHub.

## Executar os testes do compilador

Estes comandos apenas descrevem o fluxo já existente no projeto:

```bash
make
./build/lua2py programa.lua -o programa.py
python3 programa.py
make check
```

- [Instalação e CLI](conteudo/guia/instalacao.md).
- [Estratégia de testes e resultados](conteudo/qualidade/testes.md).
- [Valgrind e evidência de memória](conteudo/qualidade/valgrind.md).
- [Diagnósticos](conteudo/qualidade/erros.md).
- [Geração Python](conteudo/geracao_codigo.md).

O executável `teste` e `teste.y` na raiz pertencem ao protótipo léxico inicial. Para as verificações atuais, utilize `make check`. O rastreio de tokens é opcional (`make TRACE=1`) e requer recompilação após `make clean` ao mudar de modo.
