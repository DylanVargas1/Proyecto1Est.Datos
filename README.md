# Proyecto I — EIF207 Estructuras de Datos

Tetris simplificado en C++ que implementa las 6 estructuras de datos lineales
propias exigidas por el enunciado (cola, pila, lista doblemente enlazada,
cola ordenada de eventos, lista enlazada para el tablero, y dos algoritmos
de ordenamiento propios), con interfaz gráfica en **SFML**.

## Requisitos

- **Lenguaje:** C++ (estándar C++17).
- **Entorno recomendado:** ZinjaI (con MinGW/GCC en Windows).
- **Librería gráfica:** [SFML 2.5](https://www.sfml-dev.org/) (`sfml-graphics`,
  `sfml-window`, `sfml-system`). Las `.dll` necesarias para ejecutar en
  Windows ya se incluyen en `Debug/`.

## Cómo compilar y ejecutar (ZinjaI)

1. Abrir `Proy1Est.Datos.zpr` con ZinjaI.
2. Verificar que el proyecto tenga vinculadas las librerías de SFML
   (`sfml-graphics`, `sfml-window`, `sfml-system`) y que la carpeta de
   `include`/`lib` de SFML esté configurada en las opciones del proyecto.
3. Compilar y ejecutar con el botón de "Compilar y ejecutar" (o F9).
4. Se abre una ventana con el menú de inicio del juego.

También se puede compilar por línea de comandos (MinGW) desde la carpeta
del proyecto, por ejemplo:

```
g++ -std=c++17 -I. main.cpp engine/*.cpp board/*.cpp pieces/*.cpp ^
    replay/*.cpp events/*.cpp scores/*.cpp sorting/*.cpp core/*.cpp ^
    render/*.cpp -lsfml-graphics -lsfml-window -lsfml-system ^
    -o Proy1Est.Datos.exe
```

## Modo benchmark (para el informe)

El ejecutable también tiene un **modo de consola** que no abre la ventana
del juego: corre `Benchmark::ejecutarComparacion` (insertion sort vs.
merge sort) con los tamaños {10, 100, 1000, 10000} que pide la sección 4
del enunciado, y genera un CSV con los tiempos reales medidos.

```
Proy1Est.Datos.exe --benchmark
```

Esto genera `benchmark_resultados.csv` en la carpeta actual, con columnas
`tamano,algoritmo,tiempo_ms`. Se puede indicar una ruta de salida distinta:

```
Proy1Est.Datos.exe --benchmark resultados/mi_benchmark.csv
```

Ese CSV es el que se abre en Excel/Sheets para armar la tabla y el
gráfico de la sección 4 del informe.

## Controles

| Pantalla | Tecla | Acción |
|---|---|---|
| Menú de inicio | Enter | Iniciar nueva partida |
| Menú de inicio | 1 / 2 | Elegir algoritmo de ordenamiento (1 = insertion, 2 = merge) para la tabla de puntajes |
| Menú de inicio | T | Mostrar/ocultar tabla de mejores puntajes |
| En partida | ←/→ | Mover pieza |
| En partida | ↑ | Rotar pieza (sin wall kick) |
| En partida | ↓ | Bajar suave |
| En partida | Espacio | Bajar forzado (hard drop) |
| En partida | C | Enviar/traer pieza de hold |
| En partida | Z | Deshacer último movimiento |
| En partida | X | Rehacer movimiento |
| En partida | P | Pausar |
| En pausa | P | Reanudar |
| En pausa | Esc | Volver al menú |
| Ingresando nombre (game over) | Enter | Confirmar nombre y guardar puntaje |
| Ingresando nombre (game over) | Backspace | Borrar carácter |
| Game over | R | Ver replay de la partida completa |
| Game over | Enter | Nueva partida |
| Game over | Esc | Volver al menú |
| Replay | ←/→ | Retroceder/avanzar un paso |
| Replay | Enter | Nueva partida |
| Replay | Esc | Volver al menú |
| Cualquier pantalla de juego | H | Mostrar/ocultar ayuda de controles en pantalla |

## Estructura del proyecto

```
core/     Tipos compartidos, Random, Timer
pieces/   PieceQueue (cola FIFO propia, bolsas de 7), HoldStack (pila propia), Tetromino
board/    Board (tablero como lista enlazada de 20 filas)
replay/   MoveList (lista doblemente enlazada: deshacer/rehacer + reproducción)
events/   EventQueue (cola ordenada por momento de disparo, 3 tipos de eventos)
scores/   ScoreBoard (persistencia + selección de algoritmo de ordenamiento)
sorting/  insertionSort, mergeSort (propios) y Benchmark (medición de tiempos)
engine/   GameEngine: integra y coordina las 6 estructuras
render/   Renderer: única capa que conoce SFML, dibuja todas las pantallas
```

## Estructuras de datos utilizadas

- **Cola propia (`PieceQueue`):** piezas futuras, generadas por bolsas de 7.
- **Pila propia (`HoldStack`):** pieza en espera (hold), capacidad 1.
- **Lista doblemente enlazada propia (`MoveList`):** historial de
  movimientos, con deshacer/rehacer de varios pasos y reproducción
  completa de la partida.
- **Cola ordenada propia (`EventQueue`):** eventos programados
  (aumento de velocidad, pieza especial limpiadora, reducción de
  visibilidad de la cola), siempre ordenada por momento de disparo.
- **Lista enlazada propia (`Board`):** tablero representado como 20 nodos
  de fila; al limpiar líneas se eliminan nodos y se insertan filas vacías
  al inicio.
- **Dos algoritmos de ordenamiento propios (`insertionSort`, `mergeSort`):**
  usados para ordenar la tabla de mejores puntajes; seleccionables desde
  el menú.

Ninguna de estas estructuras usa `std::queue`, `std::stack`, `std::deque`,
`std::list`, `std::priority_queue` ni `std::sort`, según lo exige la
sección 4 del enunciado.

## Limitaciones conocidas

- La rotación no implementa sistema de *wall kick*: si la pieza no cabe en
  la orientación siguiente, simplemente no rota.
- El límite de deshacer/rehacer con Z/X aplica solo a los movimientos de la
  pieza actualmente en juego (no retrocede piezas ya fijadas al tablero).
- El puntaje solo se guarda si el jugador confirma su nombre en la
  pantalla de game over.


