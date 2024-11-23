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





#include <vector>



// Función para verificar si es seguro colocar un número en una casilla específica
bool isSafeLocal(int** board, int row, int col, int num, int dimension);

// Resolver una fila utilizando backtracking con propagación de restricciones
bool resolverFilapLocal(int** localBoard, int fila, int dimension, std::vector<std::vector<std::vector<bool>>>& restricciones);

// Manejar hilos con prioridad y propagación de restricciones
void manejarHilosConPrioridad(int** board, int dimension, bool* filasResueltas);

// Controlar la lógica de resolución
void controlarHilos(int** board, int dimension);

// Resolver el Sudoku con hilos
void resolverSudokuConHilos(std::vector<std::vector<int>>& initialBoard);




void controlarHilos(int** board, int dimension);  // Declaración de la función
void manejarHilosConValidacion(int** board, int dimension, bool* filasResueltas);
bool validarFila(int** board, int fila, int dimension);
// Función para verificar si es seguro colocar un número en una casilla específica
bool isSafeInRow(int** board, int row, int col, int num, int dimension);

// Función para resolver una fila utilizando backtracking
bool resolverFilap(int** board, int fila, int dimension);

// Función para manejar hilos esclavos con copias locales
void manejarHilosConCopias(int** board, int dimension, bool* filasResueltas);

// Función para controlar la lógica general y resolver el Sudoku
void controlarHilos(int** board, int dimension);

// Función principal para resolver el Sudoku con hilos
void resolverSudokuConHilos(std::vector<std::vector<int>>& initialBoard);



int contarPosibilidades(int** board, int row, int col, int dimension);
bool resolverFilapLocal(int** localBoard, int fila, int dimension);
bool isSafep(int** board, int size, int row, int col, int num);
bool solveSudokup(int** board, int size, int row, int col);
bool solveSudokuParallel(int** board, int size);
void resolverSudokup(const std::vector<std::vector<int>>& initialBoard);
void manejarEsclavos(int** board, int dimension, std::vector<bool>& filasResueltas, std::vector<bool>& filasEnProceso);
void asignarHilosBloque(int** board, int dimension, std::vector<bool>& filasResueltas, std::vector<int>& filasPendientes);
bool resolverFilaInversa(int** board, int fila, int dimension);
bool verificarFila(int** board, int fila, int dimension);
bool verificarTablero(int** board, int dimension, std::vector<bool>& filasResueltas);
// Inicializa las prioridades dinámicas de las filas basado en casillas vacías
void inicializarPrioridades(int** board, int dimension);
bool isSafeWithBlocks(int** board, int row, int col, int num, int dimension, bool** blockState);
// Asigna dinámicamente una fila para procesar según prioridad
int asignarFilaPorPrioridad();
void updateBlockState(bool** blockState, int row, int col, int num, int dimension, bool add);
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
