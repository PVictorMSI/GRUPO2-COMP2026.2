-- Issue #30: geração de código para laços de repetição

-- while
local i = 1
while i <= 3 do
  print("while", i)
  i = i + 1
end

-- repeat ... until (a condição enxerga as locais do corpo)
local n = 0
repeat
  local quadrado = n * n
  n = n + 1
until quadrado >= 9
print("repeat parou em", n)

-- for numérico: crescente, decrescente, com passo, float, limites avaliados uma vez
for k = 1, 3 do io.write(k, " ") end
print()
for k = 10, 1, -3 do io.write(k, " ") end
print()
for k = 0, 1, 0.25 do io.write(k, " ") end
print()
for k = 1, 0 do print("nunca executa") end
local limite = 3
for k = 1, limite do
  limite = 10                      -- não altera o número de voltas
  io.write(k, " ")
end
print(limite)
for k = 1, 2.5 do io.write(k, " ") end  -- limite float com início inteiro
print()

-- for genérico: ipairs e pairs
local frutas = {"maçã", "banana", "uva"}
for idx, fruta in ipairs(frutas) do print(idx, fruta) end
local soma = 0
local pesos = {a = 1, b = 2, c = 3}
for chave, peso in pairs(pesos) do soma = soma + peso end
print("soma dos pesos", soma)
for idx in ipairs(frutas) do io.write(idx, ";") end
print()

-- for genérico com iterador próprio (protocolo geral do Lua)
local function contador(max)
  local atual = 0
  return function()
    atual = atual + 1
    if atual <= max then return atual end
  end
end
for v in contador(3) do io.write("c", v, " ") end
print()
local function proximo(t, i)
  i = i + 1
  if t[i] then return i, t[i] end
end
for i2, v in proximo, {"x", "y"}, 0 do print(i2, v) end

-- break e laços aninhados
for a = 1, 3 do
  for b = 1, 3 do
    if b > a then break end
    io.write(a, b, " ")
  end
end
print()
local w = 0
while true do
  w = w + 1
  if w == 5 then break end
end
print("w", w)

-- cada volta do laço tem uma variável nova: closures lembram o valor da volta
local fs = {}
for j = 1, 3 do
  fs[j] = function() return j * 10 end
end
print(fs[1](), fs[2](), fs[3]())
local gs = {}
for _, nome in ipairs({"a", "b"}) do
  local maiusc = string.upper(nome)
  gs[#gs + 1] = function() return maiusc end
end
print(gs[1](), gs[2]())
