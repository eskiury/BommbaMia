/**
 * @file MovementController.cpp
 * @brief Implementación del controlador de movimiento para entidades del juego.
 *
 * Este archivo define la clase MovementController, encargada de manejar el movimiento
 * de entidades como el jugador y NPCs, procesando la entrada de teclado, 
 * controlando colisiones y actualizando posiciones y animaciones.
 *  
 * @author AlmondStudio
 * @date 2025-05-21
 * @version 1.0
 */

#include "MovementController.h"
#include "CollisionController.h"
#include "MapSystem.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>



/**
 * @brief Mueve al Player por teclado (Arriba, abajo, izquierda, derecha)
 * 
 * @param player Entidad que se quiere manejar por teclado
 * @param inputMap Mapa con el estado de las teclas (true/false)
 * @param em EntityManager
 * @param collisionController Controlador de colisiones
 * @param mapSystem Sistema del mapa
 */
void MovementController::moverEntityPorTeclado(Entity& player, const std::unordered_map<std::string, bool>& inputMap, EntityManager &em, CollisionController &collisionController, MapSystem &mapSystem){

    auto &fisicaPlayer = em.fisicas.get(player.fisica_key);
    auto &render = em.renders.get(player.render_key);

    fisicaPlayer.nextX = fisicaPlayer.x;
    fisicaPlayer.nextZ = fisicaPlayer.z;

    bool moveRight = inputMap.at("moveRight");
    bool moveLeft = inputMap.at("moveLeft");
    bool moveDown = inputMap.at("moveDown");
    bool moveUp = inputMap.at("moveUp");
    
    bool moviendoX = moveRight || moveLeft;
    bool moviendoZ = moveDown || moveUp;
    bool hayInput = moviendoX || moviendoZ;

    ///<Eje X
    if (moviendoX) {
        ///<para cambiar la animacion
        if(render.animacion != 3){
            render.animacion = 3;
            render.animacionYaIniciada = false;
            render.colocandoBombaAnimacion = false;
        }
        //-----------------
        if (moveRight) {
            if (fisicaPlayer.vx < 0){
                fisicaPlayer.vx = std::min(fisicaPlayer.maxSpeed, -fisicaPlayer.vx);
            }
            fisicaPlayer.vx = std::min(fisicaPlayer.vx + fisicaPlayer.ax, fisicaPlayer.maxSpeed);
            render.rotacion = 90.0f;
        } 
        else if (moveLeft) {
            if (fisicaPlayer.vx > 0){
                fisicaPlayer.vx = std::max(-fisicaPlayer.maxSpeed, -fisicaPlayer.vx);
            }
            fisicaPlayer.vx = std::max(fisicaPlayer.vx - fisicaPlayer.ax, -fisicaPlayer.maxSpeed);
            render.rotacion = 270.0f;
        }
    } 
    else {
        fisicaPlayer.vx = 0.0f;
    }

    ///<Eje Z
    if (moviendoZ) {
        ///<para cambiar la animacion
        if(render.animacion != 3){
            render.animacion = 3;
            render.animacionYaIniciada = false;
            render.colocandoBombaAnimacion = false;
        }
        //-----------------
        if (moveDown) {
            if (fisicaPlayer.vz < 0){
                fisicaPlayer.vz = std::min(fisicaPlayer.maxSpeed, -fisicaPlayer.vz);
            }
            fisicaPlayer.vz = std::min(fisicaPlayer.vz + fisicaPlayer.az, fisicaPlayer.maxSpeed);
            render.rotacion = 0.0f;
        } 
        else if (moveUp) {
            if (fisicaPlayer.vz > 0){
                fisicaPlayer.vz = std::max(-fisicaPlayer.maxSpeed, -fisicaPlayer.vz);
            }
            fisicaPlayer.vz = std::max(fisicaPlayer.vz - fisicaPlayer.az, -fisicaPlayer.maxSpeed);
            render.rotacion = 180.0f;
        }
    } 
    else { 
        fisicaPlayer.vz = 0.0f;
    }

    if(!moviendoZ && !moviendoX){
        if(render.animacion != 1 && render.colocandoBombaAnimacion == false){
            render.animacion = 1;
            render.animacionYaIniciada = false;
        }
    }

    ///<aplicar movimiento
    float oldNextX = fisicaPlayer.nextX;
    fisicaPlayer.nextX += fisicaPlayer.vx;
    fisicaPlayer.updateSiguienteBB();

    bool colisionX = collisionController.collisionConBloques(player, em, mapSystem);
    ///<si colisiona en el eje x
    if (colisionX){
        fisicaPlayer.nextX = oldNextX;
        fisicaPlayer.chocaX = true; ///<para el debugger visual de las fisicas
        if (!hayInput){
            fisicaPlayer.vx = 0.0f; ///<lo detenego si no hay más input
        }
    }
    else{
        fisicaPlayer.chocaX = false;
    }
    fisicaPlayer.updateBoundingBox();
    if(fisicaPlayer.chocaX){
        desplazarMovimientoVertical(em, player, moveLeft, moveRight, moveUp, moveDown, collisionController, mapSystem);
    }

    float oldNextZ = fisicaPlayer.nextZ;
    fisicaPlayer.nextZ += fisicaPlayer.vz;
    fisicaPlayer.updateSiguienteBB();

    bool colisionZ = collisionController.collisionConBloques(player, em, mapSystem);
    ///<si colisiona en el eje x
    if (colisionZ) {
        fisicaPlayer.nextZ = oldNextZ;
        fisicaPlayer.chocaZ = true; ///<para el debugger visual de las fisicas
        if (!hayInput){
            fisicaPlayer.vz = 0.0f; ///<lo detenego si no hay más input
        }
    }else{
        fisicaPlayer.chocaZ = false;
    }

    fisicaPlayer.updateBoundingBox();
    if(fisicaPlayer.chocaZ){
        desplazarMovimientoLateral(em, player, moveLeft, moveRight, moveUp, moveDown, collisionController, mapSystem);
    }


    moverEntity(em, player);
}


