#include "codegen.hpp"

#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "runtime_py.hpp"

namespace lua2py {

namespace {

// =====================================================================
// Funções do Lua que o gerador traduz para o runtime
// =====================================================================

struct Builtin {
    const char* lua;     // nome global no Lua
    const char* py;      // nome no Python gerado
    const char* part;    // parte do runtime que define o nome
    bool single;         // devolve sempre um único valor (#39)
};

const Builtin kBuiltins[] = {
    {"print", "lua_print", "lua_print", true},
    {"type", "lua_type", "lua_type", true},
    {"tostring", "lua_tostring", "lua_tostring", true},
    {"tonumber", "lua_tonumber", "lua_tonumber", true},
    {"error", "lua_error", "lua_error", true},
    {"pairs", "lua_pairs", "lua_pairs", false},
    {"ipairs", "lua_ipairs", "lua_ipairs", false},
    {"next", "lua_next", "lua_next", false},
    {"select", "lua_select", "lua_select", false},
    {"assert", "lua_assert", "lua_assert", false},
    {"pcall", "lua_pcall", "lua_pcall", false},
    {"unpack", "lua_unpack", "lua_table", false},
    {"math", "lua_math", "lua_math", true},
    {"string", "lua_string", "lua_string", true},
    {"table", "lua_table", "lua_table", true},
    {"io", "lua_io", "lua_io", true},
};

// Global do Lua padrão que o programa nunca reatribui: usa a versão do runtime.
const Builtin* builtinFor(const Symbol* s) {
    if (!s || s->kind != SymbolKind::Global || s->writes > 0) return nullptr;
    for (const auto& b : kBuiltins)
        if (s->name == b.lua) return &b;
    return nullptr;
}

bool isLibrary(const Builtin* b) {
    if (!b) return false;
    std::string n = b->lua;
    return n == "math" || n == "string" || n == "table" || n == "io";
}

// Funções de biblioteca que devolvem VÁRIOS valores; as demais devolvem um só.
bool libFieldIsMulti(const std::string& lib, const std::string& field) {
    if (lib == "string") return field == "byte" || field == "find";
    if (lib == "table") return field == "unpack";
    return false;
}

// =====================================================================
// Utilidades de texto
// =====================================================================

// Literal de string Python com o mesmo conteúdo (bytes UTF-8 válidos ficam como estão).
std::string pyString(const std::string& s) {
    std::string out = "\"";
    char buf[8];
    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        switch (c) {
        case '\\': out += "\\\\"; continue;
        case '"': out += "\\\""; continue;
        case '\n': out += "\\n"; continue;
        case '\r': out += "\\r"; continue;
        case '\t': out += "\\t"; continue;
        default: break;
        }
        if (c < 0x20 || c == 0x7f) {
            std::snprintf(buf, sizeof buf, "\\x%02x", c);
            out += buf;
            continue;
        }
        if (c < 0x80) {
            out += static_cast<char>(c);
            continue;
        }
        // sequência UTF-8: copia inteira se for válida, senão escapa o byte
        size_t len = (c >= 0xF0 && c < 0xF5) ? 4 : (c >= 0xE0) ? 3 : (c >= 0xC2 && c < 0xE0) ? 2 : 0;
        bool ok = len > 0 && i + len <= s.size();
        for (size_t k = 1; ok && k < len; ++k)
            ok = (static_cast<unsigned char>(s[i + k]) & 0xC0) == 0x80;
        if (ok) {
            out += s.substr(i, len);
            i += len - 1;
        } else {
            std::snprintf(buf, sizeof buf, "\\x%02x", c);
            out += buf;
        }
    }
    return out + "\"";
}

std::string pyNumber(const NumberLiteral& n) {
    if (n.isInteger) return std::to_string(static_cast<long long>(n.value));
    char buf[40];
    for (int prec = 15; prec <= 17; ++prec) {  // menor representação que volta ao mesmo double
        std::snprintf(buf, sizeof buf, "%.*g", prec, n.value);
        if (std::strtod(buf, nullptr) == n.value) break;
    }
    std::string s = buf;
    if (s.find_first_of(".eEn") == std::string::npos) s += ".0";
    return s;
}

std::string join(const std::vector<std::string>& v, const char* sep) {
    std::string out;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) out += sep;
        out += v[i];
    }
    return out;
}

// =====================================================================
// Expressões Python com precedência (para só pôr parênteses onde precisa)
// =====================================================================

enum Prec {
    P_COND = 1,   // a if c else b
    P_OR,         // or
    P_AND,        // and
    P_NOT,        // not
    P_CMP,        // < <= > >= == != is
    P_ADD,        // + -
    P_MUL,        // * / // %
    P_UNARY,      // -x
    P_POW,        // **
    P_ATOM,       // nomes, literais, chamadas, (...)
};

struct PyExpr {
    std::string code;
    int prec;
};

std::string wrap(const PyExpr& e, int minPrec) {
    return e.prec < minPrec ? "(" + e.code + ")" : e.code;
}

bool isIntLiteral(const Expr* e, long long& out) {
    if (!e) return false;
    if (e->kind == NodeKind::NumberLiteral) {
        const auto& n = static_cast<const NumberLiteral&>(*e);
        if (!n.isInteger) return false;
        out = static_cast<long long>(n.value);
        return true;
    }
    if (e->kind == NodeKind::UnaryOp) {
        const auto& u = static_cast<const UnaryOp&>(*e);
        if (u.op == UnOp::Neg && isIntLiteral(u.operand.get(), out)) {
            out = -out;
            return true;
        }
    }
    return false;
}

bool isComparison(BinOp op) {
    return op == BinOp::Eq || op == BinOp::Ne || op == BinOp::Lt || op == BinOp::Le ||
           op == BinOp::Gt || op == BinOp::Ge;
}

// Expressão cujo valor é sempre true/false: nela "and/or/not" do Python já
// se comportam exatamente como os do Lua.
bool isBoolean(const Expr* e) {
    if (!e) return false;
    if (e->kind == NodeKind::BoolLiteral) return true;
    if (e->kind == NodeKind::UnaryOp) return static_cast<const UnaryOp*>(e)->op == UnOp::Not;
    if (e->kind == NodeKind::BinaryOp) {
        const auto* b = static_cast<const BinaryOp*>(e);
        if (isComparison(b->op)) return true;
        if (b->op == BinOp::And || b->op == BinOp::Or)
            return isBoolean(b->left.get()) && isBoolean(b->right.get());
    }
    return false;
}

// Pode ser avaliada duas vezes sem custo nem efeito colateral.
bool isSimple(const Expr* e) {
    switch (e->kind) {
    case NodeKind::Identifier: case NodeKind::NilLiteral: case NodeKind::BoolLiteral:
    case NodeKind::NumberLiteral: case NodeKind::StringLiteral:
        return true;
    default:
        return false;
    }
}

bool hasCall(const Expr* e) {
    if (!e) return false;
    switch (e->kind) {
    case NodeKind::FunctionCall: return true;
    case NodeKind::BinaryOp: {
        const auto* b = static_cast<const BinaryOp*>(e);
        return hasCall(b->left.get()) || hasCall(b->right.get());
    }
    case NodeKind::UnaryOp: return hasCall(static_cast<const UnaryOp*>(e)->operand.get());
    case NodeKind::Index: {
        const auto* i = static_cast<const Index*>(e);
        return hasCall(i->object.get()) || hasCall(i->key.get());
    }
    case NodeKind::TableConstructor:
        for (const auto& f : static_cast<const TableConstructor*>(e)->fields)
            if (hasCall(f.key.get()) || hasCall(f.value.get())) return true;
        return false;
    default:
        return false;
    }
}

// Valor fixo, sem variáveis nem chamadas (o Python avisa "x is 1" com literais).
bool isConstant(const Expr* e) {
    if (!e) return false;
    switch (e->kind) {
    case NodeKind::NilLiteral: case NodeKind::BoolLiteral: case NodeKind::NumberLiteral:
    case NodeKind::StringLiteral:
        return true;
    case NodeKind::UnaryOp: return isConstant(static_cast<const UnaryOp*>(e)->operand.get());
    case NodeKind::BinaryOp: {
        const auto* b = static_cast<const BinaryOp*>(e);
        return isConstant(b->left.get()) && isConstant(b->right.get());
    }
    default:
        return false;
    }
}

