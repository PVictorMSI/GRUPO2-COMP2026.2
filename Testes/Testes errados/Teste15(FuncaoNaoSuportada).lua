-- CE-SEM-07: função da biblioteca padrão FORA do subconjunto suportado (#42)
-- deve gerar erro semântico na linha 5 ('os.time' não é suportado)
local x = 10
print(x)                 -- print faz parte do subconjunto: ok
local t = os.time()
print(t)
