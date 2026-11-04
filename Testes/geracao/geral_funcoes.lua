-- Programa geral: funções, closures, escopo, strings e métodos
local function fatorial(n)
  if n <= 1 then return 1 end
  return n * fatorial(n - 1)
end
print(fatorial(10))

function fib(n)
  if n < 2 then return n end
  return fib(n - 1) + fib(n - 2)
end
print(fib(15))

-- closure com estado
local function novo_contador()
  local c = 0
  return function()
    c = c + 1
    return c
  end
end
local c1, c2 = novo_contador(), novo_contador()
print(c1(), c1(), c2())

-- global atribuída dentro de função
total = 0
local function acumula(v) total = total + v end
acumula(5); acumula(7)
print(total)

-- sombreamento e escopo de bloco
local x = 1
do
  local x = 2
  print("dentro", x)
end
print("fora", x)

-- objetos com métodos
local Conta = {}
function Conta.nova(saldo)
  return {saldo = saldo, deposita = Conta.deposita}
end
function Conta:deposita(v)
  self.saldo = self.saldo + v
  return self.saldo
end
local cc = Conta.nova(100)
print(cc:deposita(50), cc.saldo)

-- strings
local s = "Compiladores"
print(s:upper(), s:len(), s:sub(1, 3), string.rep("ab", 3, "-"))
print(string.format("%d itens a R$ %.2f (%s)", 3, 9.5, "ok"))
print(string.format("%5d|%-5s|%x", 42, "ab", 255))
print(tostring(nil), tostring(true), tonumber("12"), tonumber("0x1F"), tonumber("abc"))
print(type(1), type("a"), type({}), type(print), type(nil))
print(10 / 2, 3 // 1, 1e3, 0.1 + 0.2)
print(math.floor(3.7), math.max(1, 9, 4), math.huge, -math.huge, math.pi)
print("linha1\nlinha2\t[tab]", 'aspas "duplas"', "barra \\")
print([[texto
longo]])

-- função anônima como argumento
local function aplica(f, v) return f(v) end
print(aplica(function(n) return n * 2 end, 21))

-- vararg
local function soma(...)
  local total_local = 0
  for _, v in ipairs({...}) do total_local = total_local + v end
  return total_local, select("#", ...)
end
print(soma(1, 2, 3, 4))

-- nomes que são palavras reservadas do Python
local class, def, lambda = 1, 2, 3
print(class + def + lambda)

-- pcall e error
local ok, msg = pcall(function() error("falhou", 0) end)
print(ok, msg)
print(pcall(function(a, b) return a + b end, 2, 3))
