#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

// Estructura para un nodo del árbol sintáctico
typedef struct SyntaxTreeNode {
    char *label;                      // Etiqueta del nodo (e.g., "IF", "ASSIGN")
    struct SyntaxTreeNode **children; // Puntero a un array de hijos
    int child_count;                  // Número de hijos
    int child_capacity;               // Capacidad del array de hijos
} SyntaxTreeNode;

// Declaraciones de variables globales
extern SymbolTable* tabla_simbolos; // Declarada como extern después de incluir symbol_table.h
extern SyntaxTreeNode* root;        // Declarada como extern después de la definición de SyntaxTreeNode

// Crear un nuevo nodo del árbol
SyntaxTreeNode* crear_nodo(const char *label);

// Agregar un hijo a un nodo
void agregar_hijo(SyntaxTreeNode *parent, SyntaxTreeNode *child);

// Imprimir el árbol sintáctico con indentación
void imprimir_arbol(const SyntaxTreeNode *node, int depth);

// Liberar la memoria del árbol sintáctico
void liberar_arbol(SyntaxTreeNode *node);

#endif // SYNTAX_TREE_H
