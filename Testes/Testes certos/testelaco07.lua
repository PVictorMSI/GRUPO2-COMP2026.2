local n = 0
local function inc()
    n = n + 1
    return n
end
repeat
    print(inc())
until n >= 3
-- deve printar "1" depois "2" depois "3" em ambos.
-- também funciona para funções