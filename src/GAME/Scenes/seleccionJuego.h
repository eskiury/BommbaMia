#ifndef SELECCION_H
#define SELECCION_H

#include "escena.h"
#include "sceneManager.h"
#include "../../ENGINES/GraphicEngine/motorGrafico.h" // Motor gráfico
#include "../../Sys/SoundSystem.h"


/**
 * @file seleccionJuego.h
 * @brief Declaración de la clase Seleccion, que representa la escena de selección dentro del juego.
 * @deprecated Esta clase está en desuso y se utiliza la clase SelecPersonajes.
 */

/**
 * @brief Clase que representa la escena de selección dentro del juego.
 * 
 * Esta clase gestiona la lógica, actualización y renderizado de la pantalla
 * o menú de selección, integrándose en el sistema general de escenas.
 */
class Seleccion : public Escena
{
public:
    /**
     * @brief Constructor de la clase Seleccion.
     * Inicializa la escena de selección.
     */
    Seleccion();

    /**
     * @brief Destructor de la clase Seleccion.
     * Libera recursos asignados, si es necesario.
     */
    ~Seleccion();

    /**
     * @brief Inicializa las variables y recursos necesarios para la escena de selección.
     */
    void iniciarSeleccion();

    /**
     * @brief Actualiza la lógica de la escena de selección.
     * Se llama cada frame para procesar entradas y estados.
     */
    void bucleSeleccionUpdate();

    /**
     * @brief Renderiza la escena de selección.
     * Dibuja los elementos visuales en pantalla cada frame.
     */
    void bucleSeleccionRender();

    // Implementación de métodos virtuales heredados de Escena

    /**
     * @brief Inicialización general de la escena.
     * Invoca iniciarSeleccion para preparar el estado inicial.
     */
    void init() override
    {
        iniciarSeleccion();
    };

    /**
     * @brief Actualización por frame de la escena.
     * Llama al método que contiene la lógica de actualización.
     */
    void update() override
    {
        bucleSeleccionUpdate();
    };

    /**
     * @brief Renderizado por frame de la escena.
     * Llama al método que maneja el dibujo en pantalla.
     */
    void render() override
    {
        bucleSeleccionRender();
    };

    /**
     * @brief Maneja los eventos de clic sobre los botones en la escena.
     * Procesa la interacción del usuario con los elementos seleccionables.
     */
    void manejarClicBoton();

private:
};

#endif
