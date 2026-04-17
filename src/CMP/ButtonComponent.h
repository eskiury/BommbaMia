#pragma once
#include <string>
/**
 * @file buttonComponent.h
 * @brief Define los componentes para los botones del juego.
 */

/**
 * @struct buttonComponent
 * @brief Componente para representar un botón.
 *
 */
struct buttonComponent
{
    int id{0};                      ///< Identificador único del botón.
    float x{200};                   ///< Posición X del botón en pantalla (coordenadas absolutas).
    float y{200};                   ///< Posición Y del botón en pantalla.
    float ancho{200};              ///< Ancho del botón (en píxeles o unidades del motor).
    float largo{200};              ///< Alto o largo del botón.
    
    std::string pathSprite;        ///< Ruta del sprite principal del botón (estado normal).
    std::string pathHovered;       ///< Ruta del sprite para el estado 'hovered'.
    std::string auxiliarArenderear;///< Sprite auxiliar para renderizar.

    bool isHovered{false};         ///< Indica si el botón está siendo señalado por el cursor.
    

};
