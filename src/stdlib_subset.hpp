#pragma once

// Subconjunto da biblioteca padrão do Lua suportado pelo lua2py (#42).
//
// Este arquivo é a ÚNICA definição do subconjunto: o que está aqui tem tradução
// no runtime Python (src/runtime_py.cpp); qualquer outra função ou biblioteca
// padrão do Lua usada pelo programa gera, na compilação, o erro
//
//   Erro semântico na linha N: 'os.time' não é suportado pelo lua2py (fora do
//   subconjunto da biblioteca padrão)
//
// Usos que só se conhecem na execução (s:gsub(...), local m = math; m.foo())
// dão o mesmo tipo de erro pelo runtime (LuaLib em src/runtime_py.cpp).

#include <string>
#include <vector>

#include "ast.hpp"
#include "resolver.hpp"

namespace lua2py {

// Funções globais suportadas (print, type, pairs...), sem as bibliotecas.
const std::vector<std::string>& supportedGlobalFunctions();

// Bibliotecas suportadas (math, string, table, io).
bool isSupportedLibrary(const std::string& name);

// Campos suportados de uma biblioteca (vazio se a biblioteca não é suportada).
const std::vector<std::string>& supportedLibFields(const std::string& lib);

// "math.floor" está no subconjunto?
bool isSupportedLibField(const std::string& lib, const std::string& field);

// Nomes globais do Lua padrão que o lua2py NÃO traduz (setmetatable, os, _G...).
bool isUnsupportedStdName(const std::string& name);

// Percorre a AST e reporta (reportSemanticError) cada uso de função ou
// biblioteca padrão fora do subconjunto. Globais que o próprio programa define
// (function setmetatable() ... end) não são da biblioteca padrão e passam.
void checkStdlibSubset(const Block& root, const SemanticInfo& info);

}  // namespace lua2py
