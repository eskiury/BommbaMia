#pragma once

#include "escena.h"
#include "sceneManager.h"
#include <iostream>
#include "../../ENGINES/GraphicEngine/motorGrafico.h" // Motor gráfico
#include "../../Sys/FisicaSystem.h"

/**
 * @brief Clase que representa la escena de victoria del juego.
 * 
 * Esta escena se muestra cuando el jugador gana una partida. 
 * Contiene la lógica y el renderizado del menú de victoria, 
 * así como el manejo de eventos como clics del jugador.
 */
class Win : public Escena
{
public:
    /**
     * @brief Constructor por defecto.
     * 
     * Inicializa los elementos necesarios para la escena de victoria.
     */
    Win();

    /**
     * @brief Destructor por defecto.
     * 
     * Libera los recursos utilizados por la escena.
     */
    ~Win();

    /**
     * @brief Inicializa las variables y entidades del menú de victoria.
     */
    void iniciarWin();

    /**
     * @brief Actualiza la lógica interna del menú de victoria.
     * 
     * Se ejecuta en cada frame y contiene la lógica de interacción o transición.
     */
    void bucleWinUpdate();

    /**
     * @brief Renderiza todos los elementos visuales del menú de victoria.
     * 
     * Llamado cada frame tras el `update`.
     */
    void bucleWinRender();

    /**
     * @brief Maneja las acciones de los clics sobre los botones del menú.
     */
    void manejarClicBoton();

    // Implementaciones de funciones virtuales de la clase Escena

    /**
     * @brief Inicializa la escena de victoria (sobrescribe Escena::init).
     */
    void init() override
    {
        iniciarWin();
    };

    /**
     * @brief Actualiza la escena de victoria (sobrescribe Escena::update).
     */
    void update() override
    {
        bucleWinUpdate();
    };

    /**
     * @brief Renderiza la escena de victoria (sobrescribe Escena::render).
     */
    void render() override
    {
        bucleWinRender();
    };

private:
    EntityManager em;       ///< Gestor de entidades utilizado en la escena
    FisicaSystem physics;   ///< Sistema de física para la escena
};
