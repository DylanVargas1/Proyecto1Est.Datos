#include "Renderer.h"
#include "FontData.h"
#include <sstream>
#include <iomanip>
#include <iostream>


static const sf::Color COLOR_FONDO(24, 26, 32);
static const sf::Color COLOR_PANEL(34, 37, 46);
static const sf::Color COLOR_GRILLA(50, 54, 64);
static const sf::Color COLOR_TEXTO(230, 230, 235);
static const sf::Color COLOR_TEXTO_TENUE(140, 144, 155);
static const sf::Color COLOR_ACENTO(120, 170, 255);
static const sf::Color COLOR_AVISO(240, 200, 90);

Renderer::Renderer()
	: ventana(sf::VideoMode(820, 760), "Tetris - EIF207 Estructuras de Datos"),
	fuenteCargada(false), mostrarAyuda(false), mostrarTablaPuntajes(false) {
	ventana.setFramerateLimit(60);
	
	fuenteCargada = fuente.loadFromMemory(FUENTE_TTF_DATA, FUENTE_TTF_LEN);
	if (!fuenteCargada) {
		std::cerr << "[AVISO] No se pudo cargar la fuente incrustada. "
			<< "El juego funciona igual, pero sin texto en pantalla." << std::endl;
	}
}

void Renderer::dibujarTexto(const std::string& texto, float x, float y, unsigned tamano, sf::Color color) {
	if (!fuenteCargada) return;
	sf::Text t;
	t.setFont(fuente);
	t.setString(texto);
	t.setCharacterSize(tamano);
	t.setFillColor(color);
	t.setPosition(x, y);
	ventana.draw(t);
}

sf::Color Renderer::colorParaTipo(PieceType tipo) const {
	switch (tipo) {
	case PieceType::I: return sf::Color(90, 200, 220);
	case PieceType::O: return sf::Color(230, 200, 90);
	case PieceType::T: return sf::Color(170, 120, 220);
	case PieceType::S: return sf::Color(120, 200, 120);
	case PieceType::Z: return sf::Color(220, 100, 100);
	case PieceType::J: return sf::Color(100, 140, 220);
	case PieceType::L: return sf::Color(230, 150, 90);
	default: return COLOR_GRILLA;
	}
}

std::string Renderer::nombreAlgoritmo(TipoOrdenamiento algoritmo) const {
	return (algoritmo == TipoOrdenamiento::INSERTION)
		? "Insertion Sort O(n^2)"
		: "Merge Sort O(n log n)";
}

std::string Renderer::nombreEvento(TipoEvento evento) const {
	switch (evento) {
	case TipoEvento::AUMENTAR_VELOCIDAD: return "Velocidad aumentada";
	case TipoEvento::PIEZA_ESPECIAL_LIMPIADORA: return "Pieza especial activa (limpia fila)";
	case TipoEvento::REDUCIR_VISIBILIDAD_COLA: return "Cola visible reducida a 1";
	}
	return "";
}

