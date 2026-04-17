#ifndef JUEGO_H
#define JUEGO_H

#include "../../utils/iaManager.h"
#include "../../Sys/GameplaySystem.h"
#include "../../Sys/RenderSystem.h"
#include "../../Sys/IAsystem.h"
#include "../../Sys/FisicaSystem.h"
#include "../../Sys/BombSystem.h"
#include "../../Sys/InputSystem.h"
#include "../../Sys/EventSystem.h"
#include "../../GAME/Scenes/escena.h"
#include "../../GAME/Scenes/sceneManager.h"
#include "../../Sys/MapSystem.h"
#include "../../utils/Scheduler/Scheduler.h"
#include "../../Sys/SoundSystem.h"
#include "../../utils/CronoManager.h"
// #include "../../Sys/Enemy.h"
#include <vector>
#include <memory>
#include <unordered_map>

/**
 * @brief Clase principal que representa la escena del juego.
 *
 * Administra la lógica completa del juego, incluyendo sistemas de gameplay,
 * renderizado, físicas, IA, entrada, sonido y eventos.
 * Hereda de Escena para integrarse con el sistema de escenas.
 */
class Juego : public Escena
{
private:
    ElMotorRaylib &motorRaylib;           ///< Referencia al motor gráfico Raylib.
    BombSystem bombSystem;                ///< Sistema para manejar bombas en el juego.
    GameplaySystem gameplay;              ///< Sistema para la lógica de juego principal.
    RenderSystem renderSys;               ///< Sistema para renderizar elementos del juego.
    EntityManager em;                     ///< Manager de entidades.
    FisicaSystem physics;                 ///< Sistema de físicas.
    IASystem iaSystem;                    ///< Sistema de inteligencia artificial.
    InputSystem inputSystem;              ///< Sistema para gestionar la entrada del usuario.
    MapSystem mapSystem;                  ///< Sistema para manejar el mapa y el entorno.
    EventSystem eventSystem;              ///< Sistema para manejar eventos del juego.
    Scheduler scheduler;                  ///< Programador para tareas y eventos temporizados.

    //Enemy enemy;                       ///< Enemigo (comentado, por si se utiliza luego).

    bool jugando = false;                 ///< Indica si el juego está en ejecución.
    int seleccionActual = 0;             ///< Índice o id de la selección actual (jugador, menú, etc).
    float tiempoRestante = 0.0f;         ///< Tiempo restante para la partida o ronda.
    int minutosDeJuego = 0;              ///< Duración total del juego en minutos.
    int contadorCuadrosJuego = 0;        ///< Contador de frames transcurridos en el juego.
    const int cuadrosMaximosJuego = 10800; ///< Límite máximo de frames antes de terminar el juego.
    bool compruebaJuego = false;         ///< Flag para activar comprobaciones internas.
    bool segundosIniciales = true;       ///< Indica si está en los primeros segundos de la partida.
    int contadorCuadrosSegundosIniciales = 0; ///< Contador de frames durante los segundos iniciales.
    const int cuadrosSegundosIniciales = 180; ///< Duración en frames de los segundos iniciales.
    bool perdido = false;                 ///< Indica si el jugador ha perdido.
    bool animacion = false;               ///< Indica si se está ejecutando una animación.
    bool pintura = false;                 ///< Flag para activar efectos de pintura u otro sistema.
    // int rondasGanadasPlayer = 0;      ///< Contador de rondas ganadas (comentado).
    int contP1 = 0;                      ///< Contador de puntos o estado jugador 1.
    int contP2 = 0;                      ///< Contador jugador 2.
    int contP3 = 0;                      ///< Contador jugador 3.
    int contP4 = 0;                      ///< Contador jugador 4.
    bool boolEntidades = false;          ///< Flag para controlar la inicialización de entidades.
    bool boolInit = false;               ///< Flag para inicialización completa.
    std::unordered_map<int, int> rondasPorEntidad; ///< Mapa de rondas ganadas por entidad.

    bool ganaJugador = false;            ///< Indica si un jugador ha ganado la ronda.

    /**
     * @brief Gestiona la entrada de usuario.
     *
     * Interpreta la entrada y ejecuta acciones correspondientes.
     */
    void manejarInput();

public:
    /**
     * @brief Constructor de la clase Juego.
     *
     * @param motorRaylib Referencia al motor gráfico para renderizado.
     */
    Juego(ElMotorRaylib &motorRaylib);

    /**
     * @brief Destructor.
     *
     * Limpia recursos y sistemas usados por la escena.
     */
    ~Juego();

    /**
     * @brief Inicializa la escena y los sistemas.
     */
    void init();

    /**
     * @brief Resetea el estado del juego para comenzar una nueva partida.
     */
    void reset();

    /**
     * @brief Inicializa las entidades del juego.
     */
    void iniciarEntidades();

    /**
     * @brief Inicializa la inteligencia artificial.
     */
    void iniciarIA();

    /**
     * @brief Inicializa todos los sistemas y variables necesarias.
     */
    void iniciarTodo();

    /**
     * @brief Método principal del juego.
     */
    void game();

    /**
     * @brief Actualiza la lógica del juego en cada frame.
     */
    void bucleJuegoUpdate();

    /**
     * @brief Renderiza la escena de juego.
     */
    void bucleJuegoRender();

    /**
     * @brief Obtiene el estado del tiempo restante.
     *
     * @return true si hay tiempo, false si se acabó.
     */
    bool getTiempo();

    /**
     * @brief Comprueba el estado de la escena y actúa en consecuencia.
     *
     * @param em Referencia al EntityManager.
     */
    void comprobarEscena(EntityManager &em);

    /**
     * @brief Inicializa el mapa del juego.
     */
    void iniciarMap();

    /**
     * @brief Configura los eventos del juego.
     */
    void configurarEventos();

    /**
     * @brief Inicia una nueva ronda.
     */
    void iniciarRonda();

    /**
     * @brief Finaliza una ronda.
     *
     * @param ganaJugador Indica si un jugador ganó la ronda.
     * @param id Identificador del jugador ganador.
     */
    void finalizarRonda(bool ganaJugador, int id);

    /**
     * @brief Comprueba el estado de los jugadores.
     */
    void comprobarJugadores();

    /**
     * @brief Resetea el modo actual.
     */
    void resetModo();

    /**
     * @brief Inicializa el motor de mallas.
     */
    void initMotorMallas();

    /**
     * @brief Inicializa una partida para 1 jugador.
     */
    void init1player();

    /**
     * @brief Inicializa una partida para 2 jugadores.
     */
    void init2players();

    /**
     * @brief Inicializa una partida para 3 jugadores.
     */
    void init3players();

    /**
     * @brief Inicializa una partida para 4 jugadores.
     */
    void init4players();

    /**
     * @brief Inicializa los NPCs.
     */
    void initNPCs();

    /**
     * @brief Busca un sonido asociado a una entidad.
     *
     * @param id Identificador de la entidad.
     * @return Entidad que contiene el sonido.
     */
    Entity buscarSonido(int id);

    /**
     * @brief Método para probar un PowerUp en el juego.
     */
    void probarPowerUp();

    /// Implementación del método virtual de Escena para actualización.
    void update() override
    {
        bucleJuegoUpdate();
    };

    /// Implementación del método virtual de Escena para renderizado.
    void render() override
    {
        bucleJuegoRender();
    };
};

#endif
