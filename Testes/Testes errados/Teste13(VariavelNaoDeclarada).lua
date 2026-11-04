-- CE-SEM-06: Variável não declarada é LIDA (deve gerar erro semântico)
-- "z" nunca foi declarada com local nem recebeu atribuição antes
local resultado = z + 1
print(resultado)
