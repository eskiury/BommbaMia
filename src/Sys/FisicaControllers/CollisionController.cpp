/**
 * @file CollisionController.cpp
 * @brief Implementación del controlador de colisiones del juego.
 *  
 * Este archivo define la clase CollisionController, responsable de detectar 
 * y gestionar las colisiones entre entidades, bloques, bombas y power-ups.
 * 
 * @author AlmondStudio
 * @date 2025-05-21
 * @version 1.0
 */
#include "CollisionController.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "MapSystem.h"
#include "GameplaySystem.h"
#include "../../ENGINES/GraphicEngine/motorGrafico.h"


/**
 * @brief Verifica si dos bounding boxes están colisionando.
 * 
 * @param box1 Primera bounding box.
 * @param box2 Segunda bounding box.
 * @return true si colisionan, false en caso contrario.
 */
bool CollisionController::checkBoxesCollision(BoundingBOX &box1, BoundingBOX &box2){
    return !(box1.max.x < box2.min.x || box1.min.x > box2.max.x ||
             box1.max.y < box2.min.y || box1.min.y > box2.max.y ||
             box1.max.z < box2.min.z || box1.min.z > box2.max.z);
}



/**
 * @brief Verifica la dirección de la colisión entre dos cajas.
 * 
 * @param box1 Primera bounding box.
 * @param box2 Segunda bounding box.
 * @return MiVector3 -> Vector que indica la dirección de la colisión.
 */
MiVector3 CollisionController::checkCollisionDirection(BoundingBOX &box1, BoundingBOX &box2)
{
    float distLeft = std::abs(box1.max.x - box2.min.x);
    float distRight = std::abs(box1.min.x - box2.max.x);
    float distFront = std::abs(box1.min.z - box2.max.z);
    float distBack = std::abs(box1.max.z - box2.min.z);

    float minDist = std::min({distLeft, distRight, distFront, distBack});
    if (minDist == distLeft)
        return {-1.0f, 0.0f, 0.0f};
    if (minDist == distRight)
        return {1.0f, 0.0f, 0.0f};
    if (minDist == distFront)
        return {0.0f, 0.0f, 1.0f};
    if (minDist == distBack)
        return {0.0f, 0.0f, -1.0f};
    return {0.0f, 0.0f, 0.0f};
}



/**
 * @brief Verifica si una entidad colisiona con bloques (bloques destruibles, indetsruibles y bombas) del mapa.
 * 
 * @param e Entidad a verificar.
 * @param em EntityManager.
 * @param mapSystem Mapa actual del juego.
 * 
 * @return true si colisiona, false si no.
 */
bool CollisionController::collisionConBloques(Entity &e, EntityManager &em, MapSystem &mapSystem){
    
    auto &player = em.fisicas.get(e.fisica_key);

    if (irrompibleCollision(player.siguienteBB, mapSystem))
        return true;

    if (destruibleCollision(player.siguienteBB, em))
        return true;

    if (bombPlayerCollision(e, em, mapSystem))
        return true;

    return false; ///<Si pasa todos los chequeos no colisiona
}


/**
 * @brief Verifica si una bounding box colisiona con bloques del mapa (bloques destruibles, indetsruibles y bombas).
 * 
 * @param bb bounding box.
 * @param em EntityManager.
 * @param mapSystem Mapa actual del juego.
 * 
 * @return true si colisiona, false si no.
 */
bool CollisionController::collisionConBloques(BoundingBOX bb, EntityManager &em, MapSystem &mapSystem){

    if (irrompibleCollision(bb, mapSystem))
        return true;

    if (destruibleCollision(bb, em))
        return true;

    // if (bombPlayerCollision(bb, em))
    //     return true;
        
    return false; ///<Si pasa todos los chequeos no colisiona
}



/**
 * @brief Verifica si una bounding box colisiona con bloques irrompibles.
 * 
 * @param playerBB bounding box del jugador.
 * @param mapSystem Mapa del juego.
 * 
 * @return true si colisiona, false si no.
 */
