/* exemplo.y - Parser Bison do compilador Lua -> Python (issue #23)
 * Cobre o subconjunto definido pela AST (ast.hpp): atribuicao, local, if/elseif/else,
 * while, repeat, for numerico e generico, funcoes, return, break, do, chamadas,
 * metodos (obj:m()), tabelas, indexacao e todos os operadores.
 * Fora do escopo (conforme ast.hpp): goto e rotulos.
 */
%code requires {
    #include "ast.hpp"
    #include <string>
    #include <vector>
    using namespace lua2py;

    /* Estruturas auxiliares para carregar listas entre as regras */
    struct ExprList   { std::vector<ExprPtr> v; };
    struct NameList   { std::vector<std::string> v; };
    struct ParamList  { std::vector<std::string> names; bool vararg = false; };
    struct FieldList  { std::vector<TableField> v; };
    struct ClauseList { std::vector<IfClause> v; };
    struct FuncBody   { std::vector<std::string> params; bool vararg = false; BlockPtr body; };
}

%{
#include "ast.hpp"
using namespace lua2py;
#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>

int  yylex();
void yyerror(const char* msg);
extern int linha;               /* contador de linha definido no lexer */

lua2py::Block* root = nullptr;  /* raiz da AST: o resto do compilador usa isto */

/* Registra a linha do codigo-fonte no no recem-criado */
template <class T> static T* at(T* n) { n->line = linha; return n; }

static Expr* bin(BinOp op, Expr* l, Expr* r) {
    auto* b = at(new BinaryOp(op)); b->left.reset(l); b->right.reset(r); return b;
}
static Expr* un(UnOp op, Expr* e) {
    auto* u = at(new UnaryOp(op)); u->operand.reset(e); return u;
}
static bool isLvalue(const Expr* e) {
    return e->kind == NodeKind::Identifier || e->kind == NodeKind::Index;
}
%}

%union {
    int    intValue;
    double floatValue;
    char*  strValue;             /* alocado com strdup no lexer */
    Expr*  expr;
    Stmt*  stmt;
    Block* block;
    ExprList*   exprs;
    NameList*   names;
    ParamList*  params;
    FieldList*  fields;
    ClauseList* clauses;
    FuncBody*   fbody;
    std::string* sname;
}

%token <intValue>   NUM
%token <floatValue> NUM_FLOAT
%token <strValue>   STRING ID

%token FUNCTION ELSEIF ELSE IF OR UNTIL WHILE TRUE_VAL FALSE_VAL NOT THEN NIL
%token FOR DO RETURN LOCAL BREAK REPEAT IN END AND VARARG
%token CONCAT DOT EQ NEQ GE LE GT LT ASSIGN PLUS MINUS MULT IDIV DIV MOD POW LEN
%token SEMI COLON COMMA LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE

%type <block>   chunk block stmt_list else_opt
%type <stmt>    stmt retstat
%type <expr>    exp prefixexp tableconstructor
%type <exprs>   explist args varlist
%type <names>   namelist
%type <params>  parlist
%type <fields>  field fieldlist
%type <clauses> elseif_list
%type <fbody>   funcbody
%type <sname>   funcname dotted_name

/* Precedencia do Lua (do mais fraco ao mais forte) */
%left  OR
%left  AND
%left  LT GT LE GE NEQ EQ
%right CONCAT
%left  PLUS MINUS
%left  MULT DIV IDIV MOD
%right NOT LEN UMINUS
%right POW

/* Conflito conhecido e aceito: "f (x)" apos uma expressao pode ser uma chamada
   ou o inicio de outro comando. O Lua resolve como chamada (shift), igual ao Bison. */
%expect 2

%%

chunk : block                     { root = $1; $$ = $1; }
      ;

block : stmt_list                 { $$ = $1; }
      | stmt_list retstat         { $1->statements.emplace_back($2); $$ = $1; }
      ;

stmt_list : /* vazio */           { $$ = at(new Block()); }
          | stmt_list stmt        { if ($2) $1->statements.emplace_back($2); $$ = $1; }
          ;

retstat : RETURN opt_semi         { $$ = at(new Return()); }
        | RETURN explist opt_semi {
              auto* r = at(new Return());
              r->values = std::move($2->v); delete $2;
              $$ = r;
          }
        ;

opt_semi : /* vazio */ | SEMI ;

