#pragma once

// Estrutura da AST (Árvore de Sintaxe Abstrata) para o compilador Lua / Python.
// Issue #15 - GRUPO2-COMP2026.2
// Convenções gerais:
//  - Todo nó tem um NodeKind, usado para identificar seu tipo concreto sem
//    depender de RTTI (dynamic_cast) em todo lugar. É útil para o gerador de
//    código Python fazer dispatch (ex: switch/if em cima do kind, ou um
//    visitor).
//  - "Stmt" (statement/comando) e "Expr" (expression/expressão) são as duas
//    grandes categorias de nós do Lua. Um bloco de código é uma lista de Stmt.
//  - Todos os filhos são possuídos via std::unique_ptr<T> — cada nó é dono
//    exclusivo de sua subárvore, o que facilita destruição e evita vazamento
//    de memória.
//  - "line" guarda a linha do código-fonte, para mensagens de erro e para
//    mapear de volta ao Lua original durante depuração da tradução.

#include <memory>
#include <string>
#include <vector>

namespace lua2py {

//Identificação de tipos de nó

enum class NodeKind {
    //Statements (comandos)
    Block,              // sequência de comandos
    Assign,             // a, b = 1, 2
    LocalDecl,          // local a, b = 1, 2
    If,                 // if/elseif/else
    While,              // while cond do ... end
    RepeatUntil,        // repeat ... until cond
    NumericFor,         // for i = 1, 10, 1 do ... end
    GenericFor,         // for k, v in pairs(t) do ... end
    FunctionDecl,       // function nome(...) ... end
    LocalFunctionDecl,  // local function nome(...) ... end
    Return,             // return expr, expr, ...
    Break,              // break
    Do,                 // do ... end 
    CallStmt,           // chamada de função

    //Expressions (expressões)
    NilLiteral,
    BoolLiteral,
    NumberLiteral,
    StringLiteral,
    Vararg,             
    Identifier,
    BinaryOp,
    UnaryOp,
    FunctionCall,
    TableConstructor,   // { ... }
    Index,              // obj[expr]  ou  obj.campo
    FunctionExpr,       // function(...) ... end   (função anônima)
};

// Nó base

struct ASTNode {
    NodeKind kind;
    int line = 0;

