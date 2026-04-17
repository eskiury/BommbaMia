#ifndef ENTITY_H
#define ENTITY_H

#include "../../utils/slotmap.hpp"
#include "BombaComponent.h"
#include "FisicaComponent.h"
#include "PowerupComponent.h"
#include "SoundComponent.h"
#include "RenderComponent.h"
#include "IAcomponent.h"
#include "ButtonComponent.h"

/**
 * @file Entity.h
 * @brief Define la estructura de una entidad y su relación con los componentes del ECS.
 */

/**
 * @enum EntityType
 * @brief Enumeración de tipos de entidad.
 *
 * Clasifica las entidades del juego en diferentes roles o funciones
 * para facilitar su gestión y comportamiento.
 */
enum class EntityType
{
    PLAYER,     ///< Entidad controlada por el jugador.
    NPC,        ///< Entidad controlada por la IA.
    BOMBA,      ///< Bomba colocada por un jugador o NPC.
    POWERUP,    ///< Objeto recolectable que otorga cambios de estadisticas.
    DESTRUIBLE, ///< Objeto que puede ser destruido, como bloques o muros.
    SOUND,      ///< Entidad utilizada para gestionar sonidos.
    BUTTON      ///< Entidad utilizada para gestionar botones.
};

/**
 * @struct Entity
 * @brief Representa una entidad dentro del sistema ECS.
 *
 * Una entidad es un identificador único que referencia a un conjunto de componentes.
 */
struct Entity
{
    int id; ///< Identificador único de la entidad.

    EntityType type; ///< Tipo de entidad, basado en la enumeración @ref EntityType.

    /**
     * @brief Obtiene el identificador de la entidad.
     * @return El ID de la entidad.
     */
    int getId()
    {
        return id;
    };

    // === Claves de acceso a componentes ===
    // Estas claves permiten acceder a los datos almacenados en Slotmaps de componentes.
    typename Slotmap<fisicaComponent, 100>::key_type fisica_key;   ///< Clave para el componente físico (posición, movimiento, etc.).
    typename Slotmap<renderComponent, 100>::key_type render_key;   ///< Clave para el componente de renderizado (apariencia de la entidad).
    typename Slotmap<powerUpComponent, 100>::key_type powerUp_key; ///< Clave para el componente de power-up (ventajas temporales).
    typename Slotmap<soundComponent, 100>::key_type sound_key;     ///< Clave para el componente de sonido.
    typename Slotmap<bombaComponent, 100>::key_type bomb_key;      ///< Clave para el componente de bomba.
    typename Slotmap<iaComponent, 100>::key_type ia_key;           ///< Clave para el componente de IA.
    typename Slotmap<buttonComponent, 100>::key_type button_key;   ///< Clave para el componente de botón.
};

#endif // ENTITY_H
