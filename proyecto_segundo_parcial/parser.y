%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "syntax_tree.h"

extern int yylex(void);
extern int yylineno;

int yyerror(const char *s) {
    extern char *yytext;
    fprintf(stderr, "Error en la línea %d: %s cerca de '%s'\n", yylineno, s, yytext);
    return 0;
}

SymbolTable *tabla_simbolos; // Tabla de símbolos
SyntaxTreeNode *root;        // Nodo raíz del árbol sintáctico
%}

%union {
    int num;                 // Para números
    char* id;                // Para identificadores
    SyntaxTreeNode *node;    // Para nodos del árbol sintáctico
}

%token <num> NUMERO
%token <id> IDENTIFICADOR
%token IF THEN ELSE END REPEAT UNTIL READ WRITE
%token ASSIGN LT GT EQ NE LE GE PLUS MINUS MUL DIV LPAREN RPAREN

%type <node> programa secuencia_instrucciones secuencia_instrucciones_opt
%type <node> instruccion instruccion_if instruccion_repeat instruccion_asignacion
%type <node> instruccion_read instruccion_write
%type <node> expresion expresion_simple termino factor

%%

programa:
    secuencia_instrucciones {
        root = $1;
        imprimir_arbol(root, 0);
        liberar_arbol(root);
        imprimir_tabla(tabla_simbolos);
        liberar_tabla(tabla_simbolos);
    }
    ;

secuencia_instrucciones:
    instruccion secuencia_instrucciones_opt {
        $$ = crear_nodo("Instrucciones");
        agregar_hijo($$, $1);
        if ($2) agregar_hijo($$, $2);
    }
    ;

secuencia_instrucciones_opt:
    instruccion secuencia_instrucciones_opt {
        $$ = crear_nodo("Instrucciones");
        agregar_hijo($$, $1);
        if ($2) agregar_hijo($$, $2);
    }
    |
    { $$ = NULL; }
    ;

instruccion:
    instruccion_if
    | instruccion_repeat
    | instruccion_asignacion
    | instruccion_read
    | instruccion_write
    | error { yyerror("Instrucción inválida"); $$ = crear_nodo("Error"); }
    ;

instruccion_if:
    IF expresion THEN secuencia_instrucciones END {
        $$ = crear_nodo("IF");
        agregar_hijo($$, $2); // Expresión
        agregar_hijo($$, $4); // Instrucciones
    }
    | IF expresion THEN secuencia_instrucciones ELSE secuencia_instrucciones END {
        $$ = crear_nodo("IF");
        agregar_hijo($$, $2); // Expresión
        agregar_hijo($$, $4); // Instrucciones (THEN)
        agregar_hijo($$, $6); // Instrucciones (ELSE)
    }
    ;

instruccion_repeat:
    REPEAT secuencia_instrucciones UNTIL expresion {
        $$ = crear_nodo("REPEAT");
        agregar_hijo($$, $2); // Instrucciones
        agregar_hijo($$, $4); // Condición
    }
    ;

instruccion_asignacion:
    IDENTIFICADOR ASSIGN expresion {
        agregar_simbolo(tabla_simbolos, $1, yylineno);
        agregar_referencia(tabla_simbolos, $1, yylineno);
        $$ = crear_nodo("ASSIGN");
        agregar_hijo($$, crear_nodo($1)); // Nodo del identificador
        agregar_hijo($$, $3); // Expresión
        free($1);
    }
    ;

instruccion_read:
    READ IDENTIFICADOR {
        agregar_simbolo(tabla_simbolos, $2, yylineno);
        agregar_referencia(tabla_simbolos, $2, yylineno);
        $$ = crear_nodo("READ");
        agregar_hijo($$, crear_nodo($2)); // Nodo del identificador
        free($2);
    }
    ;

instruccion_write:
    WRITE expresion {
        $$ = crear_nodo("WRITE");
        agregar_hijo($$, $2);
    }
    ;

expresion:
    expresion_simple LT expresion_simple {
        $$ = crear_nodo("LT");
        agregar_hijo($$, $1);
        agregar_hijo($$, $3);
    }
    | IDENTIFICADOR {
        agregar_referencia(tabla_simbolos, $1, yylineno);
        $$ = crear_nodo($1); // Nodo del identificador
        free($1);
    }
    | NUMERO {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%d", $1);
        $$ = crear_nodo(buffer); // Nodo del número
    }
    ;

expresion_simple:
    termino
    | expresion_simple PLUS termino {
        $$ = crear_nodo("PLUS");
        agregar_hijo($$, $1);
        agregar_hijo($$, $3);
    }
    ;

termino:
    factor
    | termino MUL factor {
        $$ = crear_nodo("MUL");
        agregar_hijo($$, $1);
        agregar_hijo($$, $3);
    }
    ;

factor:
    LPAREN expresion RPAREN {
        $$ = $2;
    }
    | NUMERO {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%d", $1);
        $$ = crear_nodo(buffer);
    }
    | IDENTIFICADOR {
        agregar_referencia(tabla_simbolos, $1, yylineno);
        $$ = crear_nodo($1);
        free($1);
    }
    ;

%%

int main() {
    tabla_simbolos = crear_tabla();
    printf("Inicio del análisis...\n");
    int resultado = yyparse();
    if (resultado == 0)
        printf("Análisis completado con éxito.\n");
    else
        printf("Fallo en el análisis.\n");
    return resultado;
}
