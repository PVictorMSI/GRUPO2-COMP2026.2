/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 22 "parser/parser.y"

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

#line 99 "build/parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUM = 3,                        /* NUM  */
  YYSYMBOL_NUM_FLOAT = 4,                  /* NUM_FLOAT  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_ID = 6,                         /* ID  */
  YYSYMBOL_FUNCTION = 7,                   /* FUNCTION  */
  YYSYMBOL_ELSEIF = 8,                     /* ELSEIF  */
  YYSYMBOL_ELSE = 9,                       /* ELSE  */
  YYSYMBOL_IF = 10,                        /* IF  */
  YYSYMBOL_OR = 11,                        /* OR  */
  YYSYMBOL_UNTIL = 12,                     /* UNTIL  */
  YYSYMBOL_WHILE = 13,                     /* WHILE  */
  YYSYMBOL_TRUE_VAL = 14,                  /* TRUE_VAL  */
  YYSYMBOL_FALSE_VAL = 15,                 /* FALSE_VAL  */
  YYSYMBOL_NOT = 16,                       /* NOT  */
  YYSYMBOL_THEN = 17,                      /* THEN  */
  YYSYMBOL_NIL = 18,                       /* NIL  */
  YYSYMBOL_FOR = 19,                       /* FOR  */
  YYSYMBOL_DO = 20,                        /* DO  */
  YYSYMBOL_RETURN = 21,                    /* RETURN  */
  YYSYMBOL_LOCAL = 22,                     /* LOCAL  */
  YYSYMBOL_BREAK = 23,                     /* BREAK  */
  YYSYMBOL_REPEAT = 24,                    /* REPEAT  */
  YYSYMBOL_IN = 25,                        /* IN  */
  YYSYMBOL_END = 26,                       /* END  */
  YYSYMBOL_AND = 27,                       /* AND  */
  YYSYMBOL_VARARG = 28,                    /* VARARG  */
  YYSYMBOL_CONCAT = 29,                    /* CONCAT  */
  YYSYMBOL_DOT = 30,                       /* DOT  */
  YYSYMBOL_EQ = 31,                        /* EQ  */
  YYSYMBOL_NEQ = 32,                       /* NEQ  */
  YYSYMBOL_GE = 33,                        /* GE  */
  YYSYMBOL_LE = 34,                        /* LE  */
  YYSYMBOL_GT = 35,                        /* GT  */
  YYSYMBOL_LT = 36,                        /* LT  */
  YYSYMBOL_ASSIGN = 37,                    /* ASSIGN  */
  YYSYMBOL_PLUS = 38,                      /* PLUS  */
  YYSYMBOL_MINUS = 39,                     /* MINUS  */
  YYSYMBOL_MULT = 40,                      /* MULT  */
  YYSYMBOL_IDIV = 41,                      /* IDIV  */
  YYSYMBOL_DIV = 42,                       /* DIV  */
  YYSYMBOL_MOD = 43,                       /* MOD  */
  YYSYMBOL_POW = 44,                       /* POW  */
  YYSYMBOL_LEN = 45,                       /* LEN  */
  YYSYMBOL_SEMI = 46,                      /* SEMI  */
  YYSYMBOL_COLON = 47,                     /* COLON  */
  YYSYMBOL_COMMA = 48,                     /* COMMA  */
  YYSYMBOL_LPAREN = 49,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 50,                    /* RPAREN  */
  YYSYMBOL_LBRACKET = 51,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 52,                  /* RBRACKET  */
  YYSYMBOL_LBRACE = 53,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 54,                    /* RBRACE  */
  YYSYMBOL_UMINUS = 55,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 56,                  /* $accept  */
  YYSYMBOL_chunk = 57,                     /* chunk  */
  YYSYMBOL_block = 58,                     /* block  */
  YYSYMBOL_stmt_list = 59,                 /* stmt_list  */
  YYSYMBOL_retstat = 60,                   /* retstat  */
  YYSYMBOL_opt_semi = 61,                  /* opt_semi  */
  YYSYMBOL_stmt = 62,                      /* stmt  */
  YYSYMBOL_elseif_list = 63,               /* elseif_list  */
  YYSYMBOL_else_opt = 64,                  /* else_opt  */
  YYSYMBOL_funcname = 65,                  /* funcname  */
  YYSYMBOL_dotted_name = 66,               /* dotted_name  */
  YYSYMBOL_funcbody = 67,                  /* funcbody  */
  YYSYMBOL_parlist = 68,                   /* parlist  */
  YYSYMBOL_namelist = 69,                  /* namelist  */
  YYSYMBOL_varlist = 70,                   /* varlist  */
  YYSYMBOL_explist = 71,                   /* explist  */
  YYSYMBOL_prefixexp = 72,                 /* prefixexp  */
  YYSYMBOL_args = 73,                      /* args  */
  YYSYMBOL_tableconstructor = 74,          /* tableconstructor  */
  YYSYMBOL_fieldlist = 75,                 /* fieldlist  */
  YYSYMBOL_fieldsep = 76,                  /* fieldsep  */
  YYSYMBOL_field = 77,                     /* field  */
  YYSYMBOL_exp = 78                        /* exp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   592

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  93
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  183

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   310


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   102,   102,   105,   106,   109,   110,   113,   114,   121,
     121,   123,   124,   130,   137,   138,   141,   144,   152,   158,
     164,   171,   178,   185,   190,   196,   199,   200,   205,   206,
     209,   210,   213,   214,   217,   225,   226,   227,   228,   231,
     232,   235,   239,   245,   246,   250,   251,   254,   258,   262,
     267,   270,   271,   272,   273,   276,   277,   281,   287,   288,
     294,   294,   296,   299,   304,   309,   310,   311,   312,   313,
     314,   315,   316,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUM", "NUM_FLOAT",
  "STRING", "ID", "FUNCTION", "ELSEIF", "ELSE", "IF", "OR", "UNTIL",
  "WHILE", "TRUE_VAL", "FALSE_VAL", "NOT", "THEN", "NIL", "FOR", "DO",
  "RETURN", "LOCAL", "BREAK", "REPEAT", "IN", "END", "AND", "VARARG",
  "CONCAT", "DOT", "EQ", "NEQ", "GE", "LE", "GT", "LT", "ASSIGN", "PLUS",
  "MINUS", "MULT", "IDIV", "DIV", "MOD", "POW", "LEN", "SEMI", "COLON",
  "COMMA", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "LBRACE", "RBRACE",
  "UMINUS", "$accept", "chunk", "block", "stmt_list", "retstat",
  "opt_semi", "stmt", "elseif_list", "else_opt", "funcname", "dotted_name",
  "funcbody", "parlist", "namelist", "varlist", "explist", "prefixexp",
  "args", "tableconstructor", "fieldlist", "fieldsep", "field", "exp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-30)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-42)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -30,    24,   -30,   248,   -30,   -30,    28,   235,   235,    38,
     -30,   179,    55,   -30,   -30,   -30,   235,   -30,   -30,    30,
      21,   -30,    10,     8,   -30,   -30,   -30,    10,   -30,   -30,
     235,   -30,   -30,   235,   235,     3,   105,   -30,   409,   435,
      -7,   -13,    23,   -30,   -30,   -19,   514,   -30,    69,    61,
      65,   323,   235,    14,   -30,    95,    99,   184,   235,   -30,
     -30,     9,   -30,   100,   102,   -30,    67,    67,    67,    86,
     235,   -30,    74,   -30,   514,   235,   -30,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   -30,   235,   235,   106,   -30,   235,   -30,    10,
     235,   235,   -30,    76,   105,   -30,    -3,   -30,    -5,   271,
     -30,    77,    81,   -30,   -30,   235,   297,   -30,   -30,   -30,
     127,   532,   -30,   548,    75,    75,    75,    75,    75,    75,
      75,   173,   173,    67,    67,    67,    67,    67,   110,   375,
     -15,   -30,   514,   -30,    76,   514,   -30,   -30,   -30,   -30,
      19,   514,    88,   -30,   -30,    71,   -30,   235,   -30,   111,
     -30,   235,   235,   -30,   112,   349,   114,   -30,   514,   469,
     -30,   -30,   -30,   235,   -30,   -30,   118,   495,   -30,   -30,
     -30,   120,   -30
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     2,     3,     1,    45,     0,     0,     0,     0,
       5,     9,     0,    25,     5,    11,     0,     4,     6,     0,
      13,    32,     0,    30,    68,    69,    70,     0,    67,    66,
       0,    65,    71,     0,     0,     0,    73,    74,     0,     0,
      39,     0,     0,    10,     7,     9,    43,    39,     0,    23,
       0,     0,     0,     0,    54,     0,     0,     0,     0,    48,
      53,    35,    21,     0,     0,    72,    92,    91,    93,    45,
       0,    55,     0,    58,    64,     0,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     0,     0,     0,    14,     0,     8,     0,
       0,     0,    50,    12,    42,    47,     0,    51,     0,     0,
      38,     0,    36,    33,    31,     0,     0,    61,    60,    56,
       0,    90,    26,    89,    82,    83,    84,    88,    86,    87,
      85,    75,    76,    77,    79,    78,    80,    81,     0,     0,
       0,    40,    44,    22,    24,    16,    49,    52,    46,     5,
       0,    63,     0,    57,    59,    28,    15,     0,     5,     0,
      37,     0,     0,     5,     0,     0,     0,    34,    62,     0,
      29,    17,     5,     0,    20,     5,     0,     0,    27,    18,
       5,     0,    19
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -30,   -30,   -10,   -30,   -30,   104,   -30,   -30,   -30,   -30,
     -30,   -26,   -30,    -1,   -30,   -29,     0,    41,    -4,   -30,
     -30,    31,     6
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     3,    17,    44,    18,   155,   164,    22,
      23,    62,   111,    41,    19,    45,    36,    59,    37,    72,
     120,    73,    46
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,    65,    54,    20,    50,   158,    24,    25,    26,    69,
      27,    49,    94,    38,    39,    47,    60,    28,    29,    30,
       5,    31,    51,   103,     4,   141,    54,    43,   108,    97,
      93,    32,    60,    97,    21,    95,    66,   110,    63,    67,
      68,    74,    33,    97,    40,   147,    57,   160,    34,    96,
      35,    55,    16,   104,    70,    64,    35,    71,   -41,    61,
     112,    47,    48,    16,   109,   140,   122,    52,    56,   -41,
      57,   144,    58,   143,    35,    99,   116,   101,    53,   162,
     163,   121,   138,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   100,   139,
      60,   105,    60,   142,    78,   106,   113,   145,   114,    95,
      54,    91,   141,    85,    86,    87,    88,    89,    90,    91,
     117,   151,   118,   115,    97,   161,    74,   149,   119,   150,
      24,    25,    26,    69,    27,    55,   156,   167,   171,   159,
     174,    28,    29,    30,   179,    31,   182,   146,   166,    98,
       0,   154,    56,   170,    57,    32,    58,     0,    35,     0,
       0,     0,   176,   165,     0,   178,    33,   168,   169,     0,
     181,     0,    34,     0,     0,     0,    16,     0,    70,   177,
      35,   153,    24,    25,    26,     5,    27,    24,    25,    26,
       5,    27,     0,    28,    29,    30,     0,    31,    28,    29,
      30,     0,    31,     0,     0,     0,     0,    32,     0,     0,
       0,     0,    32,    87,    88,    89,    90,    91,    33,     0,
       0,     0,     0,    33,    34,    43,     0,     0,    16,    34,
       0,     0,    35,    16,   107,     0,     0,    35,    24,    25,
      26,     5,    27,     0,     0,     0,     0,     0,     0,    28,
      29,    30,     0,    31,     5,     6,     0,     0,     7,     0,
       0,     8,     0,    32,     0,     0,     0,     9,    10,    11,
      12,    13,    14,     0,    33,     0,     0,     0,     0,     0,
      34,     0,    75,     0,    16,     0,     0,     0,    35,     0,
       0,     0,     0,     0,    15,     0,     0,    16,    77,     0,
      78,     0,    79,    80,    81,    82,    83,    84,    75,    85,
      86,    87,    88,    89,    90,    91,     0,     0,     0,     0,
       0,     0,     0,   148,    77,     0,    78,     0,    79,    80,
      81,    82,    83,    84,    75,    85,    86,    87,    88,    89,
      90,    91,     0,     0,     0,     0,     0,     0,     0,   152,
      77,     0,    78,     0,    79,    80,    81,    82,    83,    84,
      75,    85,    86,    87,    88,    89,    90,    91,     0,   172,
       0,     0,     0,   102,     0,     0,    77,     0,    78,     0,
      79,    80,    81,    82,    83,    84,    75,    85,    86,    87,
      88,    89,    90,    91,     0,     0,     0,   173,     0,     0,
       0,     0,    77,     0,    78,     0,    79,    80,    81,    82,
      83,    84,     0,    85,    86,    87,    88,    89,    90,    91,
      75,     0,     0,   157,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,     0,    78,     0,
      79,    80,    81,    82,    83,    84,    75,    85,    86,    87,
      88,    89,    90,    91,     0,    92,     0,     0,     0,     0,
       0,     0,    77,     0,    78,     0,    79,    80,    81,    82,
      83,    84,     0,    85,    86,    87,    88,    89,    90,    91,
      75,     0,     0,     0,     0,     0,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,     0,    78,     0,
      79,    80,    81,    82,    83,    84,    75,    85,    86,    87,
      88,    89,    90,    91,     0,   180,     0,     0,     0,     0,
       0,     0,    77,     0,    78,    75,    79,    80,    81,    82,
      83,    84,     0,    85,    86,    87,    88,    89,    90,    91,
       0,    77,     0,    78,     0,    79,    80,    81,    82,    83,
      84,     0,    85,    86,    87,    88,    89,    90,    91,    77,
       0,    78,     0,    79,    80,    81,    82,    83,    84,     0,
      85,    86,    87,    88,    89,    90,    91,    78,     0,    79,
      80,    81,    82,    83,    84,     0,    85,    86,    87,    88,
      89,    90,    91
};

static const yytype_int16 yycheck[] =
{
      10,    27,     5,     3,    14,    20,     3,     4,     5,     6,
       7,    12,    25,     7,     8,     6,    20,    14,    15,    16,
       6,    18,    16,    52,     0,     6,     5,    46,    57,    48,
      37,    28,    36,    48,     6,    48,    30,    28,    30,    33,
      34,    35,    39,    48,     6,    50,    49,    28,    45,    26,
      53,    30,    49,    53,    51,    47,    53,    54,    37,    49,
      61,     6,     7,    49,    58,    94,    76,    37,    47,    48,
      49,   100,    51,    99,    53,     6,    70,    12,    48,     8,
       9,    75,    92,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    37,    93,
     104,     6,   106,    97,    29,     6,     6,   101,     6,    48,
       5,    44,     6,    38,    39,    40,    41,    42,    43,    44,
      46,   115,    48,    37,    48,    37,   120,    50,    54,    48,
       3,     4,     5,     6,     7,    30,    26,    26,    26,   149,
      26,    14,    15,    16,    26,    18,    26,   106,   158,    45,
      -1,   120,    47,   163,    49,    28,    51,    -1,    53,    -1,
      -1,    -1,   172,   157,    -1,   175,    39,   161,   162,    -1,
     180,    -1,    45,    -1,    -1,    -1,    49,    -1,    51,   173,
      53,    54,     3,     4,     5,     6,     7,     3,     4,     5,
       6,     7,    -1,    14,    15,    16,    -1,    18,    14,    15,
      16,    -1,    18,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    28,    40,    41,    42,    43,    44,    39,    -1,
      -1,    -1,    -1,    39,    45,    46,    -1,    -1,    49,    45,
      -1,    -1,    53,    49,    50,    -1,    -1,    53,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    -1,    18,     6,     7,    -1,    -1,    10,    -1,
      -1,    13,    -1,    28,    -1,    -1,    -1,    19,    20,    21,
      22,    23,    24,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    11,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    11,    38,
      39,    40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    27,    -1,    29,    -1,    31,    32,
      33,    34,    35,    36,    11,    38,    39,    40,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      27,    -1,    29,    -1,    31,    32,    33,    34,    35,    36,
      11,    38,    39,    40,    41,    42,    43,    44,    -1,    20,
      -1,    -1,    -1,    50,    -1,    -1,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    11,    38,    39,    40,
      41,    42,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      11,    -1,    -1,    48,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    11,    38,    39,    40,
      41,    42,    43,    44,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      11,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    11,    38,    39,    40,
      41,    42,    43,    44,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    29,    11,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      -1,    27,    -1,    29,    -1,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    27,
      -1,    29,    -1,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    57,    58,    59,     0,     6,     7,    10,    13,    19,
      20,    21,    22,    23,    24,    46,    49,    60,    62,    70,
      72,     6,    65,    66,     3,     4,     5,     7,    14,    15,
      16,    18,    28,    39,    45,    53,    72,    74,    78,    78,
       6,    69,    58,    46,    61,    71,    78,     6,     7,    69,
      58,    78,    37,    48,     5,    30,    47,    49,    51,    73,
      74,    49,    67,    30,    47,    67,    78,    78,    78,     6,
      51,    54,    75,    77,    78,    11,    17,    27,    29,    31,
      32,    33,    34,    35,    36,    38,    39,    40,    41,    42,
      43,    44,    20,    37,    25,    48,    26,    48,    61,     6,
      37,    12,    50,    71,    72,     6,     6,    50,    71,    78,
      28,    68,    69,     6,     6,    37,    78,    46,    48,    54,
      76,    78,    58,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    58,    78,
      71,     6,    78,    67,    71,    78,    73,    50,    52,    50,
      48,    78,    52,    54,    77,    63,    26,    48,    20,    58,
      28,    37,     8,     9,    64,    78,    58,    26,    78,    78,
      58,    26,    20,    48,    26,    17,    58,    78,    58,    26,
      20,    58,    26
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    56,    57,    58,    58,    59,    59,    60,    60,    61,
      61,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    63,    63,    64,    64,
      65,    65,    66,    66,    67,    68,    68,    68,    68,    69,
      69,    70,    70,    71,    71,    72,    72,    72,    72,    72,
      72,    73,    73,    73,    73,    74,    74,    74,    75,    75,
      76,    76,    77,    77,    77,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     0,     2,     2,     3,     0,
       1,     1,     3,     1,     3,     5,     4,     7,     9,    11,
       7,     3,     4,     2,     4,     1,     0,     5,     0,     2,
       1,     3,     1,     3,     5,     0,     1,     3,     1,     1,
       3,     1,     3,     1,     3,     1,     4,     3,     2,     4,
       3,     2,     3,     1,     1,     2,     3,     4,     1,     3,
       1,     1,     5,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* chunk: block  */
