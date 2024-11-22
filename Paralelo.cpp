#include <iostream>
#include <vector>
#include <cmath>  // Para sqrt, necesario para subcuadros de mayor tamaño
#include <chrono> // Para medir el tiempo
#include <thread>
#include <mutex>
#include <queue>
#include <future>
#include <omp.h>
#include <random>
#include <iomanip> // Para formatear la salida
#include "sudokus.h"
#include "paralelo.h"
const int NUM_HILOS = 8;
// Función para verificar si es seguro colocar un número en una casilla específica
// Función para verificar si es seguro colocar un número en una casilla específica
// Función para verificar si es seguro colocar un número en una casilla específica en una copia local del tablero
// Función para verificar si es seguro colocar un número en una casilla específica en una copia local del tablero
// Función para verificar si es seguro colocar un número en una casilla específica en una copia local del tablero
// Verificar si es seguro colocar un número en una casilla en una copia local del tablero
// Estructura para almacenar casillas con múltiples opciones
// Estructura para almacenar casillas con múltiples opciones
struct CasillaPendiente {
    int fila;
    int columna;
    std::vector<int> opciones;  // Opciones posibles para la casilla
};

// Función para verificar si es seguro colocar un número en una casilla específica
bool isSafeLocal(int** board, int row, int col, int num, int dimension) {
    for (int x = 0; x < dimension; x++) {
        if (board[row][x] == num || board[x][col] == num) return false;
    }
    int subSize = std::sqrt(dimension);
    int startRow = row - row % subSize;
    int startCol = col - col % subSize;
    for (int i = 0; i < subSize; i++) {
        for (int j = 0; j < subSize; j++) {
            if (board[startRow + i][startCol + j] == num) return false;
        }
    }
    return true;
}

// Esta función gestiona las casillas pendientes mientras otros hilos resuelven el Sudoku
void gestionarCasillasPendientes(std::vector<CasillaPendiente>& pendientes, int** board, int dimension, bool& progreso) {
    while (true) {
        bool cambios = false;
        for (auto& casilla : pendientes) {
            std::vector<int> nuevasOpciones;
            for (int num : casilla.opciones) {
                if (isSafeLocal(board, casilla.fila, casilla.columna, num, dimension)) {
                    nuevasOpciones.push_back(num);
                }
            }

            if (nuevasOpciones.size() == 1) {
                // Actualiza la casilla con la única opción disponible
                board[casilla.fila][casilla.columna] = nuevasOpciones[0];
                progreso = true;
                cambios = true; // Hubo un cambio en el tablero
            }
            else {
                casilla.opciones = nuevasOpciones;
            }
        }

        // Si no hay cambios, terminamos la función
        if (!cambios) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Pequeña espera para no consumir demasiados recursos
    }
}

// Resolver una fila utilizando backtracking con propagación de restricciones
bool resolverFilapLocal(int** localBoard, int fila, int dimension, std::vector<std::vector<std::vector<bool>>>& restricciones) {
    for (int col = 0; col < dimension; col++) {
        if (localBoard[fila][col] == 0) {
            for (int num = 1; num <= dimension; num++) {
                if (isSafeLocal(localBoard, fila, col, num, dimension) && !restricciones[fila][col][num - 1]) {
                    localBoard[fila][col] = num;

                    // Actualizar restricciones
                    restricciones[fila][col][num - 1] = true;

                    if (resolverFilapLocal(localBoard, fila, dimension, restricciones)) return true;

                    // Revertir restricciones y retroceder
                    restricciones[fila][col][num - 1] = false;
                    localBoard[fila][col] = 0;
                }
            }
            return false;
        }
    }
    return true;
}

// Función principal para manejar los hilos
void manejarHilosConPrioridad(int** board, int dimension, bool* filasResueltas) {
    // Inicializar restricciones
    std::vector<std::vector<std::vector<bool>>> restricciones(
        dimension, std::vector<std::vector<bool>>(dimension, std::vector<bool>(dimension, false)));

    // Lista de casillas pendientes (con múltiples opciones)
    std::vector<CasillaPendiente> pendientes;

    // Copias locales por hilo
    int*** localBoards = new int** [NUM_HILOS];
    for (int i = 0; i < NUM_HILOS; i++) {
        localBoards[i] = new int* [dimension];
        for (int j = 0; j < dimension; j++) {
            localBoards[i][j] = new int[dimension];
            std::copy(board[j], board[j] + dimension, localBoards[i][j]);
        }
    }

    // Crear una variable de progreso
    bool progreso = false;

    // Crear el hilo para gestionar las casillas pendientes
    std::thread hiloPendientes(gestionarCasillasPendientes, std::ref(pendientes), board, dimension, std::ref(progreso));

    // Resolver casillas con hilos
#pragma omp parallel num_threads(NUM_HILOS)
    {
        int threadID = omp_get_thread_num();
        while (true) {
            // Resolviendo filas con hilos
            bool filaProcesada = false;
            for (int fila = 0; fila < dimension; fila++) {
                if (!filasResueltas[fila]) {
                    if (resolverFilapLocal(localBoards[threadID], fila, dimension, restricciones)) {
                        filasResueltas[fila] = true;
                        filaProcesada = true;
                    }
                }
            }
            if (!filaProcesada) break; // Si ya no hay más filas pendientes, se detiene
        }
    }

    // Esperar que el hilo de casillas pendientes termine su trabajo
    hiloPendientes.join();

    // Actualizar el tablero con los resultados de los tableros locales
#pragma omp parallel for schedule(dynamic)
    for (int fila = 0; fila < dimension; fila++) {
        if (filasResueltas[fila]) {
            for (int i = 0; i < NUM_HILOS; i++) {
                for (int col = 0; col < dimension; col++) {
                    if (localBoards[i][fila][col] != 0) {
                        board[fila][col] = localBoards[i][fila][col];
                    }
                }
            }
        }
    }

    // Liberar la memoria de las copias locales
    for (int i = 0; i < NUM_HILOS; i++) {
        for (int j = 0; j < dimension; j++) {
            delete[] localBoards[i][j];
        }
        delete[] localBoards[i];
    }
    delete[] localBoards;
}

// Controlar la lógica de resolución
void controlarHilos(int** board, int dimension) {
    bool* filasResueltas = new bool[dimension]();

    bool progreso = true;
    while (progreso) {
        progreso = false;

        manejarHilosConPrioridad(board, dimension, filasResueltas);

        int filasPendientes = 0;
        for (int i = 0; i < dimension; i++) {
            if (!filasResueltas[i]) filasPendientes++;
        }

        if (filasPendientes == 0) {
            std::cout << "Todas las filas han sido resueltas." << std::endl;
            printBoard(board, dimension);
            progreso = false;
        }
        else {
            std::cout << "Filas pendientes de resolver: " << filasPendientes << std::endl;
            progreso = true;
        }
    }

    delete[] filasResueltas;
}

// Resolver el Sudoku con hilos
void resolverSudokuConHilos(std::vector<std::vector<int>>& initialBoard) {
    int dimension = initialBoard.size();
    int** board = initializeBoard(initialBoard);

    std::cout << "Sudoku a resolver:" << std::endl;
    printBoard(board, dimension);

    auto start = std::chrono::high_resolution_clock::now();
    controlarHilos(board, dimension);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Tiempo total: " << duration_ms << " ms." << std::endl;

    freeBoard(board, dimension);
}