local x = nil
if not x then
    print("x e falso")
end
if not (x == nil) then
    print("nao")
else
    print("x e nil")
end
-- deve printar "x e falso" e "x e nil" em ambos.