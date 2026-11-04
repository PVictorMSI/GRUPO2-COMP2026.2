#include <cstdio>
#include "resolver.hpp"

namespace lua2py {

// Contador de erros semânticos: acessado pelo main.cpp para decidir o exit code.
int sem_errors = 0;

namespace {

class Resolver {
public:
    explicit Resolver(SemanticInfo& info) : info_(info), t_(info.table) {}

    void run(const Block& root) {
        t_.enterScope();   // escopo do chunk principal (função 0, profundidade 1)
        registerBuiltins();
        blockBody(root);
        t_.exitScope();
        t_.assignCodegenNames();
    }

private:
    // Registra os nomes globais do Lua padrão (built-ins) para que não sejam
    // reportados como "variável não declarada" (issue #27).
    void registerBuiltins() {
        static const char* builtins[] = {
            // funções básicas do Lua 5.4
            "print", "type", "tostring", "tonumber", "ipairs", "pairs",
            "next", "select", "error", "assert", "pcall", "xpcall",
            "rawget", "rawset", "rawequal", "rawlen",
            "setmetatable", "getmetatable",
            "require", "load", "loadfile", "dofile",
            "collectgarbage", "unpack", "table.unpack",
            // bibliotecas padrão
            "math", "string", "table", "io", "os", "coroutine", "package",
            // variáveis globais especiais
            "_G", "_VERSION", "arg",
            nullptr
        };
        for (int i = 0; builtins[i]; ++i)
            t_.global(builtins[i], 0);  // registra como global conhecida (não dispara erro de leitura)
    }


    SemanticInfo& info_;
    SymbolTable& t_;

    // ---------- blocos ----------

    // Resolve os comandos SEM abrir escopo (quem chama decide o escopo).
    void blockBody(const Block& b) {
        for (const auto& s : b.statements)
            if (s) stmt(*s);
    }

    // Bloco com escopo próprio (corpo de do/while/if/for...).
    void scopedBlock(const Block* b) {
        if (!b) return;
        t_.enterScope();
        blockBody(*b);
        t_.exitScope();
    }

    // ---------- uso de nomes ----------

    void touch(Symbol* s, bool write) {
        if (write) ++s->writes; else ++s->reads;
        // Local/parâmetro usado dentro de outra função = captura (closure).
        if (s->kind != SymbolKind::Global && s->functionId != t_.currentFunctionId())
            s->captured = true;
    }

    // Acha o símbolo visível para "name".
    // write=true  → atribuição global implícita (comportamento Lua: x = 1 cria global).
    // write=false → LEITURA: se o nome não foi declarado/usado antes, é erro semântico (issue #27).
    Symbol* find(const std::string& name, int line, bool write) {
        Symbol* s = t_.lookup(name);
        if (s) return s;

        if (write) {
            // Atribuição a variável não declarada → cria global implícita (padrão Lua).
            return t_.global(name, line);
        } else {
            // Leitura de variável não declarada → erro semântico estrito.
            std::fprintf(stderr,
                "Erro semântico na linha %d: variável '%s' não foi declarada\n",
                line, name.c_str());
            ++sem_errors;
            // Ainda registra como global para não travar o restante da análise.
            return t_.global(name, line);
        }
    }

    void use(const Identifier& id, bool write) {
        Symbol* s = find(id.name, id.line, write);
        touch(s, write);
        info_.uses[&id] = s;
    }

    // ---------- funções ----------

    // Parâmetros e corpo compartilham o mesmo escopo (como no Lua).
    void functionBody(const ASTNode& node, const std::vector<std::string>& params,
                      const Block* body, bool hasSelf) {
        t_.enterFunction();
        std::vector<Symbol*> declared;
        if (hasSelf) declared.push_back(t_.declare("self", SymbolKind::Param, node.line));
        for (const auto& p : params)
            declared.push_back(t_.declare(p, SymbolKind::Param, node.line));
        info_.declared[&node] = std::move(declared);
        if (body) blockBody(*body);
        t_.exitScope();
    }

    // ---------- comandos ----------

