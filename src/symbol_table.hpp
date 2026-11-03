#pragma once

// Tabela de símbolos com escopos (RF06).
//
// Ideia central: cada variável do programa Lua vira UM objeto Symbol, e dois
// Symbols diferentes nunca são "a mesma variável", mesmo que tenham o mesmo
// nome. É isso que faz "x global" e "x local de função" serem entidades
// distintas: eles têm ids diferentes e vivem em escopos diferentes.

#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace lua2py {

enum class SymbolKind {
    Global,  // variável global (nunca declarada; no Lua, vive na tabela _G)
    Local,   // declarada com "local", ou variável de laço "for"
    Param,   // parâmetro de função (inclui o "self" implícito de "function a:m()")
};

struct Symbol {
    int         id = 0;            // único no programa inteiro (1, 2, 3...)
    std::string name;              // nome como escrito no Lua
    std::string codegenName;       // nome a usar no Python (preenchido por assignCodegenNames)
    SymbolKind  kind = SymbolKind::Global;
    int         scopeDepth = 0;    // 0 = global, 1 = chunk principal, 2+ = blocos/funções aninhados
    int         functionId = 0;    // 0 = chunk principal; >0 = função que declarou o símbolo
    int         declLine = 0;      // linha aproximada da declaração
    bool        captured = false;  // usado dentro de OUTRA função além da que o declarou
    int         reads = 0;         // quantas vezes é lido
    int         writes = 0;        // quantas vezes recebe atribuição (útil para RF07)
};

class SymbolTable {
public:
    SymbolTable();

    // Abre um escopo de bloco (do, while, if, for...). Continua na mesma função.
    void enterScope();
    // Abre o escopo de uma função (parâmetros + corpo). Ganha um functionId novo.
    void enterFunction();
    // Fecha o escopo mais interno. Os Symbols continuam existindo (a IR ainda precisa deles).
    void exitScope();

    // Declara uma variável no escopo ATUAL. Se já houver uma com o mesmo nome neste
    // mesmo escopo, a nova a esconde dali em diante (comportamento do "local x" repetido).
    Symbol* declare(const std::string& name, SymbolKind kind, int line);

    // Procura o nome do escopo mais interno para o mais externo (inclui os globais).
    // Devolve nullptr se o nome nunca foi declarado nem usado como global.
    Symbol* lookup(const std::string& name) const;

    // Devolve o símbolo global com esse nome, criando-o se ainda não existir.
    Symbol* global(const std::string& name, int line);

    int currentFunctionId() const;
    int currentDepth() const;

    // Todos os símbolos já criados, em ordem de criação.
    const std::vector<std::unique_ptr<Symbol>>& symbols() const { return all_; }

    // Escolhe o nome Python de cada símbolo, evitando colisões (ver .cpp).
    // Chamar UMA vez, depois de resolver o programa inteiro.
    void assignCodegenNames();

    // Imprime a tabela (para depuração e testes).
    void dump(std::ostream& os) const;

private:
    struct Scope {
        std::unordered_map<std::string, Symbol*> names;
        int functionId;
    };

    Symbol* make(const std::string& name, SymbolKind kind, int depth, int functionId, int line);

    std::vector<Scope> stack_;                  // stack_[0] = escopo global
    std::vector<std::unique_ptr<Symbol>> all_;  // dona de todos os Symbols
    int nextFunctionId_ = 1;
};

}  // namespace lua2py
