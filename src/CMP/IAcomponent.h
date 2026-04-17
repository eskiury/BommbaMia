#pragma once

#include "../utils/utility.h"
#include "../utils/graph.h"

/**
 * @file IAcomponent.h
 * @brief Componente de IA que encapsula toda la lógica y estado necesario para tomar decisiones de un NPC.
 */
struct iaComponent {
    /// Indica si el NPC tiene comportamiento basado en árbol de comportamiento (BT).
    bool tieneBT{};

    // ================== Tiempos de control de comportamiento ==================

    /// Tiempo acumulado desde el último evento relevante.
    float elapsedTime{0.0f};

    /// Tiempo mínimo entre colocación de bombas.
    float tiempoEntreBombas{0.3f};

    /// Tiempo acumulado esperando.
    float waitedTime{0.0f};

    /// Tiempo máximo de espera antes de tomar otra acción.
    float tiempoEsperando{5.0f};

    // ================== Variables dinámicas del juego ==================

    /// Nivel de peligro actual (0.0 - 1.0).
    float dangerLevel{0.8f};

    /// Distancia a la bomba más cercana.
    float distanceToBomb{5.0f};

    /// Distancia al objetivo más cercano.
    float distanceToTarget{10.0f};

    /// Distancia recorrida por el NPC.
    float distanceTraveled{0.0f};

    /// Distancia total del camino actual.
    float totalPathDistance{0.0f};

    /// Indica si hay un camino ya calculado.
    bool hasPath{false};

    /// Si el NPC está en posición para colocar bomba.
    bool inPositionToPlaceBomb{false};

    /// Si puede escapar después de colocar una bomba.
    bool canEscapeAfterBomb{true};

    /// Si existe un camino de huida válido.
    bool hasHuidaPath{false};

    /// Valor calculado de peligrosidad de la casilla actual.
    float peligrosidad{0.0f};

    /// Valor calculado de cercanía a power-ups.
    float distanciaPW{0.0f};

    /// Power-ups obtenidos hasta ahora.
    float pwObtenidos{0.0f};

    /// Área libre para moverse alrededor.
    float areaNidos{0.0f};

    /// Número total de bloques en el mapa.
    float bloquesTotales{0.0f};

    /// Distancia al enemigo más cercano.
    float distanciaNPC{0.0f};

    /// Proximidad a la bomba más cercana.
    float bombaCercana{0.0f};

    /// Indica si puede alcanzar un power-up.
    bool puedePW{false};

    /// Identificador del objetivo actual en modo foco.
    int idFocoTarget{999};

    /// Si está en posición para lanzar una flashbang (caso especial).
    bool inPositionToFlash{false};

    /// Valor relacionado con fuego cercano (del mapa o explosiones).
    float fuegoCercano{0.0f};

    /// Rasgos personalizados del NPC (pueden representar personalidad).
    float rasgo1{0.0f}, rasgo2{0.0f}, rasgo3{0.0f};

    // ================== Árbol de comportamiento: nodos principales ==================

    std::shared_ptr<NodoPrimerNivel> Huir;
    std::shared_ptr<NodoPrimerNivel> Camino;
    std::shared_ptr<NodoPrimerNivel> Wait;
    std::shared_ptr<NodoPrimerNivel> Atacar;
    std::shared_ptr<NodoPrimerNivel> RecogerPowerup;

    std::shared_ptr<Root> root;

    // ================== Nodos hijos (acciones concretas) ==================

    std::shared_ptr<NodoUB> moverNPCAtacar;
    std::shared_ptr<NodoUB> colocarBombaAtacar;
    std::shared_ptr<NodoUB> crearCaminoAtacar;

    std::shared_ptr<NodoUB> moverNPCCamino;
    std::shared_ptr<NodoUB> colocarBombaCamino;
    std::shared_ptr<NodoUB> crearCaminoCamino;

    std::shared_ptr<NodoUB> moverNPCHuir;
    std::shared_ptr<NodoUB> crearCaminoHuir;

    std::shared_ptr<NodoUB> moverNPCPw;
    std::shared_ptr<NodoUB> crearCaminoPw;

    /// Acción específica del nodo Wait.
    std::shared_ptr<NodoUB> pararNPC;

    // ================== Nodo alternativo para otros modos (pintura, etc.) ==================

    std::shared_ptr<NodoUB> moverNPC;
    std::shared_ptr<NodoUB> colocarBomba;
    std::shared_ptr<NodoUB> crearCamino;

    std::shared_ptr<Root> RootPintura;
    std::shared_ptr<NodoPrimerNivel> HuirPintura;
    std::shared_ptr<NodoPrimerNivel> CaminoPintura;
    std::shared_ptr<NodoPrimerNivel> WaitPintura;
    std::shared_ptr<NodoPrimerNivel> AtacarPintura;
    std::shared_ptr<NodoPrimerNivel> RecogerPowerupPintura;

    // ================== Árbol de comportamiento en modo "Foco" ==================

    std::shared_ptr<NodoPrimerNivel> rootModoFoco;

    std::shared_ptr<NodoUB> escogerFoco;
    std::shared_ptr<NodoUB> moverFoco;
    std::shared_ptr<NodoUB> atacarFoco;

    /// Nodo actual seleccionado y su utilidad (para decisiones).
    NodoClass nodoActual;
    float utilidadNodoActual{0.0f};

    /// Valor que representa la insistencia en una decisión.
    float tozudez{0.0f};

    // ================== Variables de planificación de rutas ==================

    std::vector<Conexion*> camino;
    std::vector<Conexion*> caminoHuida;

    /// Matriz que indica casillas ya visitadas por el NPC.
    std::vector<std::vector<int>> casillaVisitada;

    // ================== Variables para debug visual ==================

    int estadoDebugger{0};
    float dondeVoyX{0}, dondeVoyZ{0};
    float buscoCaminoX{0}, buscoCaminoZ{0};
    float siguienteCaminoX{0}, siguienteCaminoZ{0};

    std::vector<float> camx;
    std::vector<float> camz;

    float waitsUsados{0};

    // ================== Scheduler de IA por pasos ==================

    int sliceStep{0};

    // ================== Debug nuevo para evaluación de caminos ==================

    int caminoi{0}, caminoj{0};
    float caminolargo{0.0f};

    int huiri{0}, huirj{0};
    float huirlargo{0.0f};

    int poweri{0}, powerj{0};
    float powerlargo{0.0f};

    int atacari{0}, atacarj{0};
    float atacarlargo{0.0f};

    int defaulti{0}, defaultj{0};
    float defaultlargo{0.0f};

    // ================== Mapas de probabilidad para toma de decisiones ==================

    std::vector<std::vector<float>> mapaProbabilidades;
    std::vector<std::vector<float>> mapaProbabilidades1;
    std::vector<std::vector<float>> mapaProbabilidades2;
    std::vector<std::vector<float>> mapaProbabilidades3;
    std::vector<std::vector<float>> mapaProbabilidades4;
};