bool CollisionController::irrompibleCollision(BoundingBOX &playerBB, MapSystem &mapSystem)
{
    std::vector<MiVector3> posiciones = getPosicionesCubos(mapSystem);

    for (const auto &pos : posiciones)
    {
        BoundingBOX cubeBox {
            {pos.x - 1.0f, pos.y - 1.0f, pos.z - 1.0f},
            {pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f}
        };

        if (checkBoxesCollision(playerBB, cubeBox)) {
            return true;
        }
    }
    ///< Si no se colisiona con ninguno, devolvemos false
    return false;
}


/**
 * @brief Verifica si una bounding box colisiona con bloques destruibles.
 * 
 * @param playerBB bounding box del jugador.
 * @param em EntityManager.
 * 
 * @return true si colisiona, false si no.
 */
bool CollisionController::destruibleCollision(BoundingBOX &playerBB, EntityManager &em)
{
    std::vector<Entity> bloquesDestruibles = em.getEntitiesByType(EntityType::DESTRUIBLE);

    for (auto &bloque : bloquesDestruibles)
    {
        auto &physBloque = em.fisicas.get(bloque.fisica_key);
        physBloque.updateBoundingBox();  ///< Aseguramos que su BB está actualizado

        if (checkBoxesCollision(playerBB, physBloque.boundingBox)) {
            return true; ///<si no colisiona devolvenos true
        }
    }
    return false; ///<si no colisiona devolvenos false
}


/**
 * @brief Verifica si el jugador colisiona con los bordes del mapa.
 * 
 * @param em EntityManager.
 * @param e Jugador.
 * @param mapSystem Mapa actual.
 * 
 * @return true si colisiona, false si no.
 */
bool CollisionController::collisionConBordes(EntityManager& em, Entity &e, MapSystem &mapSystem){
    
    auto &player = em.fisicas.get(e.fisica_key);

    std::vector<MiVector3> bordes = getPosicionesCubosBordes(mapSystem);

    for (const auto &pos : bordes)
    {
        BoundingBOX cubeBox {
            {pos.x - 1.0f, pos.y - 1.0f, pos.z - 1.0f},
            {pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f}
        };

        if (checkBoxesCollision(player.siguienteBB, cubeBox)) {
            return true;
        }
    }
    ///<Si no se colisiona con ninguno, devolvemos false
    return false;

}


/**
 * @brief Verifica si hay colisión entre jugador y bombas.
 * 
 * @param player Entidad jugador.
 * @param em EntityManager.
 * @param mapSystem Mapa actual.
 * 
 * @return true si colisiona, false si no.
 */
bool CollisionController::bombPlayerCollision(Entity &player, EntityManager &em, MapSystem &mapSystem){
    auto &playerFisica = em.fisicas.get(player.fisica_key);

    for (Entity &entidad : em.getEntities()) {
        if (entidad.type != EntityType::BOMBA) continue;
        auto &bombaFisica = em.fisicas.get(entidad.fisica_key);
        bombaFisica.updateBoundingBox();

        if (!bombaFisica.encimaBomba) {
            int ownerId = em.bombs.get(entidad.bomb_key).ownerId;
            Entity& p = em.getEntity(ownerId);
            auto &pFisicas = em.fisicas.get(p.fisica_key);
            if (!checkBoxesCollision(pFisicas.siguienteBB, bombaFisica.boundingBox)) {
                bombaFisica.encimaBomba = true;
                pFisicas.chocadoConBomba = false;
            }
            continue;
        }

        if (checkBoxesCollision(playerFisica.siguienteBB, bombaFisica.boundingBox)) {
            playerFisica.chocadoConBomba = true;
            if (playerFisica.canPushBombs) {
                MiVector3 dir = checkCollisionDirection(bombaFisica.boundingBox, playerFisica.siguienteBB);
                empujarBomba(em, entidad, dir, mapSystem);
            }
            return true;
        }
    }

    playerFisica.chocadoConBomba = false;
    return false;
}



/**
 * @brief Obtiene las posiciones de los cubos irrompibles del mapa.
 *
 * Recorre la matriz del mapa para identificar los bloques no destructibles.
 *
 * @param mapSystem Referencia al sistema de mapa.
 * @return Vector con las posiciones de los cubos.
 */
