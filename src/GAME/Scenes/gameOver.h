#pragma once

#include "escena.h"
#include "sceneManager.h"
#include <iostream>
#include "../../ENGINES/GraphicEngine/motorGrafico.h" ///< Motor gráfico

/**
 * @brief Clase que representa la escena de Game Over.
 *
 * Esta clase gestiona la lógica y renderizado de la pantalla
 * que aparece cuando el jugador pierde o termina la partida.
 */
class GameOver : public Escena
{
public:
    /**
     * @brief Constructor por defecto.
     *
     * Inicializa los componentes necesarios para la escena.
     */
    GameOver();

    /**
     * @brief Destructor.
     *
     * Libera los recursos asociados a la escena.
     */
    ~GameOver();

    /**
     * @brief Inicializa las variables y estados de la escena Game Over.
     *
     * Se llama al cargar la escena para preparar todo el estado inicial.
     */
    void iniciarGameOver();

    /**
     * @brief Actualiza la lógica de la escena Game Over.
     *
     * Se ejecuta en cada ciclo de actualización para manejar eventos y lógica.
     */
    void bucleGameOverUpdate();

    /**
     * @brief Renderiza la escena Game Over.
     *
     * Dibuja todos los elementos visuales en pantalla.
     */
    void bucleGameOverRender();

    /// Implementación del método virtual de Escena para inicialización.
    void init() override
    {
        iniciarGameOver();
    };

    /// Implementación del método virtual de Escena para actualización.
    void update() override
    {
        bucleGameOverUpdate();
    };

    /// Implementación del método virtual de Escena para renderizado.
    void render() override
    {
        bucleGameOverRender();
    };

    /**
     * @brief Maneja la interacción con los botones en la escena.
     *
     * Procesa los clics y acciones del usuario en la interfaz de Game Over.
     */
    void manejarClicBoton();

private:
    EntityManager em;   ///< Manager de entidades para la escena.
    FisicaSystem physics; ///< Sistema de física para la escena.
};