stmt  : SEMI                      { $$ = nullptr; }
      | varlist ASSIGN explist    {
            auto* a = at(new Assign());
            a->targets = std::move($1->v); delete $1;
            a->values  = std::move($3->v); delete $3;
            $$ = a;
        }
      | prefixexp                 {
            if ($1->kind != NodeKind::FunctionCall) {
                yyerror("expressao nao e um comando valido (esperava chamada de funcao ou atribuicao)");
                YYERROR;
            }
            auto* c = at(new CallStmt()); c->call.reset($1); $$ = c;
        }
      | DO block END             { auto* d = at(new Do()); d->body.reset($2); $$ = d; }
      | WHILE exp DO block END   {
            auto* w = at(new While()); w->condition.reset($2); w->body.reset($4); $$ = w;
        }
      | REPEAT block UNTIL exp   {
            auto* r = at(new RepeatUntil()); r->body.reset($2); r->condition.reset($4); $$ = r;
        }
      | IF exp THEN block elseif_list else_opt END {
            auto* n = at(new If());
            n->clauses.push_back(IfClause{ExprPtr($2), BlockPtr($4)});
            for (auto& c : $5->v) n->clauses.push_back(std::move(c));
            delete $5;
            if ($6) n->clauses.push_back(IfClause{nullptr, BlockPtr($6)});
            $$ = n;
        }
      | FOR ID ASSIGN exp COMMA exp DO block END {
            auto* f = at(new NumericFor());
            f->varName = $2; free($2);
            f->start.reset($4); f->stop.reset($6); f->body.reset($8);
            $$ = f;
        }
      | FOR ID ASSIGN exp COMMA exp COMMA exp DO block END {
            auto* f = at(new NumericFor());
            f->varName = $2; free($2);
            f->start.reset($4); f->stop.reset($6); f->step.reset($8); f->body.reset($10);
            $$ = f;
        }
      | FOR namelist IN explist DO block END {
            auto* f = at(new GenericFor());
            f->names = std::move($2->v); delete $2;
            f->exprs = std::move($4->v); delete $4;
            f->body.reset($6);
            $$ = f;
        }
      | FUNCTION funcname funcbody {
            auto* f = at(new FunctionDecl());
            f->name = *$2; delete $2;
            f->params = std::move($3->params); f->isVararg = $3->vararg;
            f->body = std::move($3->body); delete $3;
            $$ = f;
        }
      | LOCAL FUNCTION ID funcbody {
            auto* f = at(new LocalFunctionDecl());
            f->name = $3; free($3);
            f->params = std::move($4->params); f->isVararg = $4->vararg;
            f->body = std::move($4->body); delete $4;
            $$ = f;
        }
      | LOCAL namelist           {
            auto* d = at(new LocalDecl());
            d->names = std::move($2->v); delete $2;
            $$ = d;
        }
      | LOCAL namelist ASSIGN explist {
            auto* d = at(new LocalDecl());
            d->names  = std::move($2->v); delete $2;
            d->values = std::move($4->v); delete $4;
            $$ = d;
        }
      | BREAK                    { $$ = at(new Break()); }
      ;

elseif_list : /* vazio */        { $$ = new ClauseList(); }
            | elseif_list ELSEIF exp THEN block {
                  $1->v.push_back(IfClause{ExprPtr($3), BlockPtr($5)}); $$ = $1;
              }
            ;

else_opt : /* vazio */           { $$ = nullptr; }
         | ELSE block            { $$ = $2; }
         ;

funcname : dotted_name           { $$ = $1; }
         | dotted_name COLON ID  { *$1 += ":"; *$1 += $3; free($3); $$ = $1; }
         ;

dotted_name : ID                 { $$ = new std::string($1); free($1); }
            | dotted_name DOT ID { *$1 += "."; *$1 += $3; free($3); $$ = $1; }
            ;

funcbody : LPAREN parlist RPAREN block END {
               auto* f = new FuncBody();
               f->params = std::move($2->names); f->vararg = $2->vararg; delete $2;
               f->body.reset($4);
               $$ = f;
           }
         ;

parlist : /* vazio */            { $$ = new ParamList(); }
        | namelist               { $$ = new ParamList(); $$->names = std::move($1->v); delete $1; }
        | namelist COMMA VARARG  { $$ = new ParamList(); $$->names = std::move($1->v); delete $1; $$->vararg = true; }
        | VARARG                 { $$ = new ParamList(); $$->vararg = true; }
        ;

namelist : ID                    { $$ = new NameList(); $$->v.push_back($1); free($1); }
         | namelist COMMA ID     { $1->v.push_back($3); free($3); $$ = $1; }
         ;

varlist : prefixexp              {
              if (!isLvalue($1)) { yyerror("alvo de atribuicao invalido"); YYERROR; }
              $$ = new ExprList(); $$->v.emplace_back($1);
          }
        | varlist COMMA prefixexp {
              if (!isLvalue($3)) { yyerror("alvo de atribuicao invalido"); YYERROR; }
              $1->v.emplace_back($3); $$ = $1;
          }
        ;

explist : exp                    { $$ = new ExprList(); $$->v.emplace_back($1); }
        | explist COMMA exp      { $1->v.emplace_back($3); $$ = $1; }
        ;

