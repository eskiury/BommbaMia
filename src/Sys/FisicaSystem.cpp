/**
 * @file FisicaSystem.cpp
 * @brief Declaración de la clase FisicaSystem, encargada de gestionar la física del juego.
 *
 * Este archivo define la clase FisicaSystem, responsable de controlar el movimiento, colisiones, explosiones
 * y otros aspectos físicos de las entidades del juego.
 *
 * @author AlmondStudio
 * @date 2025-05-21
 * @version 1.0
 */

#include "FisicaSystem.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "MapSystem.h"
#include "GameplaySystem.h"
#include "../utils/Const.h"
#include "../ENGINES/GraphicEngine/motorGrafico.h"
#include "InputSystem.h"


/**
 * @brief Constructor del sistema de física.
 *
 * Inicializa los controladores de movimiento, colisión y explosión.
 */
FisicaSystem::FisicaSystem() : collisionController(), movementController(), explosionController() {}

void FisicaSystem::init(EntityManager &em, MapSystem &mapSystem){
    // this->players = em.getEntitiesByTypePunteros(EntityType::PLAYER);
    this->mapSystem = &mapSystem;
}



/**
 * @brief Actualiza todas las físicas.
 *
 * Se encarga del movimiento de jugadores y NPCs, detección de colisiones, activación de explosiones...
 *
 * @param em EntityManager.
 * @param inputSystem Sistema de entrada.
 * @param gameplaySystem Sistema de lógica del gameplay.
 * @param players Vector con los jugadores (players) activos.
 */
void FisicaSystem::update(EntityManager &em, InputSystem &inputSystem, GameplaySystem &gameplaySystem, std::vector<Entity>& players){

    for(Entity& player : players){
        fisicaComponent &fisicaPlayer = em.fisicas.get(player.fisica_key);
        inputSystem.actualizarInputMap(player);
        const auto &inputMap = inputSystem.getInputMap(player);
        if(!fisicaPlayer.esFantasma){
            ///< mover al player
            if(!fisicaPlayer.esta_estuneado){
                movementController.moverEntityPorTeclado(player, inputMap, em, collisionController, *mapSystem);
            }
        }
        else{
            movementController.moverFoco(player, inputMap, em, collisionController, *mapSystem);
            if(fisicaPlayer.cargaCompletada){
                atacarFoco(em, player);
            }
        }
    }
    movementController.moverNPC(em);

    collisionController.recogerPowerUp(em, gameplaySystem, *mapSystem, players);

    explosionController.bombaBloqueRompible(em, *mapSystem);
    explosionController.collisionPersonajeRastroExplosion(em, *mapSystem);

    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
    float tiempo = motorRaylib.getFixedDeltaTime();

    delayFoco(em, tiempo);
    actualizarEstuneo(em, tiempo); //////////////////////////////////////
    invulnerabilidad(em, tiempo);

    explosionController.actualizarExplosionRastro(tiempo, em, *mapSystem);

}


/**
 * @brief Estunea al personaje que tiene como objetivo.
 *
 * Verifica la colisión entre el foco y otros NPCs o jugadores y si coinciden y el jugador
 * a estunear no tiene invulnarabilidad, le estunea.
 *
 * @param em EntityManager.
 * @param npc npc que esta usando el ataque de estunear.
 */
 void FisicaSystem::atacarFoco(EntityManager &em, Entity& npc) {
    ///< Obtener los NPCs y Players
    const auto& npcs = em.getEntitiesByType(EntityType::NPC);
    auto players = em.getEntitiesByType(EntityType::PLAYER);

    auto& fisicaFoco = em.fisicas.get(npc.fisica_key);

    ///< Comparar con otros NPCs
    for (const auto& otroNPC : npcs) {
        if (npc.id != otroNPC.id) {
            auto& fisicaOtro = em.fisicas.get(otroNPC.fisica_key);
            if (fisicaOtro.esFantasma==false  &&  fisicaOtro.invulnerabilidad == 0  &&  collisionController.checkBoxesCollision(fisicaFoco.boundingBox, fisicaOtro.boundingBox)) {
                fisicaOtro.esta_estuneado = true;
                fisicaOtro.tiempoEstuneado = 0.0f;
                fisicaOtro.invulnerabilidad = 8.0f;
            }
        }
    }

    ///< Comparar con jugadores
    for (const auto& player : players) {
        auto& fisicaPlayer = em.fisicas.get(player.fisica_key);
        if (fisicaPlayer.esFantasma==false  &&  fisicaPlayer.invulnerabilidad == 0  &&  collisionController.checkBoxesCollision(fisicaFoco.boundingBox, fisicaPlayer.boundingBox)) {
            fisicaPlayer.esta_estuneado = true;
            fisicaPlayer.tiempoEstuneado = 0.0f;
            fisicaPlayer.invulnerabilidad = 8.0f;
        }
    }
}


/**
 * @brief Controla el tiempo de carga del ataque de estuneo de los NPCs.
 *
 * Este método recorre todos los NPCs y, si alguno está en estado de estuneo,
 * incrementa su tiempo de carga. Si el tiempo de carga alcanza su máximo,
 * significa que la carga se ha completado y se ejecuta el ataque de estunear.
 *
 * @param em EntityManager.
 * @param tiempo Tiempo transcurrido desde la última actualización (deltaTime).
 */