#line 102 "parser/parser.y"
                                  { root = (yyvsp[0].block); (yyval.block) = (yyvsp[0].block); }
#line 1368 "build/parser.tab.c"
    break;

  case 3: /* block: stmt_list  */
#line 105 "parser/parser.y"
                                  { (yyval.block) = (yyvsp[0].block); }
#line 1374 "build/parser.tab.c"
    break;

  case 4: /* block: stmt_list retstat  */
#line 106 "parser/parser.y"
                                  { (yyvsp[-1].block)->statements.emplace_back((yyvsp[0].stmt)); (yyval.block) = (yyvsp[-1].block); }
#line 1380 "build/parser.tab.c"
    break;

  case 5: /* stmt_list: %empty  */
#line 109 "parser/parser.y"
                                  { (yyval.block) = at(new Block()); }
#line 1386 "build/parser.tab.c"
    break;

  case 6: /* stmt_list: stmt_list stmt  */
#line 110 "parser/parser.y"
                                  { if ((yyvsp[0].stmt)) (yyvsp[-1].block)->statements.emplace_back((yyvsp[0].stmt)); (yyval.block) = (yyvsp[-1].block); }
#line 1392 "build/parser.tab.c"
    break;

  case 7: /* retstat: RETURN opt_semi  */
#line 113 "parser/parser.y"
                                  { (yyval.stmt) = at(new Return()); }
