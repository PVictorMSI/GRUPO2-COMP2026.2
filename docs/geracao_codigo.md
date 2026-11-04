# Geração de código Python (Frente D)

O gerador (`src/codegen.cpp`) percorre a AST já resolvida (`analyze`) e sem erros
semânticos (`checkSemantics`) e escreve um programa **Python 3.8+ autossuficiente**:

```
# Gerado automaticamente pelo lua2py ...
# ---- runtime: semântica do Lua que o Python não tem ----
...só as funções do runtime que o programa usa (src/runtime_py.cpp)...
# ---- programa ----
...código traduzido...
```

Uso:

```bash
make
./build/lua2py programa.lua                 # Python na saída padrão
./build/lua2py programa.lua -o programa.py  # grava no arquivo
./build/lua2py < programa.lua > programa.py # também funciona com redirecionamento
python3 programa.py
```

Erros (léxicos, sintáticos, semânticos) vão para a saída de erro e o código de saída é 1;
nesse caso nenhum Python é gerado.

## Princípios

- **Nomes únicos.** A tabela de símbolos já dá um nome Python diferente para cada variável
  (`x`, `x_1`, ...). Por isso o escopo de bloco do Lua (`do ... end`, corpo de laço) não
  precisa de nada especial no Python, e `global`/`nonlocal` são calculados com exatidão.
- **Python direto quando é equivalente; runtime quando não é.** `a + b` sai como `a + b`;
  `a .. b` sai como `lua_concat(a, b)` porque o Python não tem o mesmo operador.
- **Fidelidade conferida com o Lua real.** Cada programa de `Testes/geracao/` tem um
  `.esperado` com a saída do `lua5.4`; o `make check` exige saída idêntica.

## #36 — Operadores

| Lua | Python gerado | Por quê |
|---|---|---|
| `a + b`, `a - b`, `a * b`, `-a` | igual | mesma semântica para int/float |
| `a / b` | `a / b` se `b` é literal ≠ 0, senão `lua_div(a, b)` | no Lua `x / 0` dá `inf`/`nan`, no Python é erro |
| `a // b`, `a % b` | `a // 2`, `a % 2` com divisor inteiro literal ≠ 0, senão `lua_idiv` / `lua_mod` | piso e sinal do divisor já iguais; diferem na divisão por zero |
| `a ^ b` | `float(a) ** 2` (expoente inteiro ≥ 0 ou base positiva literal), senão `lua_pow(a, b)` | `^` sempre dá float; `(-8) ** (1/3)` seria complexo no Python |
| `a .. b .. c` | `lua_concat(a, b, c)` | aceita números e formata como o Lua (`1.0`, `1e+15`) |
| `a == nil`, `a ~= true` | `a is None`, `a is not True` | |
| `a == 1`, `s ~= "x"` | `a == 1`, `s != "x"` | literal número/string |
| `a == b` (demais) | `lua_eq(a, b)` | tabelas comparam identidade; `true ~= 1` |
| `<`, `<=`, `>`, `>=` | iguais, sem encadear | `a < b < c` do Python teria outro sentido |
| `not x` | `not x` (x booleano) ou `not lua_truthy(x)` | no Lua só `nil`/`false` são falsos; `0` e `""` são verdadeiros |
| `a and b`, `a or b` | `a and b` se `a` é booleano; `b if lua_truthy(a) else a`; com `a` composto `... lua_t := a ...` (avalia uma vez) | `and`/`or` devolvem um operando |
| `c and "x" or "y"` | `"x" if c else "y"` | idioma "ternário" do Lua |
| `#t`, `#s` | `lua_len(t)` | tabela: último índice; string: bytes |

Precedência: o gerador só põe parênteses onde a precedência do Python exige
(`-x ^ 2` → `-float(x) ** 2`, `(1 + 2) * 3` mantém, `1 + 2 * 3` não ganha parênteses).

Teste diferencial: `bash Testes/geracao/fuzz_operadores.sh 100` gera milhares de expressões
aleatórias e compara com o `lua5.4`.

## #30 — Laços de repetição

| Lua | Python gerado |
|---|---|
| `while c do ... end` | `while c:` (condição com a verdade do Lua) |
| `repeat ... until c` | `while True:` ... `if c: break` (a condição enxerga as locais do corpo) |
| `for i = 1, 10 do` | `for i in range(1, 11):` (limites literais inteiros) |
| `for i = 10, 1, -2 do` | `for i in range(10, 0, -2):` |
| `for i = a, b, c do` | `for i in lua_range(a, b, c):` (limites avaliados uma vez; float; passo 0 é erro) |
| `for k, v in pairs(t)` | `for k, v in lua_pairs_iter(t):` |
| `for i, v in ipairs(t)` | `for i, v in lua_ipairs_iter(t):` (para no primeiro `nil`) |
| `for a, b in f, s, c` | `for a, b in lua_generic_for(2, f, s, c):` (protocolo geral) |
| `break` | `break` |