/* prefixexp cobre variaveis, indexacao, chamadas e (exp) */
prefixexp : ID                   { $$ = at(new Identifier($1)); free($1); }
          | prefixexp LBRACKET exp RBRACKET {
                auto* i = at(new Index()); i->object.reset($1); i->key.reset($3); $$ = i;
            }
          | prefixexp DOT ID     {
                auto* i = at(new Index()); i->object.reset($1);
                i->key.reset(at(new StringLiteral($3))); free($3); $$ = i;
            }
          | prefixexp args       {
                auto* c = at(new FunctionCall()); c->callee.reset($1);
                c->args = std::move($2->v); delete $2; $$ = c;
            }
          | prefixexp COLON ID args {
                auto* c = at(new FunctionCall()); c->callee.reset($1);
                c->isMethodCall = true; c->methodName = $3; free($3);
                c->args = std::move($4->v); delete $4; $$ = c;
            }
          | LPAREN exp RPAREN    { $$ = $2; }
          ;

args : LPAREN RPAREN             { $$ = new ExprList(); }
     | LPAREN explist RPAREN     { $$ = $2; }
     | tableconstructor          { $$ = new ExprList(); $$->v.emplace_back($1); }
     | STRING                    { $$ = new ExprList(); $$->v.emplace_back(at(new StringLiteral($1))); free($1); }
     ;

tableconstructor : LBRACE RBRACE { $$ = at(new TableConstructor()); }
                 | LBRACE fieldlist RBRACE {
                       auto* t = at(new TableConstructor());
                       t->fields = std::move($2->v); delete $2; $$ = t;
                   }
                 | LBRACE fieldlist fieldsep RBRACE {
                       auto* t = at(new TableConstructor());
                       t->fields = std::move($2->v); delete $2; $$ = t;
                   }
                 ;

fieldlist : field                { $$ = $1; }
          | fieldlist fieldsep field {
                for (auto& f : $3->v) $1->v.push_back(std::move(f));
                delete $3; $$ = $1;
            }
          ;

fieldsep : COMMA | SEMI ;

field : LBRACKET exp RBRACKET ASSIGN exp {
            $$ = new FieldList(); $$->v.push_back(TableField{ExprPtr($2), ExprPtr($5)});
        }
      | ID ASSIGN exp {
            $$ = new FieldList();
            $$->v.push_back(TableField{ExprPtr(at(new StringLiteral($1))), ExprPtr($3)});
            free($1);
        }
      | exp {
            $$ = new FieldList(); $$->v.push_back(TableField{nullptr, ExprPtr($1)});
        }
      ;

exp   : NIL                      { $$ = at(new NilLiteral()); }
      | FALSE_VAL                { $$ = at(new BoolLiteral(false)); }
      | TRUE_VAL                 { $$ = at(new BoolLiteral(true)); }
      | NUM                      { $$ = at(new NumberLiteral($1)); }
      | NUM_FLOAT                { $$ = at(new NumberLiteral($1)); }
      | STRING                   { $$ = at(new StringLiteral($1)); free($1); }
      | VARARG                   { $$ = at(new Vararg()); }
      | FUNCTION funcbody        {
            auto* f = at(new FunctionExpr());
            f->params = std::move($2->params); f->isVararg = $2->vararg;
            f->body = std::move($2->body); delete $2;
            $$ = f;
        }
      | prefixexp                { $$ = $1; }
      | tableconstructor         { $$ = $1; }
      | exp PLUS   exp           { $$ = bin(BinOp::Add,      $1, $3); }
      | exp MINUS  exp           { $$ = bin(BinOp::Sub,      $1, $3); }
      | exp MULT   exp           { $$ = bin(BinOp::Mul,      $1, $3); }
      | exp DIV    exp           { $$ = bin(BinOp::Div,      $1, $3); }
      | exp IDIV   exp           { $$ = bin(BinOp::FloorDiv, $1, $3); }
      | exp MOD    exp           { $$ = bin(BinOp::Mod,      $1, $3); }
      | exp POW    exp           { $$ = bin(BinOp::Pow,      $1, $3); }
      | exp CONCAT exp           { $$ = bin(BinOp::Concat,   $1, $3); }
      | exp EQ     exp           { $$ = bin(BinOp::Eq,       $1, $3); }
      | exp NEQ    exp           { $$ = bin(BinOp::Ne,       $1, $3); }
      | exp LT     exp           { $$ = bin(BinOp::Lt,       $1, $3); }
      | exp LE     exp           { $$ = bin(BinOp::Le,       $1, $3); }
      | exp GT     exp           { $$ = bin(BinOp::Gt,       $1, $3); }
      | exp GE     exp           { $$ = bin(BinOp::Ge,       $1, $3); }
      | exp AND    exp           { $$ = bin(BinOp::And,      $1, $3); }
      | exp OR     exp           { $$ = bin(BinOp::Or,       $1, $3); }
      | MINUS exp %prec UMINUS   { $$ = un(UnOp::Neg, $2); }
      | NOT exp                  { $$ = un(UnOp::Not, $2); }
      | LEN exp                  { $$ = un(UnOp::Len, $2); }
      ;

%%

void yyerror(const char* msg) {
    std::fprintf(stderr, "erro de sintaxe na linha %d: %s\n", linha, msg);
}