#line 1398 "build/parser.tab.c"
    break;

  case 8: /* retstat: RETURN explist opt_semi  */
#line 114 "parser/parser.y"
                                  {
              auto* r = at(new Return());
              r->values = std::move((yyvsp[-1].exprs)->v); delete (yyvsp[-1].exprs);
              (yyval.stmt) = r;
          }
#line 1408 "build/parser.tab.c"
    break;

  case 11: /* stmt: SEMI  */
#line 123 "parser/parser.y"
                                  { (yyval.stmt) = nullptr; }
#line 1414 "build/parser.tab.c"
    break;

  case 12: /* stmt: varlist ASSIGN explist  */
#line 124 "parser/parser.y"
                                  {
            auto* a = at(new Assign());
            a->targets = std::move((yyvsp[-2].exprs)->v); delete (yyvsp[-2].exprs);
            a->values  = std::move((yyvsp[0].exprs)->v); delete (yyvsp[0].exprs);
            (yyval.stmt) = a;
        }
#line 1425 "build/parser.tab.c"
    break;

  case 13: /* stmt: prefixexp  */
#line 130 "parser/parser.y"
                                  {
            if ((yyvsp[0].expr)->kind != NodeKind::FunctionCall) {
                yyerror("expressao nao e um comando valido (esperava chamada de funcao ou atribuicao)");
                YYERROR;
            }
            auto* c = at(new CallStmt()); c->call.reset((yyvsp[0].expr)); (yyval.stmt) = c;
        }
