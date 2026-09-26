#include "Benchmark.h"
#include "SortAlgorithms.h"
#include "../core/Timer.h"
#include "../core/Random.h"
#include <fstream>
#include <cstring>

namespace {
	
	// Corre "operacion" las veces que haga falta hasta acumular al menos
	// umbralMs de tiempo TOTAL, y devuelve el tiempo promedio de una sola
	// ejecucion. 
	template <typename PrepararFn, typename OperacionFn>
	double medirPromedioMs(PrepararFn preparar, OperacionFn operacion, double umbralMs) {
		long repeticiones = 1;
		double totalMs = 0.0;
		while (true) {
			totalMs = 0.0;
			for (long i = 0; i < repeticiones; ++i) {
				preparar();
				totalMs += MedirTiempoMs(operacion);
			}
			if (totalMs >= umbralMs || repeticiones >= 1000000L) break;
			repeticiones *= 2;
		}
		return totalMs / static_cast<double>(repeticiones);
	}
	
} 

void Benchmark::ejecutarComparacion(const int* tamanos, int cantidadTamanos,
									const std::string& rutaSalidaCsv) {
	std::ofstream archivo(rutaSalidaCsv);
	archivo << "tamano,algoritmo,tiempo_ms\n";
	
	Random rng;
	const double UMBRAL_MS = 50.0; 
	
	for (int t = 0; t < cantidadTamanos; ++t) {
		int n = tamanos[t];
		
		// Se genera un arreglo base desordenado; cada algoritmo trabaja
		// sobre su propia copia, que se reconstruye antes de cada
		// repeticion (ver medirPromedioMs).
		int* base = new int[n];
		for (int i = 0; i < n; ++i) base[i] = rng.entero(0, 1000000);
		
		int* copiaInsertion = new int[n];
		int* copiaMerge = new int[n];
		
		double msInsertion = medirPromedioMs(
											 [&]() { std::memcpy(copiaInsertion, base, sizeof(int) * n); },
											 [&]() { insertionSort(copiaInsertion, n); },
											 UMBRAL_MS);
		
		double msMerge = medirPromedioMs(
										 [&]() { std::memcpy(copiaMerge, base, sizeof(int) * n); },
										 [&]() { mergeSort(copiaMerge, n); },
										 UMBRAL_MS);
		
		archivo << n << ",insertion," << msInsertion << "\n";
		archivo << n << ",merge," << msMerge << "\n";
		
		delete[] base;
		delete[] copiaInsertion;
		delete[] copiaMerge;
	}
	
	archivo.close();
}
