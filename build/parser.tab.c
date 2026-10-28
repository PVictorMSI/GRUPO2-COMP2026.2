/* A Bison parser, made by GNU Bison 3.7.5.  */

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
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

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
  YYSYMBOL_FUNCTION = 7,                   /* "function"  */
  YYSYMBOL_ELSEIF = 8,                     /* "elseif"  */
  YYSYMBOL_ELSE = 9,                       /* "else"  */
  YYSYMBOL_IF = 10,                        /* "if"  */
  YYSYMBOL_OR = 11,                        /* "or"  */
  YYSYMBOL_UNTIL = 12,                     /* "until"  */
  YYSYMBOL_WHILE = 13,                     /* "while"  */
  YYSYMBOL_TRUE_VAL = 14,                  /* "true"  */
  YYSYMBOL_FALSE_VAL = 15,                 /* "false"  */
  YYSYMBOL_NOT = 16,                       /* "not"  */
  YYSYMBOL_THEN = 17,                      /* "then"  */
  YYSYMBOL_NIL = 18,                       /* "nil"  */
  YYSYMBOL_FOR = 19,                       /* "for"  */
  YYSYMBOL_DO = 20,                        /* "do"  */
  YYSYMBOL_RETURN = 21,                    /* "return"  */
  YYSYMBOL_LOCAL = 22,                     /* "local"  */
  YYSYMBOL_BREAK = 23,                     /* "break"  */
  YYSYMBOL_REPEAT = 24,                    /* "repeat"  */
  YYSYMBOL_IN = 25,                        /* "in"  */
  YYSYMBOL_END = 26,                       /* "end"  */
  YYSYMBOL_AND = 27,                       /* "and"  */
  YYSYMBOL_VARARG = 28,                    /* "..."  */
  YYSYMBOL_CONCAT = 29,                    /* ".."  */
  YYSYMBOL_DOT = 30,                       /* "."  */
  YYSYMBOL_EQ = 31,                        /* "=="  */
  YYSYMBOL_NEQ = 32,                       /* "~="  */
  YYSYMBOL_GE = 33,                        /* ">="  */
  YYSYMBOL_LE = 34,                        /* "<="  */
  YYSYMBOL_GT = 35,                        /* ">"  */
  YYSYMBOL_LT = 36,                        /* "<"  */
  YYSYMBOL_ASSIGN = 37,                    /* "="  */
  YYSYMBOL_PLUS = 38,                      /* "+"  */
  YYSYMBOL_MINUS = 39,                     /* "-"  */
  YYSYMBOL_MULT = 40,                      /* "*"  */
  YYSYMBOL_IDIV = 41,                      /* "//"  */
  YYSYMBOL_DIV = 42,                       /* "/"  */
  YYSYMBOL_MOD = 43,                       /* "%"  */
  YYSYMBOL_POW = 44,                       /* "^"  */
  YYSYMBOL_LEN = 45,                       /* "#"  */
  YYSYMBOL_SEMI = 46,                      /* ";"  */
  YYSYMBOL_COLON = 47,                     /* ":"  */
  YYSYMBOL_COMMA = 48,                     /* ","  */
  YYSYMBOL_LPAREN = 49,                    /* "("  */
  YYSYMBOL_RPAREN = 50,                    /* ")"  */
  YYSYMBOL_LBRACKET = 51,                  /* "["  */
  YYSYMBOL_RBRACKET = 52,                  /* "]"  */
  YYSYMBOL_LBRACE = 53,                    /* "{"  */
  YYSYMBOL_RBRACE = 54,                    /* "}"  */
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

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if 1

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
#endif /* 1 */

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
       0,   115,   115,   118,   119,   122,   123,   126,   127,   134,
     134,   136,   137,   143,   151,   152,   155,   158,   166,   172,
     178,   185,   192,   199,   204,   210,   213,   214,   219,   220,
     223,   224,   227,   228,   231,   239,   240,   241,   242,   245,
     246,   249,   253,   259,   260,   264,   265,   268,   272,   276,
     281,   284,   285,   286,   287,   290,   291,   295,   301,   302,
     308,   308,   310,   313,   318,   323,   324,   325,   326,   327,
     328,   329,   330,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUM", "NUM_FLOAT",
  "STRING", "ID", "\"function\"", "\"elseif\"", "\"else\"", "\"if\"",
  "\"or\"", "\"until\"", "\"while\"", "\"true\"", "\"false\"", "\"not\"",
  "\"then\"", "\"nil\"", "\"for\"", "\"do\"", "\"return\"", "\"local\"",
  "\"break\"", "\"repeat\"", "\"in\"", "\"end\"", "\"and\"", "\"...\"",
  "\"..\"", "\".\"", "\"==\"", "\"~=\"", "\">=\"", "\"<=\"", "\">\"",
  "\"<\"", "\"=\"", "\"+\"", "\"-\"", "\"*\"", "\"//\"", "\"/\"", "\"%\"",
  "\"^\"", "\"#\"", "\";\"", "\":\"", "\",\"", "\"(\"", "\")\"", "\"[\"",
  "\"]\"", "\"{\"", "\"}\"", "UMINUS", "$accept", "chunk", "block",
  "stmt_list", "retstat", "opt_semi", "stmt", "elseif_list", "else_opt",
  "funcname", "dotted_name", "funcbody", "parlist", "namelist", "varlist",
  "explist", "prefixexp", "args", "tableconstructor", "fieldlist",
  "fieldsep", "field", "exp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310
};
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

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
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

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
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

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
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

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


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
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


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
  switch (yykind)
    {
    case YYSYMBOL_STRING: /* STRING  */
#line 97 "parser/parser.y"
            { free(((*yyvaluep).strValue)); }
#line 1391 "build/parser.tab.c"
        break;

    case YYSYMBOL_ID: /* ID  */
#line 97 "parser/parser.y"
            { free(((*yyvaluep).strValue)); }
#line 1397 "build/parser.tab.c"
        break;

    case YYSYMBOL_chunk: /* chunk  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).block); }
#line 1403 "build/parser.tab.c"
        break;

    case YYSYMBOL_block: /* block  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).block); }
#line 1409 "build/parser.tab.c"
        break;

    case YYSYMBOL_stmt_list: /* stmt_list  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).block); }
#line 1415 "build/parser.tab.c"
        break;

    case YYSYMBOL_retstat: /* retstat  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).stmt); }
#line 1421 "build/parser.tab.c"
        break;

    case YYSYMBOL_stmt: /* stmt  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).stmt); }
#line 1427 "build/parser.tab.c"
        break;

    case YYSYMBOL_elseif_list: /* elseif_list  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).clauses); }
#line 1433 "build/parser.tab.c"
        break;

    case YYSYMBOL_else_opt: /* else_opt  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).block); }
#line 1439 "build/parser.tab.c"
        break;

    case YYSYMBOL_funcname: /* funcname  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).sname); }
#line 1445 "build/parser.tab.c"
        break;

    case YYSYMBOL_dotted_name: /* dotted_name  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).sname); }
#line 1451 "build/parser.tab.c"
        break;

    case YYSYMBOL_funcbody: /* funcbody  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).fbody); }
#line 1457 "build/parser.tab.c"
        break;

    case YYSYMBOL_parlist: /* parlist  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).params); }
#line 1463 "build/parser.tab.c"
        break;

    case YYSYMBOL_namelist: /* namelist  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).names); }
#line 1469 "build/parser.tab.c"
        break;

    case YYSYMBOL_varlist: /* varlist  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).exprs); }
#line 1475 "build/parser.tab.c"
        break;

    case YYSYMBOL_explist: /* explist  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).exprs); }
#line 1481 "build/parser.tab.c"
        break;

    case YYSYMBOL_prefixexp: /* prefixexp  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).expr); }
#line 1487 "build/parser.tab.c"
        break;

    case YYSYMBOL_args: /* args  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).exprs); }
#line 1493 "build/parser.tab.c"
        break;

    case YYSYMBOL_tableconstructor: /* tableconstructor  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).expr); }
#line 1499 "build/parser.tab.c"
        break;

    case YYSYMBOL_fieldlist: /* fieldlist  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).fields); }
#line 1505 "build/parser.tab.c"
        break;

    case YYSYMBOL_field: /* field  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).fields); }
#line 1511 "build/parser.tab.c"
        break;

    case YYSYMBOL_exp: /* exp  */
#line 96 "parser/parser.y"
            { delete ((*yyvaluep).expr); }
#line 1517 "build/parser.tab.c"
        break;

      default:
        break;
    }
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

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

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
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
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
#line 115 "parser/parser.y"
                                  { root = (yyvsp[0].block); (yyval.block) = nullptr; /* posse passa para 'root'; evita o %destructor liberar a AST ao aceitar */ }
