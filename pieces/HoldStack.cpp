#include "HoldStack.h"

HoldStack::HoldStack() : contenido(PieceType::NINGUNA), vacia(true) {
}

void HoldStack::push(PieceType tipo) {
    contenido = tipo;
    vacia = false;
}

PieceType HoldStack::pop() {
    if (vacia) return PieceType::NINGUNA;
    PieceType resultado = contenido;
    contenido = PieceType::NINGUNA;
    vacia = true;
    return resultado;
}
