-- operações lógicas em ordem indevida--
a = 1
b = 2
c = 0

c = a + b
c = a b +
c = a - b
c = - a b
c = a * b
c = * b a
c = a / b
c = / b a
--[[ lógico entre não lógicos tem uma regra específica:
and: retorna o primeiro caso seja falso (ou nil), se não retorna o segundo
or: retorna o primeiro se não for falso, se não retorna o segundo
nil e falso são falsos, os outros são verdadeiros
]]--
c = a and b
c = and a b
c = a or b
c = a b or
c = not a
c = a not