#line 1437 "build/parser.tab.c"
    break;

  case 14: /* stmt: DO block END  */
#line 137 "parser/parser.y"
                                 { auto* d = at(new Do()); d->body.reset((yyvsp[-1].block)); (yyval.stmt) = d; }
#line 1443 "build/parser.tab.c"
    break;

  case 15: /* stmt: WHILE exp DO block END  */
#line 138 "parser/parser.y"
                                 {
            auto* w = at(new While()); w->condition.reset((yyvsp[-3].expr)); w->body.reset((yyvsp[-1].block)); (yyval.stmt) = w;
        }
#line 1451 "build/parser.tab.c"
    break;

  case 16: /* stmt: REPEAT block UNTIL exp  */
#line 141 "parser/parser.y"
                                 {
            auto* r = at(new RepeatUntil()); r->body.reset((yyvsp[-2].block)); r->condition.reset((yyvsp[0].expr)); (yyval.stmt) = r;
        }
#line 1459 "build/parser.tab.c"
    break;

  case 17: /* stmt: IF exp THEN block elseif_list else_opt END  */
#line 144 "parser/parser.y"
                                                   {
            auto* n = at(new If());
            n->clauses.push_back(IfClause{ExprPtr((yyvsp[-5].expr)), BlockPtr((yyvsp[-3].block))});
            for (auto& c : (yyvsp[-2].clauses)->v) n->clauses.push_back(std::move(c));
            delete (yyvsp[-2].clauses);
            if ((yyvsp[-1].block)) n->clauses.push_back(IfClause{nullptr, BlockPtr((yyvsp[-1].block))});
            (yyval.stmt) = n;
        }
