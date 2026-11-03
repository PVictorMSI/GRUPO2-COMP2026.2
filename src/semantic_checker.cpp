#include "semantic_checker.hpp"
#include <cstdio>
#include <unordered_set>

namespace lua2py {

int sem_errors = 0;

void reportSemanticError(int line, const std::string& message) {
    std::fprintf(stderr, "Erro semântico na linha %d: %s\n", line, message.c_str());
    ++sem_errors;
}

namespace {

class SemanticChecker {
public:
    void run(const Block& root) {
        checkBlock(root);
    }

private:
    int loopDepth = 0;
    bool inVarargFunc = false;

    void checkBlock(const Block& b) {
        for (const auto& s : b.statements) {
            if (s) checkStmt(*s);
        }
    }

    void checkStmt(const Stmt& s) {
        switch (s.kind) {
            case NodeKind::Block:
                checkBlock(static_cast<const Block&>(s));
                break;

            case NodeKind::Break:
                //critério de aceitação: 'break' fora de um laço de repetição
                if (loopDepth <= 0) {
                    reportSemanticError(s.line, "'break' fora de um laço de repetição");
                }
                break;

            case NodeKind::While: {
                const auto& w = static_cast<const While&>(s);
                if (w.condition) checkExpr(*w.condition);
                loopDepth++;
                if (w.body) checkBlock(*w.body);
                loopDepth--;
                break;
            }

            case NodeKind::RepeatUntil: {
                const auto& r = static_cast<const RepeatUntil&>(s);
                loopDepth++;
                if (r.body) checkBlock(*r.body);
                loopDepth--;
                if (r.condition) checkExpr(*r.condition);
                break;
            }

            case NodeKind::NumericFor: {
                const auto& f = static_cast<const NumericFor&>(s);
                if (f.start) checkExpr(*f.start);
                if (f.stop)  checkExpr(*f.stop);
                if (f.step)  checkExpr(*f.step);
                loopDepth++;
                if (f.body) checkBlock(*f.body);
                loopDepth--;
                break;
            }

            case NodeKind::GenericFor: {
                const auto& f = static_cast<const GenericFor&>(s);
                for (const auto& e : f.exprs) if (e) checkExpr(*e);
                loopDepth++;
                if (f.body) checkBlock(*f.body);
                loopDepth--;
                break;
            }

            case NodeKind::If: {
                const auto& n = static_cast<const If&>(s);
                for (const auto& c : n.clauses) {
                    if (c.condition) checkExpr(*c.condition);
                    if (c.body) checkBlock(*c.body);
                }
                break;
            }

            case NodeKind::FunctionDecl: {
                const auto& n = static_cast<const FunctionDecl&>(s);
                validateParams(n.params, n.line);
                bool prev = inVarargFunc;
                inVarargFunc = n.isVararg;
                if (n.body) checkBlock(*n.body);
                inVarargFunc = prev;
                break;
            }

            case NodeKind::LocalFunctionDecl: {
                const auto& n = static_cast<const LocalFunctionDecl&>(s);
                validateParams(n.params, n.line);
                bool prev = inVarargFunc;
                inVarargFunc = n.isVararg;
                if (n.body) checkBlock(*n.body);
                inVarargFunc = prev;
                break;
            }

            case NodeKind::Assign: {
                const auto& n = static_cast<const Assign&>(s);
                for (const auto& t : n.targets) if (t) checkExpr(*t);
                for (const auto& v : n.values) if (v) checkExpr(*v);
                break;
            }

            case NodeKind::LocalDecl: {
                const auto& n = static_cast<const LocalDecl&>(s);
                for (const auto& v : n.values) if (v) checkExpr(*v);
                break;
            }

            case NodeKind::Return: {
                const auto& n = static_cast<const Return&>(s);
                for (const auto& v : n.values) if (v) checkExpr(*v);
                break;
            }

            case NodeKind::Do: {
                const auto& d = static_cast<const Do&>(s);
                if (d.body) checkBlock(*d.body);
                break;
            }

            case NodeKind::CallStmt: {
                const auto& n = static_cast<const CallStmt&>(s);
                if (n.call) checkExpr(*n.call);
                break;
            }

            default:
                break;
        }
    }

    void checkExpr(const Expr& e) {
        switch (e.kind) {
            case NodeKind::Vararg:
                //critério de aceitação: '...' só pode existir em função com vararg
                if (!inVarargFunc) {
                    reportSemanticError(e.line, "expressão '...' não pode ser usada fora de função com vararg");
                }
                break;

            case NodeKind::UnaryOp: {
                const auto& u = static_cast<const UnaryOp&>(e);
                if (u.operand) {
                    checkExpr(*u.operand);
                    if (u.op == UnOp::Len && 
                       (u.operand->kind == NodeKind::NumberLiteral || 
                        u.operand->kind == NodeKind::BoolLiteral)) {
                        reportSemanticError(u.line, "operador de comprimento '#' aplicado a tipo não suportado");
                    }
                }
                break;
            }

            case NodeKind::BinaryOp: {
                const auto& b = static_cast<const BinaryOp&>(e);
                if (b.left)  checkExpr(*b.left);
                if (b.right) checkExpr(*b.right);
                break;
            }

            case NodeKind::FunctionCall: {
                const auto& c = static_cast<const FunctionCall&>(e);
                if (c.callee) checkExpr(*c.callee);
                for (const auto& a : c.args) if (a) checkExpr(*a);
                break;
            }

            case NodeKind::TableConstructor: {
                const auto& t = static_cast<const TableConstructor&>(e);
                for (const auto& f : t.fields) {
                    if (f.key)   checkExpr(*f.key);
                    if (f.value) checkExpr(*f.value);
                }
                break;
            }

            case NodeKind::Index: {
                const auto& i = static_cast<const Index&>(e);
                if (i.object) checkExpr(*i.object);
                if (i.key)    checkExpr(*i.key);
                break;
            }

            case NodeKind::FunctionExpr: {
                const auto& f = static_cast<const FunctionExpr&>(e);
                validateParams(f.params, f.line);
                bool prev = inVarargFunc;
                inVarargFunc = f.isVararg;
                if (f.body) checkBlock(*f.body);
                inVarargFunc = prev;
                break;
            }

            default:
                break;
        }
    }

    //critério de aceitação: não pode haver parâmetros duplicados na declaração da função
    void validateParams(const std::vector<std::string>& params, int line) {
        std::unordered_set<std::string> seen;
        for (const auto& p : params) {
            if (!seen.insert(p).second) {
                reportSemanticError(line, "parâmetro duplicado '" + p + "' na declaração de função");
            }
        }
    }
};

} // namespace

void checkSemantics(const Block& root) {
    SemanticChecker().run(root);
}

} // namespace lua2py
