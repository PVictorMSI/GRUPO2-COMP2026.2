/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_BUILD_PARSER_TAB_H_INCLUDED
# define YY_YY_BUILD_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 7 "parser/parser.y"

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

#line 64 "build/parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUM = 258,                     /* NUM  */
    NUM_FLOAT = 259,               /* NUM_FLOAT  */
    STRING = 260,                  /* STRING  */
    ID = 261,                      /* ID  */
    FUNCTION = 262,                /* FUNCTION  */
    ELSEIF = 263,                  /* ELSEIF  */
    ELSE = 264,                    /* ELSE  */
    IF = 265,                      /* IF  */
    OR = 266,                      /* OR  */
    UNTIL = 267,                   /* UNTIL  */
    WHILE = 268,                   /* WHILE  */
    TRUE_VAL = 269,                /* TRUE_VAL  */
    FALSE_VAL = 270,               /* FALSE_VAL  */
    NOT = 271,                     /* NOT  */
    THEN = 272,                    /* THEN  */
    NIL = 273,                     /* NIL  */
    FOR = 274,                     /* FOR  */
    DO = 275,                      /* DO  */
    RETURN = 276,                  /* RETURN  */
    LOCAL = 277,                   /* LOCAL  */
    BREAK = 278,                   /* BREAK  */
    REPEAT = 279,                  /* REPEAT  */
    IN = 280,                      /* IN  */
    END = 281,                     /* END  */
    AND = 282,                     /* AND  */
    VARARG = 283,                  /* VARARG  */
    CONCAT = 284,                  /* CONCAT  */
    DOT = 285,                     /* DOT  */
    EQ = 286,                      /* EQ  */
    NEQ = 287,                     /* NEQ  */
    GE = 288,                      /* GE  */
    LE = 289,                      /* LE  */
    GT = 290,                      /* GT  */
    LT = 291,                      /* LT  */
    ASSIGN = 292,                  /* ASSIGN  */
    PLUS = 293,                    /* PLUS  */
    MINUS = 294,                   /* MINUS  */
    MULT = 295,                    /* MULT  */
    IDIV = 296,                    /* IDIV  */
    DIV = 297,                     /* DIV  */
    MOD = 298,                     /* MOD  */
    POW = 299,                     /* POW  */
    LEN = 300,                     /* LEN  */
    SEMI = 301,                    /* SEMI  */
    COLON = 302,                   /* COLON  */
    COMMA = 303,                   /* COMMA  */
    LPAREN = 304,                  /* LPAREN  */
    RPAREN = 305,                  /* RPAREN  */
    LBRACKET = 306,                /* LBRACKET  */
    RBRACKET = 307,                /* RBRACKET  */
    LBRACE = 308,                  /* LBRACE  */
    RBRACE = 309,                  /* RBRACE  */
    UMINUS = 310                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 50 "parser/parser.y"

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

#line 152 "build/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_BUILD_PARSER_TAB_H_INCLUDED  */
