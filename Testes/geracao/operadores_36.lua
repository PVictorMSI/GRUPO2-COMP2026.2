-- Issue #36: tradução dos operadores Lua -> Python
local a, b = 7, 2
print(a + b, a - b, a * b)
print(a / b, a // b, a % b)          -- / sempre float; // e % com piso (como no Lua)
print(-7 // 2, -7 % 2, 7 % -2)       -- sinais: o resultado segue o divisor
print(7.5 // 2, 2 ^ 10, 2 ^ 0.5)     -- ^ sempre devolve float
print(-2 ^ 2, (-2) ^ 2)              -- ^ tem precedência maior que o menos unário
print(1 + 2 * 3 - 4 / 2, (1 + 2) * 3)
print(2 ^ 3 ^ 2)                     -- ^ associa à direita

-- concatenação
local nome = "Lua"
print("Olá, " .. nome .. "!", 1 .. 2, "v" .. 5.0, "x" .. 10 // 3)

-- relacionais
print(a == b, a ~= b, a < b, a <= b, a > b, a >= b)
print("abc" < "abd", "Z" < "a", 1 == 1.0)

-- igualdade: tabelas comparam identidade, true não é 1
local t1, t2 = {1}, {1}
local t3 = t1
print(t1 == t2, t1 == t3, t1 ~= t2)
local x
print(true == 1, nil == false, x == nil)

-- lógicos: só nil e false são falsos (0 e "" são verdadeiros)
print(0 and "zero é verdadeiro", "" and "vazio é verdadeiro")
print(nil or "padrão", false or nil, nil and 1, false and 1)
print(1 and 2 or 3, nil and 2 or 3, a > b and "maior" or "menor")
print(not nil, not 0, not not "x", not (a == b))
local cont = 0
local function conta() cont = cont + 1; return cont end
local r = conta() or conta()        -- curto-circuito: a direita não é avaliada
print(r, cont)
local opcional
local valor = opcional or 42
print(valor)

-- comprimento
print(#"hello", #{10, 20, 30}, #"")

-- precedência mista
print(not true == false)             -- (not true) == false
print(1 .. 2 == "12")
print(-a ^ 2, 2 * -3)
