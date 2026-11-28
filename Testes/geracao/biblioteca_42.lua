-- Issue #42: subconjunto da biblioteca padrão suportado pelo lua2py
-- (funções fora dele geram erro "não suportado"; ver Testes errados/Teste15 e 16)

-- básicas
print("ola", 1, 2.5, true, nil)
print(type(1), type("s"), type({}), type(print), type(nil))
print(tostring(10), tonumber("42"), tonumber("0x10"), tonumber("abc"))
print(select("#", 1, 2, 3), select(2, "a", "b", "c"))
print(pcall(function() error("falhou", 0) end))
print(assert(5))

-- math
print(math.floor(3.7), math.ceil(3.2), math.abs(-4), math.max(1, 9, 3), math.min(4, 2))
print(math.sqrt(16), math.huge > 1000, math.tointeger(8.0), math.type(1), math.type(1.5))

-- string (função e método)
local s = "Lua"
print(string.len(s), string.upper(s), s:lower(), s:rep(2, "-"), s:sub(2, 3))
print(string.format("%d-%s-%5.2f", 7, "x", 3.14159), s:byte(1), string.char(72, 105))
print(string.find("compiladores", "lado", 1, true))

-- table
local t = {3, 1, 2}
table.insert(t, 4)
table.sort(t)
print(table.concat(t, ","), #t, table.remove(t), #t)
print(table.unpack({"a", "b"}))

-- io
io.write("sem quebra ", 1, "\n")

-- pairs/ipairs/next
local cores = {"r", "g", "b"}
for i, c in ipairs(cores) do io.write(i, "=", c, " ") end
print()
local m = {x = 1}
for k, v in pairs(m) do print(k, v) end
print(next({}))
