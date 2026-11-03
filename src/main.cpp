// Ponto de entrada do compilador Lua -> Python.
//
// Uso:  ./build/lua2py < programa.lua        (como em executar_testes.md)
//       ./build/lua2py programa.lua          (também aceita o nome do arquivo)
//
// Etapas já ligadas: léxico -> sintático (AST) -> resolução de escopos (RF06) -> análise semântica (RF07/Issue #28).
// Código de saída: 0 = ok, 1 = erro (léxico, sintático, semântico ou arquivo ilegível).
#include <cstdio>

#include "parser.tab.h"
#include "resolver.hpp"
#include "semantic_checker.hpp"

extern FILE* yyin;
extern int yyparse();
extern lua2py::Block* root;   // definido em parser/parser.y
extern int lex_errors;        // definido em lexer/lexer.l (erros léxicos já reportados)

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* f = std::fopen(argv[1], "r");
        if (!f) {
            std::fprintf(stderr, "Não foi possível abrir o arquivo: %s\n", argv[1]);
            return 1;
        }
        yyin = f;
    }  // sem argumento, yyin continua sendo stdin (redirecionamento com "<")

    if (yyparse() != 0 || !root) return 1;   // erro sintático: não há AST para analisar
    if (lex_errors > 0) {                    // erro léxico: o programa não deve compilar
        delete root;
        return 1;
    }

    lua2py::SemanticInfo info;
    lua2py::analyze(*root, info);

    // Análise e reporte de erros semânticos 
    lua2py::checkSemantics(*root);

    if (lua2py::sem_errors > 0) {            // erro semântico: encerra com código 1
        delete root;
        return 1;
    }

    // próximo passo: gerar a IR e o código Python a partir de root + info

    delete root;   // quem consome a AST é dono dela (o parser nunca a libera)
    return 0;
}
