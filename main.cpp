// main.cpp
// Punto de entrada del programa. 

#include "engine/GameEngine.h"
#include "render/Renderer.h"
#include "core/Timer.h"

int main() {
    GameEngine motor;
    Renderer render;
    Timer relojDeFrame;

    while (render.ventanaAbierta()) {
        float delta = relojDeFrame.segundosTranscurridos();
        relojDeFrame.reiniciar();

        render.procesarEventosDeVentana(motor);
        motor.actualizar(delta);
        render.dibujarFrame(motor);
    }

    return 0;
}