void FisicaSystem::delayFoco(EntityManager &em, float tiempo){

    auto npcs = em.getEntitiesByType(EntityType::NPC);

    for(auto& npc : npcs){
        auto &fisica = em.fisicas.get(npc.fisica_key);
        if(fisica.estuneo){
            fisica.tiempoCarga += tiempo;
            if (fisica.tiempoCarga >= 1.0f){
                    fisica.cargaCompletada = true;

                if (fisica.cargaCompletada){
                    atacarFoco(em, npc);
                    if (fisica.tiempoCarga >= 1.5f){
                        fisica.tiempoCarga = 0.0f;
                        fisica.cargaCompletada = false;
                        fisica.estuneo = false;
                    }
                }
            }
        }
    }
}


/**
 * @brief Actualiza el estado de estuneo de los NPCs.
 *
 * Si un NPC está estuneado, incrementa su tiempo hasta que alcanza mas de 1.5 segundos y se desestunea.
 *
 * @param em EntityManager.
 * @param tiempo Delta de tiempo fijo del motor gráfico.
 */
 void FisicaSystem::actualizarEstuneo(EntityManager& em, float tiempo){

    for (auto& npc : em.getAllEntitiesInGame()) {
        auto &fisica = em.fisicas.get(npc.fisica_key);

        if (fisica.esta_estuneado) {
            fisica.tiempoEstuneado += tiempo;

            if (fisica.tiempoEstuneado >= 1.5f) {
                fisica.esta_estuneado = false;
                fisica.tiempoEstuneado = 0.0f;
            }
        }
    }
}



/**
 * @brief Actualiza el tiempo de invulnerabilidad de todos los personajes.
 *
 * Reduce el tiempo de invulnarabilidad de los jugadores que tienen invulnerabilidad porque les han estuneado hace poco
 * Cuando el tiempo llega a cero, se desactiva la invulnerabilidad.
 *
 * @param em EntityManager.
 * @param tiempo Tiempo transcurrido desde la última actualización (deltaTime).
 */
void FisicaSystem::invulnerabilidad(EntityManager& em, float tiempo){

    for(auto& personaje : em.getAllEntitiesInGame()){
        auto &fisicaPersonaje = em.fisicas.get(personaje.fisica_key);

        if(fisicaPersonaje.invulnerabilidad > 0.0f){
            fisicaPersonaje.invulnerabilidad -= tiempo;
            if(fisicaPersonaje.invulnerabilidad <= 0.0f){
                fisicaPersonaje.invulnerabilidad = 0.0f;
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
std::vector<MiVector3> FisicaSystem::getPosicionesCubos(MapSystem &mapSystem){
    const auto &mapMatrix = mapSystem.getMap();
    std::vector<MiVector3> posiciones;

    float tileSize = 2.0f;

    ///<Recorro la matriz para obtener posiciones de cubos (valor 1)
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
 * @brief Genera las coordenadas de toda la cuadrícula del mapa.
 *
 * Crea un vector con la posición centrada de cada tile del mapa.
 *
 * @return Vector de posiciones que representan los centros de los tiles.
 */
std::vector<MiVector3> FisicaSystem::getGridCoordinates(){

    float tileSize = 2.0f;
    int gridWidth = 13;
    int gridHeight = 11;

    float startX = 1.0f;
    float startZ = 1.0f;

    std::vector<MiVector3> gridCoordinates;

    for (int z = 0; z < gridHeight; z++)
    {

        for (int x = 0; x < gridWidth; x++)
        {
            float coordX = startX + (x * tileSize) + (tileSize * 0.5f);
            float coordZ = startZ + (z * tileSize) + (tileSize * 0.5f);
            gridCoordinates.push_back(MiVector3{coordX, 0.0f, coordZ});
        }
    }

    return gridCoordinates;
}


/**
 * @brief Devuelve todos los rastros de explosión activos en el juego.
 *
 * Recorre las bombas activas y recolecta sus rastros de explosión actuales.
 *
 * @param em EntityManager.
 * @return Vector con todos los rastros de explosión del juego.
 */
std::vector<ExplosionRastro> FisicaSystem::getExplosionRastros(EntityManager &em){
    std::vector<ExplosionRastro> todosLosRastros;

    for (const auto &bomba : em.getEntitiesByType(EntityType::BOMBA))
    {
        const auto &bombaComp = em.bombs.get(bomba.bomb_key);
        todosLosRastros.insert(todosLosRastros.end(), bombaComp.explosionRastros.begin(), bombaComp.explosionRastros.end());
    }

    return todosLosRastros;
}


/**
 * @brief Guarda una referencia al sistema de IA.
 *
 * Permite a los controladores acceder al sistema de IA para tomar decisiones durante explosiones y colisiones.
 *
 * @param iaSystem Referencia al sistema de IA.
 */
void FisicaSystem::saveIaSystem(IASystem &iaSystem)
{
    explosionController.saveIaSystem(iaSystem);
    collisionController.saveIaSystem(iaSystem);
}
