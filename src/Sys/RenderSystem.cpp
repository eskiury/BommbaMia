#include "RenderSystem.h"
#include "../CMP/RenderComponent.h"

RenderSystem::RenderSystem(ElMotorRaylib &motor, EntityManager &em) : motor_(motor), em_(em)
{
}

void RenderSystem::renderizarEntidad(EntityManager &em, const Entity &e)
{
   // motor_.dibujaEntidad(em, e);
}

void RenderSystem::renderizarIndestructibles()
{
    //motor_.dibujaBloquesNoDestruibles();
}

void RenderSystem::renderizarDestructibles()
{
    //motor_.dibujaBloquesDestruibles(em_);
}

void RenderSystem::renderizarMapa()
{
   // motor_.dibujaMapa();
}

void RenderSystem::renderizarHud(float r)
{
    // motor_.dibujaHud(r);
}

void RenderSystem::renderizarMenuPowerUps(int seleccionActual)
{
   // motor_.dibujaMenuPowerUps(seleccionActual);
}

void RenderSystem::renderizarDebugInfo()
{
    //motor_.dibujaDebugInfo(em_);
}



void RenderSystem::RenderUpdate(int seleccionActual, float r)
{
    // motor_.EmpezarModo3D();
    // renderizarMapa();

    // em_.forEach(EntityType::PLAYER, [this](const Entity &e)
    //             {
    //                 renderizarEntidad(em_, e);
    //             });

    // em_.forEach(EntityType::BOMBA, [this](const Entity &e)
    //             {
    //                 renderizarEntidad(em_, e);
    //             });

    // em_.forEach(EntityType::DESTRUIBLE, [this](const Entity &e)
    //             {
    //                 renderizarEntidad(em_, e);
    //             });

    // em_.forEach(EntityType::NPC, [this](const Entity &e)
    //             {
    //                 renderizarEntidad(em_, e);
    //             });

    // motor_.AcabarModo3D();

    // renderizarHud(r);
    // renderizarDebugInfo();
    // renderizarMenuPowerUps(seleccionActual);

}
