#include "syntax_tree.h"

#define INITIAL_CAPACITY 4

// Crear un nuevo nodo del árbol
SyntaxTreeNode* crear_nodo(const char *label) {
    SyntaxTreeNode *node = (SyntaxTreeNode *)malloc(sizeof(SyntaxTreeNode));
    node->label = strdup(label); // Copia la etiqueta
    node->children = NULL;       // Inicialmente sin hijos
    node->child_count = 0;
    node->child_capacity = 0;
    return node;
}

// Agregar un hijo a un nodo
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

// Imprimir el árbol sintáctico con indentación
void imprimir_arbol(const SyntaxTreeNode *node, int depth) {
    if (!node) return;

    // Imprimir espacios para la profundidad actual
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s\n", node->label);

    // Imprimir cada hijo
    for (int i = 0; i < node->child_count; i++) {
        imprimir_arbol(node->children[i], depth + 1);
    }
}

// Liberar la memoria del árbol sintáctico
void liberar_arbol(SyntaxTreeNode *node) {
    if (!node) return;

    // Liberar los hijos recursivamente
    for (int i = 0; i < node->child_count; i++) {
        liberar_arbol(node->children[i]);
    }

    free(node->label);      // Liberar la etiqueta
    free(node->children);   // Liberar el array de hijos
    free(node);             // Liberar el nodo
}
