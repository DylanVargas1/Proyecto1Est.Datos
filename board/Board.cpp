#include "Board.h"
#include "../pieces/Tetromino.h" 

Board::Board() : primeraFila(nullptr), totalFilas(0) {
    construirTableroVacio();
}

Board::~Board() {
    limpiarTablero(); // libera todos los nodos
}

void Board::construirTableroVacio() {
    limpiarTablero();
    // Se construye de abajo hacia arriba, insertando siempre al frente,
    // para que la fila 0 logica quede como primeraFila.
    for (int i = 0; i < TABLERO_FILAS; ++i) {
        insertarFilaVaciaAlInicio();
    }
}

void Board::limpiarTablero() {
    NodoFila* actual = primeraFila;
    while (actual != nullptr) {
        NodoFila* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    primeraFila = nullptr;
    totalFilas = 0;
}

NodoFila* Board::obtenerFila(int indice) const {
    NodoFila* actual = primeraFila;
    for (int i = 0; i < indice && actual != nullptr; ++i) {
        actual = actual->siguiente;
    }
    return actual;
}

int Board::obtenerCelda(int fila, int columna) const {
    NodoFila* nodo = obtenerFila(fila);
    if (nodo == nullptr || columna < 0 || columna >= TABLERO_COLUMNAS) return -1;
    return nodo->celdas[columna];
}

void Board::ponerCelda(int fila, int columna, int valor) {
    NodoFila* nodo = obtenerFila(fila);
    if (nodo != nullptr && columna >= 0 && columna < TABLERO_COLUMNAS) {
        nodo->celdas[columna] = valor;
    }
}

bool Board::filaCompleta(NodoFila* fila) const {
    if (fila == nullptr) return false;
    for (int c = 0; c < TABLERO_COLUMNAS; ++c) {
        if (fila->celdas[c] == 0) return false;
    }
    return true;
}

void Board::insertarFilaVaciaAlInicio() {
    NodoFila* nuevo = new NodoFila();
    for (int c = 0; c < TABLERO_COLUMNAS; ++c) nuevo->celdas[c] = 0;
    nuevo->siguiente = primeraFila;
    primeraFila = nuevo;
    totalFilas++;
}

void Board::eliminarFila(NodoFila* fila) {
    if (fila == primeraFila) {
        primeraFila = fila->siguiente;
        delete fila;
        totalFilas--;
        return;
    }

    NodoFila* anterior = primeraFila;
    while (anterior != nullptr && anterior->siguiente != fila) {
        anterior = anterior->siguiente;
    }
    if (anterior != nullptr) {
        anterior->siguiente = fila->siguiente;
        delete fila;
        totalFilas--;
    }
}

int Board::limpiarFilasCompletas() {
    int filasLimpiadas = 0;


    NodoFila* actual = primeraFila;
    while (actual != nullptr) {
        NodoFila* siguienteOriginal = actual->siguiente;
        if (filaCompleta(actual)) {
            eliminarFila(actual);
            filasLimpiadas++;
        }
        actual = siguienteOriginal;
    }

    for (int i = 0; i < filasLimpiadas; ++i) {
        insertarFilaVaciaAlInicio();
    }

    return filasLimpiadas;
}

bool Board::celdasLibres(const Celda* celdas, int cantidad) const {
    for (int i = 0; i < cantidad; ++i) {
        int f = celdas[i].fila;
        int c = celdas[i].columna;
        if (f < 0 || f >= TABLERO_FILAS) return false;
        if (c < 0 || c >= TABLERO_COLUMNAS) return false;
        if (obtenerCelda(f, c) != 0) return false;
    }
    return true;
}

bool Board::detectarGameOver(const Celda* celdasSpawnPieza) const {
   
    return !celdasLibres(celdasSpawnPieza, 4);
}
