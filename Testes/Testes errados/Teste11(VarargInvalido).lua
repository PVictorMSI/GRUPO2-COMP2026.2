--teste de Erro Semântico: uso de vararg (...) fora de função vararg
--deve acusar erro semântico na linha 3

function soma(a, b)
    local v = ...
    return a + b
end
