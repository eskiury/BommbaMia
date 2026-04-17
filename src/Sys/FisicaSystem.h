#ifndef FISICASYSTEM_H
#define FISICASYSTEM_H

#include "../AlmondGE/EntityManager.h"
#include "../utils/miVector3.h"
#include "IAsystem.h"
#include "FisicaControllers/MovementController.h"
#include "FisicaControllers/CollisionController.h"
#include "FisicaControllers/ExplosionController.h"




class MapSystem;
class GameplaySystem;
class InputSystem;

class FisicaSystem
{
public:

    FisicaSystem();

    void init(EntityManager& em, MapSystem &mapSystem);

    void update(EntityManager &em, InputSystem &inputSystem, GameplaySystem &gameplaySystem,std::vector<Entity>& players);

    void bombaBloqueRompible(EntityManager &em, MapSystem &mapSystem);
   
    bool calcularExplosionConBomba(int checkX, int checkZ, std::vector<Entity> &bombas, EntityManager &em, float tileSize);
    bool calcularExplosionConPersonajes(int checkX, int checkZ, EntityManager &em, float tileSize);

    std::vector<MiVector3> getPosicionesCubos(MapSystem &mapSystem);
    std::vector<MiVector3> getGridCoordinates();
    std::vector<ExplosionRastro> getExplosionRastros(EntityManager &em);

    void actualizarEstuneo(EntityManager& em, float deltaTime);
    void atacarFoco(EntityManager &em, Entity& npc);
    void delayFoco(EntityManager &em, float tiempo);
    void invulnerabilidad(EntityManager& em, float tiempo);

    void saveIaSystem(IASystem& iaSystem);

private:
    CollisionController collisionController;
    MovementController  movementController;
    ExplosionController explosionController;

    IASystem iaSystem;

    MapSystem *mapSystem;


};

#endif // FISICASYSTEM_H
