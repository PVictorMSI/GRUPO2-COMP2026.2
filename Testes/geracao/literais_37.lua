-- Teste Issue #37: tradução de literais Lua → Python
-- Cobre: nil, true/false, inteiros (pequenos, grandes, hex), floats, strings

-- nil e booleanos
local n = nil
local t = true
local f = false

-- inteiros: pequeno, maior que INT_MAX (32-bit), e máximo int64
local pequeno = 42
local grande  = 2147483648
local maximo  = 9223372036854775807

-- hex: valores acima de 0xFFFF
local hex1 = 0xFF
local hex2 = 0xDEADBEEF

-- floats
local pi   = 3.14
local expo = 1e10
local neg  = -2.5e-3

-- strings: aspas simples, duplas e escapes
local s1 = "olá mundo"
local s2 = 'com \'aspas\' simples'
local s3 = "com\nnewline\te\ttab"

print(n, t, f)
print(pequeno, grande, maximo)
print(hex1, hex2)
print(pi, expo, neg)
print(s1, s2, s3)
