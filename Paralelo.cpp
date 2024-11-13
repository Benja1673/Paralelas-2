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
bool isSafeInRow(int** board, int row, int col, int num, int dimension) {
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

// Función para resolver una fila utilizando backtracking
bool resolverFilap(int** board, int fila, int dimension) {
    for (int col = 0; col < dimension; col++) {
        if (board[fila][col] == 0) {
            for (int num = 1; num <= dimension; num++) {
                if (isSafeInRow(board, fila, col, num, dimension)) {
                    board[fila][col] = num;
                    if (resolverFilap(board, fila, dimension)) return true;
                    board[fila][col] = 0;
                }
            }
            return false;
        }
    }
    return true;
}

// Función para asignar hilos a las filas
void asignarHilos(int** board, int dimension, std::vector<bool>& filasResueltas, std::vector<bool>& filasNoResueltas) {
    int maxHilos = 7;

#pragma omp parallel for num_threads(maxHilos)
    for (int fila = 0; fila < dimension; fila++) {
        if (!filasResueltas[fila]) {
            if (resolverFilap(board, fila, dimension)) {
#pragma omp critical
                {
                    filasResueltas[fila] = true;
                    std::cout << "Hilo " << omp_get_thread_num() << " resolvio la fila " << fila << std::endl;
                }
            }
            else {
#pragma omp critical
                {
                    filasNoResueltas[fila] = true;
                    std::cout << "Hilo " << omp_get_thread_num() << " no pudo resolver la fila " << fila << std::endl;
                }
            }
        }
    }
}

// Función para controlar el estado de los hilos y verificar filas no resueltas
void controlarHilos(int** board, int dimension) {
    std::vector<bool> filasResueltas(dimension, false);
    std::vector<bool> filasNoResueltas(dimension, false);

    bool progreso = true;
    while (progreso) {
        progreso = false;
        asignarHilos(board, dimension, filasResueltas, filasNoResueltas);

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
            std::fill(filasNoResueltas.begin(), filasNoResueltas.end(), false);
            progreso = true;
        }
    }
}

// Función principal para resolver el Sudoku utilizando hilos
void resolverSudokuConHilos(std::vector<std::vector<int>>& initialBoard) {
    int dimension = initialBoard.size();
    int** board = initializeBoard(initialBoard);

    std::cout << "Sudoku a resolver:" << std::endl;
    printBoard(board, dimension);

    controlarHilos(board, dimension);

    freeBoard(board, dimension);
}