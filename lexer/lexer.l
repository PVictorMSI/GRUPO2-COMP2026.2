%{
#include <stdio.h>
#include <stdlib.h>   /* atoi, atof, malloc, realloc */
#include <string.h>   /* strdup, strndup */
#include "parser.tab.h"  /* Inclusão do cabeçalho gerado pelo Bison, que contém as definições de tokens */

/* Rastreio de tokens (depuração): com -DTRACE_TOKENS, cada regra imprime em stderr o
   seu número. Sem a flag, TRACE() não gera nenhum código.
   Para ligar:  make clean && make TRACE=1 */
#ifdef TRACE_TOKENS
#define TRACE(n) fprintf(stderr, "%d\n", (n))
#else
#define TRACE(n) ((void)0)
#endif

/* Contador de linha — parser.y declara "extern int linha;" e espera a definição aqui. */
int linha = 1;

/* Quantidade de erros léxicos já reportados. O main() consulta este contador: se for
   maior que zero, o programa NÃO é compilado (exit 1), mesmo que o parser aceite os
   tokens que o lexer devolveu depois de se recuperar do erro. */
int lex_errors = 0;
#define LEX_ERROR() (++lex_errors)

static char *str_buf = NULL;
static size_t str_buf_len = 0;
static size_t str_buf_cap = 0;

static void str_buf_reset(void) {
    str_buf_len = 0;
}

static void str_buf_append(char c) {
    if (str_buf_len + 1 >= str_buf_cap) {
        str_buf_cap = str_buf_cap ? str_buf_cap * 2 : 64;
        str_buf = (char*) realloc(str_buf, str_buf_cap);
    }
    str_buf[str_buf_len++] = c;
}

/* String longa: o Lua ignora a quebra de linha logo depois de "[[". */
static int longstring_start = 0;

/* Converte as sequências de escape de uma string curta ("..." ou '...') no valor
   real da string, como o Lua faz: \n, \t, \\, \", \ddd, \xHH, \z, \u{XXX}...
   Assim a AST guarda o VALOR da string e o gerador de código não precisa saber se
   ela veio de aspas ou de [[ ]]. Devolve uma string alocada com malloc. */
static void utf8_append(unsigned long cp) {
    if (cp < 0x80) { str_buf_append((char)cp); }
    else if (cp < 0x800) { str_buf_append((char)(0xC0 | (cp >> 6))); str_buf_append((char)(0x80 | (cp & 0x3F))); }
    else if (cp < 0x10000) { str_buf_append((char)(0xE0 | (cp >> 12))); str_buf_append((char)(0x80 | ((cp >> 6) & 0x3F)));
                             str_buf_append((char)(0x80 | (cp & 0x3F))); }
    else { str_buf_append((char)(0xF0 | (cp >> 18))); str_buf_append((char)(0x80 | ((cp >> 12) & 0x3F)));
           str_buf_append((char)(0x80 | ((cp >> 6) & 0x3F))); str_buf_append((char)(0x80 | (cp & 0x3F))); }
}

static int hexval(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static char* unescape_string(const char* s, size_t n) {
    str_buf_reset();
    for (size_t i = 0; i < n; ++i) {
        char c = s[i];
        if (c != '\\' || i + 1 >= n) { str_buf_append(c); continue; }
        char e = s[++i];
        switch (e) {
        case 'n': str_buf_append('\n'); break;
        case 't': str_buf_append('\t'); break;
        case 'r': str_buf_append('\r'); break;
        case 'a': str_buf_append('\a'); break;
        case 'b': str_buf_append('\b'); break;
        case 'f': str_buf_append('\f'); break;
        case 'v': str_buf_append('\v'); break;
        case '\\': case '"': case '\'': case '\n': str_buf_append(e); break;
        case 'x':
            if (i + 2 < n && hexval(s[i + 1]) >= 0 && hexval(s[i + 2]) >= 0) {
                str_buf_append((char)(hexval(s[i + 1]) * 16 + hexval(s[i + 2])));
                i += 2;
            } else {
                str_buf_append('\\'); str_buf_append(e);
            }
            break;
        case 'z':   /* pula os espaços seguintes */
            while (i + 1 < n && (s[i + 1] == ' ' || s[i + 1] == '\t' || s[i + 1] == '\r')) ++i;
            break;
        case 'u':
            if (i + 1 < n && s[i + 1] == '{') {
                unsigned long cp = 0; size_t j = i + 2;
                while (j < n && hexval(s[j]) >= 0) cp = cp * 16 + hexval(s[j++]);
                if (j < n && s[j] == '}') { utf8_append(cp); i = j; break; }
            }
            str_buf_append('\\'); str_buf_append(e);
            break;
        default:
            if (e >= '0' && e <= '9') {   /* \ddd: até 3 dígitos decimais */
                int v = 0, k = 0;
                while (k < 3 && i < n && s[i] >= '0' && s[i] <= '9') { v = v * 10 + (s[i] - '0'); ++i; ++k; }
                --i;
                str_buf_append((char)v);
            } else {                       /* escape desconhecido: mantém como está */
                str_buf_append('\\'); str_buf_append(e);
            }
        }
    }
    str_buf_append('\0');
    return strdup(str_buf);
}
%}

%x COMMENT_LINE
%x COMMENT_BLOCK
%x LONGSTRING

LETRA   [a-zA-Z_]
DIGITO  [0-9]
ID      ({LETRA}|_)(({LETRA}|{DIGITO}|_)*)
INT     [0-9]+
EXP     [eE][+-]?[0-9]+
FLOAT   ([0-9]+"."[0-9]+{EXP}?)|([0-9]+{EXP})|("."[0-9]+{EXP}?)
HEX     0[xX][0-9a-fA-F]+
STRING_D  \"([^\"\n\\]|\\.)*\"
STRING_S  \'([^\'\n\\]|\\.)*\'
/* Definições de padrões de token e ações */

%%


"--" {
        int c1 = yyinput();
        if (c1 == '[') {
            int c2 = yyinput();
            if (c2 == '[') {
                BEGIN(COMMENT_BLOCK);
            } else {
                if (c2 != EOF) unput(c2);
                unput(c1);
                BEGIN(COMMENT_LINE);
            }
        } else {
            if (c1 != EOF) unput(c1);
            BEGIN(COMMENT_LINE);
        }
      }

<COMMENT_LINE>.*          { BEGIN(INITIAL); /* descarta o resto da linha; o \n é tratado pela regra normal do INITIAL */ }

<COMMENT_BLOCK>"]]"       { BEGIN(INITIAL); }
<COMMENT_BLOCK>\n         { linha++; }
<COMMENT_BLOCK>.          { /* descarta o conteúdo do comentário */ }
<COMMENT_BLOCK><<EOF>>    {
                            LEX_ERROR(); fprintf(stderr, "Erro léxico na linha %d: comentário de bloco não terminado\n", linha);
                            return 0;
                          }

 /* palavras reservadas */
"function"              { TRACE(1); return FUNCTION; }
"elseif"                { TRACE(2);return ELSEIF; }
"else"                  { TRACE(3);return ELSE; }
"if"                    { TRACE(4);return IF; }
"or"                    { TRACE(5);return OR; }
"until"                 { TRACE(6);return UNTIL; }
"while"                 { TRACE(7);return WHILE; }
"true"                  { TRACE(8);return TRUE_VAL; }
"false"                 { TRACE(9);return FALSE_VAL; }
"not"                   { TRACE(10);return NOT; }
"then"                  { TRACE(11);return THEN; }
"nil"                   { TRACE(12);return NIL; }
"for"                   { TRACE(13);return FOR; }
"do"                    { TRACE(14);return DO; }
"return"                { TRACE(15);return RETURN; }
"local"                 { TRACE(16);return LOCAL; }
"break"                 { TRACE(17);return BREAK; }
"repeat"                { TRACE(18);return REPEAT; }
"in"                    { TRACE(19);return IN; }
"end"                   { TRACE(20);return END; }
"and"                   { TRACE(21);return AND; }

 /* operadores e pontuação */
"..."                   { TRACE(22);return VARARG; } /* tokenizado, mas fora do escopo da gramática suportada (ver documento de escopo) */
".."                    { TRACE(23);return CONCAT; }
"."                     { TRACE(24);return DOT; }
"=="                    { TRACE(25);return EQ; }
"~="                    { TRACE(26);return NEQ; }
">="                    { TRACE(27);return GE; }
"<="                    { TRACE(28);return LE; }
">"                     { TRACE(29);return GT; }
"<"                     { TRACE(30);return LT; }
"="                     { TRACE(31);return ASSIGN; }
"+"                     { TRACE(32);return PLUS; }
"-"                     { TRACE(33);return MINUS; }
"*"                     { TRACE(34);return MULT; }
"//"                    { TRACE(35);return IDIV; } /* NOVO: divisão inteira — adicionar "%token IDIV" no arquivo .y */
"/"                     { TRACE(36);return DIV; }
"%"                     { TRACE(37);return MOD; }
"^"                     { TRACE(38);return POW; }
"#"                     { TRACE(39);return LEN; }
";"                     { TRACE(40);return SEMI; }
":"                     { TRACE(41);return COLON; }
","                     { TRACE(42);return COMMA; }
"("                     { TRACE(43);return LPAREN; }
")"                     { TRACE(44);return RPAREN; }
"]"                     { TRACE(45);return RBRACKET; }
"{"                     { TRACE(46);return LBRACE; }
"}"                     { TRACE(47);return RBRACE; }

 /* strings longas [[ ... ]] (NOVO — atende ao RF13) */
"[["                    { TRACE(48);BEGIN(LONGSTRING); str_buf_reset(); longstring_start = 1; }
<LONGSTRING>"]]"        {
                          TRACE(49);
                          str_buf_append('\0');
                          yylval.strValue = strdup(str_buf);
                          BEGIN(INITIAL);
                          return STRING;
                        }
<LONGSTRING>\n          { TRACE(50); if (!longstring_start) str_buf_append('\n'); longstring_start = 0; linha++; }
<LONGSTRING>.           { TRACE(51); longstring_start = 0; str_buf_append(yytext[0]); }
<LONGSTRING><<EOF>>     {
                          LEX_ERROR(); fprintf(stderr, "Erro léxico na linha %d: string longa não terminada\n", linha);
                          return 0;
                        }

"["                     { TRACE(52);return LBRACKET; }

 /* literais numéricos 
    O antigo padrão "[+\-]?[0-9]{1,10}" (com sinal opcional) foi removido:
    ele competia por "casamento mais longo" com os operadores PLUS/MINUS,
    fazendo entradas como "x-5" virarem MINUS ausente + NUM(-5) em vez de
    MINUS seguido de NUM(5). O sinal em Lua é sempre operador unário
    tratado pela gramática, nunca parte do literal numérico. */
{FLOAT}                 {
                          TRACE(53);
                          yylval.floatValue = atof(yytext);
                          return NUM_FLOAT;
                        }

{HEX}                   {
                          TRACE(54);
                          yylval.intValue = (int) strtol(yytext, NULL, 16);
                          return NUM;
                        }

{INT}                   {
                          TRACE(54);
                          yylval.intValue = atoi(yytext);
                          return NUM;
                        }

 /* identificadores malformados (ex: 123nome) */
{DIGITO}+({LETRA}|_)+({LETRA}|{DIGITO}|_)* {
                          LEX_ERROR(); fprintf(stderr, "Erro léxico na linha %d: identificador malformado '%s'\n", linha, yytext);
                          yylval.strValue = strdup(yytext);
                          return ID;
                        }

 /* números malformados (ex: 1.2.3) */
[0-9]+("."[0-9]+){2,}   {
                          LEX_ERROR(); fprintf(stderr, "Erro léxico na linha %d: número malformado '%s'\n", linha, yytext);
                          yylval.floatValue = atof(yytext); /* Pega apenas a primeira parte validamente convertível */
                          return NUM_FLOAT;
                        }

 /* literais de string curtas */
{STRING_D}              {
                          TRACE(55);
                          yylval.strValue = unescape_string(yytext + 1, yyleng - 2);
                          return STRING;
                        }

{STRING_S}               {
                          TRACE(56);
                          yylval.strValue = unescape_string(yytext + 1, yyleng - 2);
                          return STRING;
                        }

 /* strings curtas não terminadas (faltou a aspa de fechamento antes da quebra de linha) */
\"[^\"\n]*\n            {
                          LEX_ERROR(); fprintf(stderr, "Erro léxico na linha %d: string não terminada\n", linha);
                          linha++;
                          yylval.strValue = strdup(yytext);
                          return STRING;
                        }
\'[^\'\n]*\n            {
                          LEX_ERROR(); fprintf(stderr, "Erro léxico na linha %d: string não terminada\n", linha);
                          linha++;
                          yylval.strValue = strdup(yytext);
                          return STRING;
                        }
 /* identificadores devem vir DEPOIS das palavras reservadas */
{ID}                    {
                          TRACE(57);
                          yylval.strValue = strdup(yytext);
                          return ID;
                        }

 /* espaços e quebras de linha */
\n                      { linha++; }
[ \t\r]+                { /* ignora espaços e tabs */ }

 /* qualquer outro caractere não reconhecido: erro léxico */
.                       {
                          LEX_ERROR(); fprintf(stderr, "Erro léxico na linha %d: caractere inesperado '%s'\n", linha, yytext);
                        }

%%

/* Função auxiliar do Flex (quando termina o arquivo de entrada) */
int yywrap(void) {
    return 1;
}
