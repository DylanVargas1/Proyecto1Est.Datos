#include "GameEngine.h"
#include <algorithm>

GameEngine::GameEngine()
	: estado(GameState::MENU_INICIO),
	marcador("mejores_puntajes.txt"),
	puntaje(0), lineasAcumuladas(0), tiempoPartidaSegundos(0.0f),
	intervaloCaidaActual(1.0f), acumuladorCaida(0.0f),
	piezaEspecialActiva(false), visibilidadColaActual(3),
	holdUsadoEsteTurno(false), estabaPausado(false),
	algoritmoSeleccionado(TipoOrdenamiento::MERGE),
	indiceReplayActual(0), puntajeYaGuardadoEstaPartida(false),
	huboEventoReciente_(false), tipoEventoReciente_(TipoEvento::AUMENTAR_VELOCIDAD),
	tiempoEventoReciente_(0.0f),
	movimientosDisponiblesParaDeshacer(0), movimientosTotalesEstaPieza(0) {
	tableroJuego = std::make_unique<Board>();
}

void GameEngine::iniciarNuevaPartida() {
	tableroJuego->construirTableroVacio();
	historialMovimientos = MoveList(); // reinicia la lista de replay
	colaEventos = EventQueue();
	puntaje = 0;
	lineasAcumuladas = 0;
	tiempoPartidaSegundos = 0.0f;
	intervaloCaidaActual = 1.0f;
	acumuladorCaida = 0.0f;
	piezaEspecialActiva = false;
	visibilidadColaActual = 3;
	holdUsadoEsteTurno = false;
	indiceReplayActual = 0;
	puntajeYaGuardadoEstaPartida = false;
	nombreJugadorEnProgreso.clear();
	huboEventoReciente_ = false;
	
	hold = HoldStack();
	colaPiezas.asegurarPiezasVisibles(3);
	
	programarEventosIniciales();
	generarNuevaPiezaActiva();
	
	estado = GameState::JUGANDO;
}

void GameEngine::programarEventosIniciales() {
	// 3 tipos de eventos obligatorios, programados a distintos momentos.
	colaEventos.insertarOrdenado({TipoEvento::AUMENTAR_VELOCIDAD, 30.0f});
	colaEventos.insertarOrdenado({TipoEvento::PIEZA_ESPECIAL_LIMPIADORA, 45.0f});
	colaEventos.insertarOrdenado({TipoEvento::REDUCIR_VISIBILIDAD_COLA, 60.0f});
	colaEventos.insertarOrdenado({TipoEvento::AUMENTAR_VELOCIDAD, 90.0f});
	colaEventos.insertarOrdenado({TipoEvento::AUMENTAR_VELOCIDAD, 150.0f});
}

void GameEngine::generarNuevaPiezaActiva() {
	PieceType tipo = colaPiezas.desencolar();
	piezaActual = std::make_unique<Tetromino>(tipo);
	piezaActual->filaBase = 0;
	piezaActual->columnaBase = (TABLERO_COLUMNAS / 2) - 2;
	piezaActual->orientacion = 0;
	holdUsadoEsteTurno = false;
	
	movimientosDisponiblesParaDeshacer = 0;
	movimientosTotalesEstaPieza = 0;
	
	Celda celdas[4];
	piezaActual->celdasAbsolutas(celdas);
	if (tableroJuego->detectarGameOver(celdas)) {
		estado = GameState::INGRESANDO_NOMBRE;
		nombreJugadorEnProgreso.clear();
	}
}

bool GameEngine::colisionaEn(const Tetromino& pieza) const {
	Celda celdas[4];
	pieza.celdasAbsolutas(celdas);
	return !tableroJuego->celdasLibres(celdas, 4);
}

void GameEngine::registrarAccion(TipoAccion tipo, int filaAntes, int columnaAntes,
								 int orientacionAntes, int filasLimpiadas) {
	AccionRecord accion;
	accion.tipo = tipo;
	accion.piezaActual = piezaActual->tipo();
	accion.filaAntes = filaAntes;
	accion.columnaAntes = columnaAntes;
	accion.orientacionAntes = orientacionAntes;
	accion.filaDespues = piezaActual->filaBase;
	accion.columnaDespues = piezaActual->columnaBase;
	accion.orientacionDespues = piezaActual->orientacion;
	accion.filasLimpiadasAlColocar = filasLimpiadas;
	historialMovimientos.registrar(accion);
}

