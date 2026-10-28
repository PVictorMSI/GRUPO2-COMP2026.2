/* A Bison parser, made by GNU Bison 3.7.5.  */

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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
    FUNCTION = 262,                /* "function"  */
    ELSEIF = 263,                  /* "elseif"  */
    ELSE = 264,                    /* "else"  */
    IF = 265,                      /* "if"  */
    OR = 266,                      /* "or"  */
    UNTIL = 267,                   /* "until"  */
    WHILE = 268,                   /* "while"  */
    TRUE_VAL = 269,                /* "true"  */
    FALSE_VAL = 270,               /* "false"  */
    NOT = 271,                     /* "not"  */
    THEN = 272,                    /* "then"  */
    NIL = 273,                     /* "nil"  */
    FOR = 274,                     /* "for"  */
    DO = 275,                      /* "do"  */
    RETURN = 276,                  /* "return"  */
    LOCAL = 277,                   /* "local"  */
    BREAK = 278,                   /* "break"  */
    REPEAT = 279,                  /* "repeat"  */
    IN = 280,                      /* "in"  */
    END = 281,                     /* "end"  */
    AND = 282,                     /* "and"  */
    VARARG = 283,                  /* "..."  */
    CONCAT = 284,                  /* ".."  */
    DOT = 285,                     /* "."  */
    EQ = 286,                      /* "=="  */
    NEQ = 287,                     /* "~="  */
    GE = 288,                      /* ">="  */
    LE = 289,                      /* "<="  */
    GT = 290,                      /* ">"  */
    LT = 291,                      /* "<"  */
    ASSIGN = 292,                  /* "="  */
    PLUS = 293,                    /* "+"  */
    MINUS = 294,                   /* "-"  */
    MULT = 295,                    /* "*"  */
    IDIV = 296,                    /* "//"  */
    DIV = 297,                     /* "/"  */
    MOD = 298,                     /* "%"  */
    POW = 299,                     /* "^"  */
    LEN = 300,                     /* "#"  */
    SEMI = 301,                    /* ";"  */
    COLON = 302,                   /* ":"  */
    COMMA = 303,                   /* ","  */
    LPAREN = 304,                  /* "("  */
    RPAREN = 305,                  /* ")"  */
    LBRACKET = 306,                /* "["  */
    RBRACKET = 307,                /* "]"  */
    LBRACE = 308,                  /* "{"  */
    RBRACE = 309,                  /* "}"  */
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
