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




// Implementación de resolverSudokuMasterSlave usando memoria dinámica
void resolverSudokuMasterSlave(const std::vector<std::vector<int>>& initialBoard) {
    int size = initialBoard.size();
    int** board = initializeBoard(initialBoard);
    bool sudokuResuelto = false;
    int filasCompletadas = 0;

    std::cout << "Sudoku a resolver:" << std::endl;
    printBoard(board, size);

#pragma omp parallel num_threads(NUM_HILOS)
    {
        int threadID = omp_get_thread_num();

        if (threadID == 0) {
            // Hilo maestro
            while (filasCompletadas < size) {
#pragma omp flush(filasCompletadas)
                if (filasCompletadas == size) {
                    sudokuResuelto = true;
#pragma omp flush(sudokuResuelto)
                    break;
                }
            }
        }
        else {
            // Hilos esclavos
            while (!sudokuResuelto) {
                int fila = -1;

#pragma omp critical
                {
                    for (int i = 0; i < size; i++) {
                        if (board[i][0] == 0) { // Detectar filas no resueltas (asumiendo que hay un 0)
                            fila = i;
                            break;
                        }
                    }
                }

                if (fila != -1 && solveSudokup(board, size, fila, 0, false)) {
#pragma omp atomic
                    filasCompletadas++;
                }

#pragma omp flush(sudokuResuelto)
            }
        }
    }

    if (filasCompletadas == size) {
        std::cout << "Sudoku resuelto exitosamente." << std::endl;
        printBoard(board, size);
    }
    else {
        std::cout << "No se pudo resolver el Sudoku." << std::endl;
    }

    freeBoard(board, size);
}





// Función para verificar si es seguro colocar un número en una celda en Sudoku de cualquier tamaño (paralelizada)
bool isSafep(int** board, int size, int row, int col, int num) {
    for (int x = 0; x < size; x++) {
        if (board[row][x] == num || board[x][col] == num)
            return false;
    }
    int subSize = static_cast<int>(std::sqrt(size));
    int startRow = row - row % subSize;
    int startCol = col - col % subSize;
    for (int i = 0; i < subSize; i++) {
        for (int j = 0; j < subSize; j++) {
            if (board[startRow + i][startCol + j] == num)
                return false;
        }
    }
    return true;
}

bool solveSudokup(int** board, int size, int row, int col, bool paralelismo) {
    if (row == size) return true;
    if (col == size) return solveSudokup(board, size, row + 1, 0, paralelismo);
    if (board[row][col] != 0) return solveSudokup(board, size, row, col + 1, paralelismo);

    bool solucionEncontrada = false;

    if (paralelismo) {
#pragma omp parallel for shared(solucionEncontrada) num_threads(NUM_HILOS)
        for (int num = 1; num <= size; num++) {
            if (!solucionEncontrada && isSafep(board, size, row, col, num)) {
                board[row][col] = num;

#pragma omp critical
                solucionEncontrada = true;
                solucionEncontrada = solveSudokup(board, size, row, col + 1, false);

                board[row][col] = 0;
            }
        }
    }
    else {
        for (int num = 1; num <= size; num++) {
            if (!solucionEncontrada && isSafep(board, size, row, col, num)) {
                board[row][col] = num;

                solucionEncontrada = solveSudokup(board, size, row, col + 1, false);

                board[row][col] = 0;
            }
        }
    }

    return solucionEncontrada;
}