// Literal numérico diferente de zero (inteiro, se onlyInt).
bool isNonZeroLiteral(const Expr* e, bool onlyInt) {
    long long n;
    if (isIntLiteral(e, n)) return n != 0;
    if (onlyInt || !e || e->kind != NodeKind::NumberLiteral) return false;
    return static_cast<const NumberLiteral*>(e)->value != 0.0;
}

// Construtor só com posições ({} ou {a, b, c}), ou nullptr.
const TableConstructor* asPositionalTable(const Expr* e) {
    if (!e || e->kind != NodeKind::TableConstructor) return nullptr;
    const auto* t = static_cast<const TableConstructor*>(e);
    for (const auto& f : t->fields)
        if (f.key) return nullptr;
    return t;
}

// =====================================================================
// Pré-análise: informações que o gerador precisa saber ANTES de escrever
// =====================================================================

struct ById {
    bool operator()(const Symbol* a, const Symbol* b) const { return a->id < b->id; }
};
using SymSet = std::set<const Symbol*, ById>;

struct FnInfo {
    const ASTNode* parent = nullptr;  // função que contém esta (nullptr = chunk principal)
    SymSet outerWrites;               // variáveis de fora escritas aqui -> global/nonlocal
    SymSet used;                      // variáveis usadas aqui ou em funções internas
    bool multi = false;               // algum "return" devolve 0 ou 2+ valores (#39)
    bool hasValueReturn = false;      // algum "return x" (senão a função não devolve nada)
    bool endsWithReturn = false;      // último comando do corpo é "return"
    bool needsExtra = true;           // pode ser chamada com argumentos a mais
    int fixedCount = -1;              // todo "return" devolve exatamente esta quantidade (<0: varia)
    bool sawReturn = false;
    std::vector<const FunctionCall*> tailCalls;  // "return ..., f(x)": depende de f
};

class Analysis {
public:
    explicit Analysis(const SemanticInfo& info) : info_(info) {}

    std::unordered_map<const ASTNode*, FnInfo> fns;          // chave nullptr = chunk
    std::unordered_map<const Symbol*, const ASTNode*> owner;  // local/param -> função dona
    std::unordered_set<const Symbol*> loopFresh;             // declarado dentro de laço
    SymSet predeclare;                                       // globais que nascem nil
    std::unordered_set<const TableConstructor*> listTables;  // {} que vira list
    std::unordered_set<const TableConstructor*> dictTables;  // {a, b} que precisa virar dict
    std::unordered_map<const Symbol*, const ASTNode*> boundFn;
    std::unordered_set<const ASTNode*> singleFns;            // devolvem exatamente 1 valor
    std::unordered_set<const ASTNode*> voidFns;              // não devolvem nenhum valor

    void run(const Block& root) {
        fns[nullptr];
        for (const auto& s : root.statements) {
            rootStmt_ = true;
            if (s) stmt(*s);
        }
        resolveTables();
        resolveSingle();
        resolveExtraArgs();
    }

    const Symbol* sym(const Identifier& id) const { return info_.uses.at(&id); }

    // Função conhecida chamada diretamente pelo nome (ou nullptr).
    const ASTNode* calledFn(const FunctionCall& c) const {
        if (c.isMethodCall || !c.callee || c.callee->kind != NodeKind::Identifier) return nullptr;
        auto it = boundFn.find(sym(static_cast<const Identifier&>(*c.callee)));
        return it == boundFn.end() ? nullptr : it->second;
    }

    // A função chamada sempre devolve um único valor (ou nenhum, que no Python
    // já é None = nil), sem contar parênteses em volta da chamada (#39).
    bool calleeSingle(const FunctionCall& c) const {
        if (c.isMethodCall || !c.callee) return false;
        if (c.callee->kind == NodeKind::Identifier) {
            if (const Builtin* b = builtinFor(sym(static_cast<const Identifier&>(*c.callee))))
                return b->single;
            const ASTNode* f = calledFn(c);
            return f && (singleFns.count(f) || voidFns.count(f));
        }
        std::string lib, field;
        if (libField(*c.callee, lib, field)) return !libFieldIsMulti(lib, field);
        return false;
    }

    // O VALOR da chamada é um só: função de um valor, ou "(f(x))" entre parênteses.
    bool callSingle(const FunctionCall& c) const { return c.parenthesized || calleeSingle(c); }

    // Quantos valores a chamada devolve, se for sempre o mesmo (senão -1).
    int callCount(const FunctionCall& c) const {
        const ASTNode* f = calledFn(c);
        if (c.parenthesized || !f) return -1;
        const FnInfo& fi = fns.at(f);
        return fi.endsWithReturn && fi.fixedCount >= 2 ? fi.fixedCount : -1;
    }

    // Chamada de função conhecida que não devolve nada (expandida, dá zero valores).
    bool callVoid(const FunctionCall& c) const {
        const ASTNode* f = calledFn(c);
        return !c.parenthesized && f && voidFns.count(f);
    }

    // "math.floor" etc.: índice de uma biblioteca padrão com chave fixa.
    bool libField(const Expr& e, std::string& lib, std::string& field) const {
        if (e.kind != NodeKind::Index) return false;
        const auto& i = static_cast<const Index&>(e);
        if (!i.object || i.object->kind != NodeKind::Identifier || !i.key ||
            i.key->kind != NodeKind::StringLiteral)
            return false;
        const Builtin* b = builtinFor(sym(static_cast<const Identifier&>(*i.object)));
        if (!isLibrary(b)) return false;
        lib = b->lua;
        field = static_cast<const StringLiteral&>(*i.key).value;
        return true;
    }

    bool isInside(const ASTNode* fn, const ASTNode* ancestor) const {
        for (; fn; fn = fns.at(fn).parent)
            if (fn == ancestor) return true;
        return false;
    }

private:
    const SemanticInfo& info_;
    const ASTNode* cur_ = nullptr;
    int loops_ = 0;
    bool rootStmt_ = false;
    std::unordered_set<const Symbol*> seenGlobal_;
    std::unordered_map<const Symbol*, const TableConstructor*> tableVars_;
    std::unordered_set<const Symbol*> arrayUse_, recordUse_, numericVars_, sequentialVars_;
    std::unordered_map<const Symbol*, int> reads_;          // leituras de cada variável
    std::unordered_map<const Symbol*, std::vector<std::pair<size_t, bool>>> directCalls_;

    // ---------- uso de variáveis ----------

    void use(const Symbol* s, bool write) {
        if (s->kind == SymbolKind::Global && s->writes > 0 && seenGlobal_.insert(s).second) {
            // Uma global nasce nil no Lua; no Python ela precisa existir antes de ser
            // lida. Só dispensa a pré-declaração quando o primeiro uso é uma
            // atribuição direto no chunk principal.
            if (!(write && rootStmt_ && cur_ == nullptr)) predeclare.insert(s);
        }
        if (!write) ++reads_[s];
        for (const ASTNode* f = cur_; f; f = fns[f].parent) fns[f].used.insert(s);
        if (write && cur_) {
            auto it = owner.find(s);
            if (it == owner.end() || it->second != cur_) fns[cur_].outerWrites.insert(s);
        }
    }

    void declare(const Symbol* s) {
        owner[s] = cur_;
        if (loops_ > 0) loopFresh.insert(s);
    }

    // ---------- comandos ----------

    void block(const Block* b) {
        if (!b) return;
        bool saved = rootStmt_;
        rootStmt_ = false;
        for (const auto& s : b->statements)
            if (s) stmt(*s);
        rootStmt_ = saved;
    }

    void loopBody(const Block* b) {
        ++loops_;
        block(b);
        --loops_;
    }

