%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

extern int yylex(void);
extern int yylineno;

int yyerror(const char *s) {
    extern char *yytext;
    fprintf(stderr, "Error en la línea %d: %s cerca de '%s'\n", yylineno, s, yytext);
    return 0;
}

SymbolTable *tablaSimbolos;
%}

%union {
    int num;        // Para valores numéricos
    char* id;       // Para identificadores
}

%token <num> NUMERO
%token <id> IDENTIFICADOR
%token IF THEN ELSE END REPEAT UNTIL READ WRITE
%token ASSIGN LT GT EQ NE LE GE PLUS MINUS MUL DIV LPAREN RPAREN
%type <num> expresion expresion_simple termino factor

%%

programa:
    secuencia_instrucciones { 
        printf("\n--- Análisis finalizado ---\n");
        printf("Tabla de símbolos:\n");
        imprimir_tabla(tablaSimbolos);
        liberar_tabla(tablaSimbolos);
    }
    ;

secuencia_instrucciones:
    instruccion secuencia_instrucciones_opt
    ;

secuencia_instrucciones_opt:
    instruccion secuencia_instrucciones_opt
    | /* vacío */
    ;

instruccion:
    instruccion_if
    | instruccion_repeat
    | instruccion_asignacion
    | instruccion_read
    | instruccion_write
    | error { yyerror("Instrucción inválida"); }
    ;

instruccion_if:
    IF expresion THEN secuencia_instrucciones END
    | IF expresion THEN secuencia_instrucciones ELSE secuencia_instrucciones END
    ;

instruccion_repeat:
    REPEAT secuencia_instrucciones UNTIL expresion
    ;

instruccion_asignacion:
    IDENTIFICADOR ASSIGN expresion { 
        agregar_simbolo(tablaSimbolos, $1, yylineno);
    }
    ;

instruccion_read:
    READ IDENTIFICADOR { 
        agregar_simbolo(tablaSimbolos, $2, yylineno);
    }
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
        { 
            agregar_referencia(tablaSimbolos, $1, yylineno);
            $$ = 0; 
        }
    | error
        { yyerror("Factor inválido"); $$ = 0; }
    ;

%%

int main() {
    tablaSimbolos = crear_tabla();
    printf("Inicio del análisis...\n");
    int resultado = yyparse();
    if (resultado == 0)
        printf("Análisis completado con éxito.\n");
    else
        printf("Hubo errores en el análisis.\n");
    return resultado;
}
