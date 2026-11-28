#include "stdlib_subset.hpp"

#include <algorithm>
#include <set>
#include <utility>

#include "semantic_checker.hpp"

namespace lua2py {

// =====================================================================
// O subconjunto (mantenha em sincronia com src/runtime_py.cpp)
// =====================================================================

const std::vector<std::string>& supportedGlobalFunctions() {
    static const std::vector<std::string> fns = {
        "print", "type", "tostring", "tonumber", "error", "assert", "pcall",
        "pairs", "ipairs", "next", "select", "unpack",
    };
    return fns;
}

namespace {

const std::vector<std::pair<std::string, std::vector<std::string>>>& libraries() {
    static const std::vector<std::pair<std::string, std::vector<std::string>>> libs = {
        {"math", {"abs", "ceil", "floor", "max", "min", "sqrt", "exp", "log", "sin", "cos", "tan",
                  "fmod", "tointeger", "type", "random", "randomseed", "huge", "pi", "maxinteger",
                  "mininteger"}},
        {"string", {"len", "sub", "upper", "lower", "rep", "reverse", "byte", "char", "find", "format"}},
        {"table", {"insert", "remove", "concat", "unpack", "sort"}},
        {"io", {"write"}},
    };
    return libs;
}

// Globais do Lua 5.4 padrão que existem, mas o lua2py não traduz.
const std::set<std::string>& unsupportedNames() {
    static const std::set<std::string> names = {
        "xpcall", "rawget", "rawset", "rawequal", "rawlen", "setmetatable", "getmetatable",
        "require", "load", "loadfile", "dofile", "collectgarbage",
        "os", "coroutine", "package", "debug", "utf8",
        "_G", "_VERSION", "arg",
    };
    return names;
}

}  // namespace

bool isSupportedLibrary(const std::string& name) {
    for (const auto& l : libraries())
        if (l.first == name) return true;
    return false;
}

const std::vector<std::string>& supportedLibFields(const std::string& lib) {
    static const std::vector<std::string> none;
    for (const auto& l : libraries())
        if (l.first == lib) return l.second;
    return none;
}

bool isSupportedLibField(const std::string& lib, const std::string& field) {
    const auto& f = supportedLibFields(lib);
    return std::find(f.begin(), f.end(), field) != f.end();
}

bool isUnsupportedStdName(const std::string& name) { return unsupportedNames().count(name) > 0; }

// =====================================================================
// Verificação na AST
// =====================================================================

namespace {

void reportUnsupported(int line, const std::string& what) {
    reportSemanticError(line, "'" + what +
                                  "' não é suportado pelo lua2py (fora do subconjunto da biblioteca padrão)");
}

class StdlibChecker {
public:
    explicit StdlibChecker(const SemanticInfo& info) : info_(info) {}

    void run(const Block& root) {
        collectBlock(root);  // 1ª passada: campos que o próprio programa cria (function string.trim)
        block(root);         // 2ª passada: reporta o que está fora do subconjunto
    }

private:
    const SemanticInfo& info_;
    std::set<std::pair<std::string, std::string>> userFields_;  // {"string", "trim"}

    // Global padrão que o programa nunca reatribui (senão a função é do usuário).
    const Symbol* stdGlobal(const Expr& e) const {
        if (e.kind != NodeKind::Identifier) return nullptr;
        auto it = info_.uses.find(static_cast<const Identifier*>(&e));
        if (it == info_.uses.end()) return nullptr;
        const Symbol* s = it->second;
        return s && s->kind == SymbolKind::Global && s->writes == 0 ? s : nullptr;
    }

    // ---------- 1ª passada ----------

    void collectBlock(const Block& b) {
        for (const auto& s : b.statements)
            if (s) collectStmt(*s);
    }

    void collectStmt(const Stmt& s) {
        switch (s.kind) {
            case NodeKind::FunctionDecl: {
                const auto& f = static_cast<const FunctionDecl&>(s);
                auto dot = f.name.find_first_of(".:");
                if (dot != std::string::npos) {
                    std::string rest = f.name.substr(dot + 1);
                    userFields_.insert({f.name.substr(0, dot), rest.substr(0, rest.find_first_of(".:"))});
                }
                if (f.body) collectBlock(*f.body);
                break;
            }
            case NodeKind::Assign:
                for (const auto& t : static_cast<const Assign&>(s).targets) {
                    if (!t || t->kind != NodeKind::Index) continue;
                    const auto& i = static_cast<const Index&>(*t);
                    if (i.object && i.object->kind == NodeKind::Identifier && i.key &&
                        i.key->kind == NodeKind::StringLiteral)
                        userFields_.insert({static_cast<const Identifier&>(*i.object).name,
                                            static_cast<const StringLiteral&>(*i.key).value});
                }
                break;
            default:
                forEachChildBlock(s, [this](const Block& b) { collectBlock(b); });
                break;
        }
    }

    template <class F>
    void forEachChildBlock(const Stmt& s, F f) {
        switch (s.kind) {
            case NodeKind::Block: f(static_cast<const Block&>(s)); break;
            case NodeKind::While: if (auto& b = static_cast<const While&>(s).body) f(*b); break;
            case NodeKind::RepeatUntil: if (auto& b = static_cast<const RepeatUntil&>(s).body) f(*b); break;
            case NodeKind::NumericFor: if (auto& b = static_cast<const NumericFor&>(s).body) f(*b); break;
            case NodeKind::GenericFor: if (auto& b = static_cast<const GenericFor&>(s).body) f(*b); break;
            case NodeKind::Do: if (auto& b = static_cast<const Do&>(s).body) f(*b); break;
            case NodeKind::LocalFunctionDecl:
                if (auto& b = static_cast<const LocalFunctionDecl&>(s).body) f(*b);
                break;
            case NodeKind::If:
                for (const auto& c : static_cast<const If&>(s).clauses)
                    if (c.body) f(*c.body);
                break;
            default: break;
        }
    }