#line 1788 "build/parser.tab.c"
    break;

  case 3: /* block: stmt_list  */
#line 118 "parser/parser.y"
                                  { (yyval.block) = (yyvsp[0].block); }
#line 1794 "build/parser.tab.c"
    break;

  case 4: /* block: stmt_list retstat  */
#line 119 "parser/parser.y"
                                  { (yyvsp[-1].block)->statements.emplace_back((yyvsp[0].stmt)); (yyval.block) = (yyvsp[-1].block); }
#line 1800 "build/parser.tab.c"
    break;

  case 5: /* stmt_list: %empty  */
#line 122 "parser/parser.y"
                                  { (yyval.block) = at(new Block()); }
#line 1806 "build/parser.tab.c"
    break;

  case 6: /* stmt_list: stmt_list stmt  */
#line 123 "parser/parser.y"
                                  { if ((yyvsp[0].stmt)) (yyvsp[-1].block)->statements.emplace_back((yyvsp[0].stmt)); (yyval.block) = (yyvsp[-1].block); }
#line 1812 "build/parser.tab.c"
    break;

  case 7: /* retstat: "return" opt_semi  */
#line 126 "parser/parser.y"
                                  { (yyval.stmt) = at(new Return()); }
#line 1818 "build/parser.tab.c"
    break;

  case 8: /* retstat: "return" explist opt_semi  */
