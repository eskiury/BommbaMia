#ifndef MENU_H
#define MENU_H

#include "escena.h"
#include "sceneManager.h"
#include "../../ENGINES/GraphicEngine/motorGrafico.h"
#include "../../Sys/FisicaSystem.h"
#include "../../ENGINES/AlmondSE/AlmondSE.h"
#include "../../Sys/SoundSystem.h"
#include <memory>

/**
 * @brief Clase que representa el menú principal del juego.
 * 
 * Hereda de Escena para integrarse en el sistema de escenas.
 * Gestiona la lógica y renderizado del menú, así como la interacción del usuario.
 */
class Menu : public Escena
{
public:
    /**
     * @brief Constructor de la clase Menu.
     * Inicializa variables internas.
     */
    Menu();

    /**
     * @brief Destructor de la clase Menu.
     * Limpia recursos utilizados.
     */
    ~Menu();

    /**
     * @brief Inicializa las variables y estados necesarios para el menú.
     * Se llama al cargar esta escena.
     */
    void iniciarMenu();

    /**
     * @brief Actualiza la lógica del menú, como la navegación, selección y entradas.
     * Se llama en cada frame.
     */
    void bucleMenuUpdate();

    /**
     * @brief Renderiza todos los elementos visuales del menú.
     * Se llama en cada frame después de la actualización.
     */
    void bucleMenuRender();

    /**
     * @brief Método virtual sobrescrito de Escena.
     * Inicializa la escena cuando se carga.
     */
    void init() override
    {
        iniciarMenu();
    };

    /**
     * @brief Inicializa los sonidos asociados al menú.
     * Carga y prepara efectos o música de fondo.
     */
    void initSonidos();

    /**
     * @brief Busca y retorna la entidad asociada a un sonido específico.
     * 
     * @param idSonido Identificador del sonido buscado.
     * @return Entity Entidad que contiene el sonido, si existe.
     */
    Entity buscarSonido(int idSonido);

    /**
     * @brief Método virtual sobrescrito de Escena.
     * Actualiza el menú en cada frame.
     */
    void update() override
    {
        bucleMenuUpdate();
    };

    /**
     * @brief Método virtual sobrescrito de Escena.
     * Renderiza el menú en cada frame.
     */
    void render() override
    {
        bucleMenuRender();
    };

private:
    EntityManager em;       ///< Administrador de entidades para el menú.
    FisicaSystem physics;   ///< Sistema de físicas para interacciones del menú.
    bool mouseReleased = true; ///< Controla el estado del botón del ratón para evitar múltiples eventos por clic.
    int contFrames = 0;     ///< Contador de frames, puede usarse para animaciones o temporizadores.
    int botonActivo = 1;    ///< Identifica qué botón del menú está actualmente seleccionado o activo.

    /**
     * @brief Maneja la acción del clic del ratón sobre los botones del menú.
     * Gestiona la navegación y la ejecución de las opciones seleccionadas.
     */
    void manejarClicBoton();
};

#endif // MENU_H
