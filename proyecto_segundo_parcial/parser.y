%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex(void);
extern int yylineno;  // Line number provided by the lexer

int yyerror(const char *s) {
    extern char *yytext; // Access the current token text
    extern int yylineno; // Access the line number
    fprintf(stderr, "Error at line %d: syntax error near '%s'\n", yylineno, yytext);
    return 0;
}
%}

%union {
    int num;        // For numeric values
    char* id;       // For identifiers
}

%token <num> NUMERO
%token <id> IDENTIFICADOR
%token IF THEN ELSE END REPEAT UNTIL READ WRITE
%token ASSIGN LT GT EQ NE LE GE PLUS MINUS MUL DIV LPAREN RPAREN
%type <num> expresion expresion_simple termino factor

%%

programa:
    secuencia_instrucciones
    ;

secuencia_instrucciones:
    instruccion secuencia_instrucciones_opt
    ;

secuencia_instrucciones_opt:
    instruccion secuencia_instrucciones_opt
    |
    ;

instruccion:
    instruccion_if
    | instruccion_repeat
    | instruccion_asignacion
    | instruccion_read
    | instruccion_write
    | error { yyerror("Invalid instruction"); }
    ;

instruccion_if:
    IF expresion THEN secuencia_instrucciones END
    | IF expresion THEN secuencia_instrucciones ELSE secuencia_instrucciones END
    ;

instruccion_repeat:
    REPEAT secuencia_instrucciones UNTIL expresion
    ;

instruccion_asignacion:
    IDENTIFICADOR ASSIGN expresion
    ;

instruccion_read:
    READ IDENTIFICADOR
    ;

instruccion_write:
    WRITE expresion
    ;

expresion:
    expresion_simple LT expresion_simple
    | expresion_simple GT expresion_simple
    | expresion_simple EQ expresion_simple
    | expresion_simple NE expresion_simple
    | expresion_simple LE expresion_simple
    | expresion_simple GE expresion_simple
    | expresion_simple
    ;

expresion_simple:
    expresion_simple PLUS termino
    | expresion_simple MINUS termino
    | termino
    ;

termino:
    termino MUL factor
    | termino DIV factor
    | factor
    ;

factor:
    LPAREN expresion RPAREN
        { $$ = $2; }
    | NUMERO
        { $$ = $1; }
    | IDENTIFICADOR
        { $$ = 0; }
    | error
        { yyerror("Invalid factor"); $$ = 0; }
    ;

%%

int main() {
    printf("Parsing started...\n");
    int result = yyparse();
    if (result == 0)
        printf("Parsing completed successfully.\n");
    else
        printf("Parsing failed.\n");
    return result;
}
