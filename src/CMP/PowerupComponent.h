#pragma once
#include <vector>

/**
 * @file PowerupComponent.h
 * @brief Define el componente relacionado con los power-ups en el sistema ECS del juego.
 */

/**
 * @enum PowerUpType
 * @brief Enumeración de tipos de power-up disponibles en el juego.
 *
 * Estos power-ups proporcionan ventajas o desventajas temporales al jugador o entidad que los recoge,
 * afectando sus capacidades como velocidad, rango de explosión o cantidad de bombas.
 */
enum class PowerUpType
{
    BOMB_UP = 10,   ///< Incrementa el número máximo de bombas que se pueden colocar.
    BOMB_DOWN,      ///< Reduce el número máximo de bombas.
    EXP_RANGE_UP,   ///< Incrementa el rango de explosión.
    EXP_RANGE_DOWN, ///< Reduce el rango de explosión.
    EXP_RANGE_MAX,  ///< Establece el rango de explosión al máximo permitido.
    SPEED_UP,       ///< Incrementa la velocidad máxima.
    SPEED_DOWN,     ///< Reduce la velocidad máxima.
    PUSH_BOMB       ///< Permite empujar bombas.
};

/**
 * @struct powerUpComponent
 * @brief Componente de power-up asociado a una entidad del juego.
 *
 * Este componente almacena información sobre el tipo de power-up activo en el mundo y
 * una lista de los power-ups recolectados por la entidad a lo largo del juego.
 */
struct powerUpComponent
{
    PowerUpType type;  ///< Tipo de power-up activo.
    
    std::vector<PowerUpType> collectedPowerUps;  ///< Lista de power-ups recogidos.
};