/**
 * @brief Actualiza la posición del Player según nextX y nextZ.
 * 
 * @param em EntityManager
 * @param e Entidad a mover
 */
void MovementController::moverEntity(EntityManager &em, Entity &e) {
    auto &jugador = em.fisicas.get(e.fisica_key);

    jugador.x = jugador.nextX;
    jugador.z = jugador.nextZ;

    jugador.updateBoundingBox();
}


/**
 * @brief Ajusta el movimiento vertical del Player cuando choca (divide la siguienteBB en mitades arriba/abajo).
 * 
 * Cuando el jugador choca, se divide la siguienteBB en dos mitades (arriba y abajo) y se comprueba
 * Si choca con la mitad superior, el player se mueve hacia abajo y viceversa.
 * 
 * @param em EntityManager
 * @param e Entidad que se mueve
 * @param moveLeft Indica si se mueve a la izquierda
 * @param moveRight Indica si se mueve a la derecha
 * @param moveUp Indica si se mueve hacia arriba
 * @param moveDown Indica si se mueve hacia abajo
 * @param collisionController Controlador de colisiones
 * @param mapSystem Sistema del mapa
 */
void MovementController::desplazarMovimientoVertical(EntityManager& em, Entity& e, bool moveLeft, bool moveRight, bool moveUp, bool moveDown, CollisionController &collisionController, MapSystem &mapSystem){
    auto &fisica = em.fisicas.get(e.fisica_key);

    ///<calcular el punto medio de z
    float midZ = (fisica.siguienteBB.min.z + fisica.siguienteBB.max.z) / 2.0f;

    ///< Mitad superior: desde midZ hasta playerBB.max.z
    BoundingBOX mitadArriba = fisica.siguienteBB;
    mitadArriba.max.z = midZ;  

    ///< Mitad inferior: desde playerBB.min.z hasta midZ
    BoundingBOX mitadAbajo = fisica.siguienteBB;
    mitadAbajo.min.z = midZ;

    ///<comprobar las colisiones en cada mitad
    bool colisionArriba = collisionController.collisionConBloques(mitadArriba, em, mapSystem);
    bool colisionAbajo  = collisionController.collisionConBloques(mitadAbajo, em, mapSystem);

    ///<solo choca la mitad de arriba
    if (colisionArriba && !colisionAbajo) {
        ///<movemos el jugador hacia abajo
        if(moveRight && (!moveUp && !moveDown)){
            fisica.nextZ += fisica.vx;
            fisica.updateSiguienteBB();
        }
        if(moveLeft && (!moveUp && !moveDown)){
            fisica.nextZ -= fisica.vx;
            fisica.updateSiguienteBB();
        }
        
    }
    ///<solo choca la mitad de abajo
    else if (colisionAbajo && !colisionArriba) {
        ///<movemos el jugador hacia arriba
        if(moveRight && (!moveUp && !moveDown)){
            fisica.nextZ -= fisica.vx;  
            fisica.updateSiguienteBB();
        }
        if(moveLeft && (!moveUp && !moveDown)){
            fisica.nextZ += fisica.vx;  
            fisica.updateSiguienteBB();
        }
        
    }
}