#line 1472 "build/parser.tab.c"
    break;

  case 18: /* stmt: FOR ID ASSIGN exp COMMA exp DO block END  */
#line 152 "parser/parser.y"
                                                 {
            auto* f = at(new NumericFor());
            f->varName = (yyvsp[-7].strValue); free((yyvsp[-7].strValue));
            f->start.reset((yyvsp[-5].expr)); f->stop.reset((yyvsp[-3].expr)); f->body.reset((yyvsp[-1].block));
            (yyval.stmt) = f;
        }
#line 1483 "build/parser.tab.c"
    break;

  case 19: /* stmt: FOR ID ASSIGN exp COMMA exp COMMA exp DO block END  */
#line 158 "parser/parser.y"
                                                           {
            auto* f = at(new NumericFor());
            f->varName = (yyvsp[-9].strValue); free((yyvsp[-9].strValue));
            f->start.reset((yyvsp[-7].expr)); f->stop.reset((yyvsp[-5].expr)); f->step.reset((yyvsp[-3].expr)); f->body.reset((yyvsp[-1].block));
            (yyval.stmt) = f;
        }
#line 1494 "build/parser.tab.c"
    break;

  case 20: /* stmt: FOR namelist IN explist DO block END  */
#line 164 "parser/parser.y"
                                             {
            auto* f = at(new GenericFor());
            f->names = std::move((yyvsp[-5].names)->v); delete (yyvsp[-5].names);
            f->exprs = std::move((yyvsp[-3].exprs)->v); delete (yyvsp[-3].exprs);
            f->body.reset((yyvsp[-1].block));
            (yyval.stmt) = f;
        }
#line 1506 "build/parser.tab.c"
    break;

  case 21: /* stmt: FUNCTION funcname funcbody  */
#line 171 "parser/parser.y"
                                   {
            auto* f = at(new FunctionDecl());
            f->name = *(yyvsp[-1].sname); delete (yyvsp[-1].sname);
            f->params = std::move((yyvsp[0].fbody)->params); f->isVararg = (yyvsp[0].fbody)->vararg;
            f->body = std::move((yyvsp[0].fbody)->body); delete (yyvsp[0].fbody);
            (yyval.stmt) = f;
        }
#line 1518 "build/parser.tab.c"
    break;

  case 22: /* stmt: LOCAL FUNCTION ID funcbody  */
#line 178 "parser/parser.y"
                                   {
            auto* f = at(new LocalFunctionDecl());
            f->name = (yyvsp[-1].strValue); free((yyvsp[-1].strValue));
            f->params = std::move((yyvsp[0].fbody)->params); f->isVararg = (yyvsp[0].fbody)->vararg;
            f->body = std::move((yyvsp[0].fbody)->body); delete (yyvsp[0].fbody);
            (yyval.stmt) = f;
        }
#line 1530 "build/parser.tab.c"
    break;

  case 23: /* stmt: LOCAL namelist  */
#line 185 "parser/parser.y"
                                 {
            auto* d = at(new LocalDecl());
            d->names = std::move((yyvsp[0].names)->v); delete (yyvsp[0].names);
            (yyval.stmt) = d;
        }
#line 1540 "build/parser.tab.c"
    break;

  case 24: /* stmt: LOCAL namelist ASSIGN explist  */
#line 190 "parser/parser.y"
                                      {
            auto* d = at(new LocalDecl());
            d->names  = std::move((yyvsp[-2].names)->v); delete (yyvsp[-2].names);
            d->values = std::move((yyvsp[0].exprs)->v); delete (yyvsp[0].exprs);
            (yyval.stmt) = d;
        }
#line 1551 "build/parser.tab.c"
    break;

  case 25: /* stmt: BREAK  */
#line 196 "parser/parser.y"
                                 { (yyval.stmt) = at(new Break()); }
#line 1557 "build/parser.tab.c"
    break;

  case 26: /* elseif_list: %empty  */
#line 199 "parser/parser.y"
                                 { (yyval.clauses) = new ClauseList(); }
#line 1563 "build/parser.tab.c"
    break;

  case 27: /* elseif_list: elseif_list ELSEIF exp THEN block  */
