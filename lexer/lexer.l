%{
#include <stdio.h>
#include <stdlib.h>   /* atoi, atof, malloc, realloc */
#include <string.h>   /* strdup, strndup */
#include "exemplo.tab.h"  /* Inclusão do cabeçalho gerado pelo Bison, que contém as definições de tokens */

/* Contador de linha, usado nas mensagens de erro e nos comentários/strings de bloco.
   CUIDADO: se "linha" já estiver declarada em outro arquivo do projeto (ex.: no .y),
   remova esta linha e troque por "extern int linha;" aqui. */
int linha = 1;

/* Buffer dinâmico usado para acumular o conteúdo de strings longas [[ ... ]] */
static char *str_buf = NULL;
static size_t str_buf_len = 0;
static size_t str_buf_cap = 0;

static void str_buf_reset(void) {
    str_buf_len = 0;
}

static void str_buf_append(char c) {
    if (str_buf_len + 1 >= str_buf_cap) {
        str_buf_cap = str_buf_cap ? str_buf_cap * 2 : 64;
        str_buf = realloc(str_buf, str_buf_cap);
    }
    str_buf[str_buf_len++] = c;
}
%}

%x COMMENT_LINE
%x COMMENT_BLOCK
%x LONGSTRING

LETRA   [a-zA-Z_]
DIGITO  [0-9]
ID      {LETRA}({LETRA}|{DIGITO})*
INT     [0-9]+
FLOAT   [0-9]+"."[0-9]+
STRING_D  \"([^\"\n\\]|\\.)*\"
STRING_S  \'([^\'\n\\]|\\.)*\'
/* Definições de padrões de token e ações */

%%

 /* comentários 
    Ao ver "--" precisamos olhar os 2 próximos caracteres para decidir se é
    comentário de linha ou de bloco "--[[ ... ]]". Isso é feito manualmente
    com input()/unput() em vez de uma única regex: uma regra só com
    "--[[" competiria por "casamento mais longo" com "--".* sempre que
    houver texto na mesma linha do "--[[", perdendo a disputa. Fazendo a
    checagem manual evitamos essa ambiguidade por completo. */
"--" {
        int c1 = input();
        if (c1 == '[') {
            int c2 = input();
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
                            fprintf(stderr, "Erro léxico na linha %d: comentário de bloco não terminado\n", linha);
                            return 0;
                          }

 /* palavras reservadas */
"function"              { return FUNCTION; }
"elseif"                { return ELSEIF; }
"else"                  { return ELSE; }
"if"                    { return IF; }
"or"                    { return OR; }
"until"                 { return UNTIL; }
"while"                 { return WHILE; }
"true"                  { return TRUE_VAL; }
"false"                 { return FALSE_VAL; }
"not"                   { return NOT; }
"then"                  { return THEN; }
"nil"                   { return NIL; }
"for"                   { return FOR; }
"do"                    { return DO; }
"return"                { return RETURN; }
"local"                 { return LOCAL; }
"break"                 { return BREAK; }
"repeat"                { return REPEAT; }
"in"                    { return IN; }
"end"                   { return END; }
"and"                   { return AND; }

 /* operadores e pontuação */
"..."                   { return VARARG; } /* tokenizado, mas fora do escopo da gramática suportada (ver documento de escopo) */
".."                    { return CONCAT; }
"."                     { return DOT; }
"=="                    { return EQ; }
"~="                    { return NEQ; }
">="                    { return GE; }
"<="                    { return LE; }
">"                     { return GT; }
"<"                     { return LT; }
"="                     { return ASSIGN; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MULT; }
"//"                    { return IDIV; } /* NOVO: divisão inteira — adicionar "%token IDIV" no arquivo .y */
"/"                     { return DIV; }
"%"                     { return MOD; }
"^"                     { return POW; }
"#"                     { return LEN; }
";"                     { return SEMI; }
":"                     { return COLON; }
","                     { return COMMA; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
"]"                     { return RBRACKET; }
"{"                     { return LBRACE; }
"}"                     { return RBRACE; }

 /* strings longas [[ ... ]] (NOVO — atende ao RF13) */
"[["                    { BEGIN(LONGSTRING); str_buf_reset(); }
<LONGSTRING>"]]"        {
                          str_buf_append('\0');
                          yylval.strValue = strdup(str_buf);
                          BEGIN(INITIAL);
                          return STRING;
                        }
<LONGSTRING>\n          { str_buf_append('\n'); linha++; }
<LONGSTRING>.           { str_buf_append(yytext[0]); }
<LONGSTRING><<EOF>>     {
                          fprintf(stderr, "Erro léxico na linha %d: string longa não terminada\n", linha);
                          return 0;
                        }

"["                     { return LBRACKET; }

 /* literais numéricos 
    O antigo padrão "[+\-]?[0-9]{1,10}" (com sinal opcional) foi removido:
    ele competia por "casamento mais longo" com os operadores PLUS/MINUS,
    fazendo entradas como "x-5" virarem MINUS ausente + NUM(-5) em vez de
    MINUS seguido de NUM(5). O sinal em Lua é sempre operador unário
    tratado pela gramática, nunca parte do literal numérico. */
{FLOAT}                 {
                          yylval.floatValue = atof(yytext);
                          return NUM_FLOAT;
                        }

{INT}                   {
                          yylval.intValue = atoi(yytext);
                          return NUM;
                        }

 /* literais de string curtas */
{STRING_D}              {
                          yylval.strValue = strndup(yytext + 1, yyleng - 2);
                          return STRING;
                        }

{STRING_S}               {
                          yylval.strValue = strndup(yytext + 1, yyleng - 2);
                          return STRING;
                        }

 /* strings curtas não terminadas (faltou a aspa de fechamento antes da quebra de linha) */
\"[^\"\n]*\n            {
                          fprintf(stderr, "Erro léxico na linha %d: string não terminada\n", linha);
                          linha++;
                        }
\'[^\'\n]*\n            {
                          fprintf(stderr, "Erro léxico na linha %d: string não terminada\n", linha);
                          linha++;
                        }

 /* identificadores devem vir DEPOIS das palavras reservadas */
{ID}                    {
                          yylval.strValue = strdup(yytext);
                          return ID;
                        }

 /* espaços e quebras de linha */
\n                      { linha++; }
[ \t\r]+                { /* ignora espaços e tabs */ }

 /* qualquer outro caractere não reconhecido: erro léxico */
.                       {
                          fprintf(stderr, "Erro léxico na linha %d: caractere inesperado '%s'\n", linha, yytext);
                        }

%%

/* Função auxiliar do Flex (quando termina o arquivo de entrada) */
int yywrap(void) {
    return 1;
}

/* Função principal: chamará o parser ao ser executado */
int main(int argc, char **argv) {
    /* Se houver arquivo de entrada como parâmetro, redireciona o yyin */
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (f) {
            yyin = f;
        } else {
            fprintf(stderr, "Não foi possível abrir o arquivo: %s\n", argv[1]);
            return 1;
        }
    }
    return yyparse();
}
