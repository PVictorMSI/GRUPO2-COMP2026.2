-- Casos difíceis para o gerador: nomes, escopos e closures

-- nomes do Lua que são reservados ou builtins no Python
local len, list, str, None, True, lua_x = 1, 2, 3, 4, 5, 6
print(len + list + str + None + True + lua_x)
local function range(n) return n * 2 end
print(range(21))
local id, type_, max = "id", "t", 9
print(id, type_, max)

-- global com nome de builtin do Python, usada dentro de função
print_count = 0
local function conta() print_count = print_count + 1 end
conta(); conta()
print(print_count)

-- três níveis de closure escrevendo na variável de fora
local function externa()
  local x = 0
  local function meio()
    local function interna()
      x = x + 1
      return x
    end
    return interna
  end
  local f = meio()
  f(); f()
  return x
end
print(externa())

-- variável local do chunk alterada dentro de função (vira global no Python)
local nivel = 1
local function sobe() nivel = nivel + 1 end
sobe()
print(nivel)

-- função global definida dentro de função
local function registra()
  function ajudante(v) return v + 100 end
end
registra()
print(ajudante(1))

-- recursão mútua com globais
function par(n) if n == 0 then return true end return impar(n - 1) end
function impar(n) if n == 0 then return false end return par(n - 1) end
print(par(10), impar(7))

-- parâmetros opcionais e argumentos a mais
local function saudacao(nome, sauda)
  sauda = sauda or "Olá"
  return sauda .. ", " .. nome
end
print(saudacao("Ana"), saudacao("Bia", "Oi"))
local function ignora(a) return a end
local callbacks = {ignora}
print(callbacks[1](1, 2, 3))

-- and/or com chamadas (avaliadas uma vez só)
local chamadas = 0
local function talvez(v) chamadas = chamadas + 1; return v end
local r1 = talvez(nil) or talvez(false) or talvez(0)
local r2 = talvez(1) and talvez("") and talvez("fim")
print(r1, r2, chamadas)

-- if/elseif/else com condições não booleanas
local function classifica(v)
  if not v then return "falso"
  elseif v == 0 then return "zero"
  elseif type(v) == "string" and #v == 0 then return "vazia"
  else return "outro" end
end
print(classifica(nil), classifica(false), classifica(0), classifica(""), classifica({}))

-- while com condição composta
local a, b = 0, 10
while a < 5 and b > 7 do a = a + 1; b = b - 1 end
print(a, b)

-- repeat com local usada no until, aninhado com break
local tentativas = 0
repeat
  local ok = tentativas >= 2
  tentativas = tentativas + 1
  if tentativas > 10 then break end
until ok
print(tentativas)

-- for com passo negativo float e limites calculados
for v = 1.0, 0, -0.5 do io.write(v, " ") end
print()
local t = {5, 6, 7}
for i = #t, 1, -1 do io.write(t[i], " ") end
print()

-- tabela de funções e métodos com self e campos aninhados
local Pilha = {}
Pilha.__index = Pilha
function Pilha.nova() return {itens = {}, push = Pilha.push, pop = Pilha.pop, tam = Pilha.tam} end
function Pilha:push(v) self.itens[#self.itens + 1] = v end
function Pilha:pop() local v = self.itens[#self.itens]; self.itens[#self.itens] = nil; return v end
function Pilha:tam() return #self.itens end
local p = Pilha.nova()
p:push("a"); p:push("b"); p:push("c")
print(p:pop(), p:tam())

-- strings com escapes e números formatados
print("tab\tfim", "quebra\\n literal", "\65\66\67", "\x41", "\u{48}i")
print(1e2, 2.5e-3, 0x10, 1 / 3, 100 // 1.0, 7 // 2.0, 2^53)
print(10 // 3, 10.0 // 3, -10 // 3, 10 % 3.5)
print(tostring(1/2), 3 == 3.0, math.floor(-2.5), math.max(2, 3.5))

-- concatenação com números
local n = 3
print("n=" .. n, n .. "", 1.5 .. "|" .. 2 .. "|" .. -0.0)

-- do ... end com mesmo nome
local v = "fora"
do local v = "dentro"; print(v) end
print(v)

-- retorno múltiplo em cadeia
local function minmax(...)
  local lo, hi = math.huge, -math.huge
  for _, x in ipairs({...}) do
    if x < lo then lo = x end
    if x > hi then hi = x end
  end
  return lo, hi
end
local lo, hi = minmax(4, 8, 1, 9)
print(lo, hi, minmax(3, 2))
