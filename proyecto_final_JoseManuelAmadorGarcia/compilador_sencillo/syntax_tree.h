#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SyntaxTreeNode {
    char *label;                      // Etiqueta del nodo 
    struct SyntaxTreeNode **children; 
    int child_count;                  
    int child_capacity;               
} SyntaxTreeNode;


SyntaxTreeNode* crear_nodo(const char *label);

void agregar_hijo(SyntaxTreeNode *parent, SyntaxTreeNode *child);

void imprimir_arbol(const SyntaxTreeNode *node, int depth);

void liberar_arbol(SyntaxTreeNode *node);

#endif 
