#ifndef PINTURA_H
#define PINTURA_H

#include "../../utils/iaManager.h"
#include "../../Sys/GameplaySystem.h"
#include "../../Sys/RenderSystem.h"
#include "../../Sys/IAsystem.h"
#include "../../Sys/FisicaSystem.h"
#include "../../Sys/BombSystem.h"
#include "../../Sys/InputSystem.h"
#include "../../Sys/EventSystem.h"
#include "../../Sys/SoundSystem.h"
#include "../../GAME/Scenes/escena.h"
#include "../../GAME/Scenes/sceneManager.h"
#include "../../Sys/MapSystem.h"
#include "../../utils/Scheduler/Scheduler.h"
// #include "../../Sys/Enemy.h"
#include <vector>
#include <memory> // Para std::unique_ptr

/**
 * @brief Clase que representa la escena de "Pintura" en el juego.
 * 
 * Esta clase gestiona toda la lógica, actualización y renderizado
 * del modo de juego basado en pintura, incluyendo la interacción con 
 * sistemas de físicas, IA, sonido, mapa, eventos y entidades.
 * Hereda de Escena para integrarse en el sistema de gestión de escenas.
 */
class Pintura : public Escena
{
private:
    ElMotorRaylib &motorRaylib;   ///< Referencia al motor gráfico Raylib utilizado para renderizar.
    BombSystem bombSystem;        ///< Sistema encargado de la gestión de bombas en el juego.
    GameplaySystem gameplay;      ///< Sistema que controla la lógica del juego.
    RenderSystem renderSys;       ///< Sistema responsable del renderizado de la escena.
    EntityManager em;             ///< Administrador de entidades para la escena.
    FisicaSystem physics;         ///< Sistema de físicas aplicado a la escena.
    IASystem iaSystem;            ///< Sistema de inteligencia artificial para enemigos o NPCs.
    InputSystem inputSystem;      ///< Sistema que gestiona la entrada de usuario.
    MapSystem mapSystem;          ///< Sistema para manejo y control del mapa.
    EventSystem eventSystem;      ///< Sistema para manejo de eventos dentro de la escena.
    Scheduler scheduler;          ///< Planificador para eventos o tareas programadas.

    // Enemy enemy; // Comentado: posible entidad enemiga para futuro uso.

    bool jugando;                 ///< Indica si el modo de juego está activo.
    int seleccionActual;          ///< Opción o entidad actualmente seleccionada.
    float tiempoRestante;         ///< Tiempo restante para finalizar el modo pintura.
    int minutosDePintura;         ///< Minutos totales configurados para el modo.
    int contadorCuadrosPintura;   ///< Contador de frames jugados en esta escena.
    const int cuadrosMaximosPintura = 10800; ///< Límite de frames antes de finalizar el modo.
    bool compruebaPintura;        ///< Flag para verificar estado o condición de pintura.
    bool segundosIniciales = true;///< Indica si está en la fase inicial de tiempo limitado.
    int contadorCuadrosSegundosIniciales = 0; ///< Contador de frames en fase inicial.
    const int cuadrosSegundosIniciales = 180; ///< Frames definidos para la fase inicial.
    bool perdido;                 ///< Indica si el jugador ha perdido.
    bool animacion;               ///< Controla si una animación está en curso.
    bool pintura = true;          ///< Activa el modo pintura.
    int contP1 = 0;               ///< Contadores específicos para jugador 1.
    int contP2 = 0;               ///< Contadores específicos para jugador 2.
    int contP3 = 0;               ///< Contadores específicos para jugador 3.
    int contP4 = 0;               ///< Contadores específicos para jugador 4.
    bool boolEntidades = false;   ///< Control para estado de entidades cargadas.
    bool boolPinturaReset = false;///< Indica si se ha realizado un reseteo del modo pintura.

    /**
     * @brief Maneja la entrada del usuario para la escena.
     * Procesa controles y comandos durante el modo pintura.
     */
    void manejarInput();

public:
    /**
     * @brief Constructor de la escena Pintura.
     * 
     * @param motorRaylib Referencia al motor gráfico Raylib.
     */
    Pintura(ElMotorRaylib &motorRaylib);

    /**
     * @brief Destructor de la escena Pintura.
     * Libera recursos asignados.
     */
    ~Pintura();

    /**
     * @brief Inicializa la escena, configurando variables y sistemas.
     */
    void init();

    /**
     * @brief Reinicia el estado de la escena para comenzar de nuevo.
     */
    void reset();

    /**
     * @brief Inicializa las entidades dentro de la escena.
     */
    void iniciarEntidades();

    /**
     * @brief Inicializa la inteligencia artificial (IA) de la escena.
     */
    void iniciarIA();

    /**
     * @brief Inicializa todos los sistemas y elementos de la escena.
     */
    void iniciarTodo();

    /**
     * @brief Lógica principal del modo pintura.
     */
    void game();

    /**
     * @brief Actualiza la lógica de la escena cada frame.
     */
    void buclePinturaUpdate();

    /**
     * @brief Renderiza la escena cada frame.
     */
    void buclePinturaRender();

    /**
     * @brief Devuelve si queda tiempo para el modo pintura.
     * 
     * @return true Si hay tiempo restante.
     * @return false Si el tiempo se ha acabado.
     */
    bool getTiempo();

    /**
     * @brief Comprueba el estado actual de la escena y eventos relacionados.
     */
    void comprobarEscena();

    /**
     * @brief Inicializa y configura el mapa para esta escena.
     */
    void iniciarMap();

    /**
     * @brief Configura los eventos para la escena.
     */
    void configurarEventos();

    /**
     * @brief Determina y retorna el jugador ganador.
     * 
     * @return int ID del jugador ganador.
     */
    int setGanador();

    /**
     * @brief Inicializa la escena para un solo jugador.
     */
    void init1player();

    /**
     * @brief Inicializa la escena para dos jugadores.
     */
    void init2players();

    /**
     * @brief Inicializa la escena para tres jugadores.
     */
    void init3players();

    /**
     * @brief Inicializa la escena para cuatro jugadores.
     */
    void init4players();

    /**
     * @brief Inicializa los NPCs dentro de la escena.
     */
    void initNPCs();

    /**
     * @brief Resetea la escena y variables después de terminar el modo pintura.
     */
    void resetDespuesDePintura();

    /**
     * @brief Busca y devuelve la entidad asociada a un sonido específico.
     * 
     * @param idSonido Identificador del sonido buscado.
     * @return Entity Entidad relacionada con el sonido.
     */
    Entity buscarSonido(int idSonido);

    /**
     * @brief Contador para el hud de la escena de los bloques pintados.
     * 
     */
    void contadorHud();

    /* Implementación de funciones virtuales de Escena */

    /**
     * @brief Actualiza la escena en cada frame.
     */
    void update() override
    {
        buclePinturaUpdate();
    };

    /**
     * @brief Renderiza la escena en cada frame.
     */
    void render() override
    {
        buclePinturaRender();
    };
};

#endif