void Renderer::procesarEventosDeVentana(GameEngine& motor) {
	sf::Event evento;
	while (ventana.pollEvent(evento)) {
		if (evento.type == sf::Event::Closed) {
			ventana.close();
		}
		
		GameState estado = motor.estadoActual();
		
		// --- Entrada de texto 
		if (evento.type == sf::Event::TextEntered && estado == GameState::INGRESANDO_NOMBRE) {
			sf::Uint32 unicode = evento.text.unicode;
			if (unicode >= 32 && unicode < 127 && unicode != 8) {
				motor.agregarCaracterNombre(static_cast<char>(unicode));
			}
		}
		
		if (evento.type == sf::Event::KeyPressed) {
			if (estado == GameState::MENU_INICIO) {
				if (evento.key.code == sf::Keyboard::Enter) {
					motor.iniciarNuevaPartida();
				} else if (evento.key.code == sf::Keyboard::Num1) {
					motor.establecerAlgoritmoOrdenamiento(TipoOrdenamiento::INSERTION);
				} else if (evento.key.code == sf::Keyboard::Num2) {
					motor.establecerAlgoritmoOrdenamiento(TipoOrdenamiento::MERGE);
				} else if (evento.key.code == sf::Keyboard::T) {
					mostrarTablaPuntajes = !mostrarTablaPuntajes;
				} else if (evento.key.code == sf::Keyboard::H) {
					mostrarAyuda = !mostrarAyuda;
				}
			} else if (estado == GameState::JUGANDO) {
				switch (evento.key.code) {
				case sf::Keyboard::Left:  motor.moverIzquierda(); break;
				case sf::Keyboard::Right: motor.moverDerecha(); break;
				case sf::Keyboard::Up:    motor.rotar(); break;
				case sf::Keyboard::Down:  motor.bajarSuave(); break;
				case sf::Keyboard::Space: motor.bajarForzado(); break;
				case sf::Keyboard::C:     motor.enviarAHold(); break;
				case sf::Keyboard::Z:     motor.deshacerUltimoMovimiento(); break;
				case sf::Keyboard::X:     motor.rehacerMovimiento(); break;
				case sf::Keyboard::P:     motor.alternarPausa(); break;
				case sf::Keyboard::H:     mostrarAyuda = !mostrarAyuda; break;
				default: break;
				}
			} else if (estado == GameState::PAUSA) {
				if (evento.key.code == sf::Keyboard::P) motor.alternarPausa();
				else if (evento.key.code == sf::Keyboard::Escape) motor.volverAlMenu();
				else if (evento.key.code == sf::Keyboard::H) mostrarAyuda = !mostrarAyuda;
			} else if (estado == GameState::INGRESANDO_NOMBRE) {
				if (evento.key.code == sf::Keyboard::BackSpace) {
					motor.borrarCaracterNombre();
				} else if (evento.key.code == sf::Keyboard::Enter) {
					motor.confirmarNombre(); 
				}
			} else if (estado == GameState::GAME_OVER) {
				if (evento.key.code == sf::Keyboard::R) {
					motor.iniciarReproduccionReplay();
				} else if (evento.key.code == sf::Keyboard::Enter) {
					motor.iniciarNuevaPartida();
				} else if (evento.key.code == sf::Keyboard::Escape) {
					motor.volverAlMenu();
				}
			} else if (estado == GameState::REPRODUCIENDO_REPLAY) {
				if (evento.key.code == sf::Keyboard::Right) motor.avanzarPasoReplay();
				if (evento.key.code == sf::Keyboard::Left) motor.retrocederPasoReplay();
				if (evento.key.code == sf::Keyboard::Enter) motor.iniciarNuevaPartida();
				if (evento.key.code == sf::Keyboard::Escape) motor.volverAlMenu();
			}
		}
	}
}

void Renderer::dibujarTablero(const GameEngine& motor) {
	sf::RectangleShape fondoTablero(sf::Vector2f(TABLERO_COLUMNAS * TAMANO_CELDA, TABLERO_FILAS * TAMANO_CELDA));
	fondoTablero.setPosition(OFFSET_X_TABLERO, OFFSET_Y_TABLERO);
	fondoTablero.setFillColor(COLOR_PANEL);
	ventana.draw(fondoTablero);
	
	for (int f = 0; f < TABLERO_FILAS; ++f) {
		for (int c = 0; c < TABLERO_COLUMNAS; ++c) {
			int valor = motor.tablero().obtenerCelda(f, c);
			sf::RectangleShape celda(sf::Vector2f(TAMANO_CELDA - 2, TAMANO_CELDA - 2));
			celda.setPosition(OFFSET_X_TABLERO + c * TAMANO_CELDA + 1,
							  OFFSET_Y_TABLERO + f * TAMANO_CELDA + 1);
			if (valor == 0) {
				celda.setFillColor(COLOR_GRILLA);
			} else {
				PieceType tipo = static_cast<PieceType>(valor - 1);
				celda.setFillColor(colorParaTipo(tipo));
			}
			ventana.draw(celda);
		}
	}
	
	// --- Animacion de limpieza de linea: parpadeo blanco sobre las
	// filas completas mientras el motor espera antes de eliminarlas ---
	if (motor.hayFlashDeLineasActivo()) {
		float progreso = motor.progresoFlash(); // 0..1
		bool visible = (static_cast<int>(progreso * 8.0f) % 2) == 0;
		if (visible) {
			for (int i = 0; i < motor.cantidadFilasEnFlash(); ++i) {
				int fila = motor.filaEnFlash(i);
				sf::RectangleShape destello(sf::Vector2f(TABLERO_COLUMNAS * TAMANO_CELDA - 2, TAMANO_CELDA - 2));
				destello.setPosition(OFFSET_X_TABLERO + 1, OFFSET_Y_TABLERO + fila * TAMANO_CELDA + 1);
				destello.setFillColor(sf::Color::White);
				ventana.draw(destello);
			}
		}
	}
}