**Closures em laços:** no Lua cada volta tem uma variável nova; no Python a variável é
uma só. Uma função criada no laço que usa uma variável da volta (que nunca é reatribuída)
recebe o valor como parâmetro só-nomeado: `def lua_fn2(*lua_extra, j=j):`.

## #39 — Retorno múltiplo

Vários valores são uma `tuple` do Python (nenhum valor Lua comum é tupla).

| Lua | Python gerado |
|---|---|
| `return a, b` | `return a, b` |
| `return f(x)` | `return f(x)` (repassa todos os valores) |
| `return a, f(x)` | `return (a, *lua_multi(f(x)))` |
| `local q, r = divmod(a, b)` (sempre 2 valores) | `q, r = divmod(a, b)` |
| `local x, y, z = f()` (quantidade variável) | `x, y, z = lua_adjust(3, *lua_multi(f()))` |
| `local x = f()` | `x = lua_first(f())` (ou `x = f()` se `f` devolve sempre 1 valor) |
| `print(f())` (último argumento) | `lua_print(*lua_multi(f()))` |
| `print(f(), 1)` (não-último) | `lua_print(lua_first(f()), 1)` |
| `(f())` | `lua_first(f())` |
| `{f()}` | `[*lua_multi(f())]` |

Uma pré-análise descobre quais funções devolvem sempre um único valor (ponto fixo
sobre `return f(x)`), para não poluir o código com `lua_first` à toa. Parâmetros ganham
`=None` (o Lua aceita chamar com menos argumentos) e funções usadas como valor
(callbacks, iteradores, métodos) ganham `*lua_extra` (o Lua ignora argumentos a mais).

## #41 — Tabelas: base 1 → base 0

- `{10, 20, 30}` vira `[10, 20, 30]`; `{x = 1}` vira `{"x": 1}`;
  `{"a", cor = "azul", "b"}` vira `{"cor": "azul", 1: "a", 2: "b"}`.
- Todo acesso passa por `lua_get(t, k)` / `lua_set(t, k, v)`. Em uma `list`, o índice
  Lua `k` é a posição Python `k - 1` (conversão concentrada em `lua_list_index`);
  `t[#t + 1] = v` acrescenta e `t[#t] = nil` remove. Em um `dict` as chaves são as do Lua.
- `{}` vazio só vira `list` quando a pré-análise prova que a variável é usada como lista
  (`t[#t + 1] = v`, `t[i] = v` com `for i = 1, n` ou `ipairs`, `#t`, `table.insert`...).
  Uma lista que depois recebe chave de texto ou índice fora de ordem (`v[5] = 50` num
  vetor de 3) é gerada como `dict` `{1: 10, 2: 20, 3: 30}`. Na dúvida, `dict`.
- `math.floor` vira `lua_math["floor"]` (bibliotecas são `dict`s do runtime).

## #54 — Validação com py_compile

`make check` traduz **todo** programa válido do repositório e exige que:

1. `python3 -W error -m py_compile` aceite o `.py` (sintaxe, indentação, sem avisos);
2. quando existe `.esperado` (saída do `lua5.4`), a execução do Python dê saída idêntica;
3. o compilador rode sem vazamento no Valgrind também nesses programas.

Para criar um teste novo: coloque `nome.lua` em `Testes/geracao/` e rode
`bash Testes/geracao/gerar_esperados.sh` (precisa do `lua5.4`).

## Limitações conhecidas

- `pcall`/`error` não acrescentam a posição (`arquivo:linha:`) à mensagem.
- `"10" + 1` (string convertida em número na aritmética) não é suportado.
- Com divisor inteiro literal, `x % 3` com `x` float negativo múltiplo de 3 dá `0.0`
  em vez de `-0.0` (só o sinal do zero).
- Comparar com `==` um número literal e uma variável que guarda `true`/`false`
  (`x == 1` com `x = true`) dá `true` no Python.
- Uma função sem `return` chamada por um valor desconhecido (não pelo nome) e expandida
  (`print(callback())`) passa `nil` em vez de nenhum valor.
- `string.find` só com busca simples (`plain`); padrões do Lua, `gsub`, `gmatch`,
  metatabelas e corrotinas não são suportados (biblioteca padrão é a #42).
- Inteiros não dão a volta em 64 bits como no Lua.
