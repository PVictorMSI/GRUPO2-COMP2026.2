// Ponto de entrada do compilador Lua -> Python.
//
// Uso:  ./build/lua2py < programa.lua                  Python sai na saída padrão
//       ./build/lua2py programa.lua                    idem, lendo do arquivo
//       ./build/lua2py programa.lua -o programa.py     grava o Python no arquivo
//
// Etapas: léxico -> sintático (AST) -> resolução de escopos (RF06)
//         -> análise semântica (#27 variáveis não declaradas, #28 erros semânticos)
//         -> geração de código Python (codegen).
// Erros vão para a saída de erro (stderr); só o código Python vai para a saída padrão.
// Código de saída: 0 = ok, 1 = erro (léxico, sintático, semântico ou de arquivo).
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "codegen.hpp"
#include "parser.tab.h"
#include "resolver.hpp"
#include "semantic_checker.hpp"

extern FILE* yyin;
extern int yyparse();
extern lua2py::Block* root;   // definido em parser/parser.y
extern int lex_errors;        // definido em lexer/lexer.l (erros léxicos já reportados)

int main(int argc, char** argv) {
    const char* input = nullptr;
    const char* output = nullptr;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-o") == 0 && i + 1 < argc) output = argv[++i];
        else if (!input) input = argv[i];
        else {
            std::fprintf(stderr, "Uso: %s [entrada.lua] [-o saida.py]\n", argv[0]);
            return 1;
        }
    }

    if (input) {
        FILE* f = std::fopen(input, "r");
        if (!f) {
            std::fprintf(stderr, "Não foi possível abrir o arquivo: %s\n", input);
            return 1;
        }
        yyin = f;
    }  // sem argumento, yyin continua sendo stdin (redirecionamento com "<")

    if (yyparse() != 0 || !root) return 1;   // erro sintático: não há AST para analisar
    if (lex_errors > 0) {                    // erro léxico: o programa não deve compilar
        delete root;
        return 1;
    }

    // Resolução de escopos; também reporta leitura de variável não declarada (#27).
    lua2py::SemanticInfo info;
    lua2py::analyze(*root, info);

    // Demais verificações semânticas: break fora de laço, vararg inválido,
    // parâmetro duplicado (#28). Roda mesmo se a resolução já achou erros,
    // para o usuário ver todos os problemas de uma vez.
    lua2py::checkSemantics(*root);

    if (lua2py::sem_errors > 0) {            // erro semântico: o programa não deve compilar
        delete root;
        return 1;
    }

    std::ostringstream py;
    lua2py::generatePython(*root, info, py);
    delete root;   // quem consome a AST é dono dela (o parser nunca a libera)

    if (output) {
        std::ofstream out(output);
        if (!(out << py.str())) {
            std::fprintf(stderr, "Não foi possível gravar o arquivo: %s\n", output);
            return 1;
        }
    } else {
        std::cout << py.str();
    }
    return 0;
}
