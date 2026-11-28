local i = 0
repeat
    local x = i
    i = i + 1
    print("iteracao", i)
until x >= 2
-- deve printar "iteracao 1" depois "iteracao 2" depois "iteracao 3" em ambos.
-- print composto printa cada valor com um " " entre cada.