# Diagnósticos e solução de problemas

| Situação | Etapa | Ação |
| --- | --- | --- |
| Caractere inesperado, string/comentário não terminado | Léxica | Conferir delimitadores e caracteres na linha informada |
| Token inesperado, bloco incompleto | Sintática | Conferir `end`, parênteses e estrutura da gramática |
| Variável nunca declarada/atribuída | Resolução | Conferir nome, escopo e atribuições globais |
| `break` fora de laço | Semântica | Mover o comando para um laço da mesma função |
| `...` fora de função variádica | Semântica | Declarar parâmetros variádicos ou retirar o uso |
| Parâmetro duplicado | Semântica | Usar nomes distintos |
| Biblioteca não suportada | Semântica/runtime | Consultar a lista do subconjunto e substituir a operação |
| Arquivo não pode ser aberto/gravado | CLI | Conferir caminho e permissões |
| Python não executa ou resultado diverge | Runtime/geração | Reduzir o caso, comparar com Lua e conferir limitações |

## Formato das mensagens

```text
Erro léxico na linha N: descrição
Erro sintático na linha N: descrição
Erro semântico na linha N: descrição
```

As linhas são aproximadas e dependem de quando o lexer/parser detecta a falha. Mensagens de arquivo seguem outro formato, pois não correspondem a uma linha da linguagem. A issue [#50](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/50) continua aberta na consulta.

## Build e testes

`flex: command not found` ou `bison: command not found` indicam dependências ausentes. Instale-as conforme o [guia](../guia/instalacao.md). Ao alternar `TRACE`, recompile após `make clean`. Verificações puladas por falta de Python ou Valgrind não devem ser registradas como aprovadas.

## Reportar um problema

Registre um programa mínimo, o comando, versões das ferramentas, commit, resultado esperado e resultado obtido. Para falhas semânticas, compare a mesma entrada com Lua 5.4 quando ela estiver dentro do subconjunto. Para memória, anexe o log completo do Valgrind e o código de saída.
