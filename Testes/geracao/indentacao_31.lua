-- Testa blocos vazios, aninhamentos profundos, encadeamento if/elif/else e closures

-- 1. Funções e blocos vazios (devem gerar "pass" indentado no Python)
local function vazia()
end
vazia()

if false then
elseif false then
else
end

while false do
end

for i = 1, 0 do
end

local function com_ponto_virgula()
    ;;;
end
com_ponto_virgula()

-- 2. Aninhamento profundo de blocos (recuo correto de 4 espaços por nível)
local total = 0
local function aninhamento(n)
    if n > 0 then
        local i = 1
        while i <= n do
            for j = 1, 2 do
                if (i + j) % 2 == 0 then
                    total = total + i * j
                else
                    ;;
                end
            end
            i = i + 1
        end
    end
    return total
end
print(aninhamento(3))

-- 3. Função anônima com blocos internos passada como argumento
local function aplicar(f, x)
    return f(x)
end

local res = aplicar(function(v)
    if v > 10 then
        return v * 2
    else
        return v + 2
    end
end, 15)
print(res)

-- 4. Laço repeat-until
local c = 0
repeat
    c = c + 1
until c >= 3
print(c)