#line 127 "parser/parser.y"
                                  {
              auto* r = at(new Return());
              r->values = std::move((yyvsp[-1].exprs)->v); delete (yyvsp[-1].exprs);
              (yyval.stmt) = r;
          }
#line 1828 "build/parser.tab.c"
    break;

  case 11: /* stmt: ";"  */
#line 136 "parser/parser.y"
                                  { (yyval.stmt) = nullptr; }
#line 1834 "build/parser.tab.c"
    break;

  case 12: /* stmt: varlist "=" explist  */
#line 137 "parser/parser.y"
                                  {
            auto* a = at(new Assign());
            a->targets = std::move((yyvsp[-2].exprs)->v); delete (yyvsp[-2].exprs);
            a->values  = std::move((yyvsp[0].exprs)->v); delete (yyvsp[0].exprs);
            (yyval.stmt) = a;
        }
#line 1845 "build/parser.tab.c"
    break;

  case 13: /* stmt: prefixexp  */
#line 143 "parser/parser.y"
                                  {
            if ((yyvsp[0].expr)->kind != NodeKind::FunctionCall) {
                yyerror("expressao nao e um comando valido (esperava chamada de funcao ou atribuicao)");
                delete (yyvsp[0].expr);
                YYERROR;
            }
            auto* c = at(new CallStmt()); c->call.reset((yyvsp[0].expr)); (yyval.stmt) = c;
        }
#line 1858 "build/parser.tab.c"
    break;

  case 14: /* stmt: "do" block "end"  */
#line 151 "parser/parser.y"
                                 { auto* d = at(new Do()); d->body.reset((yyvsp[-1].block)); (yyval.stmt) = d; }
#line 1864 "build/parser.tab.c"
    break;

  case 15: /* stmt: "while" exp "do" block "end"  */
#line 152 "parser/parser.y"
                                 {
            auto* w = at(new While()); w->condition.reset((yyvsp[-3].expr)); w->body.reset((yyvsp[-1].block)); (yyval.stmt) = w;
        }
#line 1872 "build/parser.tab.c"
    break;

  case 16: /* stmt: "repeat" block "until" exp  */
#line 155 "parser/parser.y"
                                 {
            auto* r = at(new RepeatUntil()); r->body.reset((yyvsp[-2].block)); r->condition.reset((yyvsp[0].expr)); (yyval.stmt) = r;
        }
#line 1880 "build/parser.tab.c"
    break;

  case 17: /* stmt: "if" exp "then" block elseif_list else_opt "end"  */