    void stmt(const Stmt& s) {
        switch (s.kind) {
        case NodeKind::Block: block(static_cast<const Block*>(&s)); break;

        case NodeKind::Assign: {
            const auto& n = static_cast<const Assign&>(s);
            for (const auto& v : n.values) expr(v.get());
            for (size_t i = 0; i < n.targets.size(); ++i) {
                const Expr* t = n.targets[i].get();
                if (t->kind == NodeKind::Index) {  // t[k] = v
                    const auto* ix = static_cast<const Index*>(t);
                    expr(ix->object.get());
                    expr(ix->key.get());
                    markWrite(ix->object.get(), ix->key.get());
                    continue;
                }
                if (t->kind != NodeKind::Identifier) { expr(t); continue; }
                const Symbol* sy = sym(static_cast<const Identifier&>(*t));
                use(sy, true);
                if (n.targets.size() == n.values.size()) bindValue(sy, n.values[i].get(), 1);
            }
            break;
        }

        case NodeKind::LocalDecl: {
            const auto& n = static_cast<const LocalDecl&>(s);
            for (const auto& v : n.values) expr(v.get());
            const auto& syms = info_.declared.at(&n);
            for (size_t i = 0; i < syms.size(); ++i) {
                declare(syms[i]);
                if (i < n.values.size()) bindValue(syms[i], n.values[i].get(), 0);
            }
            break;
        }

        case NodeKind::If:
            for (const auto& c : static_cast<const If&>(s).clauses) {
                expr(c.condition.get());
                block(c.body.get());
            }
            break;

        case NodeKind::While: {
            const auto& n = static_cast<const While&>(s);
            expr(n.condition.get());
            loopBody(n.body.get());
            break;
        }

        case NodeKind::RepeatUntil: {
            const auto& n = static_cast<const RepeatUntil&>(s);
            ++loops_;
            block(n.body.get());
            expr(n.condition.get());
            --loops_;
            break;
        }

        case NodeKind::NumericFor: {
            const auto& n = static_cast<const NumericFor&>(s);
            expr(n.start.get());
            expr(n.stop.get());
            expr(n.step.get());
            ++loops_;
            const Symbol* v = info_.declared.at(&n)[0];
            declare(v);
            numericVars_.insert(v);
            long long st = 1, ini = 0;
            if (isIntLiteral(n.start.get(), ini) && ini == 1 && (!n.step || (isIntLiteral(n.step.get(), st) && st == 1)))
                sequentialVars_.insert(v);  // for i = 1, n: t[i] = v preenche em ordem
            block(n.body.get());
            --loops_;
            break;
        }

        case NodeKind::GenericFor: {
            const auto& n = static_cast<const GenericFor&>(s);
            for (const auto& e : n.exprs) expr(e.get());
            ++loops_;
            const auto& syms = info_.declared.at(&n);
            for (const Symbol* v : syms) declare(v);
            if (!syms.empty() && n.exprs.size() == 1 && isBuiltinCall(n.exprs[0].get(), "ipairs"))
            {
                numericVars_.insert(syms[0]);  // "for i, v in ipairs(t)": i é índice 1, 2, 3...
                sequentialVars_.insert(syms[0]);
            }
            block(n.body.get());
            --loops_;
            break;
        }

        case NodeKind::FunctionDecl: {
            const auto& n = static_cast<const FunctionDecl&>(s);
            const Symbol* base = info_.funcName.at(&n);
            bool simple = n.name.find_first_of(".:") == std::string::npos;
            use(base, simple);
            if (simple && base->writes == 1) boundFn[base] = &n;
            function(n, n.body.get());
            break;
        }

        case NodeKind::LocalFunctionDecl: {
            const auto& n = static_cast<const LocalFunctionDecl&>(s);
            const Symbol* name = info_.funcName.at(&n);
            declare(name);
            if (name->writes == 0) boundFn[name] = &n;
            function(n, n.body.get());
            break;
        }

        case NodeKind::Return: {
            const auto& n = static_cast<const Return&>(s);
            for (const auto& v : n.values) expr(v.get());
            if (cur_) {
                FnInfo& f = fns[cur_];
                if (n.values.empty()) {
                    f.multi = true;  // "return" sem valor: zero valores
                    f.fixedCount = -2;
                    f.sawReturn = true;
                } else {
                    f.hasValueReturn = true;
                    const Expr* last = n.values.back().get();
                    bool exact = last->kind != NodeKind::FunctionCall && last->kind != NodeKind::Vararg;
                    int count = exact ? static_cast<int>(n.values.size()) : -2;
                    if (f.fixedCount == -1 && !f.sawReturn) f.fixedCount = count;
                    else if (f.fixedCount != count) f.fixedCount = -2;
                    f.sawReturn = true;
                    if (n.values.size() > 1 || last->kind == NodeKind::Vararg)
                        f.multi = true;
                    else if (last->kind == NodeKind::FunctionCall)
                        f.tailCalls.push_back(static_cast<const FunctionCall*>(last));
                }
            }
            break;
        }

        case NodeKind::Do: block(static_cast<const Do&>(s).body.get()); break;
        case NodeKind::CallStmt: expr(static_cast<const CallStmt&>(s).call.get()); break;
        default: break;
        }
    }

    // ---------- funções ----------

    void function(const ASTNode& node, const Block* body) {
        FnInfo& f = fns[&node];
        f.parent = cur_;
        const ASTNode* savedFn = cur_;
        int savedLoops = loops_;
        cur_ = &node;
        loops_ = 0;  // um laço de fora não vale dentro da função
        for (const Symbol* p : info_.declared.at(&node)) declare(p);
        block(body);
        fns[&node].endsWithReturn = body && !body->statements.empty() && body->statements.back() &&
                                    body->statements.back()->kind == NodeKind::Return;
        cur_ = savedFn;
        loops_ = savedLoops;
    }

    // ---------- expressões ----------

    void expr(const Expr* e) {
        if (!e) return;
        switch (e->kind) {
        case NodeKind::Identifier: use(sym(static_cast<const Identifier&>(*e)), false); break;

        case NodeKind::BinaryOp: {
            const auto* b = static_cast<const BinaryOp*>(e);
            expr(b->left.get());
            expr(b->right.get());
            break;
        }

        case NodeKind::UnaryOp: {
            const auto* u = static_cast<const UnaryOp*>(e);
            expr(u->operand.get());
            if (u->op == UnOp::Len) markTable(u->operand.get(), true);
            break;
        }

        case NodeKind::FunctionCall: {
            const auto* c = static_cast<const FunctionCall*>(e);
            expr(c->callee.get());
            for (const auto& a : c->args) expr(a.get());
            if (!c->isMethodCall && c->callee && c->callee->kind == NodeKind::Identifier) {
                bool expands = !c->args.empty() && (c->args.back()->kind == NodeKind::FunctionCall ||
                                                    c->args.back()->kind == NodeKind::Vararg);
                directCalls_[sym(static_cast<const Identifier&>(*c->callee))].push_back({c->args.size(), expands});
            }
            if (c->isMethodCall) {
                markTable(c->callee.get(), false);
            } else if (!c->args.empty()) {
                std::string lib, field;
                bool arrayFn = isBuiltinCall(e, "ipairs") || isBuiltinCall(e, "unpack") ||
                               (libField(*c->callee, lib, field) && lib == "table");
                if (arrayFn) markTable(c->args[0].get(), true);
            }
            break;
        }

        case NodeKind::TableConstructor:
            for (const auto& f : static_cast<const TableConstructor*>(e)->fields) {
                expr(f.key.get());
                expr(f.value.get());
            }
            break;

        case NodeKind::Index: {
            const auto* i = static_cast<const Index*>(e);
            expr(i->object.get());
            expr(i->key.get());
            markTable(i->object.get(), isArrayKey(i->key.get()));
            break;
        }

        case NodeKind::FunctionExpr: {
            const auto* f = static_cast<const FunctionExpr*>(e);
            function(*f, f->body.get());
            break;
        }

        default: break;
        }
    }

    bool isBuiltinCall(const Expr* e, const char* name) const {
        if (!e || e->kind != NodeKind::FunctionCall) return false;
        const auto* c = static_cast<const FunctionCall*>(e);
        if (c->isMethodCall || !c->callee || c->callee->kind != NodeKind::Identifier) return false;
        const Builtin* b = builtinFor(sym(static_cast<const Identifier&>(*c->callee)));
        return b && std::string(b->lua) == name;
    }

