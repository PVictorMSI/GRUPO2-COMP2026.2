local a = nil
local b = 5
if a and b > 3 then
    print("nao")
else
    print("and curto-circuitou")
end
-- deve printar "and curto-circuitou" em ambos.
-- nil já é falso, logo não tem pra que ver o valor de b