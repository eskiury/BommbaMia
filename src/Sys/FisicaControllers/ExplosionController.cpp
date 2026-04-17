/**
 * @file ExplosionController.cpp
 * @brief Implementación del controlador de explosiones del juego.
 *
 * Este archivo define la clase ExplosionController, responsable de gestionar 
 * las explosiones generadas por las bombas y su interacción con el entorno y entidades.
 *  
 * @author AlmondStudio
 * @date 2025-05-21
 * @version 1.0
 */

#include "ExplosionController.h"
#include "MapSystem.h"
#include "../utils/Const.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "../../ENGINES/GraphicEngine/motorGrafico.h"


/**
 * @brief Maneja la explosión de una bomba y su efecto en bloques destructibles.
 * 
 * Recorre todas las bombas en el mapa, y si una bomba está en estado de explosión,
 * calcula qué bloques son afectados, los elimina y actualiza el sistema de IA y motor gráfico.
 * 
 * @param em Referencia al EntityManager.
 * @param mapSystem Referencia al sistema de mapa (MapSystem).
 */
void ExplosionController::bombaBloqueRompible(EntityManager &em,  MapSystem &mapSystem){
    const auto &mapMatrix = mapSystem.getMap();
    std::vector<Entity> bombas = em.getEntitiesByType(EntityType::BOMBA);
    std::vector<Entity> bloquesAEliminar;

    for (Entity &bomba : bombas){
        auto &fisicaBomba = em.fisicas.get(bomba.fisica_key);
        auto &bombBomba = em.bombs.get(bomba.bomb_key);

        if (bombBomba.isExploding && !bombBomba.explosionPropagated){
            bombBomba.explosionPropagated = true;

            ///<Obtener bloques afectados por la explosión
            std::vector<Entity> bloques = calcularExplosion(em, fisicaBomba, bombBomba, mapSystem);
            bloquesAEliminar.insert(bloquesAEliminar.end(), bloques.begin(), bloques.end());
        }
    }

    ///<Procesar bloques eliminados
    for (Entity &bloque : bloquesAEliminar){
        auto &fisicaBloque = em.fisicas.get(bloque.fisica_key);
        auto &render = em.renders.get(bloque.render_key);

        ///<Convertir coordenadas del bloque en índices de la matriz
        int tileX = static_cast<int>(fisicaBloque.x / 2.0f);
        int tileZ = static_cast<int>(fisicaBloque.z / 2.0f);

        ///<Verificar si hay un power-up asociado en la matriz
        if (tileZ >= 0 && tileZ < mapMatrix.size() && tileX >= 0 && tileX < mapMatrix[0].size()){
            
            int tileValue = mapMatrix[tileZ][tileX];
            procesarPowerUp(tileValue, tileX, tileZ, fisicaBloque, em, mapSystem);
        }

        ///<Lo borro del arbol del motor grafico
        ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
        motorRaylib.getMotorDeJuego().EliminarPersonaje(render.SusDatosEnElArbol);
        ///< Eliminar el bloque de la entidad manager
        em.eraseEntity(bloque.id);
        iaSystem->agregarNodo(tileZ, tileX);
    }
}


/**
 * @brief Procesa la aparición de un power-up tras la destrucción de un bloque.
 *
 * Si el bloque eliminado contiene un valor mayor o igual a 100, se genera un power-up en su lugar.
 * También se actualiza la matriz del mapa y el grafo de influencia de la IA.
 * 
 * @param tileValue Valor del tile en la matriz (>=100 indica un power-up).
 * @param tileX Coordenada X del tile.
 * @param tileZ Coordenada Z del tile.
 * @param fisicaBloque Componente de física del bloque destruido.
 * @param em Referencia al EntityManager.
 * @param mapSystem Referencia al sistema de mapa.
 */