#line 200 "parser/parser.y"
                                                {
                  (yyvsp[-4].clauses)->v.push_back(IfClause{ExprPtr((yyvsp[-2].expr)), BlockPtr((yyvsp[0].block))}); (yyval.clauses) = (yyvsp[-4].clauses);
              }
#line 1571 "build/parser.tab.c"
    break;

  case 28: /* else_opt: %empty  */
#line 205 "parser/parser.y"
                                 { (yyval.block) = nullptr; }
#line 1577 "build/parser.tab.c"
    break;

  case 29: /* else_opt: ELSE block  */
#line 206 "parser/parser.y"
                                 { (yyval.block) = (yyvsp[0].block); }
#line 1583 "build/parser.tab.c"
    break;

  case 30: /* funcname: dotted_name  */
#line 209 "parser/parser.y"
                                 { (yyval.sname) = (yyvsp[0].sname); }
#line 1589 "build/parser.tab.c"
    break;

  case 31: /* funcname: dotted_name COLON ID  */
#line 210 "parser/parser.y"
                                 { *(yyvsp[-2].sname) += ":"; *(yyvsp[-2].sname) += (yyvsp[0].strValue); free((yyvsp[0].strValue)); (yyval.sname) = (yyvsp[-2].sname); }
#line 1595 "build/parser.tab.c"
    break;

  case 32: /* dotted_name: ID  */
#line 213 "parser/parser.y"
                                 { (yyval.sname) = new std::string((yyvsp[0].strValue)); free((yyvsp[0].strValue)); }
#line 1601 "build/parser.tab.c"
    break;

  case 33: /* dotted_name: dotted_name DOT ID  */
#line 214 "parser/parser.y"
                                 { *(yyvsp[-2].sname) += "."; *(yyvsp[-2].sname) += (yyvsp[0].strValue); free((yyvsp[0].strValue)); (yyval.sname) = (yyvsp[-2].sname); }
#line 1607 "build/parser.tab.c"
    break;

  case 34: /* funcbody: LPAREN parlist RPAREN block END  */
#line 217 "parser/parser.y"
                                           {
               auto* f = new FuncBody();
               f->params = std::move((yyvsp[-3].params)->names); f->vararg = (yyvsp[-3].params)->vararg; delete (yyvsp[-3].params);
               f->body.reset((yyvsp[-1].block));
               (yyval.fbody) = f;
           }
#line 1618 "build/parser.tab.c"
    break;

  case 35: /* parlist: %empty  */
#line 225 "parser/parser.y"
                                 { (yyval.params) = new ParamList(); }
#line 1624 "build/parser.tab.c"
    break;

  case 36: /* parlist: namelist  */
#line 226 "parser/parser.y"
                                 { (yyval.params) = new ParamList(); (yyval.params)->names = std::move((yyvsp[0].names)->v); delete (yyvsp[0].names); }
#line 1630 "build/parser.tab.c"
    break;

  case 37: /* parlist: namelist COMMA VARARG  */
#line 227 "parser/parser.y"
                                 { (yyval.params) = new ParamList(); (yyval.params)->names = std::move((yyvsp[-2].names)->v); delete (yyvsp[-2].names); (yyval.params)->vararg = true; }
#line 1636 "build/parser.tab.c"
    break;

  case 38: /* parlist: VARARG  */
#line 228 "parser/parser.y"
                                 { (yyval.params) = new ParamList(); (yyval.params)->vararg = true; }
#line 1642 "build/parser.tab.c"
    break;

  case 39: /* namelist: ID  */
#line 231 "parser/parser.y"
                                 { (yyval.names) = new NameList(); (yyval.names)->v.push_back((yyvsp[0].strValue)); free((yyvsp[0].strValue)); }
#line 1648 "build/parser.tab.c"
    break;

  case 40: /* namelist: namelist COMMA ID  */
#line 232 "parser/parser.y"
                                 { (yyvsp[-2].names)->v.push_back((yyvsp[0].strValue)); free((yyvsp[0].strValue)); (yyval.names) = (yyvsp[-2].names); }
#line 1654 "build/parser.tab.c"
    break;

  case 41: /* varlist: prefixexp  */
#line 235 "parser/parser.y"
                                 {
              if (!isLvalue((yyvsp[0].expr))) { yyerror("alvo de atribuicao invalido"); YYERROR; }
              (yyval.exprs) = new ExprList(); (yyval.exprs)->v.emplace_back((yyvsp[0].expr));
          }
#line 1663 "build/parser.tab.c"
    break;

  case 42: /* varlist: varlist COMMA prefixexp  */
#line 239 "parser/parser.y"
                                  {
              if (!isLvalue((yyvsp[0].expr))) { yyerror("alvo de atribuicao invalido"); YYERROR; }
              (yyvsp[-2].exprs)->v.emplace_back((yyvsp[0].expr)); (yyval.exprs) = (yyvsp[-2].exprs);
          }
#line 1672 "build/parser.tab.c"
    break;

  case 43: /* explist: exp  */
#line 245 "parser/parser.y"
                                 { (yyval.exprs) = new ExprList(); (yyval.exprs)->v.emplace_back((yyvsp[0].expr)); }
#line 1678 "build/parser.tab.c"
    break;

  case 44: /* explist: explist COMMA exp  */
#line 246 "parser/parser.y"
                                 { (yyvsp[-2].exprs)->v.emplace_back((yyvsp[0].expr)); (yyval.exprs) = (yyvsp[-2].exprs); }
#line 1684 "build/parser.tab.c"
    break;

  case 45: /* prefixexp: ID  */