void Renderer::dibujarPiezaActiva(const GameEngine& motor) {
	GameState estado = motor.estadoActual();
	// Se dibuja tanto mientras se juega como durante el replay 
	if (estado != GameState::JUGANDO && estado != GameState::REPRODUCIENDO_REPLAY) return;
	if (!motor.hayPiezaActivaParaMostrar()) return;
	
	const Tetromino& pieza = motor.piezaActiva();
	Celda celdas[4];
	pieza.celdasAbsolutas(celdas);
	sf::Color color = colorParaTipo(pieza.tipo());
	
	for (int i = 0; i < 4; ++i) {
		if (celdas[i].fila < 0) continue; 
		sf::RectangleShape celda(sf::Vector2f(TAMANO_CELDA - 2, TAMANO_CELDA - 2));
		celda.setPosition(OFFSET_X_TABLERO + celdas[i].columna * TAMANO_CELDA + 1,
						  OFFSET_Y_TABLERO + celdas[i].fila * TAMANO_CELDA + 1);
		celda.setFillColor(color);
		ventana.draw(celda);
	}
}

void Renderer::dibujarPiezaEnMiniatura(PieceType tipo, float x, float y) {
	Tetromino piezaTemp(tipo);
	piezaTemp.filaBase = 0;
	piezaTemp.columnaBase = 0;
	Celda celdas[4];
	piezaTemp.celdasAbsolutas(celdas);
	sf::Color color = colorParaTipo(tipo);
	const int tam = 16;
	for (int i = 0; i < 4; ++i) {
		sf::RectangleShape celda(sf::Vector2f(tam - 2, tam - 2));
		celda.setPosition(x + celdas[i].columna * tam, y + celdas[i].fila * tam);
		celda.setFillColor(color);
		ventana.draw(celda);
	}
}


static const float PANEL_SIGUIENTES_Y = 40;
static const float PANEL_HOLD_Y = 260;
static const float PANEL_EVENTOS_CAJA_Y = 400;   
static const float PANEL_PUNTAJE_Y = 545;
static const float PANEL_LINEAS_Y = 580;
static const float PANEL_AYUDA_Y = 680;

void Renderer::dibujarSiguientesPiezas(const GameEngine& motor) {
	float panelX = OFFSET_X_TABLERO + TABLERO_COLUMNAS * TAMANO_CELDA + 30;
	int cuantasMostrar = motor.visibilidadCola(); 
	std::ostringstream titulo;
	titulo << "SIGUIENTES (" << cuantasMostrar << ")";
	dibujarTexto(titulo.str(), panelX, PANEL_SIGUIENTES_Y, 15, COLOR_TEXTO_TENUE);
	for (int i = 0; i < cuantasMostrar; ++i) {
		PieceType tipo = motor.consultarSiguiente(i);
		if (tipo == PieceType::NINGUNA) continue;
		dibujarPiezaEnMiniatura(tipo, panelX, PANEL_SIGUIENTES_Y + 30 + i * 65.0f);
	}
}

void Renderer::dibujarHold(const GameEngine& motor) {
	float panelX = OFFSET_X_TABLERO + TABLERO_COLUMNAS * TAMANO_CELDA + 30;
	dibujarTexto("HOLD (tecla C)", panelX, PANEL_HOLD_Y, 15, COLOR_TEXTO_TENUE);
	PieceType tipo = motor.piezaEnHold();
	if (tipo != PieceType::NINGUNA) {
		dibujarPiezaEnMiniatura(tipo, panelX, PANEL_HOLD_Y + 30);
	} else {
		dibujarTexto("(vacio)", panelX, PANEL_HOLD_Y + 30, 12, COLOR_TEXTO_TENUE);
	}
}

