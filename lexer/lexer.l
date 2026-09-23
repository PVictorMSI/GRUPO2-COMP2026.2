%{
#include <stdio.h>
#include <stdlib.h>   /* atoi, atof, malloc, realloc */
#include <string.h>   /* strdup, strndup */
#include "teste.tab.h"  /* Inclusão do cabeçalho gerado pelo Bison, que contém as definições de tokens */

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
ID      ({LETRA}|_)(({LETRA}|{DIGITO}|_)*)
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
"function"              { fprintf(stderr,"1\n"); return FUNCTION; }
"elseif"                { fprintf(stderr,"2\n");return ELSEIF; }
"else"                  { fprintf(stderr,"3\n");return ELSE; }
"if"                    { fprintf(stderr,"4\n");return IF; }
"or"                    { fprintf(stderr,"5\n");return OR; }
"until"                 { fprintf(stderr,"6\n");return UNTIL; }
"while"                 { fprintf(stderr,"7\n");return WHILE; }
"true"                  { fprintf(stderr,"8\n");return TRUE_VAL; }
"false"                 { fprintf(stderr,"9\n");return FALSE_VAL; }
"not"                   { fprintf(stderr,"10\n");return NOT; }
"then"                  { fprintf(stderr,"11\n");return THEN; }
"nil"                   { fprintf(stderr,"12\n");return NIL; }
"for"                   { fprintf(stderr,"13\n");return FOR; }
"do"                    { fprintf(stderr,"14\n");return DO; }
"return"                { fprintf(stderr,"15\n");return RETURN; }
"local"                 { fprintf(stderr,"16\n");return LOCAL; }
"break"                 { fprintf(stderr,"17\n");return BREAK; }
"repeat"                { fprintf(stderr,"18\n");return REPEAT; }
"in"                    { fprintf(stderr,"19\n");return IN; }
"end"                   { fprintf(stderr,"20\n");return END; }
"and"                   { fprintf(stderr,"21\n");return AND; }

 /* operadores e pontuação */
"..."                   { fprintf(stderr,"22\n");return VARARG; } /* tokenizado, mas fora do escopo da gramática suportada (ver documento de escopo) */
".."                    { fprintf(stderr,"23\n");return CONCAT; }
"."                     { fprintf(stderr,"24\n");return DOT; }
"=="                    { fprintf(stderr,"25\n");return EQ; }
"~="                    { fprintf(stderr,"26\n");return NEQ; }
">="                    { fprintf(stderr,"27\n");return GE; }
"<="                    { fprintf(stderr,"28\n");return LE; }
">"                     { fprintf(stderr,"29\n");return GT; }
"<"                     { fprintf(stderr,"30\n");return LT; }
"="                     { fprintf(stderr,"31\n");return ASSIGN; }
"+"                     { fprintf(stderr,"32\n");return PLUS; }
"-"                     { fprintf(stderr,"33\n");return MINUS; }
"*"                     { fprintf(stderr,"34\n");return MULT; }
"//"                    { fprintf(stderr,"35\n");return IDIV; } /* NOVO: divisão inteira — adicionar "%token IDIV" no arquivo .y */
"/"                     { fprintf(stderr,"36\n");return DIV; }
"%"                     { fprintf(stderr,"37\n");return MOD; }
"^"                     { fprintf(stderr,"38\n");return POW; }
"#"                     { fprintf(stderr,"39\n");return LEN; }
";"                     { fprintf(stderr,"40\n");return SEMI; }
":"                     { fprintf(stderr,"41\n");return COLON; }
","                     { fprintf(stderr,"42\n");return COMMA; }
"("                     { fprintf(stderr,"43\n");return LPAREN; }
")"                     { fprintf(stderr,"44\n");return RPAREN; }
"]"                     { fprintf(stderr,"45\n");return RBRACKET; }
"{"                     { fprintf(stderr,"46\n");return LBRACE; }
"}"                     { fprintf(stderr,"47\n");return RBRACE; }

 /* strings longas [[ ... ]] (NOVO — atende ao RF13) */
"[["                    { fprintf(stderr,"48\n");BEGIN(LONGSTRING); str_buf_reset(); }
<LONGSTRING>"]]"        {
                          fprintf(stderr,"49\n");
                          str_buf_append('\0');
                          yylval.strValue = strdup(str_buf);
                          BEGIN(INITIAL);
                          return STRING;
                        }
<LONGSTRING>\n          { fprintf(stderr,"50\n");str_buf_append('\n'); linha++; }
<LONGSTRING>.           { fprintf(stderr,"51\n");str_buf_append(yytext[0]); }
<LONGSTRING><<EOF>>     {
                          fprintf(stderr, "Erro léxico na linha %d: string longa não terminada\n", linha);
                          return 0;
                        }

"["                     { fprintf(stderr,"52\n");return LBRACKET; }

 /* literais numéricos 
    O antigo padrão "[+\-]?[0-9]{1,10}" (com sinal opcional) foi removido:
    ele competia por "casamento mais longo" com os operadores PLUS/MINUS,
    fazendo entradas como "x-5" virarem MINUS ausente + NUM(-5) em vez de
    MINUS seguido de NUM(5). O sinal em Lua é sempre operador unário
    tratado pela gramática, nunca parte do literal numérico. */
{FLOAT}                 {
                          fprintf(stderr,"53\n");
                          yylval.floatValue = atof(yytext);
                          return NUM_FLOAT;
                        }

{INT}                   {
                          fprintf(stderr,"54\n");
                          yylval.intValue = atoi(yytext);
                          return NUM;
                        }
 /* literais de string curtas */
{STRING_D}              {
                          fprintf(stderr,"55\n");
                          yylval.strValue = strndup(yytext + 1, yyleng - 2);
                          return STRING;
                        }

{STRING_S}               {
                          fprintf(stderr,"56\n");
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
                          fprintf(stderr,"57\n");
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
