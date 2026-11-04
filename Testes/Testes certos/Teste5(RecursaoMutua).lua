-- CE-SEM: funções globais que se chamam mutuamente (deve passar sem erro)
-- "impar" é lida dentro de "par" antes de ser definida no texto, mas sempre
-- recebe valor antes de ser chamada: não é variável não declarada (issue #27).
function par(n)
    if n == 0 then return true end
    return impar(n - 1)
end

function impar(n)
    if n == 0 then return false end
    return par(n - 1)
end

print(par(10), impar(7))
