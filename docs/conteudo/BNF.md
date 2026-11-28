# Gramática (BNF) — Subconjunto de Lua suportado

Gramática descritiva alinhada ao parser atual (`parser/parser.y`). A notação usa extensões EBNF para repetição e opcionalidade. Fora do escopo: `goto`/labels, atributos `<const>`/`<close>` e operadores bitwise (`&`, `|`, `~`, `<<`, `>>`). Varargs são aceitos em funções variádicas e validados semanticamente.

A versão inicial desta página excluía varargs. A implementação atual ampliou o subconjunto; consulte [escopo e evolução](projeto/escopo.md).

Convenção: `NAME` em maiúsculas = token terminal (vindo do lexer). `{ }` = zero ou mais repetições. `[ ]` = opcional. `|` = alternativa.

## 1. Bloco e comandos

```
chunk        ::= block

block        ::= { stat } [ retstat ]

stat         ::= ';'
               | varlist '=' explist
               | functioncall
               | DO block END
               | WHILE exp DO block END
               | REPEAT block UNTIL exp
               | IF exp THEN block { ELSEIF exp THEN block } [ ELSE block ] END
               | FOR ID '=' exp ',' exp [ ',' exp ] DO block END
               | FOR namelist IN explist DO block END
               | FUNCTION funcname funcbody
               | LOCAL FUNCTION ID funcbody
               | LOCAL namelist [ '=' explist ]
               | BREAK

retstat      ::= RETURN [ explist ] [ ';' ]
```

## 2. Nomes, listas e variáveis

```
funcname     ::= ID { '.' ID } [ ':' ID ]

varlist      ::= var { ',' var }

var          ::= ID
               | prefixexp '[' exp ']'
               | prefixexp '.' ID

namelist     ::= ID { ',' ID }

explist      ::= exp { ',' exp }
```

## 3. Expressões

```
exp          ::= NIL | TRUE_VAL | FALSE_VAL | VARARG
               | NUM | NUM_FLOAT | STRING
               | functiondef
               | prefixexp
               | tableconstructor
               | exp binop exp
               | unop exp

prefixexp    ::= var
               | functioncall
               | '(' exp ')'

functioncall ::= prefixexp args
               | prefixexp ':' ID args

args         ::= '(' [ explist ] ')'
               | tableconstructor
               | STRING

functiondef  ::= FUNCTION funcbody

funcbody     ::= '(' [ parlist ] ')' block END

parlist      ::= namelist [ ',' VARARG ] | VARARG
```

## 4. Construtor de table

```
tableconstructor ::= '{' [ fieldlist ] '}'

fieldlist    ::= field { fieldsep field } [ fieldsep ]

field        ::= '[' exp ']' '=' exp
               | ID '=' exp
               | exp

fieldsep     ::= ',' | ';'
```

## 5. Operadores

```
binop        ::= '+' | '-' | '*' | '/' | '//' | '%' | '^' | '..'
               | '<' | '<=' | '>' | '>=' | '==' | '~='
               | AND | OR

unop         ::= '-' | NOT | '#'
```

## 6. Precedência de operadores (para `%left` / `%right` / `%nonassoc` no Bison)

Da **menor** para a **maior** precedência — reflete a ordem oficial de avaliação do Lua:

| Ordem | Operadores | Associatividade | Diretiva Bison |
|---|---|---|---|
| 1 (menor) | `OR` | esquerda | `%left OR` |
| 2 | `AND` | esquerda | `%left AND` |
| 3 | `<  >  <=  >=  ~=  ==` | esquerda | `%left LT GT LE GE NEQ EQ` |
| 4 | `..` (CONCAT) | **direita** | `%right CONCAT` |
| 5 | `+  -` (binários) | esquerda | `%left PLUS MINUS` |
| 6 | `*  /  //  %` | esquerda | `%left MULT DIV IDIV MOD` |
| 7 | `NOT  #  -` (unários) | — | `%right NOT LEN UMINUS` |
| 8 (maior) | `^` | **direita** | `%right POW` |

Observações:
- `..` e `^` são associativos à **direita** em Lua (ex.: `2^3^2` = `2^(3^2)`), diferente da maioria dos demais operadores.
- Os operadores unários (`-`, `not`, `#`) têm precedência **maior que os binários aritméticos** mas **menor que `^`** — por isso `-x^2` é `-(x^2)`, não `(-x)^2`. No Bison, isso normalmente é resolvido criando um pseudo-token `UMINUS` na regra de negação numérica via `%prec UMINUS`.
- O parser declara as comparações com `%left`. Encadeamentos são agrupados como operações binárias, não como comparações encadeadas do Python; seu resultado pode gerar erro de tipo em execução.

## 7. Mapeamento token → símbolo léxico

| Token (Bison) | Lexema |
|---|---|
| `AND`, `OR`, `NOT` | `and`, `or`, `not` |
| `IF`, `THEN`, `ELSE`, `ELSEIF`, `END` | `if`, `then`, `else`, `elseif`, `end` |
| `WHILE`, `DO`, `REPEAT`, `UNTIL`, `FOR`, `IN` | `while`, `do`, `repeat`, `until`, `for`, `in` |
| `FUNCTION`, `LOCAL`, `RETURN`, `BREAK` | `function`, `local`, `return`, `break` |
| `NIL`, `TRUE_VAL`, `FALSE_VAL` | `nil`, `true`, `false` |
| `CONCAT`, `IDIV` | `..`, `//` |
| `EQ`, `NEQ`, `LE`, `GE`, `LT`, `GT`, `ASSIGN` | `==`, `~=`, `<=`, `>=`, `<`, `>`, `=` |
| `PLUS`, `MINUS`, `MULT`, `DIV`, `MOD`, `POW`, `LEN` | `+`, `-`, `*`, `/`, `%`, `^`, `#` |
| `SEMI`, `COLON`, `COMMA`, `DOT` | `;`, `:`, `,`, `.` |
| `LPAREN`, `RPAREN`, `LBRACKET`, `RBRACKET`, `LBRACE`, `RBRACE` | `(`, `)`, `[`, `]`, `{`, `}` |
| `NUM`, `NUM_FLOAT`, `STRING`, `ID` | literais e identificadores |

`VARARG` (`...`) aparece em `parlist` e em `exp`. Usar `...` fora de uma função variádica gera erro semântico, como verificado pelo Teste11 da suíte.

## 8. Relação com o parser

A implementação está em [parser/parser.y](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/main/parser/parser.y). O parser constrói a AST durante as reduções, valida alvos de atribuição e declara dois conflitos shift/reduce esperados (`%expect 2`). Esta notação é uma descrição legível; o arquivo Bison é a fonte executável da gramática.
