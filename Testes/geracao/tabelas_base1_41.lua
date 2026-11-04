-- Issue #41: tabelas com base 1 (Lua) -> base 0 (Python)
local v = {10, 20, 30}
print(v[1], v[2], v[3], v[4])        -- t[4] não existe: nil
print(#v)

v[#v + 1] = 40                       -- acrescenta no fim
print(#v, v[4])
v[2] = 25
print(v[1], v[2], v[3], v[4])
v[#v] = nil                          -- remove o último
print(#v)

for i = 1, #v do io.write(v[i], " ") end
print()

-- tabela vazia usada como lista
local pilha = {}
for i = 1, 5 do pilha[#pilha + 1] = i * i end
print(#pilha, pilha[1], pilha[5])
table.insert(pilha, 36)
table.insert(pilha, 1, 0)
print(#pilha, pilha[1], pilha[7], table.concat(pilha, ","))
print(table.remove(pilha), table.remove(pilha, 1), #pilha)

-- tabela com chaves (dict): chaves numéricas continuam as do Lua
local d = {}
d.nome = "Ana"
d[1] = "um"
d["idade"] = 30
print(d.nome, d[1], d.idade, d.inexistente)
local cfg = {largura = 800, altura = 600, [3] = "tres"}
print(cfg.largura * cfg.altura, cfg[3])

-- misto: posições numeradas a partir de 1
local misto = {"a", "b", cor = "azul", "c"}
print(misto[1], misto[2], misto[3], misto.cor, #misto)

-- matriz
local m = {{1, 2}, {3, 4}}
print(m[1][1], m[1][2], m[2][1], m[2][2])
m[2][1] = 30
print(m[2][1])

-- índices calculados e float inteiro
local idx = 2
print(v[idx], v[idx + 1], v[1.0])

-- ipairs para no primeiro nil; pairs percorre tudo
local soma = 0
for _, x in ipairs({1, 2, 3}) do soma = soma + x end
print(soma)
local n = 0
for k, val in pairs(cfg) do n = n + 1 end
print(n)

-- unpack e sort
print(table.unpack({5, 6, 7}))
local nums = {5, 2, 8, 1}
table.sort(nums)
print(table.concat(nums, " "))
table.sort(nums, function(a, b) return a > b end)
print(table.concat(nums, " "))

-- lista que depois recebe índice fora de ordem e chave de texto: vira dict {1: ..}
local esparso = {10, 20, 30}
esparso[5] = 50
esparso.nome = "vetor"
print(esparso[1], esparso[4], esparso[5], esparso.nome)

-- tabela vazia preenchida a partir do índice 2 (crivo): não pode ser list
local marcado = {}
for i = 2, 6 do marcado[i] = i % 2 == 0 end
print(marcado[1], marcado[2], marcado[3], marcado[6])
