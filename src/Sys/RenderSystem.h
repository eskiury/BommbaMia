#pragma once

#include "../CMP/Entity.h"

#include "../ENGINES/GraphicEngine/motorGrafico.h" // Motor gráfico para renderizado

class RenderSystem
{
public:
    RenderSystem(ElMotorRaylib &motor, EntityManager &em);


    void RenderUpdate(int seleccionActual, float r);

    void renderizarEntidad(EntityManager &em, const Entity &e);
    void renderizarIndestructibles();
    void renderizarDestructibles();
    void renderizarMapa();
    void renderizarHud(float);
    void renderizarMenuPowerUps(int);
    void renderizarDebugInfo();

private:
    ElMotorRaylib &motor_;
    EntityManager &em_;
};