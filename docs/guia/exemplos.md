# Exemplos de tradução

Os trechos Python abaixo ilustram a tradução. O arquivo efetivamente gerado pode incluir helpers, nomes renomeados e análise adicional de retornos. Para conferir a saída real, execute o compilador.

## Variáveis e condições

```lua
local nota = 8
if nota >= 5 then
    print("aprovado")
else
    print("revisar")
end
```

A estrutura torna-se `if ...:` / `else:` com quatro espaços por nível. O `print` usa o runtime para manter a formatação Lua. Para condições não booleanas, o gerador considera que `0` e string vazia são verdadeiros em Lua.

## Laço numérico

```lua
for i = 1, 3 do
    print(i)
end
```

O intervalo Lua inclui o limite final. Para esse caso literal, a tradução usa `range(1, 4)` e imprime `1`, `2`, `3`, um por linha. Limites dinâmicos e passos fracionários podem usar `lua_range`.

## Funções e recursão

```lua
local function fatorial(n)
    if n <= 1 then return 1 end
    return n * fatorial(n - 1)
end
print(fatorial(5))
```

Resultado esperado: `120`. A resolução mantém o símbolo local visível dentro da própria função.

## Retorno múltiplo

```lua
local function dividir(a, b)
    return a // b, a % b
end
local q, r = dividir(17, 5)
print(q, r)
```

Resultado esperado: `3` e `2` separados por tabulação. A tradução representa os múltiplos resultados como tupla e ajusta a quantidade de valores quando necessário.

## Tabelas e índices

```lua
local valores = {10, 20, 30}
print(valores[1])
local pessoa = {nome = "Ana"}
print(pessoa.nome)
```

Resultado: `10` e `Ana`, em linhas separadas. A tabela sequencial pode virar `list`; o mapa vira `dict`. Helpers convertem o índice Lua para a posição Python quando necessário.

## Diagnóstico semântico

```lua
break
```

`break` fora de laço é rejeitado com erro semântico e código 1. `print(os.time())` também é rejeitado por usar biblioteca fora do subconjunto.

## Programas completos

- [Testes e procedimento de comparação](../qualidade/testes.md).
- [Mapeamentos de operadores, funções e tabelas](../geracao_codigo.md).
- [Limitações que afetam a equivalência](../projeto/limitacoes.md).
