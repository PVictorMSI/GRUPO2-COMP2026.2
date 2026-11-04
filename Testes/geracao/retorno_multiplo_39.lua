-- Issue #39: retorno múltiplo de valores
local function divmod(a, b)
  return a // b, a % b
end

local q, r = divmod(17, 5)
print(q, r)

local so_q = divmod(17, 5)          -- sobra: descarta
print(so_q)

local x, y, z = divmod(17, 5)       -- falta: completa com nil
print(x, y, z)

print(divmod(17, 5))                -- último argumento: expande todos
print(divmod(17, 5), "fim")         -- não-último: só o primeiro
print((divmod(9, 2)))

local t = {divmod(20, 6)}           -- último campo da tabela: expande
print(#t, t[1], t[2])
local t2 = {divmod(20, 6), 99}
print(#t2, t2[1], t2[2])

local function repassa(a, b)
  return divmod(a, b)               -- tail: repassa todos os valores
end
print(repassa(7, 2))

local function tres() return 1, 2, 3 end
local function com_prefixo() return 0, tres() end
print(com_prefixo())

local function nada() end
local function vazio() return end
print(nada(), vazio())
local nv = nada()
print(nv)

local a, b, c = 1, tres()
print(a, b, c)
local d, e = tres(), 10
print(d, e)

-- troca de valores
local m, n = 1, 2
m, n = n, m
print(m, n)

-- vários valores em atribuição a campos de tabela
local p = {}
p.x, p.y = divmod(11, 4)
print(p.x, p.y)

-- quantidade de argumentos diferente da de parâmetros
local function tres_params(a1, a2, a3) return a1, a2, a3 end
print(tres_params(1))
print(tres_params(tres()))
print(select("#", tres()), select(2, tres()))
