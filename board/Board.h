#pragma once
// Board.h
// El tablero se representa OBLIGATORIAMENTE como una lista enlazada de



#include "../core/Types.h"

struct NodoFila {
    int celdas[TABLERO_COLUMNAS]; // 0 = vacia, >0 = codigo de color/pieza
    NodoFila* siguiente;
};

class Board {
public:
    Board();
    ~Board();

    // Crea las 20 filas vacias encadenadas (fila superior = primeraFila).
    void construirTableroVacio();

    
    NodoFila* obtenerFila(int indice) const;

    int obtenerCelda(int fila, int columna) const;
    void ponerCelda(int fila, int columna, int valor);

    bool filaCompleta(NodoFila* fila) const;

    
    int limpiarFilasCompletas();


    bool celdasLibres(const struct Celda* celdas, int cantidad) const;

    bool detectarGameOver(const struct Celda* celdasSpawnPieza) const;

    void limpiarTablero(); 

private:
    NodoFila* primeraFila;
    int totalFilas;

    void eliminarFila(NodoFila* fila);        
    void insertarFilaVaciaAlInicio();          
};
