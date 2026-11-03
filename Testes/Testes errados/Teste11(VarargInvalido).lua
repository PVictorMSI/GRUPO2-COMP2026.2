-- Teste de Erro Semântico: uso de vararg (...) fora de função vararg
-- Deve acusar erro semântico na linha 5

function soma(a, b)
    local v = ...
    return a + b
end
