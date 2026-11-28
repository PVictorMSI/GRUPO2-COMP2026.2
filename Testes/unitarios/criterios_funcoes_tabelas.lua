-- Critérios de aceitação: funções (function ... end -> def) e tabelas (list / dict)
function soma(a, b)
  return a + b
end

local function fatorial(n)
  if n <= 1 then return 1 end
  return n * fatorial(n - 1)
end

local nums = {10, 20, 30}                 -- usada como array -> list
local pessoa = {nome = "Ana", idade = 30} -- usada como mapa  -> dict
local pilha = {}                          -- vazia, usada como array -> list
for i = 1, 3 do pilha[#pilha + 1] = i end

print(soma(2, 3), fatorial(5))
print(nums[1], #nums, pessoa.nome, pessoa.idade, #pilha)