    // ---------- tabelas vazias: list ou dict? (#41) ----------
    //
    // "{x = ...}" vira dict. "{a, b}" vira list, a não ser que a variável depois
    // receba chave de texto ou escrita fora de ordem (aí dict {1: a, 2: b}). Já "{}"
    // só vira list se a variável é usada só como lista (t[#t + 1] = v, t[i] com
    // for i = 1, n, #t, ipairs, table.insert...). Na dúvida dict, que aceita tudo.

    void bindValue(const Symbol* s, const Expr* v, int expectedWrites) {
        if (s->writes != expectedWrites) return;
        if (const TableConstructor* t = asPositionalTable(v)) tableVars_[s] = t;
        if (v && v->kind == NodeKind::FunctionExpr) boundFn[s] = v;
    }

    bool isArrayKey(const Expr* k) const {
        if (!k) return false;
        long long n;
        if (isIntLiteral(k, n)) return true;
        if (k->kind == NodeKind::Identifier) return numericVars_.count(sym(static_cast<const Identifier&>(*k))) > 0;
        if (k->kind == NodeKind::UnaryOp) return static_cast<const UnaryOp*>(k)->op == UnOp::Len;
        if (k->kind == NodeKind::BinaryOp) {
            BinOp op = static_cast<const BinaryOp*>(k)->op;
            return op == BinOp::Add || op == BinOp::Sub || op == BinOp::Mul;
        }
        return false;
    }

    // Escrita t[k] = v: só mantém t como lista se k for o próximo índice com certeza
    // (t[#t + 1], ou t[i] com i de "for i = 1, n" / ipairs). Qualquer outra chave
    // (t[2] num {} vazio, t[i] com i começando em 2...) poderia deixar buracos.
    void markWrite(const Expr* t, const Expr* k) {
        if (!t || t->kind != NodeKind::Identifier) return;
        const Symbol* s = sym(static_cast<const Identifier&>(*t));
        bool sequential = false;
        if (k && k->kind == NodeKind::Identifier)
            sequential = sequentialVars_.count(sym(static_cast<const Identifier&>(*k))) > 0;
        if (k && k->kind == NodeKind::BinaryOp) {  // #t + 1
            const auto* b = static_cast<const BinaryOp*>(k);
            long long one;
            if (b->op == BinOp::Add && b->left->kind == NodeKind::UnaryOp &&
                static_cast<const UnaryOp&>(*b->left).op == UnOp::Len && isIntLiteral(b->right.get(), one) && one == 1) {
                const Expr* lenOf = static_cast<const UnaryOp&>(*b->left).operand.get();
                sequential = lenOf->kind == NodeKind::Identifier &&
                             sym(static_cast<const Identifier&>(*lenOf)) == s;
            }
        }
        (sequential ? arrayUse_ : recordUse_).insert(s);
    }

    void markTable(const Expr* t, bool asArray) {
        if (!t || t->kind != NodeKind::Identifier) return;
        const Symbol* s = sym(static_cast<const Identifier&>(*t));
        (asArray ? arrayUse_ : recordUse_).insert(s);
    }

    void resolveTables() {
        for (const auto& [s, t] : tableVars_) {
            if (t->fields.empty()) {
                if (arrayUse_.count(s) && !recordUse_.count(s)) listTables.insert(t);
            } else if (recordUse_.count(s)) {
                dictTables.insert(t);  // {10, 20} que depois recebe t[5] ou t.nome
            }
        }
    }

    // ---------- argumentos a mais ----------
    //
    // O Lua ignora argumentos a mais; o Python dá erro. Uma função só dispensa o
    // "*lua_extra" se TODO uso dela é uma chamada direta pelo nome com no máximo
    // tantos argumentos quanto parâmetros. Funções passadas como valor (callbacks,
    // iteradores, métodos) recebem "*lua_extra".
    void resolveExtraArgs() {
        for (const auto& [s, node] : boundFn) {
            auto it = directCalls_.find(s);
            size_t ncalls = it == directCalls_.end() ? 0 : it->second.size();
            if (static_cast<size_t>(reads_[s]) != ncalls) continue;  // usada como valor
            size_t params = info_.declared.at(node).size();
            bool ok = true;
            if (ncalls)
                for (const auto& [nargs, expands] : it->second) ok = ok && !expands && nargs <= params;
            if (ok) fns[node].needsExtra = false;
        }
    }

    // ---------- funções de um único retorno (#39) ----------
    //
    // Ponto fixo: começa supondo que toda função sem "return a, b" devolve um
    // valor só, e desmarca as que terminam em "return f(x)" com f desconhecida
    // ou de vários valores, até nada mudar.
    void resolveSingle() {
        for (const auto& [node, f] : fns) {
            if (!node) continue;
            if (!f.hasValueReturn) voidFns.insert(node);
            else if (!f.multi && f.endsWithReturn) singleFns.insert(node);
        }
        for (bool changed = true; changed;) {
            changed = false;
            for (const auto& [node, f] : fns) {
                if (!node || !singleFns.count(node)) continue;
                for (const FunctionCall* c : f.tailCalls) {
                    if (!callSingle(*c) || callVoid(*c)) {
                        singleFns.erase(node);
                        changed = true;
                        break;
                    }
                }
            }
        }
    }
};

// =====================================================================
// Gerador
// =====================================================================

class Generator {
public:
    Generator(const SemanticInfo& info, const Analysis& a) : info_(info), a_(a) {}

    void run(const Block& root, std::ostream& os) {
        std::vector<std::string> body;
        out_ = &body;
        for (size_t i = 0; i < root.statements.size(); ++i)
            if (root.statements[i]) stmt(*root.statements[i], i + 1 == root.statements.size());
        flushPending();

        os << "# Gerado automaticamente pelo lua2py (GRUPO2-COMP2026.2) a partir de um programa Lua.\n";
        std::vector<std::string> parts(usedParts_.begin(), usedParts_.end());
        std::string runtime = runtimeCode(parts);
        if (!runtime.empty()) {
            os << "\n# ---- runtime: semântica do Lua que o Python não tem ----\n";
            os << runtime;
            os << "\n\n# ---- programa ----\n";
        }
        if (!a_.predeclare.empty()) {
            std::vector<std::string> names;
            for (const Symbol* s : a_.predeclare) names.push_back(s->codegenName);
            os << "\n# globais do Lua começam valendo nil\n";
            os << join(names, " = ") << " = None\n";
        }
        if (!body.empty()) os << "\n";
        for (const auto& l : body) os << l << "\n";
    }

private:
    const SemanticInfo& info_;
    const Analysis& a_;

    std::vector<std::string>* out_ = nullptr;
    std::vector<std::string> pending_;  // funções anônimas içadas para antes do comando
    int indent_ = 0;
    const ASTNode* fn_ = nullptr;       // função sendo gerada (nullptr = chunk principal)
    int tmp_ = 0;
    std::set<std::string> usedParts_;

    // ---------- saída ----------

    void line(const std::string& s) { out_->push_back(std::string(indent_ * 4, ' ') + s); }

    void flushPending() {
        out_->insert(out_->end(), pending_.begin(), pending_.end());
        pending_.clear();
    }

    std::string rt(const char* part, const char* name = nullptr) {
        usedParts_.insert(part);
        return name ? name : part;
    }

    std::string fresh(const char* prefix) { return prefix + std::to_string(++tmp_); }

    // Corpo indentado; "pass" se ficar vazio.
    void body(const Block* b) {
        ++indent_;
        size_t before = out_->size();
        if (b)
            for (const auto& s : b->statements)
                if (s) stmt(*s, false);
        flushPending();
        if (out_->size() == before) line("pass");
        --indent_;
    }

    // ---------- nomes ----------

    std::string name(const Symbol* s) {
        if (const Builtin* b = builtinFor(s)) return rt(b->part, b->py);
        return s->codegenName;
    }
    std::string name(const Identifier& id) { return name(a_.sym(id)); }

    // ---------- comandos ----------

