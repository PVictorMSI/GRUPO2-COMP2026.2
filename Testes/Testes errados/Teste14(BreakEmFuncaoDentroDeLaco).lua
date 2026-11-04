-- Teste de Erro Semântico: break dentro de função declarada dentro de um laço
-- O laço de fora NÃO vale dentro da função: deve acusar erro semântico na linha 6

while true do
    local f = function()
        break
    end
    break
end