    void stmt(const Stmt& s) {
        switch (s.kind) {
        case NodeKind::Block:
            scopedBlock(static_cast<const Block*>(&s));
            break;

        case NodeKind::Assign: {
            const auto& n = static_cast<const Assign&>(s);
            for (const auto& v : n.values) if (v) expr(*v);
            for (const auto& tg : n.targets) {
                if (!tg) continue;
                if (tg->kind == NodeKind::Identifier)
                    use(static_cast<const Identifier&>(*tg), /*write=*/true);
                else
                    expr(*tg);  // Index: "t[i] = v" lê t e i, não escreve em variável
            }
            break;
        }

        case NodeKind::LocalDecl: {
            const auto& n = static_cast<const LocalDecl&>(s);
            // Os valores são resolvidos ANTES de declarar os nomes: em
            // "local x = x" o "x" da direita é o x de FORA.
            for (const auto& v : n.values) if (v) expr(*v);
            std::vector<Symbol*> declared;
            for (const auto& name : n.names)
                declared.push_back(t_.declare(name, SymbolKind::Local, n.line));
            info_.declared[&n] = std::move(declared);
            break;
        }

        case NodeKind::If: {
            const auto& n = static_cast<const If&>(s);
            for (const auto& c : n.clauses) {
                if (c.condition) expr(*c.condition);
                scopedBlock(c.body.get());
            }
            break;
        }

        case NodeKind::While: {
            const auto& n = static_cast<const While&>(s);
            if (n.condition) expr(*n.condition);
            scopedBlock(n.body.get());
            break;
        }

        case NodeKind::RepeatUntil: {
            const auto& n = static_cast<const RepeatUntil&>(s);
            // Particularidade do Lua: a condição do "until" ENXERGA as locais do
            // corpo. Por isso corpo e condição compartilham um único escopo.
            t_.enterScope();
            if (n.body) blockBody(*n.body);
            if (n.condition) expr(*n.condition);
            t_.exitScope();
            break;
        }

        case NodeKind::NumericFor: {
            const auto& n = static_cast<const NumericFor&>(s);
            // início/fim/passo são avaliados FORA do escopo da variável do laço.
            if (n.start) expr(*n.start);
            if (n.stop)  expr(*n.stop);
            if (n.step)  expr(*n.step);
            t_.enterScope();
            info_.declared[&n] = {t_.declare(n.varName, SymbolKind::Local, n.line)};
            if (n.body) blockBody(*n.body);
            t_.exitScope();
            break;
        }

        case NodeKind::GenericFor: {
            const auto& n = static_cast<const GenericFor&>(s);
            for (const auto& e : n.exprs) if (e) expr(*e);
            t_.enterScope();
            std::vector<Symbol*> declared;
            for (const auto& name : n.names)
                declared.push_back(t_.declare(name, SymbolKind::Local, n.line));
            info_.declared[&n] = std::move(declared);
            if (n.body) blockBody(*n.body);
            t_.exitScope();
            break;
        }

        case NodeKind::FunctionDecl: {
            const auto& n = static_cast<const FunctionDecl&>(s);
            // "function f()"     -> atribui à variável f (local visível, ou global)
            // "function a.b.c()" -> LÊ a variável "a" e grava num campo dela
            // "function a:m()"   -> idem, e o corpo ganha "self" como 1º parâmetro
            auto sep = n.name.find_first_of(".:");
            std::string base = n.name.substr(0, sep);
            bool isSimple = (sep == std::string::npos);
            // Função simples "function f()" → escreve em f (cria global se não existir).
            // Função dotted "function a.b()" → lê "a"; erro se não declarado.
            Symbol* sym = find(base, n.line, /*write=*/isSimple);
            touch(sym, /*write=*/isSimple);
            info_.funcName[&n] = sym;
            bool hasSelf = n.name.find(':') != std::string::npos;
            functionBody(n, n.params, n.body.get(), hasSelf);
            break;
        }

        case NodeKind::LocalFunctionDecl: {
            const auto& n = static_cast<const LocalFunctionDecl&>(s);
            // O nome é declarado ANTES do corpo, para a função poder se chamar (recursão).
            info_.funcName[&n] = t_.declare(n.name, SymbolKind::Local, n.line);
            functionBody(n, n.params, n.body.get(), false);
            break;
        }

        case NodeKind::Return: {
            const auto& n = static_cast<const Return&>(s);
            for (const auto& v : n.values) if (v) expr(*v);
            break;
        }

        case NodeKind::Do:
            scopedBlock(static_cast<const Do&>(s).body.get());
            break;

        case NodeKind::CallStmt: {
            const auto& n = static_cast<const CallStmt&>(s);
            if (n.call) expr(*n.call);
            break;
        }

        default:  // Break e qualquer outro: nada a resolver
            break;
        }
    }

    // ---------- expressões ----------

    void expr(const Expr& e) {
        switch (e.kind) {
        case NodeKind::Identifier:
            use(static_cast<const Identifier&>(e), /*write=*/false);
            break;

        case NodeKind::BinaryOp: {
            const auto& n = static_cast<const BinaryOp&>(e);
            if (n.left)  expr(*n.left);
            if (n.right) expr(*n.right);
            break;
        }

        case NodeKind::UnaryOp: {
            const auto& n = static_cast<const UnaryOp&>(e);
            if (n.operand) expr(*n.operand);
            break;
        }

        case NodeKind::FunctionCall: {
            const auto& n = static_cast<const FunctionCall&>(e);
            if (n.callee) expr(*n.callee);  // methodName é só um campo, não uma variável
            for (const auto& a : n.args) if (a) expr(*a);
            break;
        }

        case NodeKind::TableConstructor: {
            const auto& n = static_cast<const TableConstructor&>(e);
            for (const auto& f : n.fields) {
                if (f.key)   expr(*f.key);
                if (f.value) expr(*f.value);
            }
            break;
        }

        case NodeKind::Index: {
            const auto& n = static_cast<const Index&>(e);
            if (n.object) expr(*n.object);
            if (n.key)    expr(*n.key);
            break;
        }

        case NodeKind::FunctionExpr: {
            const auto& n = static_cast<const FunctionExpr&>(e);
            functionBody(n, n.params, n.body.get(), false);
            break;
        }

        default:  // literais e Vararg: nada a resolver
            break;
        }
    }
};

}  // namespace

void analyze(const Block& root, SemanticInfo& info) {
    Resolver(info).run(root);
}

}  // namespace lua2py
