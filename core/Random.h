#pragma once
// Generador de numeros aleatorios propio

#include <random>

class Random {
public:
    
    Random();

    
    int entero(int minimo, int maximo);

    template <typename T>
    void mezclarArreglo(T* arreglo, int n) {
        for (int i = n - 1; i > 0; --i) {
            int j = entero(0, i);
            T temp = arreglo[i];
            arreglo[i] = arreglo[j];
            arreglo[j] = temp;
        }
    }

private:
    std::mt19937 motor;
};
