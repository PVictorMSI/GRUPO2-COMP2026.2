#pragma once

// Geração de código Python a partir da AST (Frente D).
//
// Entrada: a AST já resolvida (analyze) e sem erros semânticos (checkSemantics).
// Saída:   um programa Python 3.8+ autossuficiente: só o pedaço do runtime
//          (src/runtime_py.cpp) que o programa usa, seguido do código traduzido.
//
// Issues cobertas aqui:
//   #30  laços de repetição (while, repeat, for numérico, for genérico)
//   #36  operadores Lua -> Python (aritméticos, relacionais, lógicos, .., #)
//   #39  retorno múltiplo de valores
//   #41  tabelas com base 1 (Lua) -> base 0 (Python)

#include <iosfwd>

#include "resolver.hpp"

namespace lua2py {

void generatePython(const Block& root, const SemanticInfo& info, std::ostream& out);

}  // namespace lua2py
