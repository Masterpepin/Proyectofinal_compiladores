%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex(void);  // Declare yylex function from lexer
extern int yylineno;  // Provided by the lexer when %option yylineno is set
extern FILE *yyin;  // Input file or stdin

// Error handling function
int yyerror(const char *s) {
    extern char *yytext; // Access the current token text
    extern int yylineno; // Access the line number
    fprintf(stderr, "Error at line %d: syntax error near '%s'\n", yylineno, yytext);
    return 0;
}
%}

%union {
    int num;           // For NUMERO
    char* id;          // For IDENTIFICADOR
}

%token PLUS MINUS MUL DIV SEMICOLON LPAREN RPAREN 
%token IF THEN ELSE END REPEAT UNTIL READ WRITE
%token ASSIGN NUMERO IDENTIFICADOR
%token LT GT EQ NE LE GE

%%

programa:
    secuencia_instrucciones
    ;

secuencia_instrucciones:
    secuencia_instrucciones ';' instruccion
    | instruccion
    ;

instruccion:
    instruccion_if
    | instruccion_repeat
    | instruccion_asignacion
    | instruccion_read
    | instruccion_write
    | error
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
    expresion_simple '+' termino
    | expresion_simple '-' termino
    | termino
    ;

termino:
    termino '*' factor
    | termino '/' factor
    | factor
    ;

factor:
    '(' expresion ')'
    | NUMERO
    | IDENTIFICADOR
    | error
    ;

%%

int main() {
    // Open input file if provided
    printf("Parsing started...\n");
    int result = yyparse();
    if (result == 0)
        printf("Parsing completed successfully.\n");
    else
        printf("Parsing failed.\n");
    return result;
}
