%{
#include <stdio.h>
#include <stdlib.h>

/* Declarações exigidas pelo Flex/Bison */
int yylex(void);
void yyerror(const char *s);
extern int linha;
%}

/* Declaração de uniões para os valores retornados pelo Flex */
%union {
    int intValue;
    float floatValue;
    char *strValue;
}

/* Definição de todos os tokens mapeados no seu léxico */
%token FUNCTION ELSEIF ELSE IF OR UNTIL WHILE TRUE_VAL FALSE_VAL NOT THEN NIL
%token FOR DO RETURN LOCAL BREAK REPEAT IN END AND
%token VARARG CONCAT DOT EQ NEQ GE GT LT LE ASSIGN PLUS MINUS MULT IDIV DIV MOD POW LEN
%token SEMI COLON COMMA LPAREN RPAREN RBRACKET LBRACE RBRACE LBRACKET

/* Tokens com tipos definidos na união */
%token <intValue> NUM
%token <floatValue> NUM_FLOAT
%token <strValue> ID STRING

%%

/* Regra raiz: aceita um programa vazio ou uma sequência de comandos/declarações */
program:
    /* vazio */
    | program statement
    ;

/* Declaração genérica para absorver comandos de teste */
statement:
      LOCAL ID ASSIGN expr { 
          printf("[SINTÁTICO] Declaração local de variável detectada\n"); 
      }
    | ID ASSIGN expr { 
          printf("[SINTÁTICO] Atribuição detectada\n"); 
      }
    | IF expr THEN statement_list END { 
          printf("[SINTÁTICO] Bloco IF detectado\n"); 
      }
    | WHILE expr DO statement_list END { 
          printf("[SINTÁTICO] Bloco WHILE detectado\n"); 
      }
    | FUNCTION ID LPAREN opt_par_list RPAREN statement_list END { 
          printf("[SINTÁTICO] Definição de função detectada\n"); 
      }
    | expr SEMI { /* Expressão isolada seguida de ponto e vírgula */ }
    | expr      { /* Expressão isolada */ }
    ;

statement_list:
    /* vazio */
    | statement_list statement
    ;

opt_par_list:
    /* vazio */
    | ID
    | opt_par_list COMMA ID
    ;

/* Expressões básicas para permitir testes de valores, literais e identificadores */
expr:
      NUM          { printf("  -> Encontrou inteiro: %d\n", $1); }
    | NUM_FLOAT    { printf("  -> Encontrou float: %f\n", $1); }
    | STRING       { printf("  -> Encontrou string: %s\n", $1); }
    | ID           { printf("  -> Encontrou identificador: %s\n", $1); }
    | TRUE_VAL     { printf("  -> Encontrou booleano: true\n"); }
    | FALSE_VAL    { printf("  -> Encontrou booleano: false\n"); }
    | NIL          { printf("  -> Encontrou nil\n"); }
    | expr PLUS expr
    | expr MINUS expr
    | expr MULT expr
    | expr DIV expr
    | LPAREN expr RPAREN
    ;

%%

/* Função de tratamento de erros sintáticos */
void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático na linha %d: %s\n", linha, s);
}