#pragma once

// Tipos y enumeraciones compartidas por todos los modulos del proyecto.

// ---- Piezas ----
enum class PieceType {
	I, O, T, S, Z, J, L,
	NINGUNA  
};

const int NUM_TIPOS_PIEZA = 7; // I,O,T,S,Z,J,L 

// ---- Tablero ----
const int TABLERO_FILAS = 20;
const int TABLERO_COLUMNAS = 10;

// ---- Acciones registradas para el replay (lista doblemente enlazada) ----
enum class TipoAccion {
	MOVER_IZQUIERDA,
	MOVER_DERECHA,
	ROTAR,
	BAJAR_SUAVE,     
	BAJAR_FORZADO,   
	COLOCAR,         
	HOLD            
};

// ---- Eventos programados ----
enum class TipoEvento {
	AUMENTAR_VELOCIDAD,
	PIEZA_ESPECIAL_LIMPIADORA,
	REDUCIR_VISIBILIDAD_COLA
};

enum class GameState {
	MENU_INICIO,
	JUGANDO,
	PAUSA,
	INGRESANDO_NOMBRE,   
	GAME_OVER,
	REPRODUCIENDO_REPLAY
};
