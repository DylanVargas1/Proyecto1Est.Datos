#include "Benchmark.h"
#include "SortAlgorithms.h"
#include "../core/Timer.h"
#include "../core/Random.h"
#include <fstream>

void Benchmark::ejecutarComparacion(const int* tamanos, int cantidadTamanos,
                                     const std::string& rutaSalidaCsv) {
    std::ofstream archivo(rutaSalidaCsv);
    archivo << "tamano,algoritmo,tiempo_ms\n";

    Random rng;

    for (int t = 0; t < cantidadTamanos; ++t) {
        int n = tamanos[t];

        // Se genere un arreglo base y lo copia para cada algoritmo,
        
        int* base = new int[n];
        for (int i = 0; i < n; ++i) base[i] = rng.entero(0, 1000000);

        int* copiaInsertion = new int[n];
        int* copiaMerge = new int[n];
        std::memcpy(copiaInsertion, base, sizeof(int) * n);
        std::memcpy(copiaMerge, base, sizeof(int) * n);

        double msInsertion = MedirTiempoMs([&]() { insertionSort(copiaInsertion, n); });
        double msMerge = MedirTiempoMs([&]() { mergeSort(copiaMerge, n); });

        archivo << n << ",insertion," << msInsertion << "\n";
        archivo << n << ",merge," << msMerge << "\n";

        delete[] base;
        delete[] copiaInsertion;
        delete[] copiaMerge;
    }

    archivo.close();
}
