#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// Constantes
const int INSTRUCCIONES_MAXIMO = 100;
const int DATOS_MAXIMO = 1024;
const int NUMERO_REGISTROS = 8;

// Memorias y registros
int datos_Memoria[DATOS_MAXIMO] = {0};
int reg[NUMERO_REGISTROS] = {0};
string instrucciones_Memoria[INSTRUCCIONES_MAXIMO];
int pc = 0; // Contador de programa (índice de la instrucción actual)
bool halt = false; // Bandera para detener la ejecución
bool trace_activado = false; // Bandera para activar la visualización paso a paso

// Funciones para el simulador
void imprimir_ayuda() {
    cout << "Comandos disponibles:\n";
    cout << "  Ejecuta <n>    Ejecuta n (si se omite 1) instrucciones\n";
    cout << "  Go             Ejecuta todas las instrucciones hasta el HALT\n";
    cout << "  Registros      Imprime el contenido de los registros\n";
    cout << "  Codigo <b> <n> Imprime n instrucciones comenzando por b\n";
    cout << "  Trace          Muestra las instrucciones a medida que se ejecutan\n";
    cout << "  Inicio         Reinicia el simulador para una nueva ejecucion\n";
    cout << "  Fin            Finaliza la ejecucion del codigo y sale de la maquina\n";
    cout << "  Ayuda          Imprime esta lista de ayuda\n";
}

// Función para cargar las instrucciones, filtrando comentarios y líneas en blanco
bool cargar_instrucciones(const string& nombre_archivo) {
    ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo de instrucciones: " << nombre_archivo << endl;
        return false;
    }

    string linea;
    int indice = 0;
    while (getline(archivo, linea) && indice < INSTRUCCIONES_MAXIMO) {
        // Ignorar líneas en blanco
        if (linea.empty()) continue;

        // Ignorar líneas que comienzan con un asterisco 
        if (linea[0] == '*') continue;

        // Eliminar comentarios que empiezan después de un asterisco
        size_t pos_comentario = linea.find('*');
        if (pos_comentario != string::npos) {
            linea = linea.substr(0, pos_comentario);
        }

        // Buscar y eliminar el número de línea seguido de dos puntos
        size_t pos_dos_puntos = linea.find(':');
        if (pos_dos_puntos != string::npos) {
            linea = linea.substr(pos_dos_puntos + 1);
        } else {
            // Si no hay dos puntos, línea inválida
            cerr << "Error: Formato de línea inválido (falta ':') en: " << linea << endl;
            continue;
        }

        // Eliminar espacios en blanco sobrantes al inicio y al final
        size_t inicio = linea.find_first_not_of(" \t");
        size_t fin = linea.find_last_not_of(" \t");
        if (inicio == string::npos || fin == string::npos) {
            // Línea contiene solo espacios
            continue;
        }
        linea = linea.substr(inicio, fin - inicio + 1);

        if (!linea.empty()) {
            instrucciones_Memoria[indice] = linea;
            indice++;
        }
    }
    //contador para el registro 7
    int contador = 0;
    std::string lineas;
    while (std::getline(archivo, lineas) && contador < INSTRUCCIONES_MAXIMO) {
        instrucciones_Memoria[contador] = lineas;
        contador++;
    }
    archivo.close();
    return true;
}

// Función para imprimir el estado de los registros
void imprimir_estado() {
    cout << "Estado de los registros:\n";
    for (int i = 0; i < NUMERO_REGISTROS; ++i) {
        cout << "Registro[" << i << "] = " << reg[i] << "    ";
        if ((i + 1) % 4 == 0) cout << endl;
    }
    cout << "---------------------\n";
}