std::vector<MiVector3> CollisionController::getPosicionesCubos(MapSystem &mapSystem){
    const auto &mapMatrix = mapSystem.getMap();
    std::vector<MiVector3> posiciones;

    ///<cuadrícula
    float tileSize = 2.0f;

    ///<recorro la matriz para obtener posiciones de cubos (valor 1)
    for (int i=0; i<mapMatrix.size(); i++){ ///<filas

        for (int j=0; j<mapMatrix[i].size(); j++){ ///<columnas

            if (mapMatrix[i][j] == 1){ ///<cubos no destructibles
                MiVector3 cubePos = {j * tileSize, 0.0f, i * tileSize};
                posiciones.push_back(cubePos);
            }
        }
    }
    return posiciones;
}



/**
 * @brief Obtiene posiciones de los bordes del mapa.
 * 
 * @param mapSystem Mapa actual.
 * 
 * @return std::vector<MiVector3> Posiciones de bordes.
 */
std::vector<MiVector3> CollisionController::getPosicionesCubosBordes(MapSystem &mapSystem){
    const auto &mapMatrix = mapSystem.getMap();
    
    std::vector<MiVector3> bordes;
    float tileSize = 2.0f;
    int filas = mapMatrix.size();
    int columnas = mapMatrix[0].size();

    ///<recorro la matriz mapa
    for(int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){
            if (i == 0 || i == filas - 1 || j == 0 || j == columnas - 1){  ///<si es borde
                MiVector3 cubePos = { j * tileSize, 0.0f, i * tileSize };
                bordes.push_back(cubePos);
            }
        }
    }
    return bordes;
}



/**
 * @brief Verifica colisiones de personajes con power-ups y llama a las funciones para que se le aplique.
 * 
 * @param em EntityManager.
 * @param gameplaySystem GameplaySystem.
 * @param mapsystem MapSystem.
 * @param players Vector de jugadores.
 */
void CollisionController::recogerPowerUp(EntityManager &em, GameplaySystem &gameplaySystem, MapSystem &mapsystem, std::vector<Entity>& players){
    ///<Recoger el PLAYER el powerup
    for(auto &player : players)
    {
        collisionPersonajePowerUp(em, player, gameplaySystem, mapsystem);
    }

    ///<Recoger los NPCs el powerup
    for(auto& npc : em.getEntitiesByType(EntityType::NPC)){
        collisionPersonajePowerUp(em, npc, gameplaySystem, mapsystem);
    }
}


/**
 * @brief Verifica y aplica colisión de personaje con power-ups.
 * 
 * @param em EntityManager.
 * @param e Entidad del personaje.
 * @param gameplay GameplaySystem.
 * @param mapSystem MapSystem.
 */
void CollisionController::collisionPersonajePowerUp(EntityManager &em, Entity &e, GameplaySystem &gameplay, MapSystem &mapSystem)
{
    auto &jugador = em.fisicas.get(e.fisica_key);
    auto &jugadorPW = em.powerUps.get(e.powerUp_key);

    if(!jugador.estaMuerto && !jugador.esFantasma){
        for (Entity &entidad : em.getEntities()){

            if (entidad.type == EntityType::POWERUP){
                auto &powerupFisicas = em.fisicas.get(entidad.fisica_key);
                auto &powerup = em.powerUps.get(entidad.powerUp_key);
                auto &render = em.renders.get(entidad.render_key);
                powerupFisicas.updateBoundingBox();

                if (checkBoxesCollision(jugador.boundingBox, powerupFisicas.boundingBox)){
                    int tileX = static_cast<int>(round(powerupFisicas.x / 2.0f));
                    int tileZ = static_cast<int>(round(powerupFisicas.z / 2.0f));
                    ///<Se le añade el powerUp al jugador, se le aplica y se borra
                    jugadorPW.collectedPowerUps.push_back(powerup.type);
                    gameplay.aplicarPowerUp(e, powerup.type);

                    ///<Lo borro del arbol del motor grafico
                    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
                    motorRaylib.getMotorDeJuego().EliminarPersonaje(render.SusDatosEnElArbol);
                    em.eraseEntity(entidad.id);

                    iaComponent->getGrafo().actualizarZonaInfluencia(tileZ,tileX, mapSystem.getMap(), false);
                    mapSystem.setTile(tileX, tileZ, 0);
                }
            }
        }
    }
}