#line 250 "parser/parser.y"
                                 { (yyval.expr) = at(new Identifier((yyvsp[0].strValue))); free((yyvsp[0].strValue)); }
#line 1690 "build/parser.tab.c"
    break;

  case 46: /* prefixexp: prefixexp LBRACKET exp RBRACKET  */
#line 251 "parser/parser.y"
                                            {
                auto* i = at(new Index()); i->object.reset((yyvsp[-3].expr)); i->key.reset((yyvsp[-1].expr)); (yyval.expr) = i;
            }
#line 1698 "build/parser.tab.c"
    break;

  case 47: /* prefixexp: prefixexp DOT ID  */
#line 254 "parser/parser.y"
                                 {
                auto* i = at(new Index()); i->object.reset((yyvsp[-2].expr));
                i->key.reset(at(new StringLiteral((yyvsp[0].strValue)))); free((yyvsp[0].strValue)); (yyval.expr) = i;
            }
#line 1707 "build/parser.tab.c"
    break;

  case 48: /* prefixexp: prefixexp args  */
#line 258 "parser/parser.y"
                                 {
                auto* c = at(new FunctionCall()); c->callee.reset((yyvsp[-1].expr));
                c->args = std::move((yyvsp[0].exprs)->v); delete (yyvsp[0].exprs); (yyval.expr) = c;
            }
#line 1716 "build/parser.tab.c"
    break;

  case 49: /* prefixexp: prefixexp COLON ID args  */
#line 262 "parser/parser.y"
                                    {
                auto* c = at(new FunctionCall()); c->callee.reset((yyvsp[-3].expr));
                c->isMethodCall = true; c->methodName = (yyvsp[-1].strValue); free((yyvsp[-1].strValue));
                c->args = std::move((yyvsp[0].exprs)->v); delete (yyvsp[0].exprs); (yyval.expr) = c;
            }
#line 1726 "build/parser.tab.c"
    break;

  case 50: /* prefixexp: LPAREN exp RPAREN  */
#line 267 "parser/parser.y"
                                 { (yyval.expr) = (yyvsp[-1].expr); }
#line 1732 "build/parser.tab.c"
    break;

  case 51: /* args: LPAREN RPAREN  */
#line 270 "parser/parser.y"
                                 { (yyval.exprs) = new ExprList(); }
#line 1738 "build/parser.tab.c"
    break;

  case 52: /* args: LPAREN explist RPAREN  */
#line 271 "parser/parser.y"
                                 { (yyval.exprs) = (yyvsp[-1].exprs); }
#line 1744 "build/parser.tab.c"
    break;

  case 53: /* args: tableconstructor  */
#line 272 "parser/parser.y"
                                 { (yyval.exprs) = new ExprList(); (yyval.exprs)->v.emplace_back((yyvsp[0].expr)); }
#line 1750 "build/parser.tab.c"
    break;

  case 54: /* args: STRING  */
#line 273 "parser/parser.y"
                                 { (yyval.exprs) = new ExprList(); (yyval.exprs)->v.emplace_back(at(new StringLiteral((yyvsp[0].strValue)))); free((yyvsp[0].strValue)); }
#line 1756 "build/parser.tab.c"
    break;

  case 55: /* tableconstructor: LBRACE RBRACE  */
#line 276 "parser/parser.y"
                                 { (yyval.expr) = at(new TableConstructor()); }
#line 1762 "build/parser.tab.c"
    break;

  case 56: /* tableconstructor: LBRACE fieldlist RBRACE  */
#line 277 "parser/parser.y"
                                           {
                       auto* t = at(new TableConstructor());
                       t->fields = std::move((yyvsp[-1].fields)->v); delete (yyvsp[-1].fields); (yyval.expr) = t;
                   }
#line 1771 "build/parser.tab.c"
    break;

  case 57: /* tableconstructor: LBRACE fieldlist fieldsep RBRACE  */
#line 281 "parser/parser.y"
                                                    {
                       auto* t = at(new TableConstructor());
                       t->fields = std::move((yyvsp[-2].fields)->v); delete (yyvsp[-2].fields); (yyval.expr) = t;
                   }
#line 1780 "build/parser.tab.c"
    break;

  case 58: /* fieldlist: field  */
#line 287 "parser/parser.y"
                                 { (yyval.fields) = (yyvsp[0].fields); }
#line 1786 "build/parser.tab.c"
    break;

  case 59: /* fieldlist: fieldlist fieldsep field  */
#line 288 "parser/parser.y"
                                     {
                for (auto& f : (yyvsp[0].fields)->v) (yyvsp[-2].fields)->v.push_back(std::move(f));
                delete (yyvsp[0].fields); (yyval.fields) = (yyvsp[-2].fields);
            }
#line 1795 "build/parser.tab.c"
    break;

  case 62: /* field: LBRACKET exp RBRACKET ASSIGN exp  */
#line 296 "parser/parser.y"
                                         {
            (yyval.fields) = new FieldList(); (yyval.fields)->v.push_back(TableField{ExprPtr((yyvsp[-3].expr)), ExprPtr((yyvsp[0].expr))});
        }
#line 1803 "build/parser.tab.c"
    break;

  case 63: /* field: ID ASSIGN exp  */
#line 299 "parser/parser.y"
                      {
            (yyval.fields) = new FieldList();
            (yyval.fields)->v.push_back(TableField{ExprPtr(at(new StringLiteral((yyvsp[-2].strValue)))), ExprPtr((yyvsp[0].expr))});
            free((yyvsp[-2].strValue));
        }
