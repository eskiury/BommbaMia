#pragma once

#include "escena.h"
#include "sceneManager.h"
#include "../../ENGINES/GraphicEngine/motorGrafico.h" // Motor gráfico
#include "../../ENGINES/AlmondSE/AlmondSE.h"          // Interfaz AlmondSE
#include "../../Sys/SoundSystem.h"
#include "../../Sys/FisicaSystem.h"
#include "../../Sys/InputSystem.h"

/// Variable global externa que indica el número de jugadores activos
extern int numPlayers;

/**
 * @brief Clase que representa la escena de selección de personajes.
 * 
 * Gestiona la lógica y renderizado del menú de selección de personajes,
 * incluyendo entrada del usuario, botones interactivos y sonidos asociados.
 */
class SelecPersonajes : public Escena
{
public:
    /**
     * @brief Constructor de la clase SelecPersonajes.
     */
    SelecPersonajes();

    /**
     * @brief Destructor de la clase SelecPersonajes.
     */
    ~SelecPersonajes();

    /**
     * @brief Inicializa los elementos necesarios para la escena de selección de personajes.
     */
    void iniciarSelecPersonajes();

    /**
     * @brief Actualiza la lógica interna de la escena cada frame.
     */
    void bucleSelecPersonajesUpdate();

    /**
     * @brief Renderiza los elementos visuales de la escena.
     */
    void bucleSelecPersonajesRender();

    // Métodos sobrescritos de la clase base Escena

    /**
     * @brief Inicializa la escena al ser cargada.
     */
    void init() override
    {
        iniciarSelecPersonajes();
    };

    /**
     * @brief Lógica de actualización de la escena.
     */
    void update() override
    {
        bucleSelecPersonajesUpdate();
    };

    /**
     * @brief Renderizado de la escena.
     */
    void render() override
    {
        bucleSelecPersonajesRender();
    };

    /**
     * @brief Carga los bancos de sonido necesarios para la escena.
     */
    void cargarBancosSonido();

    /**
     * @brief Inicializa los botones de selección.
     */
    void iniciarBotones();

    /**
     * @brief Maneja los clics en los botones del menú.
     */
    void manejarClicBoton();

    /**
     * @brief Devuelve el número de jugadores seleccionados.
     * @return Número de jugadores.
     */
    int getNumPlayers();

    /**
     * @brief Inicializa los personajes seleccionables.
     */
    void initPersonajes();

    /**
     * @brief Busca una entidad de sonido mediante su ID.
     * @param idSonido Identificador del sonido a buscar.
     * @return Entidad que representa el sonido.
     */
    Entity buscarSonido(int idSonido);

    /**
     * @brief Inicializa los bancos de personajes disponibles.
     */
    void initBancos();

    /**
     * @brief Reinicia el estado interno de la escena.
     */
    void reset();

private:
    FisicaSystem physics;           ///< Sistema de física
    EntityManager em;               ///< Gestor de entidades
    InputSystem inputSystem;        ///< Sistema de entrada

    // Variables de control de estado interno
    bool setReset = false;          ///< Control para reinicio de escena
    bool mouseReleased = true;      ///< Control para evitar múltiples clics
    int contFrames = 0;             ///< Contador de frames para sincronización
    bool boolRetraso = true;        ///< Bandera de retardo inicial
    bool initBancosBool = true;     ///< Control para inicialización de bancos
    bool boolEntidades = true;      ///< Control de entidades activas
    bool boolReset = true;          ///< Control de reinicio general
    bool boolClickPinta = false;    ///< Estado del clic del jugador 1
    bool boolClickPinta2 = false;   ///< Estado del clic del jugador 2
    bool boolClickPinta3 = false;   ///< Estado del clic del jugador 3
    bool boolClickPinta4 = false;   ///< Estado del clic del jugador 4
};
