/**
 * @file bombaComponent.h
 * @brief Estructura que contiene las propiedades de las bombas.
 * 
 * Este archivo contiene las definiciones de los componentes relacionados con el sistema
 * de bombas del juego.
 */
#pragma once

#include <vector>

/**
 * @brief Representa el rastro de una explosión.
 *
 * Almacena la información necesaria para visualizar el rastro de una explosión
 * en una posición de la cuadrícula y el tiempo restante hasta que desaparezca.
 */
struct ExplosionRastro {
    float remainingTime;  ///< Tiempo restante para que el rastro desaparezca.
    int tileX;            ///< Coordenada X de la cuadrícula afectada.
    int tileZ;            ///< Coordenada Z de la cuadrícula afectada.
};

/**
 * @brief Componente de bomba.
 *
 * Define las propiedades relacionadas con el comportamiento de las bombas,
 * incluyendo su estado de explosión, duración, rango, cantidad máxima, y
 * otros atributos necesarios para la simulación de explosiones en el juego.
 */
struct bombaComponent {
    bool isExploding{false};         ///< Indica si la bomba está en proceso de explosión.
    float explosionDuration{1.0f};   ///< Duración en segundos de la explosión.
    int expRange{3};                 ///< Rango de la explosión (en tiles).
    int maxBombs{3};                 ///< Máximo de bombas que la entidad puede colocar simultáneamente.
    int bombsPlaced{0};              ///< Número de bombas actualmente colocadas por la entidad.
    int ownerId{-1};                 ///< Identificador de la entidad que colocó la bomba.
    int pinturaId{-1};                ///< Identificador de la entidad que colocó la bomba en el modo pintura.
    float lifetime{3.0f};            ///< Tiempo de vida restante de la bomba.
    bool explosionPropagated{false}; ///< Indica si la explosión ya se propagó.
    int ContadorSegundos{0};         ///< Contador de segundos para el temporizador de la bomba.
    float elapsedTime{0.0f};         ///< Tiempo transcurrido desde que la bomba fue activada.
    float tiempoTotalBomba{3.0f};    ///< Tiempo total de vida de la bomba.
    std::vector<ExplosionRastro> explosionRastros; ///< Lista de rastros generados por la explosión.
    bool empujada{false};            ///< Indica si la bomba ha sido empujada.
    int TX{-2};                    ///< Coordenada X de la posición de empuje.
    int TZ{-2};                    ///< Coordenada Z de la posición de empuje.
};