void ExplosionController::procesarPowerUp(int tileValue, int tileX, int tileZ, const fisicaComponent &fisicaBloque, EntityManager &em, MapSystem &mapSystem)
{
    if (tileValue >= 100)
    {
        int powerUpType = tileValue - 100;

        ///<Creamos la entidad POWERUP
        Entity &powerUp = em.createEntity(EntityType::POWERUP);
        fisicaComponent fisicaPowerUp{fisicaBloque.x, fisicaBloque.y, fisicaBloque.z};
        powerUpComponent powerUpComp{static_cast<PowerUpType>(powerUpType)};
        renderComponent renderPowerUp{1.0f, MiColor{60, 30, 255, 255}};
        soundComponent soundPowerUp{};
        bombaComponent bombaPowerUp{};
        iaComponent pwuptieneBT{false};
        buttonComponent buttonPowerUp{};
        em.createEntityComponents(powerUp, fisicaPowerUp, renderPowerUp, powerUpComp, soundPowerUp, bombaPowerUp, pwuptieneBT, buttonPowerUp);

        ///<Actualizar la matriz del mapa con el tipo de power-up
        iaSystem->getGrafo().actualizarZonaInfluencia(tileZ, tileX, mapSystem.getMap(), true);
        mapSystem.setTile(tileX, tileZ, powerUpType);
    }
    else
    {
        ///<Actualizar la matriz del mapa como bloque vacío
        mapSystem.setTile(tileX, tileZ, 0);
    }
}


/**
 * @brief Verifica si la explosión colisiona con un bloque irrompible.
 * 
 * Convierte coordenadas de tiles y las compara con posiciones de cubos irrompibles.
 * 
 * @param checkX Coordenada X a verificar.
 * @param checkZ Coordenada Z a verificar.
 * @param posicionesIrrompibles Lista de posiciones de cubos irrompibles.
 * @param tileSize Tamaño de cada tile en unidades del juego.
 * 
 * @return true Si hay colisión con un bloque irrompible.
 * @return false Si no hay colisión.
 */
bool ExplosionController::calcularExplosionConIrrompible(int checkX, int checkZ, const std::vector<MiVector3> &posicionesIrrompibles, float tileSize)
{
    for (const MiVector3 &pos : posicionesIrrompibles)
    {
        int irrompibleTileX = (int)(round(pos.x / tileSize));
        int irrompibleTileZ = (int)(round(pos.z / tileSize));
        if (checkX == irrompibleTileX && checkZ == irrompibleTileZ)
        {
            return true;
        }
    }
    return false;
}



/**
 * @brief Verifica si la explosión colisiona con un bloque destruible.
 * 
 * Si se detecta colisión, se agrega el bloque a la lista de eliminación y se detiene la propagación.
 * 
 * @param checkX Coordenada X a verificar.
 * @param checkZ Coordenada Z a verificar.
 * @param bloquesDestruibles Lista de bloques destruibles en el mapa.
 * @param bloquesAEliminar Lista donde se agregarán los bloques a eliminar.
 * @param em Referencia al EntityManager.
 * @param tileSize Tamaño del tile.
 * @param bombaPOS Componente de bomba de la entidad que explota.
 * 
 * @return true Si se colisiona con un bloque destruible.
 * @return false En caso contrario.
 */
bool ExplosionController::calcularExplosionConDestruible(int checkX, int checkZ, std::vector<Entity> &bloquesDestruibles, std::vector<Entity> &bloquesAEliminar, EntityManager &em, float tileSize, bombaComponent &bombaPOS)
{
    for (Entity &destruible : bloquesDestruibles)
    {
        auto &physBloque = em.fisicas.get(destruible.fisica_key);
        int destruibleTileX = (int)(round(physBloque.x / tileSize));
        int destruibleTileZ = (int)(round(physBloque.z / tileSize));
        
        if (checkX == destruibleTileX && checkZ == destruibleTileZ)
        {
            ///<Añadimos el rastro de explosión a la bomba correspondiente
            bombaPOS.explosionRastros.push_back({bombaPOS.explosionDuration, checkX, checkZ});
            
            ///<Agregamos el bloque destruible a la lista de eliminación
            bloquesAEliminar.push_back(destruible);
            
            return true; ///<Bloque destruible encontrado, detener propagación
        }
    }
    return false;
}


/**
 * @brief Actualiza los rastros de explosión, eliminando aquellos cuya duración ha expirado.
 * 
 * Limpia las casillas del mapa donde los rastros ya han terminado.
 * 
 * @param deltaTime Tiempo transcurrido desde el último frame.
 * @param em Referencia al EntityManager.
 * @param mapSystem Referencia al sistema de mapa.
 */