#line 158 "parser/parser.y"
                                                   {
            auto* n = at(new If());
            n->clauses.push_back(IfClause{ExprPtr((yyvsp[-5].expr)), BlockPtr((yyvsp[-3].block))});
            for (auto& c : (yyvsp[-2].clauses)->v) n->clauses.push_back(std::move(c));
            delete (yyvsp[-2].clauses);
            if ((yyvsp[-1].block)) n->clauses.push_back(IfClause{nullptr, BlockPtr((yyvsp[-1].block))});
            (yyval.stmt) = n;
        }
#line 1893 "build/parser.tab.c"
    break;

  case 18: /* stmt: "for" ID "=" exp "," exp "do" block "end"  */
#line 166 "parser/parser.y"
                                                 {
            auto* f = at(new NumericFor());
            f->varName = (yyvsp[-7].strValue); free((yyvsp[-7].strValue));
            f->start.reset((yyvsp[-5].expr)); f->stop.reset((yyvsp[-3].expr)); f->body.reset((yyvsp[-1].block));
            (yyval.stmt) = f;
        }
#line 1904 "build/parser.tab.c"
    break;

  case 19: /* stmt: "for" ID "=" exp "," exp "," exp "do" block "end"  */
#line 172 "parser/parser.y"
                                                           {
            auto* f = at(new NumericFor());
            f->varName = (yyvsp[-9].strValue); free((yyvsp[-9].strValue));
            f->start.reset((yyvsp[-7].expr)); f->stop.reset((yyvsp[-5].expr)); f->step.reset((yyvsp[-3].expr)); f->body.reset((yyvsp[-1].block));
            (yyval.stmt) = f;
        }
#line 1915 "build/parser.tab.c"
    break;

  case 20: /* stmt: "for" namelist "in" explist "do" block "end"  */
#line 178 "parser/parser.y"
                                             {
            auto* f = at(new GenericFor());
            f->names = std::move((yyvsp[-5].names)->v); delete (yyvsp[-5].names);
            f->exprs = std::move((yyvsp[-3].exprs)->v); delete (yyvsp[-3].exprs);
            f->body.reset((yyvsp[-1].block));
            (yyval.stmt) = f;
        }
#line 1927 "build/parser.tab.c"
    break;

  case 21: /* stmt: "function" funcname funcbody  */
#line 185 "parser/parser.y"
                                   {
            auto* f = at(new FunctionDecl());
            f->name = *(yyvsp[-1].sname); delete (yyvsp[-1].sname);
            f->params = std::move((yyvsp[0].fbody)->params); f->isVararg = (yyvsp[0].fbody)->vararg;
            f->body = std::move((yyvsp[0].fbody)->body); delete (yyvsp[0].fbody);
            (yyval.stmt) = f;
        }
#line 1939 "build/parser.tab.c"
    break;

  case 22: /* stmt: "local" "function" ID funcbody  */
#line 192 "parser/parser.y"
                                   {
            auto* f = at(new LocalFunctionDecl());
            f->name = (yyvsp[-1].strValue); free((yyvsp[-1].strValue));
            f->params = std::move((yyvsp[0].fbody)->params); f->isVararg = (yyvsp[0].fbody)->vararg;
            f->body = std::move((yyvsp[0].fbody)->body); delete (yyvsp[0].fbody);
            (yyval.stmt) = f;
        }
#line 1951 "build/parser.tab.c"
    break;

  case 23: /* stmt: "local" namelist  */
#line 199 "parser/parser.y"
                                 {
            auto* d = at(new LocalDecl());
            d->names = std::move((yyvsp[0].names)->v); delete (yyvsp[0].names);
            (yyval.stmt) = d;
        }
#line 1961 "build/parser.tab.c"
    break;

  case 24: /* stmt: "local" namelist "=" explist  */
#line 204 "parser/parser.y"
                                      {
            auto* d = at(new LocalDecl());
            d->names  = std::move((yyvsp[-2].names)->v); delete (yyvsp[-2].names);
            d->values = std::move((yyvsp[0].exprs)->v); delete (yyvsp[0].exprs);
            (yyval.stmt) = d;
        }
#line 1972 "build/parser.tab.c"
    break;

  case 25: /* stmt: "break"  */
#line 210 "parser/parser.y"
                                 { (yyval.stmt) = at(new Break()); }
