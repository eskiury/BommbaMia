#ifndef COLLISIONCONTROLLER_H
#define COLLISIONCONTROLLER_H

#include "../../AlmondGE/EntityManager.h"
#include "../../utils/miVector3.h"

class GameplaySystem;

class CollisionController{
    private:
        IASystem* iaComponent;
    public:
        bool checkBoxesCollision(BoundingBOX &box1, BoundingBOX &box2);
        MiVector3 checkCollisionDirection(BoundingBOX &box1, BoundingBOX &box2);
        bool collisionConBloques(Entity &e, EntityManager &em, MapSystem &mapSystem);
        bool collisionConBloques(BoundingBOX bb, EntityManager &em, MapSystem &mapSystem);
        bool irrompibleCollision(BoundingBOX &playerBB, MapSystem &mapSystem);
        bool destruibleCollision(BoundingBOX &playerBB, EntityManager &em);
        bool collisionConBordes(EntityManager& em, Entity &e, MapSystem &mapSystem);
        bool bombPlayerCollision(Entity &player, EntityManager &em, MapSystem &mapSystem);
        void collisionPersonajePowerUp(EntityManager &em, Entity &e, GameplaySystem &gameplaySystem, MapSystem &mapsystem);
        void empujarBomba(EntityManager &em, Entity &bomba, MiVector3 &direccionEnLaQueChoca, MapSystem &mapSystem);

        void recogerPowerUp (EntityManager &em, GameplaySystem &gameplaySystem, MapSystem &mapsystem, std::vector<Entity>& players);

        std::vector<MiVector3> getPosicionesCubos(MapSystem &mapSystem);
        std::vector<MiVector3> getPosicionesCubosBordes(MapSystem &mapSystem);

        void saveIaSystem(IASystem& iaSystem) {
            iaComponent = &iaSystem;
        }
};

#endif