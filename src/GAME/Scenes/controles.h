#pragma once

#include "escena.h"
#include "sceneManager.h"
#include <iostream>
#include "../../ENGINES/GraphicEngine/motorGrafico.h" // Motor gráfico
#include "../../Sys/FisicaSystem.h"
#include "../../Sys/SoundSystem.h"

/**
 * @brief Escena para el menú de controles.
 * 
 * Gestiona la interfaz y lógica para mostrar y modificar controles dentro del juego.
 */
class Controles : public Escena
{
public:
    /**
     * @brief Constructor por defecto.
     */
    Controles();

    /**
     * @brief Destructor.
     */
    ~Controles();

    /**
     * @brief Inicializa las variables y el estado del menú de controles.
     */
    void iniciarControles();

    /**
     * @brief Actualiza la lógica y eventos del menú de controles.
     */
    void bucleControlesUpdate();

    /**
     * @brief Renderiza la escena del menú de controles.
     */
    void bucleControlesRender();

    /// @name Métodos sobrescritos de Escena
    /// @{
    void init() override
    {
        iniciarControles();
    }

    void update() override
    {
        bucleControlesUpdate();
    }

    void render() override
    {
        bucleControlesRender();
    }
    /// @}

    /**
     * @brief Maneja las acciones derivadas de clics en botones del menú.
     */
    void manejarClicBoton();

private:
    EntityManager em;   ///< Gestor de entidades para la escena.
    FisicaSystem physics; ///< Sistema de físicas (por ejemplo para botones).
};
