-- Teste de Erro Semântico: parâmetro duplicado na declaração de função
-- Deve acusar erro semântico na linha 6

function calcular(x, y, x)
    return x + y
end