#line 1978 "build/parser.tab.c"
    break;

  case 26: /* elseif_list: %empty  */
#line 213 "parser/parser.y"
                                 { (yyval.clauses) = new ClauseList(); }
#line 1984 "build/parser.tab.c"
    break;

  case 27: /* elseif_list: elseif_list "elseif" exp "then" block  */
#line 214 "parser/parser.y"
                                                {
                  (yyvsp[-4].clauses)->v.push_back(IfClause{ExprPtr((yyvsp[-2].expr)), BlockPtr((yyvsp[0].block))}); (yyval.clauses) = (yyvsp[-4].clauses);
              }
#line 1992 "build/parser.tab.c"
    break;

  case 28: /* else_opt: %empty  */
#line 219 "parser/parser.y"
                                 { (yyval.block) = nullptr; }
#line 1998 "build/parser.tab.c"
    break;

  case 29: /* else_opt: "else" block  */
#line 220 "parser/parser.y"
                                 { (yyval.block) = (yyvsp[0].block); }
#line 2004 "build/parser.tab.c"
    break;

  case 30: /* funcname: dotted_name  */
#line 223 "parser/parser.y"
                                 { (yyval.sname) = (yyvsp[0].sname); }
#line 2010 "build/parser.tab.c"
    break;

  case 31: /* funcname: dotted_name ":" ID  */
#line 224 "parser/parser.y"
                                 { *(yyvsp[-2].sname) += ":"; *(yyvsp[-2].sname) += (yyvsp[0].strValue); free((yyvsp[0].strValue)); (yyval.sname) = (yyvsp[-2].sname); }
#line 2016 "build/parser.tab.c"
    break;

  case 32: /* dotted_name: ID  */
#line 227 "parser/parser.y"
                                 { (yyval.sname) = new std::string((yyvsp[0].strValue)); free((yyvsp[0].strValue)); }
#line 2022 "build/parser.tab.c"
    break;

  case 33: /* dotted_name: dotted_name "." ID  */
#line 228 "parser/parser.y"
                                 { *(yyvsp[-2].sname) += "."; *(yyvsp[-2].sname) += (yyvsp[0].strValue); free((yyvsp[0].strValue)); (yyval.sname) = (yyvsp[-2].sname); }
#line 2028 "build/parser.tab.c"
    break;

  case 34: /* funcbody: "(" parlist ")" block "end"  */
#line 231 "parser/parser.y"
                                           {
               auto* f = new FuncBody();
               f->params = std::move((yyvsp[-3].params)->names); f->vararg = (yyvsp[-3].params)->vararg; delete (yyvsp[-3].params);
               f->body.reset((yyvsp[-1].block));
               (yyval.fbody) = f;
           }
#line 2039 "build/parser.tab.c"
    break;

  case 35: /* parlist: %empty  */
#line 239 "parser/parser.y"
                                 { (yyval.params) = new ParamList(); }
#line 2045 "build/parser.tab.c"
    break;

  case 36: /* parlist: namelist  */
#line 240 "parser/parser.y"
                                 { (yyval.params) = new ParamList(); (yyval.params)->names = std::move((yyvsp[0].names)->v); delete (yyvsp[0].names); }
#line 2051 "build/parser.tab.c"
    break;

  case 37: /* parlist: namelist "," "..."  */
#line 241 "parser/parser.y"
                                 { (yyval.params) = new ParamList(); (yyval.params)->names = std::move((yyvsp[-2].names)->v); delete (yyvsp[-2].names); (yyval.params)->vararg = true; }
#line 2057 "build/parser.tab.c"
    break;

  case 38: /* parlist: "..."  */
#line 242 "parser/parser.y"
                                 { (yyval.params) = new ParamList(); (yyval.params)->vararg = true; }
#line 2063 "build/parser.tab.c"
    break;

  case 39: /* namelist: ID  */
#line 245 "parser/parser.y"
                                 { (yyval.names) = new NameList(); (yyval.names)->v.push_back((yyvsp[0].strValue)); free((yyvsp[0].strValue)); }
#line 2069 "build/parser.tab.c"
    break;

  case 40: /* namelist: namelist "," ID  */
#line 246 "parser/parser.y"
                                 { (yyvsp[-2].names)->v.push_back((yyvsp[0].strValue)); free((yyvsp[0].strValue)); (yyval.names) = (yyvsp[-2].names); }