void ExplosionController::actualizarExplosionRastro(float deltaTime, EntityManager &em, MapSystem &mapSystem) {
    for (Entity &bomba : em.getEntitiesByType(EntityType::BOMBA)) {
        auto &bombaComp = em.bombs.get(bomba.bomb_key);

        bombaComp.explosionRastros.erase(
            std::remove_if(
                bombaComp.explosionRastros.begin(),
                bombaComp.explosionRastros.end(),
                [this, &mapSystem, deltaTime](ExplosionRastro &rastro) {
                    rastro.remainingTime -= deltaTime;
                    if (rastro.remainingTime <= 0) {
                        if(mapSystem.getTile(rastro.tileX, rastro.tileZ) < 10){
                            iaSystem->getGrafo().actualizarZonaInfluencia(rastro.tileZ, rastro.tileX, mapSystem.getMap(), false);
                            mapSystem.setTile(rastro.tileX, rastro.tileZ, 0); ///< Restaurar casilla a 0
                            return true;
                        }
                    }
                    return false;
                }),
            bombaComp.explosionRastros.end());
    }
}


/**
 * @brief Verifica colisiones entre personajes (jugador o NPC) y rastros de explosión.
 * 
 * Si un personaje colisiona con un rastro, se elimina (vida = -2) y se limpian los rastros.
 * 
 * @param em Referencia al EntityManager.
 * @param mapSystem Referencia al sistema de mapa.
 */
void ExplosionController::collisionPersonajeRastroExplosion(EntityManager &em, MapSystem &mapSystem) {
    for (Entity &bomba : em.getEntitiesByType(EntityType::BOMBA)) {
        auto &bombaComp = em.bombs.get(bomba.bomb_key);

        for (const auto &rastro : bombaComp.explosionRastros) {
            for (Entity &entidad : em.getEntities()) {
                if (entidad.type == EntityType::PLAYER || entidad.type == EntityType::NPC) {
                    auto &physEntidad = em.fisicas.get(entidad.fisica_key);
                    int entidadTileX = static_cast<int>(round(physEntidad.x / 2.0f));
                    int entidadTileZ = static_cast<int>(round(physEntidad.z / 2.0f));

                    if (rastro.tileX == entidadTileX && rastro.tileZ == entidadTileZ) {
                        for (const auto &r : bombaComp.explosionRastros) {
                            mapSystem.setTile(r.tileX, r.tileZ, 0); ///<Limpiamos cada casilla afectada
                        }
                        physEntidad.vida = -2.0f; ///<Matamos al personaje
                        return;
                    }
                }
            }
        }
    }
}


/**
 * @brief Obtiene las posiciones de los cubos irrompibles del mapa.
 *
 * Recorre la matriz del mapa para identificar los bloques no destructibles.
 *
 * @param mapSystem Referencia al sistema de mapa.
 * @return Vector con las posiciones de los cubos.
 */
std::vector<MiVector3> ExplosionController::getPosicionesCubos(MapSystem &mapSystem){
    const auto &mapMatrix = mapSystem.getMap();
    std::vector<MiVector3> posiciones;

    float tileSize = 2.0f;

    ///< Recorrer la matriz para obtener posiciones de cubos (valor 1)
    for (int i = 0; i < mapMatrix.size(); i++) ///<filas
    {
        for (int j = 0; j < mapMatrix[i].size(); j++) ///<columnas
        {
            if (mapMatrix[i][j] == 1) ///<si el cubo es un cubo no destruible
            {
                MiVector3 cubePos = {j * tileSize, 0.0f, i * tileSize};
                posiciones.push_back(cubePos);
            }
        }
    }

    return posiciones;
}


/**
 * @brief Calcula los bloques afectados por la explosión de una bomba.
 * 
 * Propaga la explosión en las 4 direcciones hasta el rango máximo o hasta colisionar.
 * Afecta a bloques y bombas 
 * Actualiza el mapa con fuego.
 * 
 * @param em Referencia al EntityManager.
 * @param bomba Componente de física de la bomba.
 * @param bombaComp Componente de bomba con los datos de la explosión.
 * @param mapSystem Referencia al sistema de mapa.
 * 
 * @return std::vector<Entity> Lista de bloques a eliminar por la explosión.
 */
