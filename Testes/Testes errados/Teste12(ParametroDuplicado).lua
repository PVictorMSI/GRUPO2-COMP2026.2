--teste de Erro Semântico: parâmetro duplicado na declaração de função
--deve acusar erro semântico na linha 2

function calcular(x, y, x)
    return x + y
end
