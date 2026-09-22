#include "ScoreBoard.h"
#include <fstream>
#include <sstream>
#include <cstring>

ScoreBoard::ScoreBoard(const std::string& rutaArchivo) : contador(0), ruta(rutaArchivo) {
    cargarDesdeArchivo();
}

void ScoreBoard::cargarDesdeArchivo() {
    contador = 0;
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return;

    std::string linea;
    while (std::getline(archivo, linea) && contador < MAX_PUNTAJES_GUARDADOS) {
        std::stringstream ss(linea);
        std::string nombre, puntajeStr;
        if (std::getline(ss, nombre, ',') && std::getline(ss, puntajeStr)) {
            ScoreEntry entrada;
            std::strncpy(entrada.nombre, nombre.c_str(), sizeof(entrada.nombre) - 1);
            entrada.nombre[sizeof(entrada.nombre) - 1] = '\0';
            entrada.puntaje = std::stoi(puntajeStr);
            puntajes[contador++] = entrada;
        }
    }
}

void ScoreBoard::guardarEnArchivo() const {
    std::ofstream archivo(ruta);
    for (int i = 0; i < contador; ++i) {
        archivo << puntajes[i].nombre << "," << puntajes[i].puntaje << "\n";
    }
}

int ScoreBoard::puntosPorLineas(int lineasLimpiadas) {
    static const int TABLA[5] = {0, 100, 300, 500, 800};
    if (lineasLimpiadas < 0) return 0;
    if (lineasLimpiadas > 4) lineasLimpiadas = 4;
    return TABLA[lineasLimpiadas];
}

void ScoreBoard::ordenarCon(TipoOrdenamiento algoritmo) {
    if (algoritmo == TipoOrdenamiento::INSERTION) {
        insertionSort(puntajes, contador);
    } else {
        mergeSort(puntajes, contador);
    }
}

bool ScoreBoard::intentarAgregar(const std::string& nombre, int puntaje, TipoOrdenamiento algoritmo) {
    bool hayEspacio = contador < MAX_PUNTAJES_GUARDADOS;
    bool superaAlPeor = (contador > 0) && (puntaje > puntajes[contador - 1].puntaje);

    if (!hayEspacio && !superaAlPeor) {
        return false; // no califica entre los mejores
    }

    ScoreEntry nuevo;
    std::strncpy(nuevo.nombre, nombre.c_str(), sizeof(nuevo.nombre) - 1);
    nuevo.nombre[sizeof(nuevo.nombre) - 1] = '\0';
    nuevo.puntaje = puntaje;

    if (hayEspacio) {
        puntajes[contador++] = nuevo;
    } else {
        puntajes[contador - 1] = nuevo;
    }

    ordenarCon(algoritmo);
    guardarEnArchivo();
    return true;
}
