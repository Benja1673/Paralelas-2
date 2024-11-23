#ifndef PARALELO_H
#define PARALELO_H

#include <vector>
#include <iostream>
#include <cmath>
#include <omp.h>
#include <iostream>
#include <vector>
#include <cmath>  // Para sqrt, necesario para subcuadros de mayor tama�o
#include <chrono> // Para medir el tiempo
#include <thread>
#include <mutex>
#include <queue>
#include <future>
#include <omp.h>
#include <random>
#include <iomanip> // Para formatear la salida





#include <vector>



// Funci�n para verificar si es seguro colocar un n�mero en una casilla espec�fica
bool isSafeLocal(int** board, int row, int col, int num, int dimension);

// Resolver una fila utilizando backtracking con propagaci�n de restricciones
bool resolverFilapLocal(int** localBoard, int fila, int dimension, std::vector<std::vector<std::vector<bool>>>& restricciones);

// Manejar hilos con prioridad y propagaci�n de restricciones
void manejarHilosConPrioridad(int** board, int dimension, bool* filasResueltas);

// Controlar la l�gica de resoluci�n
void controlarHilos(int** board, int dimension);

// Resolver el Sudoku con hilos
void resolverSudokuConHilos(std::vector<std::vector<int>>& initialBoard);




void controlarHilos(int** board, int dimension);  // Declaraci�n de la funci�n
void manejarHilosConValidacion(int** board, int dimension, bool* filasResueltas);
bool validarFila(int** board, int fila, int dimension);
// Funci�n para verificar si es seguro colocar un n�mero en una casilla espec�fica
bool isSafeInRow(int** board, int row, int col, int num, int dimension);

// Funci�n para resolver una fila utilizando backtracking
bool resolverFilap(int** board, int fila, int dimension);

// Funci�n para manejar hilos esclavos con copias locales
void manejarHilosConCopias(int** board, int dimension, bool* filasResueltas);

// Funci�n para controlar la l�gica general y resolver el Sudoku
void controlarHilos(int** board, int dimension);

// Funci�n principal para resolver el Sudoku con hilos
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
// Inicializa las prioridades din�micas de las filas basado en casillas vac�as
void inicializarPrioridades(int** board, int dimension);
bool isSafeWithBlocks(int** board, int row, int col, int num, int dimension, bool** blockState);
// Asigna din�micamente una fila para procesar seg�n prioridad
int asignarFilaPorPrioridad();
void updateBlockState(bool** blockState, int row, int col, int num, int dimension, bool add);
// Funci�n para verificar si es seguro colocar un n�mero en una casilla espec�fica
bool isSafeInRow(int** board, int row, int col, int num, int dimension);

// Funci�n para resolver una fila del Sudoku utilizando backtracking
bool resolverFilap(int** board, int fila, int dimension);

// Funci�n para asignar hilos para resolver filas del Sudoku
void asignarHilos(int** board, int dimension, std::vector<bool>& filasResueltas, std::vector<bool>& filasNoResueltas);

// Funci�n para controlar los hilos y manejar filas no resueltas
void controlarHilos(int** board, int dimension);

// Funci�n principal para resolver el Sudoku utilizando hilos
void resolverSudokuConHilos(std::vector<std::vector<int>>& initialBoard);



#endif // PARALELO_H#pragma once
