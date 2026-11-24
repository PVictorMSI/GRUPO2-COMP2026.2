local function iter(s, ctrl)
    ctrl = ctrl + 1
    if ctrl <= s then
        return ctrl
    end
end
for x in iter, 3, 0 do
    print(x)
end