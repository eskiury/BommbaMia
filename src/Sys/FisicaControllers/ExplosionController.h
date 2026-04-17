#ifndef EXPLOSIONCONTROLLER_H
#define EXPLOSIONCONTROLLER_H

#include "../../AlmondGE/EntityManager.h"
#include "../../utils/miVector3.h"
#include "IAsystem.h"


class ExplosionController{
public:
    std::vector<Entity> calcularExplosion(EntityManager &em, fisicaComponent &bomba, bombaComponent &bombaPOS, MapSystem &mapSystem);
    void bombaBloqueRompible(EntityManager &em, MapSystem &mapSystem);
    void procesarPowerUp(int tileValue, int tileX, int tileZ, const fisicaComponent &fisicaBloque, EntityManager &em, MapSystem &mapSystem);
    bool calcularExplosionConIrrompible(int checkX, int checkZ, const std::vector<MiVector3> &posicionesIrrompibles, float tileSize);
    bool calcularExplosionConDestruible(int checkX, int checkZ, std::vector<Entity> &bloquesDestruibles, std::vector<Entity> &bloquesAEliminar, EntityManager &em, float tileSize, bombaComponent &bombaPOS);
    bool calcularExplosionConBomba(int checkX, int checkZ, EntityManager &em, float tileSize);
    bool calcularExplosionConPersonajes(int checkX, int checkZ, EntityManager &em, float tileSize);
    
    void actualizarExplosionRastro(float deltaTime, EntityManager &em, MapSystem &mapSystem);
    void collisionPersonajeRastroExplosion(EntityManager &em, MapSystem &mapSystem);
    
    std::vector<MiVector3> getPosicionesCubos(MapSystem &mapSystem);
    void saveIaSystem(IASystem& iaSystem);

private:
    IASystem* iaSystem;
};


#endif