void Renderer::dibujarProximoEvento(const GameEngine& motor) {
	float panelX = OFFSET_X_TABLERO + TABLERO_COLUMNAS * TAMANO_CELDA + 30;
	
	// Recuadro propio y bien separado del resto
	sf::RectangleShape caja(sf::Vector2f(300, 120));
	caja.setPosition(panelX - 10, PANEL_EVENTOS_CAJA_Y - 10);
	caja.setFillColor(sf::Color(30, 32, 40));
	caja.setOutlineColor(COLOR_AVISO);
	caja.setOutlineThickness(1);
	ventana.draw(caja);
	
	dibujarTexto("EVENTOS PROGRAMADOS", panelX, PANEL_EVENTOS_CAJA_Y, 13, COLOR_AVISO);
	
	std::ostringstream ossTiempo;
	ossTiempo << "Tiempo de partida: " << static_cast<int>(motor.tiempoDePartida()) << "s";
	dibujarTexto(ossTiempo.str(), panelX, PANEL_EVENTOS_CAJA_Y + 28, 13, COLOR_TEXTO);
	
	TipoEvento tipoProx;
	float restante;
	if (motor.proximoEvento(tipoProx, restante)) {
		std::ostringstream ossProx;
		ossProx << "Proximo evento en " << (restante > 0 ? static_cast<int>(restante) : 0) << "s";
		dibujarTexto(ossProx.str(), panelX, PANEL_EVENTOS_CAJA_Y + 52, 12, COLOR_TEXTO_TENUE);
	} else {
		dibujarTexto("(no quedan mas eventos programados)", panelX, PANEL_EVENTOS_CAJA_Y + 52, 11, COLOR_TEXTO_TENUE);
	}
	
	if (motor.huboEventoReciente()) {
		std::ostringstream ossEvento;
		ossEvento << "Ultimo (t=" << static_cast<int>(motor.tiempoUltimoEvento()) << "s):";
		dibujarTexto(ossEvento.str(), panelX, PANEL_EVENTOS_CAJA_Y + 78, 12, COLOR_TEXTO);
		dibujarTexto(nombreEvento(motor.tipoUltimoEvento()), panelX, PANEL_EVENTOS_CAJA_Y + 96, 12, COLOR_AVISO);
	} else {
		dibujarTexto("(todavia no ha ocurrido ningun evento)", panelX, PANEL_EVENTOS_CAJA_Y + 78, 11, COLOR_TEXTO_TENUE);
	}
}

void Renderer::dibujarHud(const GameEngine& motor) {
	float panelX = OFFSET_X_TABLERO + TABLERO_COLUMNAS * TAMANO_CELDA + 30;
	
	dibujarProximoEvento(motor);
	
	std::ostringstream oss;
	oss << "Puntaje: " << motor.puntajeActual();
	dibujarTexto(oss.str(), panelX, PANEL_PUNTAJE_Y, 18, COLOR_ACENTO);
	
	std::ostringstream oss2;
	oss2 << "Lineas: " << motor.lineasTotales();
	dibujarTexto(oss2.str(), panelX, PANEL_LINEAS_Y, 15, COLOR_TEXTO);
	
	dibujarTexto("H: ayuda   P: pausa", panelX, PANEL_AYUDA_Y, 12, COLOR_TEXTO_TENUE);
}

void Renderer::dibujarAyuda() {
	
	sf::RectangleShape fondo(sf::Vector2f(520, 380));
	fondo.setPosition(150, 190);
	fondo.setFillColor(sf::Color(15, 16, 20, 240));
	fondo.setOutlineColor(COLOR_ACENTO);
	fondo.setOutlineThickness(2);
	ventana.draw(fondo);
	
	float x = 175, y = 210;
	dibujarTexto("AYUDA - CONTROLES  (presiona H para cerrar)", x, y, 15, COLOR_ACENTO); y += 40;
	dibujarTexto("Flechas izquierda/derecha : mover la pieza", x, y, 13, COLOR_TEXTO); y += 26;
	dibujarTexto("Flecha arriba : rotar (si no cabe, no rota)", x, y, 13, COLOR_TEXTO); y += 26;
	dibujarTexto("Flecha abajo : bajar mas rapido", x, y, 13, COLOR_TEXTO); y += 26;
	dibujarTexto("Espacio : caida instantanea (hard drop)", x, y, 13, COLOR_TEXTO); y += 26;
	dibujarTexto("C : enviar/intercambiar pieza con Hold", x, y, 13, COLOR_TEXTO); y += 26;
	dibujarTexto("Z : deshacer tu ultimo movimiento", x, y, 13, COLOR_TEXTO); y += 26;
	dibujarTexto("X : rehacer el movimiento que deshiciste", x, y, 13, COLOR_TEXTO); y += 26;
	dibujarTexto("P : pausar o reanudar la partida", x, y, 13, COLOR_TEXTO); y += 26;
	dibujarTexto("ESC (en pausa/game over) : volver al menu", x, y, 13, COLOR_TEXTO); y += 26;
	dibujarTexto("1 / 2 (en el menu) : elegir algoritmo de orden", x, y, 13, COLOR_TEXTO); y += 26;
	dibujarTexto("T (en el menu) : ver tabla de mejores puntajes", x, y, 13, COLOR_TEXTO);
}