    virtual ~ASTNode() = default;

protected:
    explicit ASTNode(NodeKind k) : kind(k) {}
};

// Categorias-base. Servem apenas para deixar explícito, via tipo, se um nó é um comando ou uma expressão (útil nas assinaturas do parser).
struct Stmt : ASTNode { using ASTNode::ASTNode; };
struct Expr : ASTNode { using ASTNode::ASTNode; };

using StmtPtr  = std::unique_ptr<Stmt>;
using ExprPtr  = std::unique_ptr<Expr>;
using BlockPtr = std::unique_ptr<struct Block>;

//Statements

struct Block : Stmt {
    std::vector<StmtPtr> statements;
    Block() : Stmt(NodeKind::Block) {}
};

// a, b, ... = expr1, expr2, ...
// Os "targets" só podem ser Identifier ou Index (lvalues válidos em Lua).
struct Assign : Stmt {
    std::vector<ExprPtr> targets;
    std::vector<ExprPtr> values;
    Assign() : Stmt(NodeKind::Assign) {}
};

// local a, b, ... = expr1, expr2, ...
struct LocalDecl : Stmt {
    std::vector<std::string> names;
    std::vector<ExprPtr> values; // pode ter menos valores que nomes
    LocalDecl() : Stmt(NodeKind::LocalDecl) {}
};

// Um "if" com elseifs é modelado como uma lista de cláusulas.
// A última cláusula pode ter condition == nullptr, representando o "else".
struct IfClause {
    ExprPtr condition;  // nullptr => é o "else" final
    BlockPtr body;
};

struct If : Stmt {
    std::vector<IfClause> clauses;
    If() : Stmt(NodeKind::If) {}
};

struct While : Stmt {
    ExprPtr condition;
    BlockPtr body;
    While() : Stmt(NodeKind::While) {}
};

struct RepeatUntil : Stmt {
    BlockPtr body;
    ExprPtr condition; // avaliada depois do corpo, e vê variáveis locais dele
    RepeatUntil() : Stmt(NodeKind::RepeatUntil) {}
};

// for i = start, stop, step do ... end
struct NumericFor : Stmt {
    std::string varName;
    ExprPtr start;
    ExprPtr stop;
    ExprPtr step; // nullptr => passo implícito de 1
    BlockPtr body;
    NumericFor() : Stmt(NodeKind::NumericFor) {}
};

// for k, v, ... in explist do ... end
struct GenericFor : Stmt {
    std::vector<std::string> names;
    std::vector<ExprPtr> exprs; // normalmente: iterador, estado, controle
    BlockPtr body;
    GenericFor() : Stmt(NodeKind::GenericFor) {}
};

struct FunctionDecl : Stmt {
    std::string name;             // pode ser dotted/method (ex: "obj.metodo")
    std::vector<std::string> params;
    bool isVararg = false;        // função aceita "..."
    BlockPtr body;
    FunctionDecl() : Stmt(NodeKind::FunctionDecl) {}
};

struct LocalFunctionDecl : Stmt {
    std::string name;
    std::vector<std::string> params;
    bool isVararg = false;
    BlockPtr body;
    LocalFunctionDecl() : Stmt(NodeKind::LocalFunctionDecl) {}
};

struct Return : Stmt {
    std::vector<ExprPtr> values;
    Return() : Stmt(NodeKind::Return) {}
};

struct Break : Stmt {
    Break() : Stmt(NodeKind::Break) {}
};

struct Do : Stmt {
    BlockPtr body;
    Do() : Stmt(NodeKind::Do) {}
};

// Chamada de função usada como comando (o valor de retorno é descartado).
struct CallStmt : Stmt {
    ExprPtr call; // deve apontar para um nó FunctionCall
    CallStmt() : Stmt(NodeKind::CallStmt) {}
};

//Expressions

struct NilLiteral : Expr {
    NilLiteral() : Expr(NodeKind::NilLiteral) {}
};

struct BoolLiteral : Expr {
    bool value;
    explicit BoolLiteral(bool v) : Expr(NodeKind::BoolLiteral), value(v) {}
};

struct NumberLiteral : Expr {
    double value;
    explicit NumberLiteral(double v) : Expr(NodeKind::NumberLiteral), value(v) {}
};

struct StringLiteral : Expr {
    std::string value;
    explicit StringLiteral(std::string v)
        : Expr(NodeKind::StringLiteral), value(std::move(v)) {}
};

struct Vararg : Expr {
    Vararg() : Expr(NodeKind::Vararg) {}
};

struct Identifier : Expr {
    std::string name;
    explicit Identifier(std::string n)
        : Expr(NodeKind::Identifier), name(std::move(n)) {}
};

enum class BinOp {
    Add, Sub, Mul, Div, FloorDiv, Mod, Pow, Concat,
    Eq, Ne, Lt, Le, Gt, Ge,
    And, Or,
};

struct BinaryOp : Expr {
    BinOp op;
    ExprPtr left;
    ExprPtr right;
    explicit BinaryOp(BinOp o) : Expr(NodeKind::BinaryOp), op(o) {}
};

enum class UnOp { Neg, Not, Len };

struct UnaryOp : Expr {
    UnOp op;
    ExprPtr operand;
    explicit UnaryOp(UnOp o) : Expr(NodeKind::UnaryOp), op(o) {}
};

// Cobre tanto f(a, b) quanto obj:metodo(a, b) (isMethodCall = true insere implicitamente "obj" como primeiro argumento na geração de código Python).
struct FunctionCall : Expr {
    ExprPtr callee;
    std::vector<ExprPtr> args;
    bool isMethodCall = false;
    std::string methodName; // usado apenas quando isMethodCall == true
    FunctionCall() : Expr(NodeKind::FunctionCall) {}
};

// Um campo de tabela: se "key" for nullptr, é um elemento posicional
// (estilo array); caso contrário é um par chave/valor.
struct TableField {
    ExprPtr key;   // nullptr => posicional
    ExprPtr value;
};

struct TableConstructor : Expr {
    std::vector<TableField> fields;
    TableConstructor() : Expr(NodeKind::TableConstructor) {}
};

// obj[key]  ou  obj.campo (nesse caso "key" é um StringLiteral)
struct Index : Expr {
    ExprPtr object;
    ExprPtr key;
    Index() : Expr(NodeKind::Index) {}
};

struct FunctionExpr : Expr {
    std::vector<std::string> params;
    bool isVararg = false;
    BlockPtr body;
    FunctionExpr() : Expr(NodeKind::FunctionExpr) {}
};

} // namespace lua2py