    // ---------- 2ª passada ----------

    void block(const Block& b) {
        for (const auto& s : b.statements)
            if (s) stmt(*s);
    }

    void exprs(const std::vector<ExprPtr>& es) {
        for (const auto& e : es)
            if (e) expr(*e);
    }

    void stmt(const Stmt& s) {
        switch (s.kind) {
            case NodeKind::Block: block(static_cast<const Block&>(s)); break;
            case NodeKind::Assign: {
                const auto& n = static_cast<const Assign&>(s);
                for (const auto& t : n.targets) {
                    if (!t) continue;
                    if (t->kind == NodeKind::Index) {  // string.trim = ... define, não usa
                        const auto& i = static_cast<const Index&>(*t);
                        if (i.object && !stdGlobal(*i.object)) expr(*i.object);
                        if (i.key) expr(*i.key);
                    }
                }
                exprs(n.values);
                break;
            }
            case NodeKind::LocalDecl: exprs(static_cast<const LocalDecl&>(s).values); break;
            case NodeKind::If:
                for (const auto& c : static_cast<const If&>(s).clauses) {
                    if (c.condition) expr(*c.condition);
                    if (c.body) block(*c.body);
                }
                break;
            case NodeKind::While: {
                const auto& n = static_cast<const While&>(s);
                if (n.condition) expr(*n.condition);
                if (n.body) block(*n.body);
                break;
            }
            case NodeKind::RepeatUntil: {
                const auto& n = static_cast<const RepeatUntil&>(s);
                if (n.body) block(*n.body);
                if (n.condition) expr(*n.condition);
                break;
            }
            case NodeKind::NumericFor: {
                const auto& n = static_cast<const NumericFor&>(s);
                if (n.start) expr(*n.start);
                if (n.stop) expr(*n.stop);
                if (n.step) expr(*n.step);
                if (n.body) block(*n.body);
                break;
            }
            case NodeKind::GenericFor: {
                const auto& n = static_cast<const GenericFor&>(s);
                exprs(n.exprs);
                if (n.body) block(*n.body);
                break;
            }
            case NodeKind::FunctionDecl:
                if (auto& b = static_cast<const FunctionDecl&>(s).body) block(*b);
                break;
            case NodeKind::LocalFunctionDecl:
                if (auto& b = static_cast<const LocalFunctionDecl&>(s).body) block(*b);
                break;
            case NodeKind::Return: exprs(static_cast<const Return&>(s).values); break;
            case NodeKind::Do:
                if (auto& b = static_cast<const Do&>(s).body) block(*b);
                break;
            case NodeKind::CallStmt:
                if (auto& c = static_cast<const CallStmt&>(s).call) expr(*c);
                break;
            default: break;
        }
    }

    void expr(const Expr& e) {
        switch (e.kind) {
            case NodeKind::Identifier:
                if (const Symbol* s = stdGlobal(e))
                    if (isUnsupportedStdName(s->name)) reportUnsupported(e.line, s->name);
                break;
            case NodeKind::Index: {
                const auto& i = static_cast<const Index&>(e);
                const Symbol* lib = i.object ? stdGlobal(*i.object) : nullptr;
                if (lib && i.key && i.key->kind == NodeKind::StringLiteral) {
                    // math.floor, os.time: reporta o nome completo uma única vez
                    const std::string& field = static_cast<const StringLiteral&>(*i.key).value;
                    bool known = isSupportedLibrary(lib->name) || isUnsupportedStdName(lib->name);
                    if (known && !isSupportedLibField(lib->name, field) && !userFields_.count({lib->name, field}))
                        reportUnsupported(e.line, lib->name + "." + field);
                    return;
                }
                if (i.object) expr(*i.object);
                if (i.key) expr(*i.key);
                break;
            }
            case NodeKind::FunctionCall: {
                const auto& c = static_cast<const FunctionCall&>(e);
                // ("abc"):gsub(...) — método de string literal: dá para saber já na compilação
                if (c.isMethodCall && c.callee && c.callee->kind == NodeKind::StringLiteral &&
                    !isSupportedLibField("string", c.methodName) && !userFields_.count({"string", c.methodName}))
                    reportUnsupported(e.line, "string." + c.methodName);
                if (c.callee) expr(*c.callee);
                exprs(c.args);
                break;
            }
            case NodeKind::BinaryOp: {
                const auto& b = static_cast<const BinaryOp&>(e);
                if (b.left) expr(*b.left);
                if (b.right) expr(*b.right);
                break;
            }
            case NodeKind::UnaryOp:
                if (auto& o = static_cast<const UnaryOp&>(e).operand) expr(*o);
                break;
            case NodeKind::TableConstructor:
                for (const auto& f : static_cast<const TableConstructor&>(e).fields) {
                    if (f.key) expr(*f.key);
                    if (f.value) expr(*f.value);
                }
                break;
            case NodeKind::FunctionExpr: {
                const auto& f = static_cast<const FunctionExpr&>(e);
                if (f.body) {
                    collectBlock(*f.body);
                    block(*f.body);
                }
                break;
            }
            default: break;
        }
    }
};

}  // namespace

void checkStdlibSubset(const Block& root, const SemanticInfo& info) { StdlibChecker(info).run(root); }

}  // namespace lua2py
