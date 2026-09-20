#include "Tetromino.h"



const int Tetromino::TABLA_FORMAS[7][4][4][2] = {
    // I
    {
        {{1,0},{1,1},{1,2},{1,3}},
        {{0,2},{1,2},{2,2},{3,2}},
        {{2,0},{2,1},{2,2},{2,3}},
        {{0,1},{1,1},{2,1},{3,1}}
    },
    // (las 4 orientaciones son identicas: no tiene sentido rotar un cuadrado)
    {
        {{0,1},{0,2},{1,1},{1,2}},
        {{0,1},{0,2},{1,1},{1,2}},
        {{0,1},{0,2},{1,1},{1,2}},
        {{0,1},{0,2},{1,1},{1,2}}
    },
    // T
    {
        {{0,1},{1,0},{1,1},{1,2}},
        {{0,1},{1,1},{1,2},{2,1}},
        {{1,0},{1,1},{1,2},{2,1}},
        {{0,1},{1,0},{1,1},{2,1}}
    },
    // S
    {
        {{0,1},{0,2},{1,0},{1,1}},
        {{0,1},{1,1},{1,2},{2,2}},
        {{0,1},{0,2},{1,0},{1,1}},
        {{0,1},{1,1},{1,2},{2,2}}
    },
    // Z
    {
        {{0,0},{0,1},{1,1},{1,2}},
        {{0,2},{1,1},{1,2},{2,1}},
        {{0,0},{0,1},{1,1},{1,2}},
        {{0,2},{1,1},{1,2},{2,1}}
    },
    // J
    {
        {{0,0},{1,0},{1,1},{1,2}},
        {{0,1},{0,2},{1,1},{2,1}},
        {{1,0},{1,1},{1,2},{2,2}},
        {{0,1},{1,1},{2,0},{2,1}}
    },
    // L
    {
        {{0,2},{1,0},{1,1},{1,2}},
        {{0,1},{1,1},{2,1},{2,2}},
        {{1,0},{1,1},{1,2},{2,0}},
        {{0,0},{0,1},{1,1},{2,1}}
    }
};

static int indiceDeTipo(PieceType t) {
    switch (t) {
        case PieceType::I: return 0;
        case PieceType::O: return 1;
        case PieceType::T: return 2;
        case PieceType::S: return 3;
        case PieceType::Z: return 4;
        case PieceType::J: return 5;
        case PieceType::L: return 6;
        default: return 0;
    }
}

Tetromino::Tetromino(PieceType tipo)
    : filaBase(0), columnaBase(0), orientacion(0), tipoPieza(tipo) {
}

void Tetromino::celdasParaOrientacion(int orientacionCandidata, Celda salida[4]) const {
    int idx = indiceDeTipo(tipoPieza);
    for (int i = 0; i < 4; ++i) {
        int df = TABLA_FORMAS[idx][orientacionCandidata][i][0];
        int dc = TABLA_FORMAS[idx][orientacionCandidata][i][1];
        salida[i].fila = filaBase + df;
        salida[i].columna = columnaBase + dc;
    }
}

void Tetromino::celdasAbsolutas(Celda salida[4]) const {
    celdasParaOrientacion(orientacion, salida);
}
