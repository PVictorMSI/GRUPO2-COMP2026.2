local function fat(n)
  if n <= 1 then return 1 end
  return n * fat(n - 1)
end

local i = 100
for i = 1, 3 do
  print(i)
end
print(i)

local n = 0
repeat
  local parar = n > 2
  n = n + 1
until parar

function contador()
  local c = 0
  return function() c = c + 1; return c end
end

function obj.metodo(a)
  return self, a
end
function obj:outro(a)
  return self, a
end

local class = 1
def = 2
