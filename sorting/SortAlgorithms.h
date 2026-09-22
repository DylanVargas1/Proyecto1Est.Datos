#pragma once
// SortAlgorithms.h
// Dos algoritmos de ordenamiento propios

#include <cstring>

template <typename T>
void insertionSort(T* arreglo, int n) {
    for (int i = 1; i < n; ++i) {
        T actual = arreglo[i];
        int j = i - 1;
        
        while (j >= 0 && arreglo[j] < actual) { // orden DESCENDENTE (mayor puntaje primero)
            arreglo[j + 1] = arreglo[j];
            j--;
        }
        arreglo[j + 1] = actual;
    }
}

template <typename T>
void mezclar(T* arreglo, T* temporal, int inicio, int medio, int fin) {
    int i = inicio, j = medio + 1, k = inicio;
    while (i <= medio && j <= fin) {
        // orden DESCENDENTE: el mayor va primero
        if (arreglo[i] >= arreglo[j]) temporal[k++] = arreglo[i++];
        else temporal[k++] = arreglo[j++];
    }
    while (i <= medio) temporal[k++] = arreglo[i++];
    while (j <= fin) temporal[k++] = arreglo[j++];
    for (int x = inicio; x <= fin; ++x) arreglo[x] = temporal[x];
}

template <typename T>
void mergeSortRec(T* arreglo, T* temporal, int inicio, int fin) {
    if (inicio >= fin) return; 
    int medio = inicio + (fin - inicio) / 2;
    mergeSortRec(arreglo, temporal, inicio, medio);
    mergeSortRec(arreglo, temporal, medio + 1, fin);
    mezclar(arreglo, temporal, inicio, medio, fin);
}

template <typename T>
void mergeSort(T* arreglo, int n) {
    if (n <= 1) return;
    T* temporal = new T[n];
    mergeSortRec(arreglo, temporal, 0, n - 1);
    delete[] temporal;
}

enum class TipoOrdenamiento { INSERTION, MERGE };
