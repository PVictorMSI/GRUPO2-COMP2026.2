# Lua → Python · Grupo 2

Transpilador de um subconjunto de **Lua para Python**, desenvolvido em **C++17, Flex e Bison** na disciplina **Compiladores 1 (FGA0003), UnB/FCTE, 2026.2**. Professor: Dr. Sergio Antônio Andrade de Freitas.

O `lua2py` faz análise léxica, parsing, construção da AST, resolução de escopos e validação semântica antes de gerar Python com funções auxiliares de runtime. O código atual gera a saída diretamente da AST resolvida. As diferenças em relação à RI originalmente planejada estão [documentadas](docs/tecnico/representacao-intermediaria.md).

**[Documentação](https://pvictormsi.github.io/GRUPO2-COMP2026.2/)** · **[Cronograma](docs/gestao/cronograma.md)** · **[Pontos de controle](docs/entregas/index.md)** · **[Issues](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues)** · **[GitHub Projects](https://github.com/users/PVictorMSI/projects/1/views/6)**

O endereço da documentação será servido pelo GitHub Pages após a ativação e a primeira execução bem-sucedida do [workflow](.github/workflows/docs.yml).

## Equipe

<table>
<tr><td align="center"><a href="https://github.com/Camile0318"><img src="docs/assets/Camille.jpg" alt="Foto de Camile" width="120"><br><strong>Camile</strong><br>@Camile0318</a></td><td align="center"><a href="https://github.com/Giih-martins"><img src="docs/assets/Giovana.jpeg" alt="Foto de Giovana Martins de Brito" width="120"><br><strong>Giovana Martins de Brito</strong><br>@Giih-martins</a></td><td align="center"><a href="https://github.com/Joaolramos"><img src="docs/assets/JoaoR.jpg" alt="Foto de João Ramos" width="120"><br><strong>João Ramos</strong><br>@Joaolramos</a></td><td align="center"><a href="https://github.com/PVictorMSI"><img src="docs/assets/Paulo.jpeg" alt="Foto de Paulo Victor Mendes Silva" width="120"><br><strong>Paulo Victor Mendes Silva</strong><br>@PVictorMSI</a></td><td align="center"><a href="https://github.com/ChickRicks"><img src="docs/assets/Ricardo.jpeg" alt="Foto de Ricardo" width="120"><br><strong>Ricardo</strong><br>@ChickRicks</a></td></tr>
</table>

| Integrante | Responsabilidades registradas |
| --- | --- |
| Camile | AST, análise semântica, operadores e geração de código |
| Giovana Martins de Brito | Léxico, AST, erros semânticos, indentação e memória |
| João Ramos | Erros léxicos, AST, semântica, geração e memória |
| Paulo Victor Mendes Silva | Gerenciamento, especificação, parser, símbolos e biblioteca padrão |
| Ricardo | Testes, gramática, símbolos, funções, tabelas e robustez |

As frentes acima resumem os responsáveis das issues em 27/11/2026. Consulte a [distribuição completa](docs/projeto/equipe.md). A periodicidade da rotação solicitada na issue #7 não está registrada nas fontes consultadas.

## Instalação e primeiro uso

Em Ubuntu/Debian:

```bash
sudo apt update
sudo apt install build-essential flex bison python3 lua5.4 valgrind
git clone https://github.com/PVictorMSI/GRUPO2-COMP2026.2.git
cd GRUPO2-COMP2026.2
make
./build/lua2py "Testes/Testes certos/Teste1(HelloWorld).lua" -o /tmp/hello.py
python3 /tmp/hello.py
make check
```

Python 3.8+ executa os programas gerados. Lua 5.4 é usado como referência nos testes diferenciais. Valgrind é usado na verificação de memória. Veja [instalação e CLI](docs/guia/instalacao.md).

## Recursos implementados

- Variáveis, escopos, atribuições e operadores aritméticos, relacionais e lógicos.
- Condicionais `if/elseif/else`, laços `while`, `repeat/until`, `for` numérico e genérico.
- Funções, recursão, closures, parâmetros variádicos e retornos múltiplos.
- Tabelas traduzidas para `list`/`dict`, com adaptação dos índices Lua.
- Subconjunto de `math`, `string`, `table`, `io` e funções globais.
- Diagnósticos léxicos, sintáticos e semânticos; geração de Python validada por `py_compile`.

O escopo implementado e suas restrições estão no [manual da linguagem](docs/projeto/escopo.md). O projeto não implementa Lua 5.4 integralmente.

## Documentação do semestre

- [18 semanas do calendário, de agosto a dezembro](docs/gestao/cronograma.md).
- [P1, P2, entrevista final T e avaliação C](docs/entregas/index.md).
- [Catálogo das 55 issues, responsáveis e critérios](docs/gestao/issues.md).
- [Arquitetura e decisões técnicas](docs/tecnico/arquitetura.md).
- [Testes e resultados](docs/qualidade/testes.md).
- [Evidência do Valgrind — issue #55](docs/qualidade/valgrind.md).

## Executar e publicar a documentação

```bash
python3 -m venv .venv-docs
source .venv-docs/bin/activate
python -m pip install -r requirements-docs.txt
python -m mkdocs serve
```

`python -m mkdocs build --strict` valida e gera `site/`. O workflow publica no GitHub Pages a partir da `main`. A configuração inicial e o procedimento completo estão em [publicação](docs/guia/publicacao.md).

## Organização

```text
lexer/              Especificação Flex
parser/             Gramática Bison e construção da AST
src/                AST, símbolos, semântica, gerador e runtime
Testes/             Casos válidos, inválidos, unitários e diferenciais
docs/               Manual, gestão, semanas, entregas e evidências
mkdocs.yml          Navegação e tema da documentação
.github/workflows/  Validação e publicação no GitHub Pages
```

Referência desta documentação: `main` em `1c5e2e3`, consultada em 27/11/2026. Resultado local: **144 verificações passaram, 0 falharam**, com a etapa de Valgrind pulada por ausência da ferramenta. A captura histórica de memória está preservada na página da issue #55.