#line 2075 "build/parser.tab.c"
    break;

  case 41: /* varlist: prefixexp  */
#line 249 "parser/parser.y"
                                 {
              if (!isLvalue((yyvsp[0].expr))) { yyerror("alvo de atribuicao invalido"); delete (yyvsp[0].expr); YYERROR; }
              (yyval.exprs) = new ExprList(); (yyval.exprs)->v.emplace_back((yyvsp[0].expr));
          }
#line 2084 "build/parser.tab.c"
    break;

  case 42: /* varlist: varlist "," prefixexp  */
#line 253 "parser/parser.y"
                                  {
              if (!isLvalue((yyvsp[0].expr))) { yyerror("alvo de atribuicao invalido"); delete (yyvsp[0].expr); delete (yyvsp[-2].exprs); YYERROR; }
              (yyvsp[-2].exprs)->v.emplace_back((yyvsp[0].expr)); (yyval.exprs) = (yyvsp[-2].exprs);
          }
#line 2093 "build/parser.tab.c"
    break;

  case 43: /* explist: exp  */
#line 259 "parser/parser.y"
                                 { (yyval.exprs) = new ExprList(); (yyval.exprs)->v.emplace_back((yyvsp[0].expr)); }
#line 2099 "build/parser.tab.c"
    break;

  case 44: /* explist: explist "," exp  */
#line 260 "parser/parser.y"
                                 { (yyvsp[-2].exprs)->v.emplace_back((yyvsp[0].expr)); (yyval.exprs) = (yyvsp[-2].exprs); }
#line 2105 "build/parser.tab.c"
    break;

  case 45: /* prefixexp: ID  */
#line 264 "parser/parser.y"
                                 { (yyval.expr) = at(new Identifier((yyvsp[0].strValue))); free((yyvsp[0].strValue)); }
#line 2111 "build/parser.tab.c"
    break;

  case 46: /* prefixexp: prefixexp "[" exp "]"  */
#line 265 "parser/parser.y"
                                            {
                auto* i = at(new Index()); i->object.reset((yyvsp[-3].expr)); i->key.reset((yyvsp[-1].expr)); (yyval.expr) = i;
            }
#line 2119 "build/parser.tab.c"
    break;

  case 47: /* prefixexp: prefixexp "." ID  */
#line 268 "parser/parser.y"
                                 {
                auto* i = at(new Index()); i->object.reset((yyvsp[-2].expr));
                i->key.reset(at(new StringLiteral((yyvsp[0].strValue)))); free((yyvsp[0].strValue)); (yyval.expr) = i;
            }
#line 2128 "build/parser.tab.c"
    break;

  case 48: /* prefixexp: prefixexp args  */
#line 272 "parser/parser.y"
                                 {
                auto* c = at(new FunctionCall()); c->callee.reset((yyvsp[-1].expr));
                c->args = std::move((yyvsp[0].exprs)->v); delete (yyvsp[0].exprs); (yyval.expr) = c;
            }
#line 2137 "build/parser.tab.c"
    break;

  case 49: /* prefixexp: prefixexp ":" ID args  */
#line 276 "parser/parser.y"
                                    {
                auto* c = at(new FunctionCall()); c->callee.reset((yyvsp[-3].expr));
                c->isMethodCall = true; c->methodName = (yyvsp[-1].strValue); free((yyvsp[-1].strValue));
                c->args = std::move((yyvsp[0].exprs)->v); delete (yyvsp[0].exprs); (yyval.expr) = c;
            }
#line 2147 "build/parser.tab.c"
    break;

  case 50: /* prefixexp: "(" exp ")"  */
#line 281 "parser/parser.y"
                                 { (yyval.expr) = (yyvsp[-1].expr); }
#line 2153 "build/parser.tab.c"
    break;

  case 51: /* args: "(" ")"  */
#line 284 "parser/parser.y"
                                 { (yyval.exprs) = new ExprList(); }
#line 2159 "build/parser.tab.c"
    break;

  case 52: /* args: "(" explist ")"  */
#line 285 "parser/parser.y"
                                 { (yyval.exprs) = (yyvsp[-1].exprs); }
#line 2165 "build/parser.tab.c"
    break;

  case 53: /* args: tableconstructor  */