/**
 * @brief Empuja la bomba en una dirección hasta que colisiona.
 * 
 * @param em EntityManager.
 * @param bomba Entidad bomba.
 * @param direccionEnLaQueChoca Dirección del empuje.
 * @param mapSystem MapSystem.
 */
void CollisionController::empujarBomba(EntityManager &em, Entity &bomba, MiVector3 &direccionEnLaQueChoca, MapSystem &mapSystem)
{
    auto &bombaFisica = em.fisicas.get(bomba.fisica_key);
    auto &bombComp = em.bombs.get(bomba.bomb_key);

    std::vector<MiVector3> posicionesIrrompibles = getPosicionesCubos(mapSystem);
    std::vector<Entity> bloquesDestruibles = em.getEntitiesByType(EntityType::DESTRUIBLE);
    std::vector<Entity> otrasBombas = em.getEntitiesByType(EntityType::BOMBA);

    float tileSize = 2.0f;
    int maxSteps = 100; ///<Límite de pasos para evitar bucles infinitos
    int stepCount = 0;

    ///<almacenar la última posición
    int ultimoTileX = (int)(round(bombaFisica.x / tileSize));
    int ultimoTileZ = (int)(round(bombaFisica.z / tileSize));

    int tileOriginalX = ultimoTileX;
    int tileOriginalZ = ultimoTileZ;

    while (stepCount < maxSteps)
    {
        stepCount++;

        ///<Calcular la nueva posición de la bomba
        int nuevoTileX = ultimoTileX + (int)direccionEnLaQueChoca.x;
        int nuevoTileZ = ultimoTileZ + (int)direccionEnLaQueChoca.z;

        ///<Comprobar las colisiones con los bloques irrmpibles
        bool colisiona = false;
        for (const auto &pos : posicionesIrrompibles)
        {
            int irrompibleTileX = (int)(round(pos.x / tileSize));
            int irrompibleTileZ = (int)(round(pos.z / tileSize));
            if (nuevoTileX == irrompibleTileX && nuevoTileZ == irrompibleTileZ)
            {
                colisiona = true;
                break;
            }
        }

        ///<Comprobar las colisiones con los bloques destruibles
        for (const auto &bloque : bloquesDestruibles)
        {
            auto &bloqueFisica = em.fisicas.get(bloque.fisica_key);
            int bloqueTileX = (int)(round(bloqueFisica.x / tileSize));
            int bloqueTileZ = (int)(round(bloqueFisica.z / tileSize));
            if (nuevoTileX == bloqueTileX && nuevoTileZ == bloqueTileZ)
            {
                colisiona = true;
                break;
            }
        }

        ///<Comprobar las colisiones con otras bomas
        for (const auto &otraBomba : otrasBombas)
        {
            if (otraBomba.id == bomba.id)
                continue;
            auto &otraBombaFisica = em.fisicas.get(otraBomba.fisica_key);
            int otraBombaTileX = (int)(round(otraBombaFisica.x / tileSize));
            int otraBombaTileZ = (int)(round(otraBombaFisica.z / tileSize));
            if (nuevoTileX == otraBombaTileX && nuevoTileZ == otraBombaTileZ)
            {
                colisiona = true;
                break;
            }
        }

        if (colisiona)
        {
            break;
        }

        ultimoTileX = nuevoTileX;
        ultimoTileZ = nuevoTileZ;
    }

    ///<Muover la bomba a su nueva posición
    bombaFisica.x = ultimoTileX * tileSize;
    bombaFisica.z = ultimoTileZ * tileSize;
    bombaFisica.updateBoundingBox();


    bombaFisica.tileX = (int)(round(bombaFisica.x / tileSize));
    bombaFisica.tileZ = (int)(round(bombaFisica.z / tileSize));

    ///<Cambio el 7 del mapa (Significa que en ese tile hay bomba) donde estaba la bomba al pricipio por un 0 (vacío) y pongo el 7 en la nueva posición
    mapSystem.setTile(tileOriginalX, tileOriginalZ, 0);
    mapSystem.setTile( bombaFisica.tileX, bombaFisica.tileZ, 7);
}