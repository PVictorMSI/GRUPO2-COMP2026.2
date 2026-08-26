%{
#include "exemplo.tab.h"  /* Inclusão do cabeçalho gerado pelo Bison, que contém as definições de tokens */
%}

LETRA   [a-zA-Z_]
DIGITO  [0-9]
ID      {LETRA}({LETRA}|{DIGITO})*
INT     [0-9]+
FLOAT   [0-9]+"."[0-9]+
STRING_D  \"([^\"\n\\]|\\.)*\"
STRING_S  \'([^\'\n\\]|\\.)*\'
/* Definições de padrões de token e ações */

%%

/* comentarios descartados, não geram token */
"--[["([^\]]|\][^\]])*"]]"   { /* comentário de bloco: ignora, mas conta as quebras de linha internas */
                                for (int i = 0; yytext[i] != '\0'; i++) {
                                    if (yytext[i] == '\n') linha++;
                                }
                              }
"--".*                       { /* comentário de linha única: ignora */ }

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

/*operadores de pontuacao*/
"..."                   { return VARARG; }
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
"/"                     { return DIV; }
"%"                     { return MOD; }
"^"                     { return POW; }
"#"                     { return LEN; }
";"                     { return SEMI; }
":"                     { return COLON; }
","                     { return COMMA; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
"["                     { return LBRACKET; }
"]"                     { return RBRACKET; }
"{"                     { return LBRACE; }
"}"                     { return RBRACE; }
[+\-]?[0-9]{1,10}     {
                        /* Converter o lexema para inteiro (pode estourar se for maior que INT, 
                           mas servirá como exemplo didático) */
                        yylval.intValue = atoi(yytext);
                        return NUM;
                      }

/* literais */
{FLOAT}                 {
                          yylval.floatValue = atof(yytext);
                          return NUM_FLOAT;
                        }

{INT}                   {
                          yylval.intValue = atoi(yytext);
                          return NUM;
                        }

{STRING_D}              {
                          /* remove as aspas externas antes de guardar o lexema */
                          yylval.strValue = strndup(yytext + 1, yyleng - 2);
                          return STRING;
                        }

{STRING_S}               {
                          yylval.strValue = strndup(yytext + 1, yyleng - 2);
                          return STRING;
                        }

/* identificadores deve vir DEPOIS das palavras reservadas */
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
            fprintf(stderr, "Não foi possível abrir o arquivo: "%s\n", argv[1]);
            return 1;    
        }
    }
    return yyparse();
}