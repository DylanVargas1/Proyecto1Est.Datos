#pragma once

//aqui viven las piezas "futuras". Se generan de a
// bolsas de 7 (las 7 piezas clasicas en orden aleatorio); cuando quedan
// pocas piezas en la cola se genera y se encola una bolsa nueva, para
// que siempre se puedan mostrar las proximas 3 en pantalla.

#include "../core/Types.h"
#include "../core/Random.h"

struct NodoPieza {
    PieceType tipo;
    NodoPieza* siguiente;
};

class PieceQueue {
public:
    PieceQueue();
    ~PieceQueue(); 

    // Inserta un elemento al final de la cola. 
    void encolar(PieceType tipo);

    
    PieceType desencolar();

    
    PieceType consultar(int posicion) const;

    bool vacia() const { return frente == nullptr; }
    int cantidad() const { return contador; }

    // Genera una bolsa de las 7 piezas en orden aleatorio y las encola
    // una por una.
    void generarBolsa();

    // Garantiza que siempre haya al menos `minimoVisible` piezas en la
    // cola (por defecto 3, para poder mostrarlas en la interfaz).
    void asegurarPiezasVisibles(int minimoVisible = 3);

private:
    NodoPieza* frente;
    NodoPieza* final;
    int contador;
    Random generadorAleatorio;
};