    void stmt(const Stmt& s, bool lastOfChunk) {
        switch (s.kind) {
        case NodeKind::Block:
            for (const auto& st : static_cast<const Block&>(s).statements)
                if (st) stmt(*st, false);
            break;

        case NodeKind::Do:  // escopo já resolvido pelos nomes únicos: o corpo entra direto
            if (const auto& b = static_cast<const Do&>(s).body)
                for (const auto& st : b->statements)
                    if (st) stmt(*st, false);
            break;

        case NodeKind::LocalDecl: {
            const auto& n = static_cast<const LocalDecl&>(s);
            const auto& syms = info_.declared.at(&n);
            if (syms.size() == 1 && n.values.size() == 1 &&
                n.values[0]->kind == NodeKind::FunctionExpr) {
                funcExprAsDef(syms[0]->codegenName, static_cast<const FunctionExpr&>(*n.values[0]));
                break;
            }
            std::vector<Target> targets;
            for (const Symbol* sy : syms) targets.push_back(Target{false, sy->codegenName, "", ""});
            assign(targets, n.values);
            break;
        }

        case NodeKind::Assign: {
            const auto& n = static_cast<const Assign&>(s);
            if (n.targets.size() == 1 && n.values.size() == 1 &&
                n.targets[0]->kind == NodeKind::Identifier &&
                n.values[0]->kind == NodeKind::FunctionExpr) {
                funcExprAsDef(name(static_cast<const Identifier&>(*n.targets[0])),
                              static_cast<const FunctionExpr&>(*n.values[0]));
                break;
            }
            assignStmt(n);
            break;
        }

        case NodeKind::CallStmt: {
            std::string c = gen(*static_cast<const CallStmt&>(s).call).code;
            flushPending();
            line(c);
            break;
        }

        case NodeKind::If: {
            const auto& n = static_cast<const If&>(s);
            std::vector<std::string> conds;  // todas antes, por causa de funções içadas
            for (const auto& c : n.clauses)
                if (c.condition) conds.push_back(cond(*c.condition).code);
            flushPending();
            size_t k = 0;
            for (size_t i = 0; i < n.clauses.size(); ++i) {
                const auto& c = n.clauses[i];
                if (c.condition) line((i == 0 ? "if " : "elif ") + conds[k++] + ":");
                else line("else:");
                body(c.body.get());
            }
            break;
        }

        // ----- laços (#30) -----

        case NodeKind::While: {
            const auto& n = static_cast<const While&>(s);
            std::string c = cond(*n.condition).code;
            flushPending();
            line("while " + c + ":");
            body(n.body.get());
            break;
        }

        case NodeKind::RepeatUntil: {
            // repeat <corpo> until c  ->  while True: <corpo>; if c: break
            // A condição fica DENTRO do laço porque enxerga as locais do corpo.
            const auto& n = static_cast<const RepeatUntil&>(s);
            line("while True:");
            ++indent_;
            if (n.body)
                for (const auto& st : n.body->statements)
                    if (st) stmt(*st, false);
            std::string c = cond(*n.condition).code;
            flushPending();
            line("if " + c + ":");
            line("    break");
            --indent_;
            break;
        }

        case NodeKind::NumericFor: {
            const auto& n = static_cast<const NumericFor&>(s);
            std::string var = info_.declared.at(&n)[0]->codegenName;
            long long a, b, st = 1;
            std::string iter;
            if (isIntLiteral(n.start.get(), a) && isIntLiteral(n.stop.get(), b) &&
                (!n.step || isIntLiteral(n.step.get(), st)) && st != 0) {
                // Tudo constante inteiro: range() puro, já com o limite inclusivo do Lua.
                long long end = st > 0 ? b + 1 : b - 1;
                iter = "range(" + std::to_string(a) + ", " + std::to_string(end) +
                       (st != 1 ? ", " + std::to_string(st) : "") + ")";
            } else {
                // Caso geral: lua_range avalia os limites uma vez e trata float/int
                // e passo negativo como o Lua 5.4.
                iter = rt("lua_range") + "(" + single(*n.start).code + ", " + single(*n.stop).code +
                       (n.step ? ", " + single(*n.step).code : "") + ")";
            }
            flushPending();
            line("for " + var + " in " + iter + ":");
            body(n.body.get());
            break;
        }

        case NodeKind::GenericFor: {
            const auto& n = static_cast<const GenericFor&>(s);
            std::vector<std::string> vars;
            for (const Symbol* v : info_.declared.at(&n)) vars.push_back(v->codegenName);
            std::string iter;
            const char* fast = n.exprs.size() == 1 && vars.size() <= 2 ? pairsLike(*n.exprs[0]) : nullptr;
            if (fast) {
                // for k, v in pairs(t) / ipairs(t): gerador direto, sem o protocolo geral
                const auto& call = static_cast<const FunctionCall&>(*n.exprs[0]);
                iter = rt(fast) + "(" + single(*call.args[0]).code + ")";
                if (vars.size() == 1) vars.push_back("lua_ign");
            } else {
                Values v = adjust(n.exprs, 3);
                iter = rt("lua_generic_for") + "(" + std::to_string(vars.size()) + ", " +
                       (v.packed ? "*" + v.tuple : join(v.items, ", ")) + ")";
            }
            flushPending();
            line("for " + join(vars, ", ") + " in " + iter + ":");
            body(n.body.get());
            break;
        }

        case NodeKind::Break:
            line("break");
            break;

        // ----- funções -----

        case NodeKind::LocalFunctionDecl: {
            const auto& n = static_cast<const LocalFunctionDecl&>(s);
            flushPending();
            def(info_.funcName.at(&n)->codegenName, n, n.isVararg, n.body.get());
            break;
        }

        case NodeKind::FunctionDecl: {
            const auto& n = static_cast<const FunctionDecl&>(s);
            const Symbol* base = info_.funcName.at(&n);
            flushPending();
            if (n.name.find_first_of(".:") == std::string::npos) {
                def(name(base), n, n.isVararg, n.body.get());
                break;
            }
            // function a.b.c(...) / function a:m(...): define e guarda no campo
            std::vector<std::string> parts;
            std::string cur;
            for (char c : n.name) {
                if (c == '.' || c == ':') { parts.push_back(cur); cur.clear(); }
                else cur += c;
            }
            parts.push_back(cur);
            std::string pyName = "lua_fn_" + join(parts, "_");
            def(pyName, n, n.isVararg, n.body.get());
            std::string obj = name(base);
            for (size_t i = 1; i + 1 < parts.size(); ++i)
                obj = rt("lua_get") + "(" + obj + ", " + pyString(parts[i]) + ")";
            line(rt("lua_set") + "(" + obj + ", " + pyString(parts.back()) + ", " + pyName + ")");
            break;
        }

        // ----- retorno (#39) -----

        case NodeKind::Return: {
            const auto& n = static_cast<const Return&>(s);
            if (!fn_) {  // return no chunk principal: encerra o programa
                std::vector<std::string> effects;
                for (const auto& v : n.values)
                    if (hasCall(v.get())) effects.push_back(gen(*v).code);
                flushPending();
                for (const auto& e : effects) line(e);
                if (!lastOfChunk) line("raise SystemExit");
                break;
            }
            std::string r = returnValue(n.values);
            flushPending();
            line(r.empty() ? "return" : "return " + r);
            break;
        }

        default:
            break;
        }
    }

    const char* pairsLike(const Expr& e) {
        if (e.kind != NodeKind::FunctionCall) return nullptr;
        const auto& c = static_cast<const FunctionCall&>(e);
        if (c.isMethodCall || c.args.size() != 1 || c.callee->kind != NodeKind::Identifier) return nullptr;
        const Builtin* b = builtinFor(a_.sym(static_cast<const Identifier&>(*c.callee)));
        if (!b) return nullptr;
        std::string n = b->lua;
        if (n == "pairs") return "lua_pairs_iter";
        if (n == "ipairs") return "lua_ipairs_iter";
        return nullptr;
    }

    // "return" dentro de função (#39):
    //   return            -> return ()    (nenhum valor; return se a função é de 1 valor)
    //   return x          -> return x
    //   return f(x)       -> return f(x)  (repassa todos os valores de f)
    //   return a, b       -> return a, b  (tupla = vários valores)
    //   return a, f(x)    -> return (a, *lua_multi(f(x)))
    std::string returnValue(const std::vector<ExprPtr>& values) {
        if (values.empty()) return a_.voidFns.count(fn_) ? "" : "()";
        if (values.size() == 1) {
            const Expr& v = *values[0];
            if (v.kind == NodeKind::Vararg) return "lua_rest";
            return gen(v).code;  // uma chamada repassa seus valores sem lua_first
        }
        std::vector<std::string> items;
        for (size_t i = 0; i + 1 < values.size(); ++i) items.push_back(single1(*values[i]));
        const Expr& last = *values.back();
        if (isMulti(last)) {
            items.push_back(expand(last));
            return "(" + join(items, ", ") + ")";
        }
        items.push_back(single1(last));
        return join(items, ", ");
    }

