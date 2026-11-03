-- Teste de Erro Semântico: break fora de laço
-- Deve acusar erro semântico na linha 6

local x = 10
if x > 5 then
    break
end