std::vector<Entity> ExplosionController::calcularExplosion(EntityManager &em, fisicaComponent &bomba, bombaComponent &bombaComp, MapSystem &mapSystem) {
    
    std::vector<Entity> bloquesAEliminar;
    std::vector<MiVector3> posicionesIrrompibles = getPosicionesCubos(mapSystem);
    std::vector<Entity> bloquesDestruibles = em.getEntitiesByType(EntityType::DESTRUIBLE);

    auto entidadbomba = em.getEntity(bombaComp.ownerId);
    auto &bombaCMPentidad = em.bombs.get(entidadbomba.bomb_key);

    float tileSize = 2.0f;
    int range = bombaComp.expRange;

    //Añado el tile donde coloco la bomba a la lista de explosionRastros.
    bombaComp.explosionRastros.push_back({bombaComp.explosionDuration, bomba.tileX, bomba.tileZ});

    std::vector<std::pair<int, int>> direcciones = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (auto [dx, dz] : direcciones) {
        for (int step = 1; step <= range; step++) {
            int checkX = (int)round(bomba.x / tileSize) + dx * step;
            int checkZ = (int)round(bomba.z / tileSize) + dz * step;

            ///< Verificar colisiones
            if (calcularExplosionConIrrompible(checkX, checkZ, posicionesIrrompibles, tileSize)) break;
            if (calcularExplosionConDestruible(checkX, checkZ, bloquesDestruibles, bloquesAEliminar, em, tileSize, bombaComp)) break;
            if(calcularExplosionConBomba(checkX, checkZ, em, tileSize)) break;

            ///< Si no hay colisión, añadir rastro a la bomba actual
            bombaComp.explosionRastros.push_back({bombaComp.explosionDuration, checkX, checkZ});

            if(mapSystem.getTile(checkX, checkZ) < 10){
                iaSystem->getGrafo().actualizarZonaInfluencia(checkZ,checkX, mapSystem.getMap(), true);
                mapSystem.setTile(checkX, checkZ, FUEGO);                
                mapSystem.setTileColor(checkX, checkZ, bombaCMPentidad.pinturaId);
            }
            
        }
    }

    return bloquesAEliminar;
}


/**
 * @brief Verifica si la explosión alcanza otra bomba.
 * 
 * Si una bomba se encuentra en la zona de explosión, se detonará también.
 * 
 * @param checkX Coordenada X de la explosión.
 * @param checkZ Coordenada Z de la explosión.
 * @param em Referencia al EntityManager.
 * @param tileSize Tamaño del tile.
 * 
 * @return true Si hay una bomba que debe explotar.
 * @return false En caso contrario.
 */
bool ExplosionController::calcularExplosionConBomba(int checkX, int checkZ, EntityManager &em, float tileSize){
    
    std::vector<Entity> bombas = em.getEntitiesByType(EntityType::BOMBA);
    for (Entity &bombaEntidad : bombas){
        auto &physBomba = em.fisicas.get(bombaEntidad.fisica_key);
        auto &bombBomba = em.bombs.get(bombaEntidad.bomb_key);
        int bombaTileX = (int)(round(physBomba.x / tileSize));
        int bombaTileZ = (int)(round(physBomba.z / tileSize));

        if (checkX == bombaTileX && checkZ == bombaTileZ){
            if (!bombBomba.isExploding){
                bombBomba.isExploding = true;
                bombBomba.lifetime = bombBomba.explosionDuration;
            }
            return true;
        }
    }
    return false;
}

/**
 * @brief Verifica si una explosión alcanza a un personaje o un rastro de fuego.
 * 
 * Evalúa si una casilla en la que hay un personaje hay también rastro de explosión activo.
 * 
 * @param checkX Coordenada X.
 * @param checkZ Coordenada Z.
 * @param em Referencia al EntityManager.
 * @param tileSize Tamaño del tile.
 * 
 * @return true Si hay colisión.
 * @return false Si no hay colisión.
 */
bool ExplosionController::calcularExplosionConPersonajes(int checkX, int checkZ, EntityManager &em, float tileSize) {
    for (Entity &bomba : em.getEntitiesByType(EntityType::BOMBA)) {
        auto &bombaComp = em.bombs.get(bomba.bomb_key);

        for (const auto &rastro : bombaComp.explosionRastros) {
            if (rastro.tileX == checkX && rastro.tileZ == checkZ) {
                return true; ///<Colisión con rastro de explosión
            }
        }
    }
    return false; ///<No colisión
}


/**
 * @brief Guarda una referencia al sistema de IA para poder modificar su grafo.
 * 
 * @param ia Referencia al sistema de IA.
 */
void ExplosionController::saveIaSystem(IASystem &ia)
{
    this->iaSystem = &ia;
}