/**
 * @brief Ajusta el movimiento lateral del Player cuando choca (divide la siguienteBB en mitades izquierda/derecha).
 * 
 * Cuando el jugador choca, se divide la siguienteBB en dos mitades (izquierda y derecha) y se comprueba
 * Si choca con la mitad izquierda, el player se mueve hacia la derecha y viceversa.
 * 
 * @param em EntityManager
 * @param e Entidad que se mueve
 * @param moveLeft Indica si se mueve a la izquierda
 * @param moveRight Indica si se mueve a la derecha
 * @param moveUp Indica si se mueve hacia arriba
 * @param moveDown Indica si se mueve hacia abajo
 * @param collisionController Controlador de colisiones
 * @param mapSystem Sistema del mapa
 */
void MovementController::desplazarMovimientoLateral(EntityManager& em, Entity& e, bool moveLeft, bool moveRight, bool moveUp, bool moveDown, CollisionController &collisionController,  MapSystem &mapSystem){
    auto &fisica = em.fisicas.get(e.fisica_key);

    ///<calcular el punto medio en x
    float midX = (fisica.siguienteBB.min.x + fisica.siguienteBB.max.x) / 2.0f;

    ///< Mitad izquierda: desde min.x hasta midX
    BoundingBOX mitadIzquierda = fisica.siguienteBB;
    mitadIzquierda.max.x = midX;  

    ///< Mitad derecha: desde midX hasta max.x
    BoundingBOX mitadDerecha = fisica.siguienteBB;
    mitadDerecha.min.x = midX;

    ///<comprobar las colisiones en cada mitad
    bool colisionIzquierda = collisionController.collisionConBloques(mitadIzquierda, em, mapSystem);
    bool colisionDerecha   = collisionController.collisionConBloques(mitadDerecha,  em, mapSystem);

    ///<solo choca la mitad de la izquierda
    if (colisionIzquierda && !colisionDerecha) {
        ///<movemos el jugador hacia la derecha
        if(moveUp && (!moveLeft && !moveRight)){
            fisica.nextX -= fisica.vz;
            fisica.updateSiguienteBB();
        }
        if(moveDown && (!moveLeft && !moveRight)){
            fisica.nextX += fisica.vz;
            fisica.updateSiguienteBB();
        } 
    }
    ///<solo choca la mitad de la derecha
    else if (colisionDerecha && !colisionIzquierda) {
        ///<movemos el jugador hacia la izquierda
        if(moveUp && (!moveLeft && !moveRight)){
            fisica.nextX += fisica.vz;  
            fisica.updateSiguienteBB();
        }
        if(moveDown && (!moveLeft && !moveRight)){
            fisica.nextX -= fisica.vz;  
            fisica.updateSiguienteBB();
        }
    }
}


/**
 * @brief Mueve el foco o entidad actualizando velocidades y posiciones con input.
 * 
 * @param player Entidad a mover
 * @param inputMap Mapa con el estado de las teclas (true/false)
 * @param em EntityManager
 * @param collisionController Controlador de colisiones
 * @param mapSystem Sistema del mapa
 */
void MovementController::moverFoco(Entity& player, const std::unordered_map<std::string, bool> &inputMap, EntityManager &em, CollisionController &collisionController, MapSystem &mapSystem){
    
    auto &fisicaPlayer = em.fisicas.get(player.fisica_key);

    fisicaPlayer.nextX = fisicaPlayer.x;
    fisicaPlayer.nextZ = fisicaPlayer.z;

    bool moveRight = inputMap.at("moveRight");
    bool moveLeft = inputMap.at("moveLeft");
    bool moveDown = inputMap.at("moveDown");
    bool moveUp = inputMap.at("moveUp");

    bool moviendoX = moveRight || moveLeft;
    bool moviendoZ = moveDown  || moveUp;
    bool hayInput = moviendoX  || moviendoZ;


    if(moveRight){
        fisicaPlayer.vx = std::min(fisicaPlayer.vx + fisicaPlayer.ax, fisicaPlayer.maxSpeed);
    }
    else if(moveLeft){
        fisicaPlayer.vx = std::max(fisicaPlayer.vx - fisicaPlayer.ax, -fisicaPlayer.maxSpeed);
    }
    else { 
        if (fisicaPlayer.vx > 0) {
            fisicaPlayer.vx = std::max(0.0f, fisicaPlayer.vx - 0.0005f);
        } 
        else if (fisicaPlayer.vx < 0) {
            fisicaPlayer.vx = std::min(0.0f, fisicaPlayer.vx + 0.0005f);
        }
    }

    if(moveDown){
        fisicaPlayer.vz = std::min(fisicaPlayer.vz + fisicaPlayer.az, fisicaPlayer.maxSpeed);
    }
    else if(moveUp){
        fisicaPlayer.vz = std::max(fisicaPlayer.vz - fisicaPlayer.az, -fisicaPlayer.maxSpeed);
    }
    else { 
        ///< Desaceleración cuando NO hay input en Z
        if (fisicaPlayer.vz > 0) {
            fisicaPlayer.vz = std::max(0.0f, fisicaPlayer.vz - 0.0005f);
        } 
        else if (fisicaPlayer.vz < 0) {
            fisicaPlayer.vz = std::min(0.0f, fisicaPlayer.vz + 0.0005f);
        }
    }

    float oldNextX = fisicaPlayer.nextX;
    fisicaPlayer.nextX += fisicaPlayer.vx;
    fisicaPlayer.updateSiguienteBB();
    if(collisionController.collisionConBordes(em, player, mapSystem)){
        fisicaPlayer.nextX = oldNextX;
    }
    fisicaPlayer.updateBoundingBox();


    float oldNextZ = fisicaPlayer.nextZ;
    fisicaPlayer.nextZ += fisicaPlayer.vz;
    fisicaPlayer.updateSiguienteBB();
    if(collisionController.collisionConBordes(em, player, mapSystem)){
        fisicaPlayer.nextZ = oldNextZ;
    }
    fisicaPlayer.updateBoundingBox();


    moverEntity(em, player);
}


