#pragma once
// Renderer.h
// Unica clase que incluye encabezados de SFML.

#include <SFML/Graphics.hpp>
#include "../engine/GameEngine.h"

class Renderer {
public:
	Renderer();
	
	bool ventanaAbierta() const { return ventana.isOpen(); }
	void procesarEventosDeVentana(GameEngine& motor);
	void dibujarFrame(const GameEngine& motor);
	
private:
	sf::RenderWindow ventana;
	sf::Font fuente;
	bool fuenteCargada;
	bool mostrarAyuda; 
	
	static const int TAMANO_CELDA = 28;
	static const int OFFSET_X_TABLERO = 40;
	static const int OFFSET_Y_TABLERO = 40;
	
	sf::Color colorParaTipo(PieceType tipo) const;
	
	void dibujarTablero(const GameEngine& motor);
	void dibujarPiezaActiva(const GameEngine& motor);
	void dibujarSiguientesPiezas(const GameEngine& motor);
	void dibujarHold(const GameEngine& motor);
	void dibujarHud(const GameEngine& motor);
	void dibujarPantallaInicio(const GameEngine& motor);
	void dibujarPantallaPausa();
	void dibujarPantallaGameOver(const GameEngine& motor);
	void dibujarControlesReplay(const GameEngine& motor);
	void dibujarPantallaIngresoNombre(const GameEngine& motor);
	void dibujarPantallaTablaPuntajes(const GameEngine& motor);
	void dibujarAyuda();                  // overlay con todas las teclas, se dibuja encima de todo
	void dibujarProximoEvento(const GameEngine& motor);
	std::string nombreAlgoritmo(TipoOrdenamiento algoritmo) const;
	std::string nombreEvento(TipoEvento evento) const;
	
	void dibujarTexto(const std::string& texto, float x, float y, unsigned tamano, sf::Color color);
	void dibujarPiezaEnMiniatura(PieceType tipo, float x, float y);
	
	bool mostrarTablaPuntajes; // se activa con T en el menu de inicio
};
