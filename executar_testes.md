# Executar e testar o compilador

O fluxo atual usa `build/lua2py`, gerado pelo Makefile:

```bash
make
./build/lua2py programa.lua -o programa.py
python3 programa.py
make check
```

Consulte os guias completos:

- [Instalação e CLI](docs/guia/instalacao.md).
- [Estratégia de testes e resultados](docs/qualidade/testes.md).
- [Valgrind e evidência de memória](docs/qualidade/valgrind.md).
- [Diagnósticos](docs/qualidade/erros.md).
- [Geração Python](docs/geracao_codigo.md).

O executável `teste` e `teste.y` na raiz pertencem ao protótipo léxico inicial. Para as verificações atuais, utilize `make check`. O rastreio de tokens é opcional (`make TRACE=1`) e requer recompilação após `make clean` ao mudar de modo.
