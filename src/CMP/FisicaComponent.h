#pragma once

#include "../utils/miVector3.h"
#include "../utils/miVector2.h"

/**
 * @file FisicaComponent.h
 * @brief Define componentes relacionados con la física de las entidades.
 */

/**
 * @struct BoundingBOX
 * @brief Representa una caja delimitadora (Bounding Box) en el espacio 3D.
 *
 * Esta estructura se utiliza para definir un volumen de colisión que encierra a una entidad,
 * especificado por sus coordenadas mínimas y máximas.
 */
struct BoundingBOX
{
    MiVector3 min; ///< Coordenadas mínimas del BoundingBox.
    MiVector3 max; ///< Coordenadas máximas del BoundingBox.
};

// struct BoundingCIRCLE
// {
//     MiVector2 center;
//     float radius;
// };

/**
 * @struct fisicaComponent
 * @brief Componente que contiene los datos físicos de una entidad.
 *
 * Este componente gestiona la posición, velocidad, colisiones, estado físico y otros atributos
 * necesarios para simular el movimiento y comportamiento dinámico de una entidad en el mundo del juego.
 */
struct fisicaComponent
{
    // === Posición y movimiento ===
    float x{}, y{}, z{};                            ///< Posición actual.
    float nextX{x}, nextY{y}, nextZ{z};             ///< Posición futura.
    int tileX{static_cast<int>(x / 2)};             ///< Posición X en la cuadrícula.
    int tileZ{static_cast<int>(z / 2)};             ///< Posición Z en la cuadrícula.
    float prev_tileX{}, prev_tileY{}, prev_tileZ{}; ///< Posición anterior para detección de cambios.
    float vx{0.0f}, vy{}, vz{0.0f};                 ///< Velocidades actuales en cada eje.
    float ax{0.01f}, az{0.01f};                     ///< Aceleraciones en X y Z.
    float maxSpeed{0.12f};                          ///< Velocidad máxima permitida.

    // === Atributos adicionales de juego ===
    float delay{0.5f};           ///< Delay entre acciones (como colocar bombas).
    float vida{200};             ///< Vida de la entidad.
    int range{3};                ///< Rango de las bombas colocadas.
    int maxBombs{3};             ///< Máximo de bombas colocables simultáneamente.
    bool encimaBomba{false};     ///< Si está sobre una bomba.
    bool chocadoConBomba{false}; ///< Si colisiona con una bomba.

    // === Estados especiales ===
    bool esFantasma{false};         ///< Si atraviesa objetos.
    bool cargaCompletada{false};    ///< Indica si finalizó una carga.
    float tiempoCarga{0.0f};        ///< Tiempo acumulado de carga.
    bool estuneo{false};            ///< Si puede estunear.
    bool esta_estuneado{false};     ///< Estado actual de aturdimiento.
    float tiempoEstuneado{0.0f};    ///< Tiempo restante de aturdimiento.
    float invulnerabilidad{0.0f};   ///< Tiempo de invulnerabilidad tras ser aturdido.

    // === Movimiento (input) ===
    bool moverArriba{false};
    bool moverAbajo{false};
    bool moverDerecha{false};
    bool moverIzquierda{false};

    // === Estados de vida y respawn ===
    bool estaMuerto{false};                      ///< Indica si la entidad ha muerto.
    float respawn{3.0f};                         ///< Tiempo antes de reaparecer.
    float respawnX{x}, respawnY{y}, respawnZ{z}; ///< Posición de respawn.

    // === Estado de victoria y colisiones ===
    bool gana{};              ///< Indica si esta entidad ha ganado una ronda.
    bool chocaX{false};       ///< Colisión en eje X.
    bool chocaZ{false};       ///< Colisión en eje Z.
    bool canPushBombs{false}; ///< Si puede empujar bombas.
    int rondasGanadas{0};     ///< Rondas ganadas por la entidad.

    // === Tamaño y colisiones ===
    MiVector3 size{2.0f, 2.0f, 2.0f}; ///< Tamaño de la entidad.
    BoundingBOX boundingBox;          ///< Bounding Box actual.
    BoundingBOX siguienteBB;          ///< Bounding Box futuro (para movimiento anticipado).

    /**
     * @brief Actualiza la caja delimitadora (boundingBox) actual.
     *
     * Esta función recalcula el volumen de colisión a partir de la posición actual,
     * reduciéndolo con un factor para evitar colisiones indeseadas.
     */
    void updateBoundingBox()
    {
        float reduccion = 0.9f;
        boundingBox.min = {x - (size.x * reduccion) / 2.0f,
                           y - (size.y * reduccion) / 2.0f,
                           z - (size.z * reduccion) / 2.0f};
        boundingBox.max = {x + (size.x * reduccion) / 2.0f,
                           y + (size.y * reduccion) / 2.0f,
                           z + (size.z * reduccion) / 2.0f};
    }

    /**
     * @brief Actualiza la caja delimitadora futura (siguienteBB).
     *
     * Se basa en la posición prevista (nextX, nextY, nextZ) para anticipar colisiones.
     */
    void updateSiguienteBB()
    {
        float reduccion = 0.9f;
        siguienteBB.min = {nextX - (size.x * reduccion) / 2.0f,
                           nextY - (size.y * reduccion) / 2.0f,
                           nextZ - (size.z * reduccion) / 2.0f};
        siguienteBB.max = {nextX + (size.x * reduccion) / 2.0f,
                           nextY + (size.y * reduccion) / 2.0f,
                           nextZ + (size.z * reduccion) / 2.0f};
    }
};