// Función para ejecutar una instrucción
void ejecutar_instruccion() {
    if (halt || pc >= INSTRUCCIONES_MAXIMO) return;

    string instruccion = instrucciones_Memoria[pc];
    if (instruccion.empty()) {
        cerr << "Error: Instruccion vacía en PC = " << pc << endl;
        halt = true;
        return;
    }

    // Extraer la operación y los argumentos
    size_t pos_espacio = instruccion.find(" ");
    string operacion = (pos_espacio != string::npos) ? instruccion.substr(0, pos_espacio) : instruccion;
    string argumentos = (pos_espacio != string::npos) ? instruccion.substr(pos_espacio + 1) : "";

    if (trace_activado) {
        cout << "[" << pc << "]: " << operacion << " " << argumentos << endl;
    }
    if (operacion == "JLT") {
        int r, a;
        if (sscanf(argumentos.c_str(), "%d,%d", &r, &a) == 2) {
            if (r >= 0 && r < NUMERO_REGISTROS && a >= 0 && a < INSTRUCCIONES_MAXIMO) {
                if (reg[r] < 0) {
                    pc = a;
                    return;
                }
            } else {
                cerr << "Error: Índice de registro o dirección fuera de rango en JLT." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para JLT: " << argumentos << endl;
            halt = true;
            return;
        }
    } else if (operacion == "JLE") {
        int r, a;
        if (sscanf(argumentos.c_str(), "%d,%d", &r, &a) == 2) {
            if (r >= 0 && r < NUMERO_REGISTROS && a >= 0 && a < INSTRUCCIONES_MAXIMO) {
                if (reg[r] <= 0) {
                    pc = a;
                    return;
                }
            } else {
                cerr << "Error: Índice de registro o dirección fuera de rango en JLE." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para JLE: " << argumentos << endl;
            halt = true;
            return;
        }
    } else if (operacion == "JGE") {
        int r, a;
        if (sscanf(argumentos.c_str(), "%d,%d", &r, &a) == 2) {
            if (r >= 0 && r < NUMERO_REGISTROS && a >= 0 && a < INSTRUCCIONES_MAXIMO) {
                if (reg[r] >= 0) {
                    pc = a;
                    return;
                }
            } else {
                cerr << "Error: Índice de registro o dirección fuera de rango en JGE." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para JGE: " << argumentos << endl;
            halt = true;
            return;
        }
    } else if (operacion == "JGT") {
        int r, a;
        if (sscanf(argumentos.c_str(), "%d,%d", &r, &a) == 2) {
            if (r >= 0 && r < NUMERO_REGISTROS && a >= 0 && a < INSTRUCCIONES_MAXIMO) {
                if (reg[r] > 0) {
                    pc = a;
                    return;
                }
            } else {
                cerr << "Error: Índice de registro o dirección fuera de rango en JGT." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para JGT: " << argumentos << endl;
            halt = true;
            return;
        }
    } 
    // Instrucción LDC (Load Constant)
    else if (operacion == "LDC") {
        int r, d;
        if (sscanf(argumentos.c_str(), "%d,%d", &r, &d) == 2) {
            if (r >= 0 && r < NUMERO_REGISTROS) {
                reg[r] = d;
            } else {
                cerr << "Error: Índice de registro fuera de rango en LDC." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para LDC: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    // Instrucción IN
    if (operacion == "IN") {
        int reg_num;
        if (sscanf(argumentos.c_str(), "%d", &reg_num) == 1) {
            if (reg_num >= 0 && reg_num < NUMERO_REGISTROS) {
                cout << "Entrada para Registro[" << reg_num << "]: ";
                cin >> reg[reg_num];
            } else {
                cerr << "Error: Índice de registro fuera de rango en IN." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para IN: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    // Instrucción OUT
    else if (operacion == "OUT") {
        int reg_num;
        if (sscanf(argumentos.c_str(), "%d", &reg_num) == 1) {
            if (reg_num >= 0 && reg_num < NUMERO_REGISTROS) {
                cout << "Salida de Registro[" << reg_num << "]: " << reg[reg_num] << endl;
            } else {
                cerr << "Error: Índice de registro fuera de rango en OUT." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para OUT: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    // Instrucción MUL
    else if (operacion == "MUL") {
        int reg_num, reg_fuente1, reg_fuente2;
        if (sscanf(argumentos.c_str(), "%d,%d,%d", &reg_num, &reg_fuente1, &reg_fuente2) == 3) {
            if (reg_num >= 0 && reg_num < NUMERO_REGISTROS &&
                reg_fuente1 >= 0 && reg_fuente1 < NUMERO_REGISTROS &&
                reg_fuente2 >= 0 && reg_fuente2 < NUMERO_REGISTROS) {
                reg[reg_num] = reg[reg_fuente1] * reg[reg_fuente2];
            } else {
                cerr << "Error: Índice de registro fuera de rango en MUL." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para MUL: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    // Instrucción DIV
    else if (operacion == "DIV") {
        int reg_num, reg_fuente1, reg_fuente2;
        if (sscanf(argumentos.c_str(), "%d,%d,%d", &reg_num, &reg_fuente1, &reg_fuente2) == 3) {
            if (reg_num >= 0 && reg_num < NUMERO_REGISTROS &&
                reg_fuente1 >= 0 && reg_fuente1 < NUMERO_REGISTROS &&
                reg_fuente2 >= 0 && reg_fuente2 < NUMERO_REGISTROS) {
                if (reg[reg_fuente2] != 0) {
                    reg[reg_num] = reg[reg_fuente1] / reg[reg_fuente2];
                } else {
                    cerr << "Error: División por cero en DIV." << endl;
                    halt = true;
                    return;
                }
            } else {
                cerr << "Error: Índice de registro fuera de rango en DIV." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para DIV: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    // Instruccion LD
    if (operacion == "LD") {
        int reg_num, dir, base;
        if (sscanf(argumentos.c_str(), "%d,%d(%d)", &reg_num, &dir, &base) == 3) {
            if (reg_num >= 0 && reg_num < NUMERO_REGISTROS &&
                base >= 0 && base < NUMERO_REGISTROS &&
                (dir + reg[base]) >= 0 && (dir + reg[base]) < DATOS_MAXIMO) {
                reg[reg_num] = datos_Memoria[dir + reg[base]];
            } else {
                cerr << "Error: Índice de registro o dirección de memoria fuera de rango en LD." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para LD: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    // Instruccion ST
    else if (operacion == "ST") {
        int reg_num, dir, base;
        if (sscanf(argumentos.c_str(), "%d,%d(%d)", &reg_num, &dir, &base) == 3) {
            if (reg_num >= 0 && reg_num < NUMERO_REGISTROS &&
                base >= 0 && base < NUMERO_REGISTROS &&
                (dir + reg[base]) >= 0 && (dir + reg[base]) < DATOS_MAXIMO) {
                datos_Memoria[dir + reg[base]] = reg[reg_num];
            } else {
                cerr << "Error: Índice de registro o dirección de memoria fuera de rango en ST." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para ST: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    // Instrucciones aritméticas
    else if (operacion == "ADD") {
        int reg_num, reg_fuente1, reg_fuente2;
        if (sscanf(argumentos.c_str(), "%d,%d,%d", &reg_num, &reg_fuente1, &reg_fuente2) == 3) {
            if (reg_num >= 0 && reg_num < NUMERO_REGISTROS &&
                reg_fuente1 >= 0 && reg_fuente1 < NUMERO_REGISTROS &&
                reg_fuente2 >= 0 && reg_fuente2 < NUMERO_REGISTROS) {
                reg[reg_num] = reg[reg_fuente1] + reg[reg_fuente2];
            } else {
                cerr << "Error: Índice de registro fuera de rango en ADD." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para ADD: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    else if (operacion == "SUB") {
        int reg_num, reg_fuente1, reg_fuente2;
        if (sscanf(argumentos.c_str(), "%d,%d,%d", &reg_num, &reg_fuente1, &reg_fuente2) == 3) {
            if (reg_num >= 0 && reg_num < NUMERO_REGISTROS &&
                reg_fuente1 >= 0 && reg_fuente1 < NUMERO_REGISTROS &&
                reg_fuente2 >= 0 && reg_fuente2 < NUMERO_REGISTROS) {
                reg[reg_num] = reg[reg_fuente1] - reg[reg_fuente2];
            } else {
                cerr << "Error: Índice de registro fuera de rango en SUB." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para SUB: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    // Instrucciones de salto
    else if (operacion == "JEQ") {
        int r, a, s;
        // Modificar para leer el formato r,a(s)
        if (sscanf(argumentos.c_str(), "%d,%d(%d)", &r, &a, &s) == 3) {
            // Validar que los registros y el salto estén en rango
            if (r >= 0 && r < NUMERO_REGISTROS && s >= 0 && s < NUMERO_REGISTROS) {
                if (reg[r] == 0) {  // Si el valor del registro r es 0
                    int salto_relativo = a + reg[s];  // Salto relativo es 'a' más el valor del registro s
                    if (pc + salto_relativo < INSTRUCCIONES_MAXIMO && pc + salto_relativo >= 0) {
                        pc = pc + salto_relativo;  // Realiza el salto
                        return; 
                    } else {
                        cerr << "Error: Salto fuera de los límites de la memoria de instrucciones." << endl;
                        halt = true;
                        return;
                    }
                }
            } else {
                cerr << "Error: Índice de registro fuera de rango en JEQ." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para JEQ: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    //Instrucción JNE
    else if (operacion == "JNE") {
        int r, s, dir;
        if (sscanf(argumentos.c_str(), "%d,%d,%d", &r, &s, &dir) == 3) {
            if (reg[r] != reg[s]) {
                pc = dir;
                return; 
            }
        } else {
            cerr << "Error: Sintaxis inválida para JNE: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    //instruccion LDA
    else if (operacion == "LDA") {
        int reg_num, dir;
        if (sscanf(argumentos.c_str(), "%d,%d", &reg_num, &dir) == 2) {
            if (reg_num >= 0 && reg_num < NUMERO_REGISTROS) {
                reg[reg_num] = dir;
            } else {
                cerr << "Error: Índice de registro fuera de rango en LDA." << endl;
                halt = true;
                return;
            }
        } else {
            cerr << "Error: Sintaxis inválida para LDA: " << argumentos << endl;
            halt = true;
            return;
        }
    }
    // Instrucción HALT
    else if (operacion == "HALT") {
        halt = true;
        cout << "Ejecucion detenida con HALT." << endl;
    }

    pc++;
}

// Función para ejecutar n instrucciones
void ejecutar_n_instrucciones(int n) {
    for (int i = 0; i < n && pc < INSTRUCCIONES_MAXIMO && !halt; ++i) {
        if (instrucciones_Memoria[i].empty()) break; // Detener si no hay más instrucciones
        reg[7] = i; // Actualizar registro 7 con la línea actual
        ejecutar_instruccion();
        if (trace_activado) {
            imprimir_estado();
        }
    }
}

// Función para ejecutar todas las instrucciones hasta HALT
void ejecutar_todas_instrucciones() {
    while (pc < INSTRUCCIONES_MAXIMO && !halt) {
        ejecutar_instruccion();
        if (trace_activado) {
            imprimir_estado();
        }
    }
}

// Función para imprimir el contenido de los registros
void imprimir_registros() {
    for (int i = 0; i < NUMERO_REGISTROS; ++i) {
        cout << "Registro[" << i << "] = " << reg[i] << "    ";
        if ((i + 1) % 4 == 0) cout << endl;
    }
}

// Función para imprimir instrucciones
void imprimir_instrucciones(int b, int n) {
    for (int i = b; i < b + n && i < INSTRUCCIONES_MAXIMO; ++i) {
        cout << "Instruccion[" << i << "]: " << instrucciones_Memoria[i] << endl;
    }
}

// Función para activar/desactivar trace
void activar_trace() {
    trace_activado = !trace_activado;
    cout << "Modo trace " << (trace_activado ? "activado" : "desactivado") << endl;
}

// Función para reiniciar el simulador
void reiniciar_simulador() {
    pc = 0;
    halt = false;
    for (int i = 0; i < NUMERO_REGISTROS; i++) {
        reg[i] = 0;
    }
    for (int i = 0; i < DATOS_MAXIMO; i++) {
        datos_Memoria[i] = 0;
    }
    cout << "Simulador reiniciado.\n";
}

// Función para manejar el modo interactivo
void ejecutar_maquina_virtual() {
    string comando;
    bool ejecutando = true;

    while (ejecutando) {
        cout << "Comando: ";
        getline(cin, comando);

        // Usar stringstream para manejar múltiples parámetros
        stringstream ss(comando);
        string cmd;
        ss >> cmd;

        if (cmd == "a" || cmd == "Ayuda") {
            imprimir_ayuda();
        }
        else if (cmd == "Ejecuta") {
            int n = 1; // Valor por defecto
            if (!(ss >> n)) {
                n = 1; // Si no se proporciona n, usar 1
            }
            ejecutar_n_instrucciones(n);
            cout << "OK\n";
        }
        else if (cmd == "Go") {
            ejecutar_todas_instrucciones();
            cout << "OK\n";
        }
        else if (cmd == "Registros") {
            imprimir_registros();
        }
        else if (cmd == "Codigo") {
            int b, n;
            if (ss >> b >> n) {
                imprimir_instrucciones(b, n);
            }
            else {
                cerr << "Error: Sintaxis invalida para Codigo. Uso: Codigo <b> <n>\n";
            }
        }
        else if (cmd == "Trace") {
            activar_trace();
        }
        else if (cmd == "Inicio") {
            reiniciar_simulador();
        }
        else if (cmd == "Fin") {
            cout << "Finalizando...\n";
            ejecutando = false;
        }
        else {
            cout << "Comando no reconocido. Teclee 'a' para ayuda.\n";
        }
    }
}

// Función principal
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo_de_instrucciones>\n";
        return 1;
    }

    // Cargar instrucciones desde el archivo
    if (!cargar_instrucciones(argv[1])) {
        return 1;
    }

    cout << "Instrucciones cargadas correctamente. (Ingrese 'a' para ver el menu de ayuda)\n";
    ejecutar_maquina_virtual();

    return 0;
}