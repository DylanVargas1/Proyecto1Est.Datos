#pragma once
// Tetromino.h
// Representa una pieza con sus 4 orientaciones PRECALCULADAS.
//

#include "../core/Types.h"

struct Celda { int fila; int columna; };

class Tetromino {
public:
    Tetromino(PieceType tipo);

    PieceType tipo() const { return tipoPieza; }

    int filaBase, columnaBase;
    int orientacion; // 0..3

    void celdasAbsolutas(Celda salida[4]) const;

    void celdasParaOrientacion(int orientacionCandidata, Celda salida[4]) const;

    void rotarSiguiente() { orientacion = (orientacion + 1) % 4; }

private:
    PieceType tipoPieza;

    static const int TABLA_FORMAS[7][4][4][2];
};