#line 286 "parser/parser.y"
                                 { (yyval.exprs) = new ExprList(); (yyval.exprs)->v.emplace_back((yyvsp[0].expr)); }
#line 2171 "build/parser.tab.c"
    break;

  case 54: /* args: STRING  */
#line 287 "parser/parser.y"
                                 { (yyval.exprs) = new ExprList(); (yyval.exprs)->v.emplace_back(at(new StringLiteral((yyvsp[0].strValue)))); free((yyvsp[0].strValue)); }
#line 2177 "build/parser.tab.c"
    break;

  case 55: /* tableconstructor: "{" "}"  */
#line 290 "parser/parser.y"
                                 { (yyval.expr) = at(new TableConstructor()); }
#line 2183 "build/parser.tab.c"
    break;

  case 56: /* tableconstructor: "{" fieldlist "}"  */
#line 291 "parser/parser.y"
                                           {
                       auto* t = at(new TableConstructor());
                       t->fields = std::move((yyvsp[-1].fields)->v); delete (yyvsp[-1].fields); (yyval.expr) = t;
                   }
#line 2192 "build/parser.tab.c"
    break;

  case 57: /* tableconstructor: "{" fieldlist fieldsep "}"  */
#line 295 "parser/parser.y"
                                                    {
                       auto* t = at(new TableConstructor());
                       t->fields = std::move((yyvsp[-2].fields)->v); delete (yyvsp[-2].fields); (yyval.expr) = t;
                   }
#line 2201 "build/parser.tab.c"
    break;

  case 58: /* fieldlist: field  */
#line 301 "parser/parser.y"
                                 { (yyval.fields) = (yyvsp[0].fields); }
#line 2207 "build/parser.tab.c"
    break;

  case 59: /* fieldlist: fieldlist fieldsep field  */
#line 302 "parser/parser.y"
                                     {
                for (auto& f : (yyvsp[0].fields)->v) (yyvsp[-2].fields)->v.push_back(std::move(f));
                delete (yyvsp[0].fields); (yyval.fields) = (yyvsp[-2].fields);
            }
#line 2216 "build/parser.tab.c"
    break;

  case 62: /* field: "[" exp "]" "=" exp  */
#line 310 "parser/parser.y"
                                         {
            (yyval.fields) = new FieldList(); (yyval.fields)->v.push_back(TableField{ExprPtr((yyvsp[-3].expr)), ExprPtr((yyvsp[0].expr))});
        }
#line 2224 "build/parser.tab.c"
    break;

  case 63: /* field: ID "=" exp  */
#line 313 "parser/parser.y"
                      {
            (yyval.fields) = new FieldList();
            (yyval.fields)->v.push_back(TableField{ExprPtr(at(new StringLiteral((yyvsp[-2].strValue)))), ExprPtr((yyvsp[0].expr))});
            free((yyvsp[-2].strValue));
        }
#line 2234 "build/parser.tab.c"
    break;

  case 64: /* field: exp  */
#line 318 "parser/parser.y"
            {
            (yyval.fields) = new FieldList(); (yyval.fields)->v.push_back(TableField{nullptr, ExprPtr((yyvsp[0].expr))});
        }
#line 2242 "build/parser.tab.c"
    break;

  case 65: /* exp: "nil"  */
#line 323 "parser/parser.y"
                                 { (yyval.expr) = at(new NilLiteral()); }
#line 2248 "build/parser.tab.c"
    break;

  case 66: /* exp: "false"  */
#line 324 "parser/parser.y"
                                 { (yyval.expr) = at(new BoolLiteral(false)); }
#line 2254 "build/parser.tab.c"
    break;

  case 67: /* exp: "true"  */
#line 325 "parser/parser.y"
                                 { (yyval.expr) = at(new BoolLiteral(true)); }
#line 2260 "build/parser.tab.c"
    break;

  case 68: /* exp: NUM  */
#line 326 "parser/parser.y"
                                 { (yyval.expr) = at(new NumberLiteral((yyvsp[0].intValue))); }
#line 2266 "build/parser.tab.c"
    break;

  case 69: /* exp: NUM_FLOAT  */
#line 327 "parser/parser.y"
                                 { (yyval.expr) = at(new NumberLiteral((yyvsp[0].floatValue))); }
