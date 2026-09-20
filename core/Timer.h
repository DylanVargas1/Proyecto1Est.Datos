#pragma once

#include <chrono>

class Timer {
public:
    Timer();

    // Reinicia el cronometro a 0.
    void reiniciar();

    // Segundos transcurridos desde la ultima llamada a reiniciar().
    float segundosTranscurridos() const;

private:
    std::chrono::high_resolution_clock::time_point inicio;
};


template <typename Funcion>
double MedirTiempoMs(Funcion f) {
    auto t0 = std::chrono::high_resolution_clock::now();
    f();
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracion = t1 - t0;
    return duracion.count();
}