void GameEngine::moverIzquierda() {
	if (estado != GameState::JUGANDO) return;
	int filaAntes = piezaActual->filaBase, colAntes = piezaActual->columnaBase, oriAntes = piezaActual->orientacion;
	piezaActual->columnaBase -= 1;
	if (colisionaEn(*piezaActual)) {
		piezaActual->columnaBase += 1; 
		return;
	}
	registrarAccion(TipoAccion::MOVER_IZQUIERDA, filaAntes, colAntes, oriAntes);
	movimientosTotalesEstaPieza = ++movimientosDisponiblesParaDeshacer;
}

void GameEngine::moverDerecha() {
	if (estado != GameState::JUGANDO) return;
	int filaAntes = piezaActual->filaBase, colAntes = piezaActual->columnaBase, oriAntes = piezaActual->orientacion;
	piezaActual->columnaBase += 1;
	if (colisionaEn(*piezaActual)) {
		piezaActual->columnaBase -= 1;
		return;
	}
	registrarAccion(TipoAccion::MOVER_DERECHA, filaAntes, colAntes, oriAntes);
	movimientosTotalesEstaPieza = ++movimientosDisponiblesParaDeshacer;
}

void GameEngine::rotar() {
	if (estado != GameState::JUGANDO) return;
	int filaAntes = piezaActual->filaBase, colAntes = piezaActual->columnaBase, oriAntes = piezaActual->orientacion;
	
	int orientacionCandidata = (piezaActual->orientacion + 1) % 4;
	Celda celdas[4];
	piezaActual->celdasParaOrientacion(orientacionCandidata, celdas);
	
	if (!tableroJuego->celdasLibres(celdas, 4)) {
		return;
	}
	piezaActual->orientacion = orientacionCandidata;
	registrarAccion(TipoAccion::ROTAR, filaAntes, colAntes, oriAntes);
	movimientosTotalesEstaPieza = ++movimientosDisponiblesParaDeshacer;
}

void GameEngine::bajarSuave() {
	descenderUnaFila(true); 
}

void GameEngine::descenderUnaFila(bool esAccionDelJugador) {
	if (estado != GameState::JUGANDO) return;
	int filaAntes = piezaActual->filaBase, colAntes = piezaActual->columnaBase, oriAntes = piezaActual->orientacion;
	piezaActual->filaBase += 1;
	if (colisionaEn(*piezaActual)) {
		piezaActual->filaBase -= 1;
		fijarPiezaAlTablero(); // colocar SIEMPRE se registra, venga de gravedad o del jugador
		return;
	}
	if (esAccionDelJugador) {
		registrarAccion(TipoAccion::BAJAR_SUAVE, filaAntes, colAntes, oriAntes);
		movimientosTotalesEstaPieza = ++movimientosDisponiblesParaDeshacer;
	}
}

void GameEngine::bajarForzado() {
	if (estado != GameState::JUGANDO) return;
	int filaAntes = piezaActual->filaBase, colAntes = piezaActual->columnaBase, oriAntes = piezaActual->orientacion;
	while (!colisionaEn(*piezaActual)) {
		piezaActual->filaBase += 1;
	}
	piezaActual->filaBase -= 1; 
	registrarAccion(TipoAccion::BAJAR_FORZADO, filaAntes, colAntes, oriAntes);
	fijarPiezaAlTablero();
}

