#include "Random.h"
#include <chrono>

Random::Random() {
    unsigned semilla = static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
    motor.seed(semilla);
}

int Random::entero(int minimo, int maximo) {
    std::uniform_int_distribution<int> distribucion(minimo, maximo);
    return distribucion(motor);
}
