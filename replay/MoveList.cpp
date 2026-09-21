#include "MoveList.h"

MoveList::MoveList()
    : cabeza(nullptr), cola(nullptr), actual(nullptr),
      cursorReproduccion(nullptr), contador(0) {
}

MoveList::~MoveList() {
    NodoMovimiento* nodo = cabeza;
    while (nodo != nullptr) {
        NodoMovimiento* siguiente = nodo->siguiente;
        delete nodo;
        nodo = siguiente;
    }
}

void MoveList::truncarDespuesDe(NodoMovimiento* nodo) {
	
    NodoMovimiento* desde = (nodo == nullptr) ? cabeza : nodo->siguiente;
    NodoMovimiento* actualLibre = desde;
    while (actualLibre != nullptr) {
        NodoMovimiento* siguiente = actualLibre->siguiente;
        delete actualLibre;
        contador--;
        actualLibre = siguiente;
    }
    if (nodo == nullptr) {
        cabeza = nullptr;
        cola = nullptr;
    } else {
        nodo->siguiente = nullptr;
        cola = nodo;
    }
}

void MoveList::registrar(const AccionRecord& accion) {

    if (actual != cola) {
        truncarDespuesDe(actual);
    }

    NodoMovimiento* nuevo = new NodoMovimiento{accion, nullptr, nullptr};

    if (cabeza == nullptr) {
        cabeza = cola = nuevo;
    } else {
        nuevo->anterior = cola;
        cola->siguiente = nuevo;
        cola = nuevo;
    }
    actual = nuevo;
    contador++;
}

bool MoveList::deshacer(AccionRecord& accionDeshecha) {
    if (actual == nullptr) return false; // No habria nada que deshacer
    accionDeshecha = actual->dato;
    actual = actual->anterior;
    return true;
}

bool MoveList::rehacer(AccionRecord& accionRehecha) {
    NodoMovimiento* siguienteNodo = (actual == nullptr) ? cabeza : actual->siguiente;
    if (siguienteNodo == nullptr) return false; // No habria nada que rehacer
    accionRehecha = siguienteNodo->dato;
    actual = siguienteNodo;
    return true;
}

void MoveList::iniciarReproduccion() {
    cursorReproduccion = cabeza;
}

bool MoveList::avanzarReproduccion(AccionRecord& accion) {
    if (cursorReproduccion == nullptr) return false;
    accion = cursorReproduccion->dato;
    cursorReproduccion = cursorReproduccion->siguiente;
    return true;
}
