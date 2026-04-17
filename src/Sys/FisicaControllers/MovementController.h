#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include "../../AlmondGE/EntityManager.h"
#include "../../utils/miVector3.h"

class CollisionController;

class MovementController{
public:
    void init();

    void moverEntityPorTeclado(Entity& player, const std::unordered_map<std::string, bool> &inputMap, EntityManager &em, CollisionController &collisionController, MapSystem &mapSystem);
    void moverEntity(EntityManager& em, Entity& e);
    void desplazarMovimientoVertical(EntityManager& em, Entity& e, bool moveLeft, bool moveRight, bool moveUp, bool moveDown, CollisionController &collisionController, MapSystem &mapSystem);
    void desplazarMovimientoLateral(EntityManager& em, Entity& e, bool moveLeft, bool moveRight, bool moveUp, bool moveDown, CollisionController &collisionController,  MapSystem &mapSystem);

    void moverNPC(EntityManager& em);

    void moverFoco(Entity& player, const std::unordered_map<std::string, bool> &inputMap, EntityManager &em, CollisionController &collisionController, MapSystem &mapSystem);
    void moverFocoNPC(EntityManager &em, Entity &e);

private:
    bool isOnTileEnemy(float position, float tileSize, float velocity);

};

#endif