void GameEngine::fijarPiezaAlTablero() {
	Celda celdas[4];
	piezaActual->celdasAbsolutas(celdas);
	
	int colorCodigo = static_cast<int>(piezaActual->tipo()) + 1; 
	for (int i = 0; i < 4; ++i) {
		tableroJuego->ponerCelda(celdas[i].fila, celdas[i].columna, colorCodigo);
	}
	
	if (piezaEspecialActiva) {
		int filaMasBaja = celdas[0].fila;
		for (int i = 1; i < 4; ++i) filaMasBaja = std::max(filaMasBaja, celdas[i].fila);
		for (int c = 0; c < TABLERO_COLUMNAS; ++c) {
			tableroJuego->ponerCelda(filaMasBaja, c, colorCodigo); // fuerza fila llena
		}
		piezaEspecialActiva = false;
	}
	
	int filasLimpiadas = tableroJuego->limpiarFilasCompletas();
	lineasAcumuladas += filasLimpiadas;
	puntaje += ScoreBoard::puntosPorLineas(filasLimpiadas);
	
	registrarAccion(TipoAccion::COLOCAR, piezaActual->filaBase, piezaActual->columnaBase,
					piezaActual->orientacion, filasLimpiadas);
	
	generarNuevaPiezaActiva();
}

void GameEngine::enviarAHold() {
	if (estado != GameState::JUGANDO || holdUsadoEsteTurno) return;
	
	int filaAntes = piezaActual->filaBase, colAntes = piezaActual->columnaBase, oriAntes = piezaActual->orientacion;
	PieceType tipoActual = piezaActual->tipo();
	
	if (hold.estaVacia()) {
		hold.push(tipoActual);
		generarNuevaPiezaActiva();
	} else {
		PieceType tipoGuardado = hold.pop();
		hold.push(tipoActual);
		piezaActual = std::make_unique<Tetromino>(tipoGuardado);
		piezaActual->filaBase = 0;
		piezaActual->columnaBase = (TABLERO_COLUMNAS / 2) - 2;
		piezaActual->orientacion = 0;
		// Es una pieza distinta, asi que el
		// limite de deshacer/rehacer tambien se reinicia aqui.
		movimientosDisponiblesParaDeshacer = 0;
		movimientosTotalesEstaPieza = 0;
	}
	
	holdUsadoEsteTurno = true;
	registrarAccion(TipoAccion::HOLD, filaAntes, colAntes, oriAntes);
}

void GameEngine::deshacerUltimoMovimiento() {
	if (estado != GameState::JUGANDO) return;
	
	// Limite duro: no se puede deshacer mas alla del momento en que
	// aparecio la pieza ACTUAL.
	if (movimientosDisponiblesParaDeshacer <= 0) return;
	
	AccionRecord accion;
	if (!historialMovimientos.deshacer(accion)) return;
	
	piezaActual->filaBase = accion.filaAntes;
	piezaActual->columnaBase = accion.columnaAntes;
	piezaActual->orientacion = accion.orientacionAntes;
	movimientosDisponiblesParaDeshacer--;
}

void GameEngine::rehacerMovimiento() {
	if (estado != GameState::JUGANDO) return;
	if (movimientosDisponiblesParaDeshacer >= movimientosTotalesEstaPieza) return;
	
	AccionRecord accion;
	if (!historialMovimientos.rehacer(accion)) return;
	
	piezaActual->filaBase = accion.filaDespues;
	piezaActual->columnaBase = accion.columnaDespues;
	piezaActual->orientacion = accion.orientacionDespues;
	movimientosDisponiblesParaDeshacer++;
}

void GameEngine::aplicarEvento(const GameEvent& evento) {
	// Se guarda cual fue el ultimo evento y en que momento se aplico,
	huboEventoReciente_ = true;
	tipoEventoReciente_ = evento.tipo;
	tiempoEventoReciente_ = tiempoPartidaSegundos;
	
	switch (evento.tipo) {
	case TipoEvento::AUMENTAR_VELOCIDAD:
		intervaloCaidaActual = std::max(0.15f, intervaloCaidaActual * 0.8f);
	break;
	case TipoEvento::PIEZA_ESPECIAL_LIMPIADORA:
		piezaEspecialActiva = true;
	break;
	case TipoEvento::REDUCIR_VISIBILIDAD_COLA:
		visibilidadColaActual = 1;
	break;
	}
}

void GameEngine::actualizar(float deltaSegundos) {
	if (estado != GameState::JUGANDO) return;
	
	tiempoPartidaSegundos += deltaSegundos;
	
	while (colaEventos.hayEventoListo(tiempoPartidaSegundos)) {
		GameEvent evento = colaEventos.extraerFrente();
		aplicarEvento(evento);
	}
	
	acumuladorCaida += deltaSegundos;
	if (acumuladorCaida >= intervaloCaidaActual) {
		acumuladorCaida = 0.0f;
		descenderUnaFila(false); // gravedad automatica: NO se registra en el historial
	}
	
	colaPiezas.asegurarPiezasVisibles(3);
}

