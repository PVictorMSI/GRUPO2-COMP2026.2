// Driver de teste da tabela de símbolos: parseia um .lua, resolve os nomes e imprime
// (1) a tabela de símbolos e (2) a qual símbolo cada identificador foi ligado.
//
// O main() do lexer_1.l é renomeado na compilação (-Dmain=lexer_main), então este
// arquivo pode ter o seu próprio main sem mexer no lexer.
#include <algorithm>
#include <cstdio>
#include <iostream>

#include "parser.tab.h"
#include "resolver.hpp"

extern FILE* yyin;
extern int yyparse();
extern lua2py::Block* root;

int main(int argc, char** argv) {
    if (argc < 2) { std::fprintf(stderr, "uso: %s arquivo.lua\n", argv[0]); return 2; }
    yyin = std::fopen(argv[1], "r");
    if (!yyin) { std::perror(argv[1]); return 2; }

    if (yyparse() != 0 || !root) return 1;

    lua2py::SemanticInfo info;
    lua2py::analyze(*root, info);

    info.table.dump(std::cout);

    std::vector<std::pair<const lua2py::Identifier*, lua2py::Symbol*>> uses(
        info.uses.begin(), info.uses.end());
    std::sort(uses.begin(), uses.end(), [](const auto& a, const auto& b) {
        if (a.first->line != b.first->line) return a.first->line < b.first->line;
        return a.second->id < b.second->id;
    });
    std::cout << "\nusos (linha aproximada):\n";
    for (const auto& [id, sym] : uses)
        std::printf("  linha %-3d %-8s -> #%d  python: %s\n", id->line, id->name.c_str(),
                    sym->id, sym->codegenName.c_str());

    delete root;
    return 0;
}
