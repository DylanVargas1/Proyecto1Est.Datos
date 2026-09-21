#pragma once
// Pila propia (LIFO) de capacidad maxima 1, para la pieza "en espera".

#include "../core/Types.h"

class HoldStack {
public:
    HoldStack();

    bool estaVacia() const { return vacia; }

    void push(PieceType tipo);

    // Extrae el elemento (deja la pila vacia) y lo devuelve.
    PieceType pop();

    PieceType peek() const { return contenido; }

private:
    PieceType contenido;
    bool vacia;
};