void GameEngine::alternarPausa() {
	if (estado == GameState::JUGANDO) {
		estado = GameState::PAUSA;
	} else if (estado == GameState::PAUSA) {
		estado = GameState::JUGANDO;
	}
}

void GameEngine::volverAlMenu() {
	estado = GameState::MENU_INICIO;
}

void GameEngine::iniciarReproduccionReplay() {
	estado = GameState::REPRODUCIENDO_REPLAY;
	historialMovimientos.iniciarReproduccion();
	indiceReplayActual = 0;
	tableroJuego->construirTableroVacio();
	puntaje = 0;
	piezaActual.reset(); // se ira creando pieza a pieza segun avance el replay
}

void GameEngine::avanzarPasoReplay() {
	if (estado != GameState::REPRODUCIENDO_REPLAY) return;
	AccionRecord accion;
	if (!historialMovimientos.avanzarReproduccion(accion)) return;
	
	indiceReplayActual++;
	
	if (!piezaActual || piezaActual->tipo() != accion.piezaActual) {
		piezaActual = std::make_unique<Tetromino>(accion.piezaActual);
	}
	piezaActual->filaBase = accion.filaDespues;
	piezaActual->columnaBase = accion.columnaDespues;
	piezaActual->orientacion = accion.orientacionDespues;
	
	if (accion.tipo == TipoAccion::COLOCAR) {
		Celda celdas[4];
		piezaActual->celdasAbsolutas(celdas);
		int colorCodigo = static_cast<int>(accion.piezaActual) + 1;
		for (int i = 0; i < 4; ++i) {
			tableroJuego->ponerCelda(celdas[i].fila, celdas[i].columna, colorCodigo);
		}
		tableroJuego->limpiarFilasCompletas();
	}
}

void GameEngine::retrocederPasoReplay() {
	// Reconstruye el tablero desde cero y reproduce hasta el paso
	// anterior.
	if (estado != GameState::REPRODUCIENDO_REPLAY) return;
	int objetivo = std::max(0, indiceReplayActual - 1);
	
	tableroJuego->construirTableroVacio();
	historialMovimientos.iniciarReproduccion();
	indiceReplayActual = 0;
	piezaActual.reset(); 
	
	for (int i = 0; i < objetivo; ++i) {
		avanzarPasoReplay();
	}
}

void GameEngine::guardarPuntajeFinal(const std::string& nombreJugador) {
	if (puntajeYaGuardadoEstaPartida) return;
	marcador.intentarAgregar(nombreJugador, puntaje, algoritmoSeleccionado);
	puntajeYaGuardadoEstaPartida = true;
}

bool GameEngine::proximoEvento(TipoEvento& tipoSalida, float& segundosRestantesSalida) const {
	if (colaEventos.vacia()) return false;
	GameEvent evento = colaEventos.verFrente();
	tipoSalida = evento.tipo;
	segundosRestantesSalida = evento.momentoDisparo - tiempoPartidaSegundos;
	return true;
}

void GameEngine::agregarCaracterNombre(char c) {
	if (estado != GameState::INGRESANDO_NOMBRE) return;
	if (nombreJugadorEnProgreso.size() < 12) { 
		nombreJugadorEnProgreso.push_back(c);
	}
}

void GameEngine::borrarCaracterNombre() {
	if (estado != GameState::INGRESANDO_NOMBRE) return;
	if (!nombreJugadorEnProgreso.empty()) {
		nombreJugadorEnProgreso.pop_back();
	}
}

void GameEngine::confirmarNombre() {
	if (estado != GameState::INGRESANDO_NOMBRE) return;
	std::string nombreFinal = nombreJugadorEnProgreso.empty() ? "Jugador" : nombreJugadorEnProgreso;
	guardarPuntajeFinal(nombreFinal); // aqui SI se persiste de verdad en el archivo
	estado = GameState::GAME_OVER;
}