#line 2272 "build/parser.tab.c"
    break;

  case 70: /* exp: STRING  */
#line 328 "parser/parser.y"
                                 { (yyval.expr) = at(new StringLiteral((yyvsp[0].strValue))); free((yyvsp[0].strValue)); }
#line 2278 "build/parser.tab.c"
    break;

  case 71: /* exp: "..."  */
#line 329 "parser/parser.y"
                                 { (yyval.expr) = at(new Vararg()); }
#line 2284 "build/parser.tab.c"
    break;

  case 72: /* exp: "function" funcbody  */
#line 330 "parser/parser.y"
                                 {
            auto* f = at(new FunctionExpr());
            f->params = std::move((yyvsp[0].fbody)->params); f->isVararg = (yyvsp[0].fbody)->vararg;
            f->body = std::move((yyvsp[0].fbody)->body); delete (yyvsp[0].fbody);
            (yyval.expr) = f;
        }
#line 2295 "build/parser.tab.c"
    break;

  case 73: /* exp: prefixexp  */
#line 336 "parser/parser.y"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 2301 "build/parser.tab.c"
    break;

  case 74: /* exp: tableconstructor  */
#line 337 "parser/parser.y"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 2307 "build/parser.tab.c"
    break;

  case 75: /* exp: exp "+" exp  */
#line 338 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Add,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2313 "build/parser.tab.c"
    break;

  case 76: /* exp: exp "-" exp  */
#line 339 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Sub,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2319 "build/parser.tab.c"
    break;

  case 77: /* exp: exp "*" exp  */
#line 340 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Mul,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2325 "build/parser.tab.c"
    break;

  case 78: /* exp: exp "/" exp  */
#line 341 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Div,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2331 "build/parser.tab.c"
    break;

  case 79: /* exp: exp "//" exp  */
#line 342 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::FloorDiv, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2337 "build/parser.tab.c"
    break;

  case 80: /* exp: exp "%" exp  */
#line 343 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Mod,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2343 "build/parser.tab.c"
    break;

  case 81: /* exp: exp "^" exp  */
#line 344 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Pow,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2349 "build/parser.tab.c"
    break;

  case 82: /* exp: exp ".." exp  */
#line 345 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Concat,   (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2355 "build/parser.tab.c"
    break;

  case 83: /* exp: exp "==" exp  */
#line 346 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Eq,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2361 "build/parser.tab.c"
    break;

  case 84: /* exp: exp "~=" exp  */
#line 347 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Ne,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2367 "build/parser.tab.c"
    break;

  case 85: /* exp: exp "<" exp  */
#line 348 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Lt,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2373 "build/parser.tab.c"
    break;

  case 86: /* exp: exp "<=" exp  */
#line 349 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Le,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2379 "build/parser.tab.c"
    break;

  case 87: /* exp: exp ">" exp  */
#line 350 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Gt,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2385 "build/parser.tab.c"
    break;

  case 88: /* exp: exp ">=" exp  */
#line 351 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Ge,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2391 "build/parser.tab.c"
    break;

  case 89: /* exp: exp "and" exp  */
#line 352 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::And,      (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2397 "build/parser.tab.c"
    break;

  case 90: /* exp: exp "or" exp  */
#line 353 "parser/parser.y"
                                 { (yyval.expr) = bin(BinOp::Or,       (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2403 "build/parser.tab.c"
    break;

  case 91: /* exp: "-" exp  */
#line 354 "parser/parser.y"
                                 { (yyval.expr) = un(UnOp::Neg, (yyvsp[0].expr)); }
#line 2409 "build/parser.tab.c"
    break;

  case 92: /* exp: "not" exp  */
#line 355 "parser/parser.y"
                                 { (yyval.expr) = un(UnOp::Not, (yyvsp[0].expr)); }
#line 2415 "build/parser.tab.c"
    break;

  case 93: /* exp: "#" exp  */
#line 356 "parser/parser.y"
                                 { (yyval.expr) = un(UnOp::Len, (yyvsp[0].expr)); }
#line 2421 "build/parser.tab.c"
    break;


#line 2425 "build/parser.tab.c"

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
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
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
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
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
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 359 "parser/parser.y"


void yyerror(const char* msg) {
    std::fprintf(stderr, "Erro sintático na linha %d: %s\n", linha, msg);
}
