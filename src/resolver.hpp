#pragma once

// Resolução de nomes (análise semântica, RF06): percorre a AST e liga cada uso de
// variável ao Symbol correto, respeitando as regras de escopo do Lua.
//
// O resultado fica em tabelas LATERAIS (não altera o ast.hpp). Quem vier depois
// (IR / geração de código) consulta estas tabelas:
//   uses     : cada nó Identifier  -> Symbol que ele referencia
//   declared : nó que declara nomes (LocalDecl, NumericFor, GenericFor,
//              FunctionDecl/LocalFunctionDecl/FunctionExpr = parâmetros)
//              -> Symbols declarados, NA ORDEM em que aparecem no código.
//              Em "function a:m()" o primeiro elemento é o "self" implícito.
//   funcName : FunctionDecl / LocalFunctionDecl -> Symbol do nome da função
//              (em "function a.b.c()" é o Symbol de "a").

#include <unordered_map>
#include <vector>

#include "ast.hpp"
#include "symbol_table.hpp"

namespace lua2py {

struct SemanticInfo {
    SymbolTable table;
    std::unordered_map<const Identifier*, Symbol*> uses;
    std::unordered_map<const ASTNode*, std::vector<Symbol*>> declared;
    std::unordered_map<const ASTNode*, Symbol*> funcName;
};

// Resolve a AST inteira e, no fim, atribui os nomes Python (assignCodegenNames).
// "info" deve estar vazio (chame uma vez por programa).
void analyze(const Block& root, SemanticInfo& info);

}  // namespace lua2py
