#pragma once

#include "ast.hpp"
#include <string>

namespace lua2py {

//contador global de erros semânticos detectados
extern int sem_errors;

//função centralizada para reportar qualquer erro semântico
void reportSemanticError(int line, const std::string& message);

//função principal que analisa a AST e reporta violações semânticas
void checkSemantics(const Block& root);

} // namespace lua2py
