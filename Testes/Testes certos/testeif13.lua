local a = false
local b = 5
if a or b > 3 then
    print("or avaliou b")
end
-- deve printar "or avaliou b" em ambos.
-- a é falso, logo precisa ver se b não for falso ou nil para continuar