    // ---------- atribuição e ajuste de valores (#39) ----------

    struct Target {
        bool isIndex;
        std::string name;   // variável
        std::string obj;    // t em t[k]
        std::string key;    // k em t[k]
    };

    struct Values {
        bool packed = false;            // true: "tuple" gera uma tupla com exatamente n valores
        std::string tuple;
        std::vector<std::string> items; // senão: um código por variável
    };

    bool isMulti(const Expr& e) {
        if (e.kind == NodeKind::Vararg) return true;
        return e.kind == NodeKind::FunctionCall && !a_.callSingle(static_cast<const FunctionCall&>(e));
    }

    // Último valor de uma lista, expandido: *lua_multi(f(x)) ou *lua_rest
    std::string expand(const Expr& e) {
        if (e.kind == NodeKind::Vararg) return "*lua_rest";
        return "*" + rt("lua_multi") + "(" + gen(e).code + ")";
    }

    bool isVoidCall(const Expr& e) {
        return e.kind == NodeKind::FunctionCall && a_.callVoid(static_cast<const FunctionCall&>(e));
    }

    // Último item de uma lista de argumentos/campos: expande todos os valores.
    std::string lastItem(const Expr& e) {
        if (isVoidCall(e)) return "*" + rt("lua_void") + "(" + gen(e).code + ")";  // f() sem retorno: nada
        return isMulti(e) ? expand(e) : single1(e);
    }

    std::string single1(const Expr& e) { return wrap(single(e), P_COND); }

    // Regra do Lua: n variáveis recebem os valores; falta -> nil, sobra -> descarta,
    // e só a ÚLTIMA expressão, se for chamada, pode fornecer vários valores.
    Values adjust(const std::vector<ExprPtr>& values, size_t n) {
        Values r;
        size_t m = values.size();
        bool lastMulti = m > 0 && isMulti(*values.back());
        // "q, r = divmod(a, b)" com divmod sempre devolvendo 2 valores: atribui direto
        if (m == 1 && n > 1 && values[0]->kind == NodeKind::FunctionCall &&
            a_.callCount(static_cast<const FunctionCall&>(*values[0])) == static_cast<int>(n)) {
            r.packed = true;
            r.tuple = gen(*values[0]).code;
            return r;
        }
        bool extrasHaveCalls = false;
        for (size_t i = n; i < m; ++i) extrasHaveCalls = extrasHaveCalls || hasCall(values[i].get());
        if ((lastMulti && n > m) || extrasHaveCalls) {
            std::vector<std::string> items;
            for (size_t i = 0; i < m; ++i) {
                bool expandIt = lastMulti && n > m && i + 1 == m;
                items.push_back(expandIt ? expand(*values[i]) : single1(*values[i]));
            }
            r.packed = true;
            r.tuple = rt("lua_adjust") + "(" + std::to_string(n) + ", " + join(items, ", ") + ")";
            return r;
        }
        for (size_t i = 0; i < n; ++i) r.items.push_back(i < m ? single1(*values[i]) : "None");
        return r;
    }

    void assignStmt(const Assign& n) {
        std::vector<Target> targets;
        // os valores primeiro: no Lua todos são avaliados antes de atribuir
        Values v = adjust(n.values, n.targets.size());
        for (const auto& t : n.targets) {
            if (t->kind == NodeKind::Identifier) {
                targets.push_back(Target{false, name(static_cast<const Identifier&>(*t)), "", ""});
            } else {
                const auto& ix = static_cast<const Index&>(*t);
                targets.push_back(Target{true, "", wrap(single(*ix.object), P_COND),
                                         wrap(single(*ix.key), P_COND)});
            }
        }
        emitAssign(targets, v);
    }

    void assign(const std::vector<Target>& targets, const std::vector<ExprPtr>& values) {
        emitAssign(targets, adjust(values, targets.size()));
    }

    void emitAssign(const std::vector<Target>& targets, const Values& v) {
        flushPending();
        size_t n = targets.size();
        std::string rhs = v.packed ? (n == 1 ? v.tuple + "[0]" : v.tuple) : join(v.items, ", ");
        bool anyIndex = false;
        for (const auto& t : targets) anyIndex = anyIndex || t.isIndex;
        if (!anyIndex) {
            std::vector<std::string> names;
            for (const auto& t : targets) names.push_back(t.name);
            line(join(names, ", ") + " = " + rhs);
            return;
        }
        if (n == 1) {
            line(setCall(targets[0], rhs));
            return;
        }
        // várias variáveis com t[k] entre elas: valores em temporárias primeiro
        std::vector<std::string> temps;
        for (size_t i = 0; i < n; ++i) temps.push_back(fresh("lua_v"));
        line(join(temps, ", ") + " = " + rhs);
        for (size_t i = 0; i < n; ++i)
            line(targets[i].isIndex ? setCall(targets[i], temps[i]) : targets[i].name + " = " + temps[i]);
    }

    std::string setCall(const Target& t, const std::string& value) {
        return rt("lua_set") + "(" + t.obj + ", " + t.key + ", " + value + ")";
    }

    // ---------- definição de funções ----------

    // local f = function(...) ... end  ->  def f(...):
    void funcExprAsDef(const std::string& pyName, const FunctionExpr& f) {
        flushPending();
        def(pyName, f, f.isVararg, f.body.get());
    }

    void def(const std::string& pyName, const ASTNode& node, bool vararg, const Block* b) {
        const FnInfo& info = a_.fns.at(&node);

        // Parâmetros: o Lua aceita chamar com menos argumentos (os que faltam são nil).
        std::vector<std::string> params;
        for (const Symbol* p : info_.declared.at(&node)) params.push_back(p->codegenName + "=None");
        if (vararg) params.push_back("*lua_rest");
        else if (info.needsExtra) params.push_back("*lua_extra");  // o Lua ignora argumentos a mais

        // Laços (#30): no Lua cada volta do laço tem uma variável NOVA, e uma função
        // criada ali "lembra" o valor daquela volta. No Python a variável é uma só,
        // então os valores que a função usa e que nunca mudam são capturados como
        // parâmetros só-nomeados (avaliados na criação): def f(*, i=i).
        const Symbol* self = nullptr;
        auto fit = info_.funcName.find(&node);
        if (fit != info_.funcName.end()) self = fit->second;
        std::vector<std::string> captures;
        for (const Symbol* s : info.used) {
            auto own = a_.owner.find(s);
            if (s == self || !a_.loopFresh.count(s) || s->writes != 0 || own == a_.owner.end() ||
                a_.isInside(own->second, &node))
                continue;
            captures.push_back(s->codegenName + "=" + s->codegenName);
        }
        if (!captures.empty()) {
            if (!vararg && !info.needsExtra) params.push_back("*");
            params.insert(params.end(), captures.begin(), captures.end());
        }

        line("def " + pyName + "(" + join(params, ", ") + "):");
        ++indent_;
        const ASTNode* savedFn = fn_;
        fn_ = &node;

        // Variáveis de fora atribuídas aqui dentro.
        std::vector<std::string> globals, nonlocals;
        for (const Symbol* s : info.outerWrites) {
            auto own = a_.owner.find(s);
            bool moduleLevel = s->kind == SymbolKind::Global || own == a_.owner.end() || !own->second;
            (moduleLevel ? globals : nonlocals).push_back(name(s));
        }
        if (!globals.empty()) line("global " + join(globals, ", "));
        if (!nonlocals.empty()) line("nonlocal " + join(nonlocals, ", "));

        size_t before = out_->size();
        if (b)
            for (const auto& st : b->statements)
                if (st) stmt(*st, false);
        flushPending();
        // função que às vezes devolve valores e pode chegar ao fim sem "return":
        // no fim ela devolve zero valores
        if (!a_.voidFns.count(&node) && !a_.singleFns.count(&node) && !info.endsWithReturn)
            line("return ()");
        if (out_->size() == before) line("pass");

        fn_ = savedFn;
        --indent_;
    }