void Renderer::dibujarPantallaInicio(const GameEngine& motor) {
	dibujarTexto("TETRIS", 320, 170, 34, COLOR_ACENTO);
	dibujarTexto("EIF207 - Estructuras de Datos", 300, 215, 14, COLOR_TEXTO_TENUE);
	
	dibujarTexto("ENTER : iniciar partida", 300, 280, 16, COLOR_TEXTO);
	
	std::ostringstream oss;
	oss << "Algoritmo para ordenar puntajes: " << nombreAlgoritmo(motor.algoritmoActual());
	dibujarTexto(oss.str(), 300, 330, 13, COLOR_TEXTO);
	dibujarTexto("(presiona 1 para Insertion Sort, 2 para Merge Sort)", 300, 352, 12, COLOR_TEXTO_TENUE);
	
	dibujarTexto("T : ver mejores puntajes", 300, 400, 13, COLOR_TEXTO_TENUE);
	dibujarTexto("H : ver ayuda de todos los controles", 300, 422, 13, COLOR_TEXTO_TENUE);
	
	if (mostrarTablaPuntajes) dibujarPantallaTablaPuntajes(motor);
}

void Renderer::dibujarPantallaTablaPuntajes(const GameEngine& motor) {
	sf::RectangleShape fondo(sf::Vector2f(380, 340));
	fondo.setPosition(220, 470);
	fondo.setFillColor(sf::Color(15, 16, 20, 240));
	fondo.setOutlineColor(COLOR_ACENTO);
	fondo.setOutlineThickness(2);
	ventana.draw(fondo);
	
	float x = 240, y = 490;
	dibujarTexto("MEJORES PUNTAJES (top 10)", x, y, 15, COLOR_ACENTO); y += 34;
	
	const ScoreBoard& tabla = motor.tablaMejoresPuntajes();
	if (tabla.cantidad() == 0) {
		dibujarTexto("(todavia no hay puntajes guardados)", x, y, 13, COLOR_TEXTO_TENUE);
	} else {
		for (int i = 0; i < tabla.cantidad(); ++i) {
			std::ostringstream fila;
			fila << (i + 1) << ". " << tabla.obtener(i).nombre << " - " << tabla.obtener(i).puntaje;
			dibujarTexto(fila.str(), x, y, 13, COLOR_TEXTO);
			y += 24;
		}
	}
}

void Renderer::dibujarPantallaPausa() {
	sf::RectangleShape fondo(sf::Vector2f(340, 160));
	fondo.setPosition(240, 300);
	fondo.setFillColor(sf::Color(15, 16, 20, 240));
	fondo.setOutlineColor(COLOR_ACENTO);
	fondo.setOutlineThickness(2);
	ventana.draw(fondo);
	
	dibujarTexto("PAUSA", 350, 320, 24, COLOR_ACENTO);
	dibujarTexto("P : continuar la partida", 270, 365, 14, COLOR_TEXTO);
	dibujarTexto("ESC : salir al menu (no se guarda el puntaje)", 270, 390, 12, COLOR_TEXTO_TENUE);
	dibujarTexto("H : ver ayuda de controles", 270, 412, 12, COLOR_TEXTO_TENUE);
}

