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

#ifndef YY_YY_TESTE_TAB_H_INCLUDED
# define YY_YY_TESTE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    FUNCTION = 258,                /* FUNCTION  */
    ELSEIF = 259,                  /* ELSEIF  */
    ELSE = 260,                    /* ELSE  */
    IF = 261,                      /* IF  */
    OR = 262,                      /* OR  */
    UNTIL = 263,                   /* UNTIL  */
    WHILE = 264,                   /* WHILE  */
    TRUE_VAL = 265,                /* TRUE_VAL  */
    FALSE_VAL = 266,               /* FALSE_VAL  */
    NOT = 267,                     /* NOT  */
    THEN = 268,                    /* THEN  */
    NIL = 269,                     /* NIL  */
    FOR = 270,                     /* FOR  */
    DO = 271,                      /* DO  */
    RETURN = 272,                  /* RETURN  */
    LOCAL = 273,                   /* LOCAL  */
    BREAK = 274,                   /* BREAK  */
    REPEAT = 275,                  /* REPEAT  */
    IN = 276,                      /* IN  */
    END = 277,                     /* END  */
    AND = 278,                     /* AND  */
    VARARG = 279,                  /* VARARG  */
    CONCAT = 280,                  /* CONCAT  */
    DOT = 281,                     /* DOT  */
    EQ = 282,                      /* EQ  */
    NEQ = 283,                     /* NEQ  */
    GE = 284,                      /* GE  */
    GT = 285,                      /* GT  */
    LT = 286,                      /* LT  */
    LE = 287,                      /* LE  */
    ASSIGN = 288,                  /* ASSIGN  */
    PLUS = 289,                    /* PLUS  */
    MINUS = 290,                   /* MINUS  */
    MULT = 291,                    /* MULT  */
    IDIV = 292,                    /* IDIV  */
    DIV = 293,                     /* DIV  */
    MOD = 294,                     /* MOD  */
    POW = 295,                     /* POW  */
    LEN = 296,                     /* LEN  */
    SEMI = 297,                    /* SEMI  */
    COLON = 298,                   /* COLON  */
    COMMA = 299,                   /* COMMA  */
    LPAREN = 300,                  /* LPAREN  */
    RPAREN = 301,                  /* RPAREN  */
    RBRACKET = 302,                /* RBRACKET  */
    LBRACE = 303,                  /* LBRACE  */
    RBRACE = 304,                  /* RBRACE  */
    LBRACKET = 305,                /* LBRACKET  */
    NUM = 306,                     /* NUM  */
    NUM_FLOAT = 307,               /* NUM_FLOAT  */
    ID = 308,                      /* ID  */
    STRING = 309                   /* STRING  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 12 "teste.y"

    int intValue;
    float floatValue;
    char *strValue;

#line 124 "teste.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TESTE_TAB_H_INCLUDED  */
