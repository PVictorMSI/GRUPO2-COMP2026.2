function funcao1()
    return("Hello world!")
end
function funcao2(var1,var2,var3)
    local resultado = var1*var2
    resultado = resultado + var3
    return(resultado)
end
function funcao3(var1)
    if var1 > 10 then
        print("teste")
    end
end
function funcao4(var1)
    local i = 0
        while i < var1 do
        print("iteração atual: ",i)
        i = i + 1
    end
end
print(funcao1(),funcao2(2,3,5))
funcao3(100)
funcao3(8)
funcao4(20)