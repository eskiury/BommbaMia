#ifndef INTRO_H
#define INTRO_H

#include "escena.h"  
#include "sceneManager.h" 
#include <iostream>
#include "../../ENGINES/GraphicEngine/motorGrafico.h" ///< Motor gráfico
#include "../../Sys/SoundSystem.h"

/**
 * @brief Clase que representa la escena de introducción.
 *
 * Controla la animación y lógica de la pantalla inicial,
 * incluyendo transiciones de entrada y salida, y carga de recursos.
 */
class Intro : public Escena
{
public:
    /**
     * @brief Constructor por defecto.
     *
     * Inicializa el estado interno y variables necesarias para la intro.
     */
    Intro();

    /**
     * @brief Destructor.
     *
     * Libera recursos asociados a la escena de introducción.
     */
    ~Intro();

    /**
     * @brief Inicializa las variables y estados de la intro.
     *
     * Se llama al cargar la escena para preparar la animación y carga de recursos.
     */
    void iniciarIntro();

    /**
     * @brief Actualiza la lógica de la escena de introducción.
     *
     * Controla transiciones, animaciones y carga progresiva.
     */
    void bucleIntroUpdate();

    /**
     * @brief Renderiza la escena de introducción.
     *
     * Dibuja el logo y elementos visuales, incluyendo transiciones de opacidad.
     */
    void bucleIntroRender();

    /**
     * @brief Indica si la intro ha finalizado.
     *
     * @return true si la intro terminó, false en caso contrario.
     */
    bool isFinished();

    /// Implementación del método virtual de Escena para inicialización.
    void init() override {
        iniciarIntro();
    };

    /// Implementación del método virtual de Escena para actualización.
    void update() override {
        bucleIntroUpdate();
    };

    /// Implementación del método virtual de Escena para renderizado.
    void render() override {
        bucleIntroRender();
    };

private:
    float alpha = 0.0f;         ///< Opacidad para las transiciones (0.0f a 1.0f).
    bool fadingIn = true;       ///< Estado de transición de entrada.
    bool logoVisible = true;    ///< Indica si el logo inicial está visible.

    EntityManager em;           ///< Manager de entidades.

    std::vector<std::string> rutasParaCargar; ///< Lista de recursos a cargar.
    bool recursosCargados = false;             ///< Flag que indica si los recursos están cargados.
    size_t indiceCarga = 0;                    ///< Índice actual para la carga progresiva.
    bool pantallasDeCarga = false;             ///< Indica si se están mostrando pantallas de carga.
};

#endif