    // Função anônima dentro de uma expressão: o Python não tem lambda com comandos,
    // então ela vira um "def" logo antes do comando que a usa.
    std::string hoist(const FunctionExpr& f) {
        std::string pyName = fresh("lua_fn");
        std::vector<std::string> buf;
        std::vector<std::string>* savedOut = out_;
        std::vector<std::string> savedPending = std::move(pending_);
        pending_.clear();
        out_ = &buf;
        def(pyName, f, f.isVararg, f.body.get());
        out_ = savedOut;
        pending_ = std::move(savedPending);
        pending_.insert(pending_.end(), buf.begin(), buf.end());
        return pyName;
    }

    // ---------- expressões ----------

    // Exatamente um valor: uma chamada que pode devolver vários fica só com o primeiro.
    PyExpr single(const Expr& e) {
        if (e.kind == NodeKind::Vararg) return {rt("lua_first") + "(lua_rest)", P_ATOM};
        if (isMulti(e)) return {rt("lua_first") + "(" + gen(e).code + ")", P_ATOM};
        return gen(e);
    }

    std::string truthy(const Expr& e) { return rt("lua_truthy") + "(" + single1(e) + ")"; }

    // Expressão usada como condição (if/while/until/not): só importa se é verdadeira.
    PyExpr cond(const Expr& e) {
        if (e.kind == NodeKind::BinaryOp) {
            const auto& b = static_cast<const BinaryOp&>(e);
            if (b.op == BinOp::And)
                return {wrap(cond(*b.left), P_AND) + " and " + wrap(cond(*b.right), P_AND + 1), P_AND};
            if (b.op == BinOp::Or)
                return {wrap(cond(*b.left), P_OR) + " or " + wrap(cond(*b.right), P_OR + 1), P_OR};
        }
        if (e.kind == NodeKind::UnaryOp && static_cast<const UnaryOp&>(e).op == UnOp::Not)
            return {"not " + wrap(cond(*static_cast<const UnaryOp&>(e).operand), P_NOT), P_NOT};
        if (isBoolean(&e)) return gen(e);
        if (e.kind == NodeKind::NilLiteral) return {"False", P_ATOM};
        if (e.kind == NodeKind::NumberLiteral || e.kind == NodeKind::StringLiteral ||
            e.kind == NodeKind::TableConstructor)
            return {"True", P_ATOM};
        return {truthy(e), P_ATOM};
    }

    PyExpr gen(const Expr& e) {
        switch (e.kind) {
        case NodeKind::NilLiteral: return {"None", P_ATOM};
        case NodeKind::BoolLiteral: return {static_cast<const BoolLiteral&>(e).value ? "True" : "False", P_ATOM};
        case NodeKind::NumberLiteral: return {pyNumber(static_cast<const NumberLiteral&>(e)), P_ATOM};
        case NodeKind::StringLiteral: return {pyString(static_cast<const StringLiteral&>(e).value), P_ATOM};
        case NodeKind::Vararg: return {"lua_rest", P_ATOM};
        case NodeKind::Identifier: return {name(static_cast<const Identifier&>(e)), P_ATOM};
        case NodeKind::BinaryOp: return binary(static_cast<const BinaryOp&>(e));
        case NodeKind::UnaryOp: return unary(static_cast<const UnaryOp&>(e));
        case NodeKind::FunctionCall: return call(static_cast<const FunctionCall&>(e));
        case NodeKind::TableConstructor: return table(static_cast<const TableConstructor&>(e));
        case NodeKind::Index: return index(static_cast<const Index&>(e));
        case NodeKind::FunctionExpr: return {hoist(static_cast<const FunctionExpr&>(e)), P_ATOM};
        default: return {"None", P_ATOM};
        }
    }

    std::string args(const std::vector<ExprPtr>& as) {
        std::vector<std::string> out;
        for (size_t i = 0; i < as.size(); ++i) {
            out.push_back(i + 1 == as.size() ? lastItem(*as[i]) : single1(*as[i]));
        }
        return join(out, ", ");
    }

    PyExpr call(const FunctionCall& c) {
        if (c.isMethodCall) {  // obj:m(a)  ->  lua_invoke(obj, "m", a)
            std::string a = args(c.args);
            std::string code = rt("lua_invoke") + "(" + single1(*c.callee) + ", " + pyString(c.methodName) +
                               (a.empty() ? "" : ", " + a) + ")";
            if (c.parenthesized) code = rt("lua_first") + "(" + code + ")";
            return {code, P_ATOM};
        }
        std::string f = wrap(single(*c.callee), P_ATOM);
        std::string code = f + "(" + args(c.args) + ")";
        if (c.parenthesized && !a_.calleeSingle(c)) code = rt("lua_first") + "(" + code + ")";  // (f(x))
        return {code, P_ATOM};
    }

    // Índices (#41): t[k] -> lua_get(t, k), que faz a conversão de base 1 para 0.
    PyExpr index(const Index& i) {
        std::string lib, field;
        if (a_.libField(i, lib, field))  // math.floor -> lua_math["floor"]
            return {name(a_.sym(static_cast<const Identifier&>(*i.object))) + "[" + pyString(field) + "]", P_ATOM};
        return {rt("lua_get") + "(" + single1(*i.object) + ", " + single1(*i.key) + ")", P_ATOM};
    }

    // Construtor de tabela (#41): só posições -> list; com chaves -> dict.
    PyExpr table(const TableConstructor& t) {
        if (t.fields.empty()) return {a_.listTables.count(&t) ? "[]" : "{}", P_ATOM};
        bool keyed = false;
        for (const auto& f : t.fields) keyed = keyed || f.key;
        bool lastExpands = isMulti(*t.fields.back().value) || isVoidCall(*t.fields.back().value);
        if (!keyed && a_.dictTables.count(&t) && !lastExpands) {
            std::vector<std::string> items;  // lista que o programa usa como dicionário
            for (size_t i = 0; i < t.fields.size(); ++i)
                items.push_back(std::to_string(i + 1) + ": " + single1(*t.fields[i].value));
            return {"{" + join(items, ", ") + "}", P_ATOM};
        }
        if (!keyed) {
            std::vector<std::string> items;
            for (size_t i = 0; i < t.fields.size(); ++i) {
                const Expr& v = *t.fields[i].value;
                items.push_back(i + 1 == t.fields.size() ? lastItem(v) : single1(v));
            }
            return {"[" + join(items, ", ") + "]", P_ATOM};
        }
        // Com chaves: posições recebem as chaves 1, 2, 3... e vêm por último
        // (no Lua, {[1] = "a", "b"} deixa t[1] == "b").
        std::vector<std::string> items;
        for (const auto& f : t.fields)
            if (f.key) items.push_back(single1(*f.key) + ": " + single1(*f.value));
        int pos = 0;
        for (const auto& f : t.fields)
            if (!f.key) items.push_back(std::to_string(++pos) + ": " + single1(*f.value));
        return {"{" + join(items, ", ") + "}", P_ATOM};
    }

    // ---------- operadores (#36) ----------

