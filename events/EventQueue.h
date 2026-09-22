#pragma once
// EventQueue.h
// Cola que se mantiene ordenada por el
// momento en que cada evento debe dispararse.
//
// Eventos 
//   1) AUMENTAR_VELOCIDAD: cada cierto tiempo la caida se hace mas
//      rapida. Facil de explicar y de verificar visualmente.
//   2) PIEZA_ESPECIAL_LIMPIADORA: en un momento dado, la siguiente
//      pieza que se coloque limpia automaticamente su fila.
//   3) REDUCIR_VISIBILIDAD_COLA: por un periodo, solo se muestra 1
//      pieza siguiente en vez de 3 (sube la dificultad sin tocar la
//      logica del tablero).

#include "../core/Types.h"

struct GameEvent {
    TipoEvento tipo;
    float momentoDisparo; 
};

struct NodoEvento {
    GameEvent dato;
    NodoEvento* siguiente;
};

class EventQueue {
public:
    EventQueue();
    ~EventQueue();

    void insertarOrdenado(const GameEvent& evento);

    bool vacia() const { return frente == nullptr; }

    GameEvent verFrente() const;

    GameEvent extraerFrente();

    bool hayEventoListo(float tiempoActualPartida) const;

    int cantidad() const { return contador; }

private:
    NodoEvento* frente;
    int contador;
};
