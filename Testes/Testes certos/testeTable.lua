X = {}
X[1] = "teste1"
X[2] = "teste2"
table.insert(X,"Aaaaaaa")
table.insert(X,"unsdiubsd")
print(X[1],X[2],X[3],X[4])
table.sort(X)
print(type(X))
for k,v in ipairs(X) do
    print(k,v)
end
table.remove(X)
print(table.concat(X))