#line 1813 "build/parser.tab.c"
    break;

  case 64: /* field: exp  */
#line 304 "parser/parser.y"
            {
            (yyval.fields) = new FieldList(); (yyval.fields)->v.push_back(TableField{nullptr, ExprPtr((yyvsp[0].expr))});
        }
#line 1821 "build/parser.tab.c"
    break;

  case 65: /* exp: NIL  */
#line 309 "parser/parser.y"
                                 { (yyval.expr) = at(new NilLiteral()); }
#line 1827 "build/parser.tab.c"
    break;

  case 66: /* exp: FALSE_VAL  */
#line 310 "parser/parser.y"
                                 { (yyval.expr) = at(new BoolLiteral(false)); }
#line 1833 "build/parser.tab.c"
    break;

  case 67: /* exp: TRUE_VAL  */
#line 311 "parser/parser.y"
                                 { (yyval.expr) = at(new BoolLiteral(true)); }
#line 1839 "build/parser.tab.c"
    break;

  case 68: /* exp: NUM  */
#line 312 "parser/parser.y"
                                 { (yyval.expr) = at(new NumberLiteral((yyvsp[0].intValue))); }
#line 1845 "build/parser.tab.c"
    break;

  case 69: /* exp: NUM_FLOAT  */
#line 313 "parser/parser.y"
                                 { (yyval.expr) = at(new NumberLiteral((yyvsp[0].floatValue))); }
#line 1851 "build/parser.tab.c"
    break;

  case 70: /* exp: STRING  */
#line 314 "parser/parser.y"
                                 { (yyval.expr) = at(new StringLiteral((yyvsp[0].strValue))); free((yyvsp[0].strValue)); }
#line 1857 "build/parser.tab.c"
    break;

  case 71: /* exp: VARARG  */
#line 315 "parser/parser.y"
                                 { (yyval.expr) = at(new Vararg()); }
#line 1863 "build/parser.tab.c"
    break;

  case 72: /* exp: FUNCTION funcbody  */
#line 316 "parser/parser.y"
                                 {
            auto* f = at(new FunctionExpr());
            f->params = std::move((yyvsp[0].fbody)->params); f->isVararg = (yyvsp[0].fbody)->vararg;
            f->body = std::move((yyvsp[0].fbody)->body); delete (yyvsp[0].fbody);
            (yyval.expr) = f;
        }
#line 1874 "build/parser.tab.c"
    break;

  case 73: /* exp: prefixexp  */
#line 322 "parser/parser.y"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 1880 "build/parser.tab.c"
    break;

  case 74: /* exp: tableconstructor  */
#line 323 "parser/parser.y"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 1886 "build/parser.tab.c"
    break;

  case 75: /* exp: exp PLUS exp  */
#line 324 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Add,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1892 "build/parser.tab.c"
    break;

  case 76: /* exp: exp MINUS exp  */
#line 325 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Sub,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1898 "build/parser.tab.c"
    break;

  case 77: /* exp: exp MULT exp  */
#line 326 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Mul,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1904 "build/parser.tab.c"
    break;

  case 78: /* exp: exp DIV exp  */
#line 327 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Div,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1910 "build/parser.tab.c"
    break;

  case 79: /* exp: exp IDIV exp  */
#line 328 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::FloorDiv, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1916 "build/parser.tab.c"
    break;

  case 80: /* exp: exp MOD exp  */
#line 329 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Mod,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1922 "build/parser.tab.c"
    break;

  case 81: /* exp: exp POW exp  */
#line 330 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Pow,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1928 "build/parser.tab.c"
    break;

  case 82: /* exp: exp CONCAT exp  */
#line 331 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Concat,   (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1934 "build/parser.tab.c"
    break;

  case 83: /* exp: exp EQ exp  */
#line 332 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Eq,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1940 "build/parser.tab.c"
    break;

  case 84: /* exp: exp NEQ exp  */
#line 333 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Ne,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1946 "build/parser.tab.c"
    break;

  case 85: /* exp: exp LT exp  */
#line 334 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Lt,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1952 "build/parser.tab.c"
    break;

  case 86: /* exp: exp LE exp  */
#line 335 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Le,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1958 "build/parser.tab.c"
    break;

  case 87: /* exp: exp GT exp  */
#line 336 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Gt,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1964 "build/parser.tab.c"
    break;

  case 88: /* exp: exp GE exp  */
#line 337 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Ge,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1970 "build/parser.tab.c"
    break;

  case 89: /* exp: exp AND exp  */
#line 338 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::And,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1976 "build/parser.tab.c"
    break;

  case 90: /* exp: exp OR exp  */
#line 339 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Or,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1982 "build/parser.tab.c"
    break;

  case 91: /* exp: MINUS exp  */
#line 340 "parser/parser.y"
                                 { (yyval.expr) = un(UnOp::Neg, (yyvsp[0].expr)); }
#line 1988 "build/parser.tab.c"
    break;

  case 92: /* exp: NOT exp  */
#line 341 "parser/parser.y"
                                 { (yyval.expr) = un(UnOp::Not, (yyvsp[0].expr)); }
#line 1994 "build/parser.tab.c"
    break;

  case 93: /* exp: LEN exp  */
#line 342 "parser/parser.y"
                                 { (yyval.expr) = un(UnOp::Len, (yyvsp[0].expr)); }
#line 2000 "build/parser.tab.c"
    break;


#line 2004 "build/parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 345 "parser/parser.y"


void yyerror(const char* msg) {
    std::fprintf(stderr, "erro de sintaxe na linha %d: %s\n", linha, msg);
}
