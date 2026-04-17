#pragma once

#include "escena.h"
#include "sceneManager.h"
#include "../../ENGINES/GraphicEngine/motorGrafico.h" // Motor gráfico
#include "../../Sys/SoundSystem.h"
#include "../../Sys/FisicaSystem.h"

/**
 * @brief Escena del menú de ajustes del juego.
 *
 * Esta clase gestiona el menú de ajustes, incluyendo la configuración
 * del volumen de música y efectos de sonido, la actualización y renderizado del menú.
 */
class Ajustes : public Escena
{
public:
    /**
     * @brief Constructor por defecto.
     */
    Ajustes();

    /**
     * @brief Destructor.
     */
    ~Ajustes();

    /**
     * @brief Inicializa las variables y estados del menú de ajustes.
     */
    void iniciarAjustes();

    /**
     * @brief Actualiza el estado del menú de ajustes (input, lógica, etc.).
     */
    void bucleAjustesUpdate();

    /**
     * @brief Renderiza el menú de ajustes en pantalla.
     */
    void bucleAjustesRender();

    /// @name Métodos sobrescritos de Escena
    ///@{
    void init() override
    {
        iniciarAjustes();
    }

    void update() override
    {
        bucleAjustesUpdate();
    }

    void render() override
    {
        bucleAjustesRender();
    }
    ///@}


    /**
     * @brief Maneja las acciones derivadas de clics en botones del menú.
     */
    void manejarClicBoton();

private:
    EntityManager em;       ///< Gestor de entidades para el menú.
    FisicaSystem physics;   ///< Sistema de físicas (posiblemente para botones o animaciones).
    bool mouseReleased = true; ///< Estado del botón del mouse para evitar múltiples triggers por clic.

    // Valores actuales de volumen, en rango [0.0, 1.0].
    float valor_musica = 1.0f;
    float valor_sfx = 1.0f;
};