void Renderer::dibujarPantallaIngresoNombre(const GameEngine& motor) {
	sf::RectangleShape fondo(sf::Vector2f(440, 210));
	fondo.setPosition(190, 250);
	fondo.setFillColor(sf::Color(15, 16, 20, 240));
	fondo.setOutlineColor(sf::Color(220, 100, 100));
	fondo.setOutlineThickness(2);
	ventana.draw(fondo);
	
	dibujarTexto("GAME OVER", 215, 270, 24, sf::Color(220, 100, 100));
	
	std::ostringstream ossPuntaje;
	ossPuntaje << "Puntaje: " << motor.puntajeActual();
	dibujarTexto(ossPuntaje.str(), 215, 310, 16, COLOR_TEXTO);
	
	dibujarTexto("Escribe tu nombre y presiona ENTER para guardarlo:", 215, 345, 13, COLOR_TEXTO);
	
	std::string nombreConCursor = motor.nombreEnProgreso() + "_";
	dibujarTexto(nombreConCursor, 215, 375, 18, COLOR_ACENTO);
	
	dibujarTexto("(si no escribes nada, se guarda como \"Jugador\")", 215, 415, 11, COLOR_TEXTO_TENUE);
}

void Renderer::dibujarPantallaGameOver(const GameEngine& motor) {
	sf::RectangleShape fondo(sf::Vector2f(400, 190));
	fondo.setPosition(210, 270);
	fondo.setFillColor(sf::Color(15, 16, 20, 240));
	fondo.setOutlineColor(sf::Color(220, 100, 100));
	fondo.setOutlineThickness(2);
	ventana.draw(fondo);
	
	dibujarTexto("GAME OVER", 235, 290, 26, sf::Color(220, 100, 100));
	std::ostringstream oss;
	oss << "Puntaje final: " << motor.puntajeActual() << " (ya guardado)";
	dibujarTexto(oss.str(), 235, 335, 14, COLOR_TEXTO);
	
	dibujarTexto("R : ver repeticion de la partida", 235, 375, 13, COLOR_TEXTO);
	dibujarTexto("ENTER : jugar de nuevo", 235, 398, 13, COLOR_TEXTO);
	dibujarTexto("ESC : volver al menu", 235, 421, 13, COLOR_TEXTO_TENUE);
}

void Renderer::dibujarControlesReplay(const GameEngine& motor) {
	sf::RectangleShape fondo(sf::Vector2f(TABLERO_COLUMNAS * TAMANO_CELDA, 70));
	fondo.setPosition(OFFSET_X_TABLERO, OFFSET_Y_TABLERO + TABLERO_FILAS * TAMANO_CELDA + 15);
	fondo.setFillColor(sf::Color(15, 16, 20, 220));
	fondo.setOutlineColor(COLOR_ACENTO);
	fondo.setOutlineThickness(1);
	ventana.draw(fondo);
	
	float x = OFFSET_X_TABLERO + 10;
	float y = OFFSET_Y_TABLERO + TABLERO_FILAS * TAMANO_CELDA + 25;
	std::ostringstream oss;
	oss << "REPRODUCIENDO REPLAY -- paso " << motor.pasoReplayActual() << " / " << motor.totalPasosReplay();
	dibujarTexto(oss.str(), x, y, 14, COLOR_ACENTO);
	dibujarTexto("<- retroceder     -> avanzar     ENTER: nueva partida     ESC: menu", x, y + 26, 12, COLOR_TEXTO);
}

void Renderer::dibujarFrame(const GameEngine& motor) {
	ventana.clear(COLOR_FONDO);
	
	switch (motor.estadoActual()) {
	case GameState::MENU_INICIO:
		dibujarPantallaInicio(motor);
	break;
	case GameState::JUGANDO:
		dibujarTablero(motor);
	dibujarPiezaActiva(motor);
	dibujarSiguientesPiezas(motor);
	dibujarHold(motor);
	dibujarHud(motor);
	if (mostrarAyuda) dibujarAyuda();
	break;
	case GameState::PAUSA:
		dibujarTablero(motor);
	dibujarPiezaActiva(motor);
	dibujarSiguientesPiezas(motor);
	dibujarHold(motor);
	dibujarPantallaPausa();
	if (mostrarAyuda) dibujarAyuda();
	break;
	case GameState::INGRESANDO_NOMBRE:
		dibujarTablero(motor);
	dibujarPantallaIngresoNombre(motor);
	break;
	case GameState::GAME_OVER:
		dibujarTablero(motor);
	dibujarPantallaGameOver(motor);
	break;
	case GameState::REPRODUCIENDO_REPLAY:
		dibujarTablero(motor);
	dibujarPiezaActiva(motor);
	dibujarControlesReplay(motor);
	break;
	}
	
	ventana.display();
}
