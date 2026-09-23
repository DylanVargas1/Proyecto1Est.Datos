#pragma once
// GameEngine.h
// El motor de juego contiene TODA la logica de reglas y conecta las 6
// estructuras de datos obligatorias.

#include "../core/Types.h"
#include "../core/Timer.h"
#include "../core/Random.h"
#include "../pieces/Tetromino.h"
#include "../pieces/PieceQueue.h"
#include "../pieces/HoldStack.h"
#include "../board/Board.h"
#include "../replay/MoveList.h"
#include "../events/EventQueue.h"
#include "../scores/ScoreBoard.h"
#include <memory>

class GameEngine {
public:
	GameEngine();
	void moverIzquierda();
	void moverDerecha();
	void rotar();
	void bajarSuave();
	void bajarForzado(); 
	void enviarAHold();
	void deshacerUltimoMovimiento();
	void rehacerMovimiento();
	void iniciarNuevaPartida();
	void alternarPausa();
	void volverAlMenu();        
	void iniciarReproduccionReplay();
	void avanzarPasoReplay();   
	void retrocederPasoReplay(); 
	
	// --- Actualizacion por frame (llamada desde el bucle principal) ---
	void actualizar(float deltaSegundos);
	
	// --- Consultas para la capa de render ---
	GameState estadoActual() const { return estado; }
	const Board& tablero() const { return *tableroJuego; }
	const Tetromino& piezaActiva() const { return *piezaActual; }
	bool hayPiezaActivaParaMostrar() const { return piezaActual != nullptr; }
	PieceType consultarSiguiente(int posicion) const { return colaPiezas.consultar(posicion); }
	PieceType piezaEnHold() const { return hold.peek(); }
	int puntajeActual() const { return puntaje; }
	int lineasTotales() const { return lineasAcumuladas; }
	float tiempoDePartida() const { return tiempoPartidaSegundos; }
	const ScoreBoard& tablaMejoresPuntajes() const { return marcador; }
	
	int pasoReplayActual() const { return indiceReplayActual; }
	int totalPasosReplay() const { return historialMovimientos.totalMovimientos(); }
	
	void establecerAlgoritmoOrdenamiento(TipoOrdenamiento algoritmo) { algoritmoSeleccionado = algoritmo; }
	TipoOrdenamiento algoritmoActual() const { return algoritmoSeleccionado; }
	void guardarPuntajeFinal(const std::string& nombreJugador);
	
	bool proximoEvento(TipoEvento& tipoSalida, float& segundosRestantesSalida) const;
	
	bool huboEventoReciente() const { return huboEventoReciente_; }
	TipoEvento tipoUltimoEvento() const { return tipoEventoReciente_; }
	float tiempoUltimoEvento() const { return tiempoEventoReciente_; }
	
	// Corresponde a cuantas piezas siguientes se deben mostrar ahora mismo (normalmente
	// 3; baja a 1 mientras el evento REDUCIR_VISIBILIDAD_COLA esta activo).
	int visibilidadCola() const { return visibilidadColaActual; }
	
	void agregarCaracterNombre(char c);
	void borrarCaracterNombre();
	void confirmarNombre(); 
	const std::string& nombreEnProgreso() const { return nombreJugadorEnProgreso; }
	
private:
		GameState estado;
		
		std::unique_ptr<Board> tableroJuego;
		std::unique_ptr<Tetromino> piezaActual;
		PieceQueue colaPiezas;
		HoldStack hold;
		MoveList historialMovimientos;
		EventQueue colaEventos;
		ScoreBoard marcador;
		Random rng;
		
		int puntaje;
		int lineasAcumuladas;
		float tiempoPartidaSegundos;
		float intervaloCaidaActual;   // modificable por eventos (AUMENTAR_VELOCIDAD)
		float acumuladorCaida;        
		bool piezaEspecialActiva;     
		int visibilidadColaActual;    
		bool holdUsadoEsteTurno;      
		bool estabaPausado;
		TipoOrdenamiento algoritmoSeleccionado;
		int indiceReplayActual;
		bool puntajeYaGuardadoEstaPartida; 
		std::string nombreJugadorEnProgreso; // se va llenando mientras el jugador escribe
		
		bool huboEventoReciente_;
		TipoEvento tipoEventoReciente_;
		float tiempoEventoReciente_;
		
		// Limite de deshacer/rehacer PARA LA PIEZA ACTUAL.
		int movimientosDisponiblesParaDeshacer;
		int movimientosTotalesEstaPieza;
		
		void generarNuevaPiezaActiva();
		bool colisionaEn(const Tetromino& pieza) const;
		void fijarPiezaAlTablero();
		void programarEventosIniciales();
		void aplicarEvento(const GameEvent& evento);
		void registrarAccion(TipoAccion tipo, int filaAntes, int columnaAntes, int orientacionAntes,
							 int filasLimpiadas = 0);
		void descenderUnaFila(bool esAccionDelJugador);
};
