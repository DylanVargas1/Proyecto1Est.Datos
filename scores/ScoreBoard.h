#pragma once
// ScoreBoard.h
// Registro de puntajes con persistencia en archivo y ordenamiento
#include <string>
#include "../sorting/SortAlgorithms.h"

const int MAX_PUNTAJES_GUARDADOS = 10;

struct ScoreEntry {
    char nombre[32];
    int puntaje;

    bool operator<(const ScoreEntry& otro) const { return puntaje < otro.puntaje; }
    bool operator>=(const ScoreEntry& otro) const { return puntaje >= otro.puntaje; }
};

class ScoreBoard {
public:
    ScoreBoard(const std::string& rutaArchivo);

    // Carga los puntajes existentes del archivo (si existe).
    void cargarDesdeArchivo();

    // Guarda el estado actual en el archivo
    void guardarEnArchivo() const;

    // Calcula la formula de puntos 
    static int puntosPorLineas(int lineasLimpiadas);

    // Intenta agregar un nuevo puntaje. Si entra entre los mejores
    bool intentarAgregar(const std::string& nombre, int puntaje, TipoOrdenamiento algoritmo);

    int cantidad() const { return contador; }
    const ScoreEntry& obtener(int indice) const { return puntajes[indice]; }

private:
    ScoreEntry puntajes[MAX_PUNTAJES_GUARDADOS];
    int contador;
    std::string ruta;

    void ordenarCon(TipoOrdenamiento algoritmo);
};
