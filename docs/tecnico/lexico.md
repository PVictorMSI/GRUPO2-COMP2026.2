# Análise léxica

O Flex transforma `lexer/lexer.l` no scanner `build/lex.yy.c`. Cada regra reconhece um padrão e devolve um token definido pelo Bison. Valores de identificadores, números e strings são armazenados em `yylval`.

## Classes reconhecidas

| Classe | Exemplos |
| --- | --- |
| Palavras-chave | `local`, `function`, `if`, `then`, `end`, `while`, `for`, `return` |
| Identificadores | letras/underscore seguidos de letras, números ou underscore |
| Números | inteiros decimais/hexadecimais e floats com expoente |
| Strings | aspas simples, duplas e `[[...]]` |
| Operadores | aritméticos, relacionais, lógicos, concatenação e comprimento |
| Delimitadores | parênteses, chaves, colchetes, vírgula, ponto e ponto e vírgula |

Palavras reservadas aparecem antes da regra genérica de identificadores. O sinal de um número é tratado como operador unário pelo parser, permitindo distinguir `x-5` de um literal com sinal.

## Literais inteiros

O PR #65 alterou `intValue` para `long long`, usa `strtoll` nos decimais e conserva o inteiro em `NumberLiteral.intVal`. O gerador emite esse valor diretamente, evitando perda de precisão por conversão intermediária a `double`. A suíte inclui `literais_37.lua`, com valores acima de 32 bits e o máximo inteiro assinado de 64 bits. Isso não elimina as limitações de overflow aritmético do runtime.

## Estados do scanner

`COMMENT_LINE`, `COMMENT_BLOCK` e `LONGSTRING` tratam comentários e strings de várias linhas. `linha` é incrementado nas quebras de linha, inclusive nos estados que consomem texto de múltiplas linhas. `unescape_string` converte sequências de escape antes de construir a AST.

## Erros

O scanner reconhece identificadores malformados, números como `1.2.3`, strings não terminadas, comentário de bloco não terminado e caracteres inesperados. `LEX_ERROR()` incrementa `lex_errors`; o `main` impede a geração mesmo que o parser consiga seguir após um erro recuperável.

## Depuração

```bash
make clean
make TRACE=1
./build/lua2py entrada.lua > saida.py
```

O rastreio imprime números de tokens em `stderr`. Para retornar ao modo normal, recompile após `make clean` sem `TRACE=1`.

## Rastreabilidade

Issues [#21](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/21), [#22](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/22), [#37](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/37) e [#44](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/44). Fonte: [lexer/lexer.l](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/lexer/lexer.l). Casos negativos: `Teste1` a `Teste5` em `Testes/Testes errados/`. Consulte as [limitações numéricas](../projeto/limitacoes.md).
