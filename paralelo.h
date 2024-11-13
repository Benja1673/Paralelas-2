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

// Prototipos de funciones paralelas

void resolverSudokuMasterSlave(const std::vector<std::vector<int>>& initialBoard);

bool isSafep(int** board, int size, int row, int col, int num);
bool solveSudokup(int** board, int size, int row, int col, bool paralelismo);

#endif // PARALELO_H#pragma once
