/**
 * @file EntityManager.h
 * @brief Declaración de la clase EntityManager, responsable de gestionar entidades y sus componentes en el juego.
 * 
 * Este archivo define la clase EntityManager, la cual administra la creación, destrucción y manipulación de entidades,
 * incluyendo la asociación de diversos tipos de componentes como física, render, IA, etc. Forma parte central del sistema ECS.
 * 
 * @author AlmondStudio
 * @date 2025-05-21
 * @version 1.0
 */
#pragma once

#include "../CMP/Entity.h"
#include "../utils/slotmap.hpp"
#include <vector>
#include <functional>
#include "../utils/miVector2.h"
#include "../utils/miVector3.h"

/**
 * @class EntityManager
 * @brief Gestor de entidades del motor de juego ECS.
 * 
 * Administra la creación, actualización y eliminación de entidades en el juego, además de manejar sus componentes
 * asociados mediante estructuras tipo slotmap.
 */
class EntityManager
{
public:
    /**
     * @brief Constructor de EntityManager.
     *
     * Inicializa el gestor de entidades con un tamaño inicial predeterminado.
     *
     * @param defaultSize Tamaño inicial para la reserva de espacio en el vector de entidades (por defecto 100).
     */
    EntityManager(std::size_t defaultSize = 100);

    /**
     * @brief Crea una nueva entidad.
     *
     * Crea una nueva entidad con el tipo especificado y le asigna un identificador único.
     *
     * @param type Tipo de entidad a crear.
     * @return Referencia a la entidad recién creada.
     */
    Entity &createEntity(EntityType type);

    /**
     * @brief Asocia componentes a una entidad.
     *
     * Vincula los componentes de física, renderizado, power-up, sonido, bomba e IA a la entidad.
     *
     * @param entity Referencia a la entidad a la que se asignarán los componentes.
     * @param fisica Componente de física.
     * @param render Componente de renderizado.
     * @param powerUp Componente de power-up.
     * @param sound Componente de sonido.
     * @param bomba Componente de bomba.
     * @param ia Componente de IA.
     */
    void createEntityComponents(Entity &entity, const fisicaComponent &fisica, const renderComponent &render, const powerUpComponent &powerUp, const soundComponent &sound, const bombaComponent &bomba, const iaComponent &, const buttonComponent &button);

    /**
     * @brief Dibuja todas las entidades.
     *
     * Dibuja todas las entidades que tienen componentes válidos de física y renderizado.
     */
    void drawAll();

    /**
     * @brief Elimina una entidad.
     *
     * Elimina una entidad específica junto con todos sus componentes asociados.
     *
     * @param entityId Identificador único de la entidad a eliminar.
     */
    void eraseEntity(int entityId);

    /**
     * @brief Devuelve todas las entidades.
     *
     * @return Vector que contiene todas las entidades.
     */
    std::vector<Entity> getEntities();

    /**
     * @brief Aplica una función a cada entidad de un tipo específico.
     *
     * Itera sobre todas las entidades y, si coinciden con el tipo especificado, aplica la función dada.
     *
     * @param tipo Tipo de entidad a filtrar.
     * @param func Función a aplicar a cada entidad filtrada.
     */
    void forEach(EntityType tipo, std::function<void(Entity &)> func);

    /**
     * @brief Obtiene la primera entidad de un tipo específico.
     *
     * @param tipo Tipo de entidad a buscar.
     * @return Referencia a la entidad encontrada.
     * @throws std::runtime_error Si no se encuentra ninguna entidad del tipo especificado.
     */
    Entity &getEntityOfType(EntityType tipo);

    /**
     * @brief Te devuelve el numero con el cual esta pintando el modo pintura esa partida una entidad
     * 
     * @param entityId id de la entidad
     * @return int numero de la entidad asociado a la pintura
     */
    int getPinturaId(int entityId);

    /**
     * @brief Devuelve todas las entidades de un tipo específico.
     *
     * @param tipo Tipo de entidad a buscar.
     * @return Vector con todas las entidades del tipo especificado.
     */
    std::vector<Entity> getEntitiesByType(EntityType tipo);

    /**
     * @brief Obtiene punteros a todas las entidades de un tipo específico.
     *
     * @param tipo Tipo de entidad a buscar.
     * @return Vector con punteros a las entidades del tipo especificado.
     */
    std::vector<Entity*> getEntitiesByTypePunteros(EntityType tipo);

    /**
     * @brief Obtiene una entidad por su ID.
     *
     * Devuelve la entidad que coincide con el identificador único dado.
     *
     * @param entityId Identificador único de la entidad a buscar.
     * @return Referencia a la entidad encontrada.
     * @throws std::runtime_error Si no se encuentra ninguna entidad con el ID especificado.
     */
    Entity &getEntity(int entityId);

    /**
     * @brief Calcula la posición en el mapa.
     *
     * A partir de una posición 3D, calcula la posición en la cuadrícula (tile) correspondiente.
     *
     * @param position Posición en 3D.
     * @return Posición en 2D (tile).
     */
    MiVector2 getTilePosition(MiVector3 position);


    /**
     * @brief Elimina todas las entidades y componentes.
     *
     * Borra todas las entidades y sus componentes asociados, y reinicia el contador de IDs.
     */
    void clear();
    std::vector<Entity> getEnemies(Entity& entidad);
    const std::vector<Entity> getBombs();

    /**
     * @brief Elimina todas las entidades de cierto tipo.
     * 
     * @param tipo Tipo de entidad.
     */
    void forEachErase(EntityType tipo);

    /**
     * @brief Te busca todos las entidades de la partida
     *  
     * @return Vector con todas las entidades en juego.
     */
    std::vector<Entity> getAllEntitiesInGame();


    // Slotmaps para la gestión de componentes:
    // - fisicas: Componentes de física (posición, movimiento, etc.).
    // - renders: Componentes de renderizado.
    // - powerUps: Componentes de power-up.
    // - sounds: Componentes de sonido.
    // - bombs: Componentes de bomba.
    // - ia: Componentes de IA.
    Slotmap<fisicaComponent, 100> fisicas;   ///< Gestor de componentes físicos.
    Slotmap<renderComponent, 100> renders;   ///< Gestor de componentes de renderizado.
    Slotmap<powerUpComponent, 100> powerUps; ///< Gestor de componentes de power-ups.
    Slotmap<soundComponent, 100> sounds;     ///< Gestor de componentes de sonido.
    Slotmap<bombaComponent, 100> bombs;      ///< Gestor de componentes de bombas.
    Slotmap<iaComponent, 100> ia;            ///< Gestor de componentes de IA.
    Slotmap<buttonComponent, 100> buttons;   ///< Gestor de componentes de botones.

private:
    /**
     * @brief Vector de entidades.
     *
     * Almacena todas las entidades activas en el juego.
     */
    std::vector<Entity> entities_;

    int TILE_SIZE = 2.0f; ///< Tamaño de cada tile (unidad de medida del mapa).

    /**
     * @brief Contador de IDs únicos.
     *
     * Se incrementa con cada nueva entidad creada para garantizar identificadores únicos.
     */
    int nextId;
};
