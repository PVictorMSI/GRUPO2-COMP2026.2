local i = 1
while true do
    if i % 2 == 0 then
        print(i, "par")
    elseif i > 5 then
        break
    end
    i = i + 1
end
-- deve printar "2 par" depois "4 par" depois "6 par" em ambos.