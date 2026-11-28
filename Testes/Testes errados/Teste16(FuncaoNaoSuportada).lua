-- CE-SEM-08: funções padrão fora do subconjunto (#42)
-- deve gerar erro semântico na linha 4 ('setmetatable') e na linha 5 ('string.gsub')
local obj = {}
setmetatable(obj, {})
local s = string.gsub("abc", "b", "x")
print(s)
