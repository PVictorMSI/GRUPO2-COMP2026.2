-- Algoritmos clássicos: exercitam laços (#30), tabelas base 1 (#41),
-- operadores (#36) e retorno múltiplo (#39) juntos.

local function bubble_sort(v)
  local n = #v
  for i = 1, n - 1 do
    local trocou = false
    for j = 1, n - i do
      if v[j] > v[j + 1] then
        v[j], v[j + 1] = v[j + 1], v[j]
        trocou = true
      end
    end
    if not trocou then break end
  end
  return v
end
print(table.concat(bubble_sort({5, 3, 8, 1, 9, 2}), " "))

local function busca_binaria(v, alvo)
  local lo, hi = 1, #v
  local passos = 0
  while lo <= hi do
    passos = passos + 1
    local meio = (lo + hi) // 2
    if v[meio] == alvo then return meio, passos
    elseif v[meio] < alvo then lo = meio + 1
    else hi = meio - 1 end
  end
  return nil, passos
end
local ordenado = {1, 3, 5, 7, 9, 11, 13}
print(busca_binaria(ordenado, 9))
print(busca_binaria(ordenado, 4))

local function crivo(n)
  local eh_primo = {}
  for i = 2, n do eh_primo[i] = true end
  for i = 2, n do
    if eh_primo[i] then
      for j = i * i, n, i do eh_primo[j] = false end
    end
  end
  local primos = {}
  for i = 2, n do
    if eh_primo[i] then primos[#primos + 1] = i end
  end
  return primos
end
local p = crivo(50)
print(#p, table.concat(p, ","))

local function conta_palavras(texto)
  local contagem, ordem = {}, {}
  local palavra = ""
  for i = 1, #texto + 1 do
    local c = string.sub(texto, i, i)
    if c ~= " " and c ~= "" then
      palavra = palavra .. c
    elseif palavra ~= "" then
      if not contagem[palavra] then ordem[#ordem + 1] = palavra end
      contagem[palavra] = (contagem[palavra] or 0) + 1
      palavra = ""
    end
  end
  for _, w in ipairs(ordem) do io.write(w, "=", contagem[w], " ") end
  print()
end
conta_palavras("o rato roeu a roupa do rei de roma o rato")

local function fibonacci(n)
  local a, b = 0, 1
  for _ = 1, n do a, b = b, a + b end
  return a
end
local fibs = {}
for i = 0, 15 do fibs[#fibs + 1] = fibonacci(i) end
print(table.concat(fibs, " "))

local function media_desvio(...)
  local n, soma = select("#", ...), 0
  for _, x in ipairs({...}) do soma = soma + x end
  local media = soma / n
  local acc = 0
  for _, x in ipairs({...}) do acc = acc + (x - media) ^ 2 end
  return media, math.sqrt(acc / n)
end
print(string.format("%.3f %.3f", media_desvio(2, 4, 4, 4, 5, 5, 7, 9)))

local matriz = {}
for i = 1, 3 do
  matriz[i] = {}
  for j = 1, 3 do matriz[i][j] = (i - 1) * 3 + j end
end
local transposta = {}
for i = 1, 3 do
  transposta[i] = {}
  for j = 1, 3 do transposta[i][j] = matriz[j][i] end
end
for i = 1, 3 do print(table.concat(transposta[i], " ")) end

local contador = 10
repeat
  contador = contador - 3
until contador < 0
print(contador, 10 / 4, 10 // 4, 2 ^ -1, 7 % 0.5)
