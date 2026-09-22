#pragma once
// Benchmark.h
// Genera datos de prueba aleatorios de distintos tamanos y mide el
// tiempo real de insertionSort vs mergeSort

#include <string>

class Benchmark {
public:
    
    static void ejecutarComparacion(const int* tamanos, int cantidadTamanos,
                                     const std::string& rutaSalidaCsv);
};