/**
 * @brief Indica si una posición está sobre un tile (casilla) permitida, con margen basado en velocidad.
 * 
 * @param position Posición actual
 * @param tileSize Tamaño del tile
 * @param velocity Velocidad o margen permitido
 * 
 * @return true Si está sobre el tile
 * @return false Si no está sobre el tile
 */
bool MovementController::isOnTileEnemy(float position, float tileSize, float velocity) {
    int tileIndex = static_cast<int>(position / tileSize);
    float exactTilePosition = tileIndex * tileSize;
    return std::abs(position - exactTilePosition) <= velocity; ///< Permitir un margen pequeño
}


/**
 * @brief Actualiza la posición del NPC basándose en las velocidades.
 * 
 * @param em EntityManager
 */
void MovementController::moverNPC(EntityManager &em)
{
    for(auto &npc : em.getEntities())
    {
        if(npc.type == EntityType::NPC)
        {
            auto &fisica = em.fisicas.get(npc.fisica_key);
            auto &render = em.renders.get(npc.render_key);
            if(fisica.esFantasma){
                moverFocoNPC(em, npc);
            }else{
                if(!fisica.esta_estuneado){
                    if(render.animacion != 3){
                        render.animacion = 3;
                        render.animacionYaIniciada = false;
                        render.colocandoBombaAnimacion = false;
                    }
                    // Usamos ControlDeAcceleraccion2 para actualizar velocidades
                    // MiVector3 velocity = ControlDeAcceleraccion2(fisica);
                    // Actualizamos la posición del NPC
                    fisica.x += fisica.vz;
                    fisica.z += fisica.vx;

                    fisica.nextX = fisica.x;
                    fisica.nextZ = fisica.z;
                    if(isOnTileEnemy(fisica.x, 2.0f, 0.15f)){
                        fisica.tileX = static_cast<int>(fisica.x / 2);
                    }
                    if(isOnTileEnemy(fisica.z, 2.0f, 0.15f)){
                        fisica.tileZ = static_cast<int>(fisica.z / 2);
                    }
                    fisica.updateBoundingBox();
                }
                else{
                    fisica.vx = 0.0f;
                    fisica.vz = 0.0f;
                    if(render.animacion != 1 && render.colocandoBombaAnimacion == false){
                        render.animacion = 1;
                        render.animacionYaIniciada = false;
                    }
                }
            }
        }        
    }
}


/**
 * @brief Mueve un NPC en modo foco dependiendo de la dirección que se ha calculado en el IaSystem.
 * 
 * @param em EntityManager
 * @param npc Entidad NPC a mover
 */
void MovementController::moverFocoNPC(EntityManager &em, Entity& npc){
    auto &fisica = em.fisicas.get(npc.fisica_key);
    if(fisica.moverArriba){
        fisica.vz = std::max(fisica.vz - fisica.az, -fisica.maxSpeed);
    }
    else if(fisica.moverAbajo){
        fisica.vz = std::min(fisica.vz + fisica.az, fisica.maxSpeed);
    }
    
    if(fisica.moverIzquierda){
        fisica.vx = std::max(fisica.vx - fisica.ax, -fisica.maxSpeed);
    }
    else if(fisica.moverDerecha){
        fisica.vx = std::min(fisica.vx + fisica.ax, fisica.maxSpeed);
    }
    fisica.nextX += fisica.vx;
    fisica.nextZ += fisica.vz;
    
    moverEntity(em, npc);
}