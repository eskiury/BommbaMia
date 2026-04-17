#pragma once 

#include "../utils/miColor.h"
#include "../ENGINES/GraphicEngine/BellotaEngine/AlmondNode.h"

/**
 * @file RenderComponent.h
 * @brief Define el componente de renderizado utilizado por las entidades dentro del ECS del juego.
 */

/**
 * @struct renderComponent
 * @brief Componente encargado de representar gráficamente una entidad en pantalla.
 *
 * Este componente contiene la información visual de la entidad, como su tamaño, color,
 * modelo 3D y su estado dentro del árbol de renderizado. También gestiona aspectos gráficos
 * específicos de botones y animaciones.
 */
struct renderComponent
{
    float size{2.0f};                       ///< Tamaño general del objeto 3D (esfera, cubo, etc.).
    MiColor color{255, 255, 255, 255};      ///< Color del objeto en formato RGBA.
    float rotacion{0.0f};                   ///< Ángulo de rotación del modelo.

    std::string modeloPersonaje;            ///< Ruta al archivo del modelo 3D del personaje.

    AlmondNode* SusDatosEnElArbol{nullptr}; ///< Puntero al nodo dentro del árbol de renderizado.

    // --- Parámetros de interfaz gráfica (botones) ---
    int x{}, y{}, ancho{}, largo{};         ///< Posición y dimensiones del botón en pantalla.
    std::string pathSprite;                 ///< Ruta al sprite utilizado por el botón.
    bool isHovered{false};                  ///< Indica si el cursor está sobre el botón.

    // --- Estado dentro del motor de renderizado ---
    bool cargadoEnElArbol{false};           ///< Indica si la entidad ya fue añadida al árbol gráfico.
    bool explosionaelrender{false};         ///< Señal visual para representar una explosión.
    bool isRendering{false};                ///< Indica si la entidad debe renderizarse actualmente.

    // --- Modo de foco (cámara o efectos especiales) ---
    AlmondNode* TieneFoco{nullptr};         ///< Nodo con foco de cámara u otro efecto.
    bool cargadoEnElArbolFoco{false};       ///< Indica si el nodo de foco ya fue añadido al árbol.

    // --- Animaciones ---
    int animacion{0};                       ///< Índice de la animación activa actualmente.
    bool animacionYaIniciada{false};        ///< Indica si la animación ya ha comenzado.
    bool colocandoBombaAnimacion{false};    ///< Indica si se está ejecutando la animación de colocación de bomba.
};
