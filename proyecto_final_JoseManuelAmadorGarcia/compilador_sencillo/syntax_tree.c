#include "syntax_tree.h"

#define INITIAL_CAPACITY 4

SyntaxTreeNode* crear_nodo(const char *label) {
    SyntaxTreeNode *node = (SyntaxTreeNode *)malloc(sizeof(SyntaxTreeNode));
    node->label = strdup(label); 
    node->children = NULL;       
    node->child_count = 0;
    node->child_capacity = 0;
    return node;
}

void agregar_hijo(SyntaxTreeNode *parent, SyntaxTreeNode *child) {
    if (!parent->children) {
        parent->children = (SyntaxTreeNode **)malloc(INITIAL_CAPACITY * sizeof(SyntaxTreeNode *));
        parent->child_capacity = INITIAL_CAPACITY;
    } else if (parent->child_count == parent->child_capacity) {
        parent->child_capacity *= 2;
        parent->children = (SyntaxTreeNode **)realloc(parent->children, parent->child_capacity * sizeof(SyntaxTreeNode *));
    }
    parent->children[parent->child_count++] = child;
}

void imprimir_arbol(const SyntaxTreeNode *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s\n", node->label);

    for (int i = 0; i < node->child_count; i++) {
        imprimir_arbol(node->children[i], depth + 1);
    }
}

void liberar_arbol(SyntaxTreeNode *node) {
    if (!node) return;

    for (int i = 0; i < node->child_count; i++) {
        liberar_arbol(node->children[i]);
    }

    free(node->label);      
    free(node->children);   
    free(node);             
}
