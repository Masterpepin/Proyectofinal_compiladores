#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct Symbol {
    char *nombre;
    int linea_declaracion;
    int *lineas_referencia;
    int referencias_count;
    int referencias_capacity;
    struct Symbol *siguiente;
} Symbol;

typedef struct SymbolTable {
    Symbol *primero;
} SymbolTable;

extern SymbolTable* tabla_simbolos;
// Funciones para gestionar la tabla de símbolos
SymbolTable* crear_tabla();
void agregar_simbolo(SymbolTable *tabla, const char *nombre, int linea_declaracion);
void agregar_referencia(SymbolTable *tabla, const char *nombre, int linea_referencia);
void imprimir_tabla(const SymbolTable *tabla);
void liberar_tabla(SymbolTable *tabla);

#endif // SYMBOL_TABLE_H
