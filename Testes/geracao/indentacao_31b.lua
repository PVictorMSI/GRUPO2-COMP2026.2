-- Casos de borda de indentação (#31) — complemento ao indentacao_31.lua
-- Foco: do..end, elseif em cadeia, funções dentro de funções,
--       break em laços aninhados, return em vários níveis, tabela com função.

-- ================================================================
-- 1. do...end vazio e aninhado (deve gerar pass)
-- ================================================================
do
end

do
    do
        do
        end
    end
end

-- ================================================================
-- 2. Cadeia de elseif com e sem corpo (alinhamento rigoroso)
-- ================================================================
local x = 7
local resultado = ""
if x == 1 then
    resultado = "um"
elseif x == 2 then
    resultado = "dois"
elseif x == 3 then
    resultado = "tres"
elseif x == 4 then
    resultado = "quatro"
elseif x == 5 then
    resultado = "cinco"
elseif x == 6 then
    resultado = "seis"
elseif x == 7 then
    resultado = "sete"
elseif x == 8 then
    resultado = "oito"
elseif x == 9 then
    resultado = "nove"
else
    resultado = "outro"
end
print(resultado)

-- ================================================================
-- 3. Função dentro de função (closure) — aninhamento de def
-- ================================================================
local function externa(a)
    local function interna(b)
        local function mais_interna(c)
            return a + b + c
        end
        return mais_interna(b * 2)
    end
    return interna(a + 1)
end
print(externa(3))

-- ================================================================
-- 4. local function dentro de if dentro de for (triplo aninhamento
--    com declaração local — indentação soma 3 níveis)
-- ================================================================
local soma = 0
for i = 1, 3 do
    if i > 1 then
        local function dobro(v)
            return v * 2
        end
        soma = soma + dobro(i)
    end
end
print(soma)

-- ================================================================
-- 5. break em laços aninhados (indentação do break deve seguir o
--    laço mais interno em que está, não vazar para fora)
-- ================================================================
local achou = false
for i = 1, 5 do
    for j = 1, 5 do
        if i * j == 6 then
            achou = true
            break
        end
    end
    if achou then
        break
    end
end
print(achou)

-- ================================================================
-- 6. return em vários níveis (incluindo return dentro de if
--    dentro de while dentro de função)
-- ================================================================
local function busca(lista, alvo)
    local i = 1
    while i <= #lista do
        if lista[i] == alvo then
            return i
        end
        i = i + 1
    end
    return 0
end
local t = {10, 20, 30, 40}
print(busca(t, 30))
print(busca(t, 99))

-- ================================================================
-- 7. Tabela cujo campo é uma função anônima com bloco interno
--    (içamento de def antes do construtor — indentação deve ser
--    no nível do módulo, não do campo)
-- ================================================================
local ops = {
    dobra = function(v)
        if v > 0 then
            return v * 2
        else
            return 0
        end
    end,
    tripla = function(v)
        return v * 3
    end,
}
print(ops.dobra(5))
print(ops.dobra(-1))
print(ops.tripla(4))

-- ================================================================
-- 8. repeat-until com bloco vazio (corpo vazio + condição simples)
-- ================================================================
local k = 0
repeat
    k = k + 1
until k >= 1
print(k)

-- ================================================================
-- 9. for genérico dentro de if dentro de função
-- ================================================================
local function filtra_pares(lista)
    local r = {}
    if #lista > 0 then
        for _, v in ipairs(lista) do
            if v % 2 == 0 then
                r[#r + 1] = v
            end
        end
    end
    return r
end
local pares = filtra_pares({1, 2, 3, 4, 5, 6})
for _, v in ipairs(pares) do
    print(v)
end
