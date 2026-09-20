#include "Timer.h"

Timer::Timer() {
    reiniciar();
}

void Timer::reiniciar() {
    inicio = std::chrono::high_resolution_clock::now();
}

float Timer::segundosTranscurridos() const {
    auto ahora = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duracion = ahora - inicio;
    return duracion.count();
}
