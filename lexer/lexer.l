%{
#include "exemplo.tab.h"  /* Inclusão do cabeçalho gerado pelo Bison, que contém as definições de tokens */
%}

/* Definições de padrões de token e ações */
%%
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

[ \t\n]+              { /* Não faz nada, apenas ignora */ }

LETRA [a-zA-Z_]
DIGITO [0-9]


[+\-]?[0-9]{1,10}     {
                        /* Converter o lexema para inteiro (pode estourar se for maior que INT, 
                           mas servirá como exemplo didático) */
                        yylval.intValue = atoi(yytext);
                        return NUM;
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
        }
    }
    return yyparse();
}