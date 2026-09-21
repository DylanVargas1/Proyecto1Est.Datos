#pragma once
// MoveList.h
// Cada nodo guarda la ACCION realizada


#include "../core/Types.h"

struct AccionRecord {
    TipoAccion tipo;
    PieceType piezaActual;

    // Datos suficientes para reconstruir el "antes" y "despues" sin
    // guardar el tablero completo:
    int orientacionAntes, orientacionDespues;
    int filaAntes, filaDespues;
    int columnaAntes, columnaDespues;

    int filasLimpiadasAlColocar;
};

struct NodoMovimiento {
    AccionRecord dato;
    NodoMovimiento* anterior;
    NodoMovimiento* siguiente;
};

class MoveList {
public:
    MoveList();
    ~MoveList();

    void registrar(const AccionRecord& accion);

    // Retrocede un paso: mueve 'actual' a 'actual->anterior'.
    bool deshacer(AccionRecord& accionDeshecha);

    // Avanza un paso: mueve 'actual' a 'actual->siguiente'.
    bool rehacer(AccionRecord& accionRehecha);

    bool hayDeshacerDisponible() const { return actual != nullptr; }
    bool hayRehacerDisponible() const {
        return (actual == nullptr) ? (cabeza != nullptr) : (actual->siguiente != nullptr);
    }

    void iniciarReproduccion();

    
    bool avanzarReproduccion(AccionRecord& accion);

    int totalMovimientos() const { return contador; }
    bool vacia() const { return cabeza == nullptr; }

private:
    NodoMovimiento* cabeza;
    NodoMovimiento* cola;
    NodoMovimiento* actual;              
    NodoMovimiento* cursorReproduccion;  
    int contador;

    void truncarDespuesDe(NodoMovimiento* nodo); 
};
