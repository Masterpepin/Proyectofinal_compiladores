#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "symbol_table.h"
#include "syntax_tree.h"
#include "parser.tab.h"

using namespace std;

// Constantes
const int INSTRUCCIONES_MAXIMO = 100;

// Memoria de instrucciones
string instrucciones_Memoria[INSTRUCCIONES_MAXIMO];
int pc = 0;          // Contador de programa (índice de la instrucción actual)
bool halt = false;   // Bandera para detener la ejecución
bool trace_activado = false; // Bandera para mostrar las instrucciones paso a paso

// Árbol sintáctico y tabla de símbolos (usados por el compilador)
SyntaxTreeNode* root = nullptr;
SymbolTable* tabla_simbolos = nullptr;

// Declarar externamente `yyin`
extern FILE* yyin;

// Funciones para la máquina virtual
void imprimir_ayuda() {
    cout << "Comandos disponibles:\n";
    cout << "  Ejecuta <n>    Ejecuta n instrucciones (por defecto, 1)\n";
    cout << "  Go             Ejecuta todas las instrucciones hasta el HALT\n";
    cout << "  Arbol          Imprime el árbol sintáctico del programa\n";
    cout << "  Tabla          Imprime la tabla de símbolos generada\n";
    cout << "  Compilar       Compila un programa fuente\n";
    cout << "  Cargar         Carga un archivo de instrucciones compilado\n";
    cout << "  Trace          Activa o desactiva el modo de ejecución paso a paso\n";
    cout << "  Inicio         Reinicia el simulador\n";
    cout << "  Fin            Finaliza la ejecución de la máquina\n";
    cout << "  Ayuda          Muestra esta lista de comandos\n";
}

// Cargar instrucciones desde un archivo
bool cargar_instrucciones(const string& nombre_archivo) {
    ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo de instrucciones: " << nombre_archivo << endl;
        return false;
    }

    string linea;
    int indice = 0;
    while (getline(archivo, linea) && indice < INSTRUCCIONES_MAXIMO) {
        if (linea.empty() || linea[0] == '*') continue; // Ignorar líneas vacías o comentarios
        instrucciones_Memoria[indice++] = linea;
    }
    archivo.close();
    cout << "Archivo de instrucciones cargado correctamente.\n";
    return true;
}

// Ejecutar una instrucción
void ejecutar_instruccion() {
    if (halt || pc >= INSTRUCCIONES_MAXIMO) return;

    string instruccion = instrucciones_Memoria[pc];
    if (instruccion.empty()) {
        cerr << "Error: Instrucción vacía en PC = " << pc << endl;
        halt = true;
        return;
    }

    if (trace_activado) {
        cout << "[" << pc << "]: " << instruccion << endl;
    }

    if (instruccion == "HALT") {
        halt = true;
        cout << "Ejecución detenida (HALT).\n";
    }
    pc++;
}

// Ejecutar `n` instrucciones
void ejecutar_n_instrucciones(int n) {
    for (int i = 0; i < n && pc < INSTRUCCIONES_MAXIMO && !halt; ++i) {
        ejecutar_instruccion();
    }
}

// Ejecutar todas las instrucciones hasta `HALT`
void ejecutar_todas_instrucciones() {
    while (pc < INSTRUCCIONES_MAXIMO && !halt) {
        ejecutar_instruccion();
    }
}

// Reiniciar el simulador
void reiniciar_simulador() {
    pc = 0;
    halt = false;
    for (int i = 0; i < INSTRUCCIONES_MAXIMO; ++i) {
        instrucciones_Memoria[i].clear();
    }
    cout << "Simulador reiniciado.\n";
}

// Activar o desactivar el modo de trace
void activar_trace() {
    trace_activado = !trace_activado;
    cout << "Modo trace " << (trace_activado ? "activado" : "desactivado") << endl;
}

// Compilar un programa fuente
bool compilar_programa(const string& nombre_archivo) {
    tabla_simbolos = crear_tabla();
    cout << "Compilando archivo fuente: " << nombre_archivo << "...\n";

    // Abrir el archivo fuente
    yyin = fopen(nombre_archivo.c_str(), "r");
    if (!yyin) {
        cerr << "Error: No se puede abrir el archivo fuente " << nombre_archivo << endl;
        return false;
    }

    // Ejecutar el parser
    if (myparserparse() != 0) {
        cerr << "Error: Fallo en la compilación.\n";
        liberar_tabla(tabla_simbolos);
        liberar_arbol(root);
        fclose(yyin);
        return false;
    }

    fclose(yyin); // Cerrar el archivo fuente
    cout << "Compilación exitosa.\n";
    return true;
}

// Menú interactivo
void ejecutar_maquina_virtual() {
    string comando;
    bool ejecutando = true;

    while (ejecutando) {
        cout << "Comando: ";
        getline(cin, comando);

        stringstream ss(comando);
        string cmd;
        ss >> cmd;

        if (cmd == "a" || cmd == "Ayuda") {
            imprimir_ayuda();
        }
        else if (cmd == "Ejecuta") {
            int n = 1; // Por defecto, 1 instrucción
            if (!(ss >> n)) n = 1;
            ejecutar_n_instrucciones(n);
        }
        else if (cmd == "Go") {
            ejecutar_todas_instrucciones();
        }
        else if (cmd == "Arbol") {
            imprimir_arbol(root, 0);
        }
        else if (cmd == "Tabla") {
            imprimir_tabla(tabla_simbolos);
        }
        else if (cmd == "Compilar") {
            string archivo_fuente;
            if (ss >> archivo_fuente) {
                if (compilar_programa(archivo_fuente)) {
                    cout << "Árbol y tabla generados correctamente.\n";
                }
            } else {
                cerr << "Error: Especifique el archivo fuente.\n";
            }
        }
        else if (cmd == "Cargar") {
            string archivo_instrucciones;
            if (ss >> archivo_instrucciones) {
                cargar_instrucciones(archivo_instrucciones);
            } else {
                cerr << "Error: Especifique el archivo de instrucciones.\n";
            }
        }
        else if (cmd == "Trace") {
            activar_trace();
        }
        else if (cmd == "Inicio") {
            reiniciar_simulador();
        }
        else if (cmd == "Fin") {
            cout << "Finalizando la máquina virtual...\n";
            ejecutando = false;
        }
        else {
            cout << "Comando no reconocido. Teclee 'a' para ayuda.\n";
        }
    }
}

// Función principal
int main(int argc, char* argv[]) {
    cout << "Máquina virtual iniciada. (Teclee 'a' para ver el menú de ayuda)\n";
    ejecutar_maquina_virtual();
    return 0;
}
