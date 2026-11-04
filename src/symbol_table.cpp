#include "symbol_table.hpp"

#include <cassert>
#include <iomanip>
#include <ostream>
#include <unordered_set>

namespace lua2py {

namespace {

// Palavras reservadas do Python: um identificador Lua com um desses nomes
// (ex.: "class", "def", "lambda") geraria Python inválido.
const char* const kPythonKeywords[] = {
    "False", "None", "True", "and", "as", "assert", "async", "await", "break",
    "class", "continue", "def", "del", "elif", "else", "except", "finally",
    "for", "from", "global", "if", "import", "in", "is", "lambda", "nonlocal",
    "not", "or", "pass", "raise", "return", "try", "while", "with", "yield",
};

// Nomes que o GERADOR DE CÓDIGO emite e que o código Python gerado usa diretamente:
// builtins do Python chamados pelo runtime (src/runtime_py.cpp) e pelo código gerado.
// Uma variável Lua com um desses nomes os esconderia, então ela é renomeada
// (locais: x_1, x_2...; globais: x_). Além desta lista, todo nome que começa com
// "lua_" é reservado ao runtime (ver isReservedName).
const char* const kRuntimeNames[] = {
    "LuaError", "LuaTable", "range", "len", "isinstance", "str", "int", "float",
    "list", "dict", "tuple", "enumerate", "print", "repr", "bool", "callable",
    "chr", "ord", "id", "max", "min", "abs", "iter", "super", "sorted", "type",
    "next", "Exception", "ValueError", "TypeError", "SystemExit", "RecursionError",
};

// Nome que não pode ser usado como está no Python gerado.
bool isReservedName(const std::string& s) {
    if (s.rfind("lua_", 0) == 0) return true;   // prefixo do runtime
    for (const char* n : kRuntimeNames)
        if (s == n) return true;
    return false;
}

bool isPythonKeyword(const std::string& s) {
    for (const char* k : kPythonKeywords)
        if (s == k) return true;
    return false;
}

}  // namespace

SymbolTable::SymbolTable() {
    stack_.push_back(Scope{{}, 0});  // escopo global (profundidade 0)
}

void SymbolTable::enterScope() {
    stack_.push_back(Scope{{}, stack_.back().functionId});
}

void SymbolTable::enterFunction() {
    stack_.push_back(Scope{{}, nextFunctionId_++});
}

void SymbolTable::exitScope() {
    assert(stack_.size() > 1 && "o escopo global nunca pode ser fechado");
    stack_.pop_back();
}

int SymbolTable::currentFunctionId() const { return stack_.back().functionId; }
int SymbolTable::currentDepth() const { return static_cast<int>(stack_.size()) - 1; }

Symbol* SymbolTable::make(const std::string& name, SymbolKind kind, int depth,
                          int functionId, int line) {
    auto sym = std::make_unique<Symbol>();
    sym->id = static_cast<int>(all_.size()) + 1;
    sym->name = name;
    sym->codegenName = name;
    sym->kind = kind;
    sym->scopeDepth = depth;
    sym->functionId = functionId;
    sym->declLine = line;
    Symbol* raw = sym.get();
    all_.push_back(std::move(sym));
    return raw;
}

Symbol* SymbolTable::declare(const std::string& name, SymbolKind kind, int line) {
    assert(kind != SymbolKind::Global && "globais são criadas por global()");
    Symbol* sym = make(name, kind, currentDepth(), currentFunctionId(), line);
    stack_.back().names[name] = sym;  // sobrescreve (esconde) um homônimo do mesmo escopo
    return sym;
}

Symbol* SymbolTable::lookup(const std::string& name) const {
    for (auto it = stack_.rbegin(); it != stack_.rend(); ++it) {
        auto found = it->names.find(name);
        if (found != it->names.end()) return found->second;
    }
    return nullptr;
}

Symbol* SymbolTable::global(const std::string& name, int line) {
    auto& globals = stack_.front().names;
    auto found = globals.find(name);
    if (found != globals.end()) return found->second;
    Symbol* sym = make(name, SymbolKind::Global, 0, 0, line);
    globals[name] = sym;
    return sym;
}

// Política de nomes (conservadora, mas sempre correta):
//  1. Globais mantêm o nome (só ganham "_" no fim se forem palavra reservada do Python).
//  2. Locais/parâmetros mantêm o nome se ele ainda estiver livre. Caso contrário
//     ganham sufixo _1, _2... Assim dois símbolos NUNCA dividem o mesmo nome Python,
//     o que elimina de uma vez o problema de escopo de bloco (Python não tem) e
//     o de uma local "esconder" uma global que a mesma função também usa.
// Custo: nomes como "i" repetidos em vários laços viram i, i_1, i_2... Uma política
// mais refinada (reutilizar o nome entre blocos irmãos) pode vir depois.
void SymbolTable::assignCodegenNames() {
    std::unordered_set<std::string> taken;
    for (const char* n : kRuntimeNames) taken.insert(n);

    // Passo 1: globais primeiro, para que nenhuma local "roube" o nome delas.
    // Uma global que colide com palavra reservada ou com nome do runtime ganha "_".
    for (auto& s : all_) {
        if (s->kind != SymbolKind::Global) continue;
        bool clash = isPythonKeyword(s->name) || isReservedName(s->name);
        s->codegenName = clash ? s->name + "_" : s->name;
        taken.insert(s->codegenName);
    }

    // Passo 2: locais e parâmetros, na ordem de criação.
    for (auto& s : all_) {
        if (s->kind == SymbolKind::Global) continue;
        bool lua = s->name.rfind("lua_", 0) == 0;
        std::string base = (isPythonKeyword(s->name) || lua) ? s->name + "_" : s->name;
        std::string candidate = base;
        for (int n = 1; taken.count(candidate); ++n)
            candidate = base + "_" + std::to_string(n);
        s->codegenName = candidate;
        taken.insert(candidate);
    }
}

void SymbolTable::dump(std::ostream& os) const {
    static const char* kindName[] = {"global", "local", "param"};
    os << std::left << std::setw(4) << "id" << std::setw(12) << "nome"
       << std::setw(8) << "tipo" << std::setw(7) << "funcao" << std::setw(7) << "prof."
       << std::setw(10) << "capturada" << std::setw(7) << "lido" << std::setw(9) << "escrito"
       << "python\n";
    for (const auto& s : all_) {
        os << std::setw(4) << s->id << std::setw(12) << s->name
           << std::setw(8) << kindName[static_cast<int>(s->kind)]
           << std::setw(7) << s->functionId << std::setw(7) << s->scopeDepth
           << std::setw(10) << (s->captured ? "sim" : "-")
           << std::setw(7) << s->reads << std::setw(9) << s->writes
           << s->codegenName << "\n";
    }
}

}  // namespace lua2py
