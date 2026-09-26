// main.cpp
// Punto de entrada del programa.
// 2 Modos
// Modo normal
// Modo benchmark
#include "engine/GameEngine.h"
#include "render/Renderer.h"
#include "core/Timer.h"
#include "sorting/Benchmark.h"
#include <cstring>
#include <iostream>

static int ejecutarModoBenchmark(const std::string& rutaSalida) {
	const int tamanos[] = {10, 100, 1000, 10000};
	const int cantidadTamanos = sizeof(tamanos) / sizeof(tamanos[0]);
	
	std::cout << "Ejecutando benchmark (insertion sort vs merge sort)...\n";
	Benchmark::ejecutarComparacion(tamanos, cantidadTamanos, rutaSalida);
	std::cout << "Listo. Resultados guardados en: " << rutaSalida << "\n";
	std::cout << "Abri ese archivo (Excel/Sheets) para armar la tabla y el grafico del informe.\n";
	return 0;
}

int main(int argc, char* argv[]) {
	// Modo benchmark: solo consola, no toca el juego ni SFML 
	for (int i = 1; i < argc; ++i) {
		if (std::strcmp(argv[i], "--benchmark") == 0 || std::strcmp(argv[i], "-b") == 0) {
			std::string rutaSalida = "benchmark_resultados.csv";
			if (i + 1 < argc) {
				rutaSalida = argv[i + 1]; 
			}
			return ejecutarModoBenchmark(rutaSalida);
		}
	}
	
	// --- Modo normal(el juego)
	GameEngine motor;
	Renderer render;
	Timer relojDeFrame;
	
	while (render.ventanaAbierta()) {
		float delta = relojDeFrame.segundosTranscurridos();
		relojDeFrame.reiniciar();
		
		render.procesarEventosDeVentana(motor);
		motor.actualizar(delta);
		render.dibujarFrame(motor);
	}
	
	return 0;
}
