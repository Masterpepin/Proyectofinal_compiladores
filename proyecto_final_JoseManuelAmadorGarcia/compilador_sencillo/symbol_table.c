#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

#define INITIAL_CAPACITY 4

SymbolTable* crear_tabla() {
    SymbolTable *tabla = (SymbolTable *)malloc(sizeof(SymbolTable));
    tabla->primero = NULL;
    return tabla;
}

Symbol* buscar_simbolo(SymbolTable *tabla, const char *nombre) {
    Symbol *actual = tabla->primero;
    while (actual) {
        if (strcmp(actual->nombre, nombre) == 0) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

void agregar_simbolo(SymbolTable *tabla, const char *nombre, int linea_declaracion) {
    if (buscar_simbolo(tabla, nombre)) {
        return; // El símbolo ya existe, no se agrega
    }

    Symbol *nuevo = (Symbol *)malloc(sizeof(Symbol));
    nuevo->nombre = strdup(nombre);
    nuevo->linea_declaracion = linea_declaracion;
    nuevo->lineas_referencia = NULL;
    nuevo->referencias_count = 0;
    nuevo->referencias_capacity = 0;
    nuevo->siguiente = tabla->primero;
    tabla->primero = nuevo;
}

void agregar_referencia(SymbolTable *tabla, const char *nombre, int linea_referencia) {
    Symbol *simbolo = buscar_simbolo(tabla, nombre);
    if (!simbolo) {
        agregar_simbolo(tabla, nombre, -1); // Se agrega como referencia sin declaración
        simbolo = buscar_simbolo(tabla, nombre);
    }

    if (simbolo->referencias_count == simbolo->referencias_capacity) {
        simbolo->referencias_capacity = simbolo->referencias_capacity == 0 ? INITIAL_CAPACITY : simbolo->referencias_capacity * 2;
        simbolo->lineas_referencia = (int *)realloc(simbolo->lineas_referencia, simbolo->referencias_capacity * sizeof(int));
    }

    simbolo->lineas_referencia[simbolo->referencias_count++] = linea_referencia;
}

void imprimir_tabla(const SymbolTable *tabla) {
    Symbol *actual = tabla->primero;
    printf("Nombre\tLínea Declaración\tLíneas Referencia\n");
    while (actual) {
        printf("%s\t%d\t\t", actual->nombre, actual->linea_declaracion);
        for (int i = 0; i < actual->referencias_count; i++) {
            printf("%d ", actual->lineas_referencia[i]);
        }
        printf("\n");
        actual = actual->siguiente;
    }
}

void liberar_tabla(SymbolTable *tabla) {
    Symbol *actual = tabla->primero;
    while (actual) {
        Symbol *temp = actual;
        actual = actual->siguiente;
        free(temp->nombre);
        free(temp->lineas_referencia);
        free(temp);
    }
    free(tabla);
}
