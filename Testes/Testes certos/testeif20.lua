local a = {}
local b = {}
local c = a
if a == b then print("iguais") end
if a == c then print("mesma") end
-- deve printar "mesma" em ambos.
-- a e b são tabelas diferentes, enquanto c possui a tabela c
