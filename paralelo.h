#ifndef PARALELO_H
#define PARALELO_H

#include <vector>
#include <iostream>
#include <cmath>
#include <omp.h>
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





// Función para verificar si es seguro colocar un número en una casilla específica
bool isSafeInRow(int** board, int row, int col, int num, int dimension);

// Función para resolver una fila del Sudoku utilizando backtracking
bool resolverFilap(int** board, int fila, int dimension);

// Función para asignar hilos para resolver filas del Sudoku
void asignarHilos(int** board, int dimension, std::vector<bool>& filasResueltas, std::vector<bool>& filasNoResueltas);

// Función para controlar los hilos y manejar filas no resueltas
void controlarHilos(int** board, int dimension);

// Función principal para resolver el Sudoku utilizando hilos
void resolverSudokuConHilos(std::vector<std::vector<int>>& initialBoard);



#endif // PARALELO_H#pragma once