    PyExpr binary(const BinaryOp& b) {
        const Expr& L = *b.left;
        const Expr& R = *b.right;
        switch (b.op) {
        // + - * : mesmos operadores do Python.
        case BinOp::Add: return arith(L, R, " + ", P_ADD);
        case BinOp::Sub: return arith(L, R, " - ", P_ADD);
        case BinOp::Mul: return arith(L, R, " * ", P_MUL);

        // / // % : o Python já faz divisão "piso" e resto com o sinal do divisor,
        // como o Lua. A diferença é a divisão por zero com float (Lua: inf/nan;
        // Python: erro). Com divisor literal diferente de zero o operador do Python
        // serve; nos outros casos usa o helper do runtime.
        case BinOp::Div:
            if (isNonZeroLiteral(&R, false)) return arith(L, R, " / ", P_MUL);
            return helper("lua_div", L, R);
        case BinOp::FloorDiv:
            if (isNonZeroLiteral(&R, true)) return arith(L, R, " // ", P_MUL);
            return helper("lua_idiv", L, R);
        case BinOp::Mod:
            if (isNonZeroLiteral(&R, true)) return arith(L, R, " % ", P_MUL);
            return helper("lua_mod", L, R);

        case BinOp::Pow: {
            // a ^ b sempre dá float no Lua (2^3 == 8.0): float(a) ** b. Expoente
            // negativo/fracionário com base qualquer pode dar complexo ou erro no
            // Python, então nesses casos usa lua_pow.
            long long e;
            bool safe = (isIntLiteral(&R, e) && e >= 0) ||
                        (L.kind == NodeKind::NumberLiteral && static_cast<const NumberLiteral&>(L).value > 0);
            if (!safe) return helper("lua_pow", L, R);
            std::string base;
            if (L.kind == NodeKind::NumberLiteral) {
                const auto& n = static_cast<const NumberLiteral&>(L);
                base = n.isInteger ? pyNumber(n) + ".0" : pyNumber(n);
            } else {
                base = "float(" + single1(L) + ")";
            }
            return {base + " ** " + wrap(single(R), P_UNARY), P_POW};
        }

        case BinOp::Concat: {  // a .. b .. c -> lua_concat(a, b, c)
            std::vector<std::string> parts;
            concatParts(b, parts);
            return {rt("lua_concat") + "(" + join(parts, ", ") + ")", P_ATOM};
        }

        case BinOp::Eq: return equality(L, R, false);
        case BinOp::Ne: return equality(L, R, true);
        case BinOp::Lt: return compare(L, R, " < ");
        case BinOp::Le: return compare(L, R, " <= ");
        case BinOp::Gt: return compare(L, R, " > ");
        case BinOp::Ge: return compare(L, R, " >= ");

        case BinOp::And:
        case BinOp::Or: return logical(b.op == BinOp::And, L, R);
        }
        return {"None", P_ATOM};
    }

    PyExpr helper(const char* fn, const Expr& L, const Expr& R) {
        return {rt(fn) + "(" + single1(L) + ", " + single1(R) + ")", P_ATOM};
    }

    PyExpr arith(const Expr& L, const Expr& R, const char* op, int prec) {
        return {wrap(single(L), prec) + op + wrap(single(R), prec + 1), prec};
    }

    void concatParts(const Expr& e, std::vector<std::string>& parts) {
        if (e.kind == NodeKind::BinaryOp && static_cast<const BinaryOp&>(e).op == BinOp::Concat) {
            const auto& b = static_cast<const BinaryOp&>(e);
            concatParts(*b.left, parts);
            concatParts(*b.right, parts);
            return;
        }
        parts.push_back(single1(e));
    }

    // Comparações nunca são encadeadas (no Python "a < b < c" teria outro sentido).
    PyExpr compare(const Expr& L, const Expr& R, const char* op) {
        return {wrap(single(L), P_CMP + 1) + op + wrap(single(R), P_CMP + 1), P_CMP};
    }

    // Pode valer true/false: booleana, ou and/or (que devolvem um dos operandos).
    static bool mayBeBool(const Expr& e) {
        if (isBoolean(&e)) return true;
        if (e.kind != NodeKind::BinaryOp) return false;
        BinOp op = static_cast<const BinaryOp&>(e).op;
        return op == BinOp::And || op == BinOp::Or;
    }

    // == e ~= : com nil/true/false usa "is"; com número ou string literal usa ==;
    // nos demais casos lua_eq (tabelas comparam identidade e true ~= 1, como no Lua).
    PyExpr equality(const Expr& L, const Expr& R, bool negate) {
        auto lit = [](const Expr& e) { return e.kind == NodeKind::NilLiteral || e.kind == NodeKind::BoolLiteral; };
        auto val = [](const Expr& e) { return e.kind == NodeKind::NumberLiteral || e.kind == NodeKind::StringLiteral; };
        // cada lado é gerado UMA vez (pode içar funções); a decisão olha o código final
        PyExpr pl = single(L), pr = single(R);
        auto literalCode = [](const std::string& c) {  // "x is 1" / "x is 'a'" não servem
            size_t i = (c.size() > 1 && c[0] == '-') ? 1 : 0;
            return !c.empty() && (std::isdigit(static_cast<unsigned char>(c[i])) || c[0] == '"');
        };
        bool useIs = (lit(L) && !isConstant(&R) && !literalCode(pr.code)) ||
                     (lit(R) && !isConstant(&L) && !literalCode(pl.code));
        // número/string literal com "==" do Python, exceto se o outro lado é booleano
        // (no Python True == 1; no Lua true ~= 1)
        bool useEq = !useIs && (val(L) || val(R)) && !(lit(L) || lit(R)) &&
                     !mayBeBool(L) && !mayBeBool(R);
        if (useIs || useEq) {
            const char* op = useIs ? (negate ? " is not " : " is ") : (negate ? " != " : " == ");
            return {wrap(pl, P_CMP + 1) + op + wrap(pr, P_CMP + 1), P_CMP};
        }
        std::string c = rt("lua_eq") + "(" + wrap(pl, P_COND) + ", " + wrap(pr, P_COND) + ")";
        return negate ? PyExpr{"not " + c, P_NOT} : PyExpr{c, P_ATOM};
    }

    // and/or do Lua devolvem um dos operandos, e só nil/false são falsos (0 e ""
    // são verdadeiros, diferente do Python):
    //   esquerda booleana  : a and b              (Python já se comporta igual)
    //   esquerda simples   : b if lua_truthy(a) else a
    //   esquerda composta  : b if lua_truthy(lua_t := a) else lua_t   (avalia a uma vez)
    PyExpr logical(bool isAnd, const Expr& L, const Expr& R) {
        // Esquerda literal: já se sabe se é verdadeira (0 e "" são verdadeiros no Lua).
        if (L.kind == NodeKind::NilLiteral || L.kind == NodeKind::NumberLiteral ||
            L.kind == NodeKind::StringLiteral) {
            bool truthy = L.kind != NodeKind::NilLiteral;
            return single(truthy == isAnd ? R : L);
        }
        // Idioma "ternário" do Lua: c and X or Y, com X sempre verdadeiro (literal
        // número/string ou tabela) -> X if c else Y
        if (!isAnd && L.kind == NodeKind::BinaryOp && static_cast<const BinaryOp&>(L).op == BinOp::And) {
            const auto& inner = static_cast<const BinaryOp&>(L);
            NodeKind k = inner.right->kind;
            if (k == NodeKind::NumberLiteral || k == NodeKind::StringLiteral || k == NodeKind::TableConstructor)
                return {wrap(single(*inner.right), P_OR) + " if " + wrap(cond(*inner.left), P_OR) + " else " +
                            wrap(single(R), P_COND),
                        P_COND};
        }
        if (isBoolean(&L)) {
            int p = isAnd ? P_AND : P_OR;
            return {wrap(single(L), p) + (isAnd ? " and " : " or ") + wrap(single(R), p + 1), p};
        }
        std::string r = wrap(single(R), P_OR);
        std::string l, test;
        if (isSimple(&L)) {
            l = wrap(single(L), P_OR);
            test = rt("lua_truthy") + "(" + single1(L) + ")";
        } else {
            l = "lua_t";
            test = rt("lua_truthy") + "(lua_t := " + single1(L) + ")";
        }
        std::string code = isAnd ? r + " if " + test + " else " + l : l + " if " + test + " else " + r;
        return {code, P_COND};
    }

    PyExpr unary(const UnaryOp& u) {
        const Expr& x = *u.operand;
        switch (u.op) {
        case UnOp::Neg: return {"-" + wrap(single(x), P_UNARY), P_UNARY};
        case UnOp::Not: return {"not " + wrap(cond(x), P_NOT), P_NOT};
        case UnOp::Len: return {rt("lua_len") + "(" + single1(x) + ")", P_ATOM};
        }
        return {"None", P_ATOM};
    }
};

}  // namespace

void generatePython(const Block& root, const SemanticInfo& info, std::ostream& out) {
    Analysis a(info);
    a.run(root);
    Generator(info, a).run(root, out);
}

}  // namespace lua2py
