local function f(...)
    for i, v in ipairs({...}) do
        print(i, v)
    end
end
f(10, 20)