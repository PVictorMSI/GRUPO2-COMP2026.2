# Representação intermediária e geração

## O que foi planejado

As issues [#33](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/33), [#34](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/34) e [#35](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/issues/35) especificam uma representação intermediária própria, com decomposição de expressões em três endereços, preservação de blocos e geração Python independente da AST. Exemplo previsto:

```text
Lua: a + b * c
RI:  t1 = b * c
     t2 = a + t1
```

## O que está implementado

No commit `1c5e2e3`, `generatePython(const Block& root, const SemanticInfo& info, std::ostream& out)` recebe diretamente a AST e as associações de símbolos. `main.cpp` chama `analyze`, `checkSemantics`, `checkStdlibSubset` e então `generatePython`. Não há passagem para uma estrutura autônoma de instruções de três endereços.

A AST é uma representação interna estruturada do programa, mas isso não satisfaz automaticamente o critério de uma RI separada descrito nas issues. Temporários usados pelo gerador para preservar a avaliação também não demonstram, por si só, uma etapa independente de RI.

## Matriz de aderência

| Issue | Estado no GitHub | Evidência no código |
| --- | --- | --- |
| #33 | Fechada | Não localizado módulo de instruções de três endereços |
| #34 | Fechada | Blocos preservados na AST; RI autônoma não localizada |
| #35 | Fechada | Gerador acessa a AST diretamente, divergindo do critério |
| #47 | Fechada | Não há estrutura de RI separada para avaliar sua memória isoladamente |

## Otimizações observáveis

O gerador contém pré-análises e escolhas locais: nomes únicos, identificação de retornos, classificação de tabelas, uso de operadores Python quando apropriado e seleção de helpers necessários. Esses mecanismos não constituem evidência de um otimizador independente sobre código de três endereços. A semana de otimização do plano de ensino é registrada como atividade curricular, sem atribuir à equipe uma etapa inexistente.

## Consequência para a apresentação

A equipe deve apresentar a arquitetura efetivamente entregue e esclarecer a conciliação com os critérios das issues. Não há evidência de uma decisão formal aprovada para substituir a RI; a documentação não presume essa aprovação.

Fontes: [src/main.cpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/main.cpp), [src/codegen.hpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/codegen.hpp), [src/codegen.cpp](https://github.com/PVictorMSI/GRUPO2-COMP2026.2/blob/1c5e2e3bcb7de59f86bffb9cf0c527081a313a43/src/codegen.cpp).
