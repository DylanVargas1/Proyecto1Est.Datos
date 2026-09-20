#include "PieceQueue.h"

PieceQueue::PieceQueue() : frente(nullptr), final(nullptr), contador(0) {
}

PieceQueue::~PieceQueue() {
    NodoPieza* actual = frente;
    while (actual != nullptr) {
        NodoPieza* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    frente = final = nullptr;
    contador = 0;
}

void PieceQueue::encolar(PieceType tipo) {
    NodoPieza* nuevo = new NodoPieza{tipo, nullptr};
    if (final == nullptr) {
        // cola vacia: el nuevo nodo es frente y final a la vez
        frente = final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }
    contador++;
}

PieceType PieceQueue::desencolar() {
    asegurarPiezasVisibles(1); // garantiza que haya al menos algo que extraer
    NodoPieza* viejo = frente;
    PieceType resultado = viejo->tipo;
    frente = frente->siguiente;
    if (frente == nullptr) {
        final = nullptr; // la cola quedo vacia
    }
    delete viejo;
    contador--;
    return resultado;
}

PieceType PieceQueue::consultar(int posicion) const {
    NodoPieza* actual = frente;
    for (int i = 0; i < posicion && actual != nullptr; ++i) {
        actual = actual->siguiente;
    }
    return (actual != nullptr) ? actual->tipo : PieceType::NINGUNA;
}

void PieceQueue::generarBolsa() {
    PieceType bolsa[NUM_TIPOS_PIEZA] = {
        PieceType::I, PieceType::O, PieceType::T, PieceType::S,
        PieceType::Z, PieceType::J, PieceType::L
    };
    generadorAleatorio.mezclarArreglo(bolsa, NUM_TIPOS_PIEZA);
    for (int i = 0; i < NUM_TIPOS_PIEZA; ++i) {
        encolar(bolsa[i]);
    }
}

void PieceQueue::asegurarPiezasVisibles(int minimoVisible) {
    while (contador < minimoVisible) {
        generarBolsa();
    }
}
