# Gramática (BNF) — Subconjunto de Lua suportado

Gramática alinhada aos tokens produzidos por `lexer.l` e ao escopo definido no documento de requisitos (RF03–RF05). Fora do escopo: `goto`/labels, varargs (`...`), atributos `<const>`/`<close>`, operadores bitwise (`&`, `|`, `~`, `<<`, `>>`).

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
exp          ::= NIL | TRUE_VAL | FALSE_VAL
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

funcbody     ::= '(' [ namelist ] ')' block END
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
| 3 | `<  >  <=  >=  ~=  ==` | não associativo | `%nonassoc LT GT LE GE NEQ EQ` |
| 4 | `..` (CONCAT) | **direita** | `%right CONCAT` |
| 5 | `+  -` (binários) | esquerda | `%left PLUS MINUS` |
| 6 | `*  /  //  %` | esquerda | `%left MULT DIV IDIV MOD` |
| 7 | `NOT  #  -` (unários) | — | `%precedence UNARY` |
| 8 (maior) | `^` | **direita** | `%right POW` |

Observações:
- `..` e `^` são associativos à **direita** em Lua (ex.: `2^3^2` = `2^(3^2)`), diferente da maioria dos demais operadores.
- Os operadores unários (`-`, `not`, `#`) têm precedência **maior que os binários aritméticos** mas **menor que `^`** — por isso `-x^2` é `-(x^2)`, não `(-x)^2`. No Bison, isso normalmente é resolvido criando um pseudo-token `UNARY` só para uso em `%precedence`, aplicado explicitamente na regra do operador unário via `%prec UNARY`.
- `<`, `>`, `<=`, `>=`, `~=`, `==` são não associativos em Lua — `a < b < c` não é uma expressão válida (evita ambiguidade de encadeamento).

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

`VARARG` (`...`) é reconhecido pelo lexer mas **não aparece em nenhuma regra desta gramática** — de propósito, já que varargs está fora do escopo do projeto. Um uso de `...` no código-fonte deve resultar em erro sintático (comportamento esperado), não em erro léxico.

## 8. Próximo passo sugerido

Esta gramática está pronta para virar o arquivo `.y` do Bison: cada produção acima mapeia quase diretamente para uma regra Bison, e a tabela da seção 6 já traz as diretivas de precedência prontas para declarar no topo do arquivo.
