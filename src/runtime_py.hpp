#pragma once

// Runtime Python do transpilador: pequenas funções que o código gerado chama
// para reproduzir a semântica do Lua onde o Python é diferente (verdade só para
// nil/false, tabelas com base 1, retorno múltiplo, concatenação, laços...).
//
// O runtime é dividido em PARTES; o gerador só copia para o .py as partes que o
// programa realmente usa (e as dependências delas), na ordem desta lista.

#include <string>
#include <vector>

namespace lua2py {

struct RuntimePart {
    const char* name;  // nome principal definido pela parte (ex.: "lua_get")
    const char* deps;  // outras partes necessárias, separadas por espaço
    const char* code;  // código Python
};

const std::vector<RuntimePart>& runtimeParts();

// Fecha a lista de partes pedidas com as dependências e devolve o código Python,
// na ordem de definição de runtimeParts().
std::string runtimeCode(const std::vector<std::string>& wanted);

}  // namespace lua2py
