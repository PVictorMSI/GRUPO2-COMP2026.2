# Instalação e linha de comando

## Dependências

| Finalidade | Ferramentas |
| --- | --- |
| Compilar o transpilador | Linux, GNU Make, g++ com C++17, Flex, Bison |
| Executar a saída | Python 3.8+ |
| Regerar referências e testes diferenciais | Lua 5.4 |
| Analisar memória | Valgrind |
| Construir a documentação | Python 3.9+ e `requirements-docs.txt` |

```bash
sudo apt update
sudo apt install build-essential flex bison python3 python3-venv lua5.4 valgrind
git clone https://github.com/PVictorMSI/GRUPO2-COMP2026.2.git
cd GRUPO2-COMP2026.2
make
```

Execute os comandos na raiz do repositório. O Makefile gera `build/parser.tab.c`, `build/parser.tab.h` e `build/lex.yy.c`, compila tudo como C++ e produz `build/lua2py`.

## Compilar um programa

```bash
./build/lua2py entrada.lua -o saida.py
python3 saida.py
```

Sem `-o`, a saída é enviada a `stdout`. Sem arquivo de entrada, o compilador lê `stdin`:

```bash
./build/lua2py entrada.lua > saida.py
./build/lua2py < entrada.lua > saida.py
```

Não use o mesmo caminho para a entrada Lua e a saída Python. O programa não oferece um modo de execução automática: a tradução e a execução são etapas distintas.

## Códigos de saída

| Código | Significado |
| --- | --- |
| 0 | Compilação concluída |
| 1 | Erro léxico, sintático, semântico ou de acesso a arquivo |

O código não implementa uma opção `--help`; a forma de uso suportada é `lua2py [entrada.lua] [-o saida.py]`.

## Alvos do Makefile

| Comando | Efeito |
| --- | --- |
| `make` | Compila o transpilador |
| `make check` | Compila os binários e executa a suíte |
| `make test-simbolos` | Exibe símbolos dos três programas de demonstração |
| `make clean` | Remove artefatos de compilação em `build/` |
| `make TRACE=1` | Compila com rastreio de tokens em `stderr` |

Para mudar o modo de rastreio, use `make clean` antes de recompilar. O alvo de limpeza atua apenas nos artefatos previstos pelo Makefile.

## Primeiro exemplo verificável

```bash
./build/lua2py "Testes/Testes certos/Teste1(HelloWorld).lua" -o /tmp/hello.py
python3 -W error -m py_compile /tmp/hello.py
python3 /tmp/hello.py
make check
```

Os arquivos antigos `teste`, `teste.y` e `src/main.c` são remanescentes da etapa inicial. O fluxo atual usa `Makefile`, `parser/parser.y` e `src/main.cpp`.

Fontes: [Makefile](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/Makefile) e [src/main.cpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/main.cpp).
