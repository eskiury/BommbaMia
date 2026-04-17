#include <chrono>
#include <iostream>
#include "IAsystem.h"
#include "../utils/utility.h"
#include "../utils/Const.h"

constexpr int K_rondas = 5;
constexpr float K_speed = 0.10f;

/**
 * @brief Inicializa la Inteligencia Artificial
 * Limpia los nodos el grafo residuales, en caso de hayan
 * Inicializa los nodos del grafo para A*, con el mapSystem
 * Guarda las filas y columnas de la matriz del grado
 * @param mapSystem 
 */
void IASystem::init(const std::vector<std::vector<int>>& mapSystem) {
    grafo.limpiarNodos(); // Limpia el grafo, incluso si ya está vacío
    grafo.inicializar(mapSystem); // Llama al método `inicializar` del grafo para crear nodos y conexiones
    filas = mapSystem.size();
    columnas = mapSystem[0].size();

}

/**
 * @brief Agregar Nodo
 * Crea y agrega un nodo al grafo ya existente, ademas lo conecta al resto de nodos y lo agrega al nido correspondiente
 * 
 * @param x 
 * @param y 
 * @return Nodo* 
 */
Nodo *IASystem::agregarNodo(int x, int y) {
    auto nodo = grafo.crearNodo(grafo.getLastNodoId() + 1, x, y);
    grafo.instalarNodo(x,y);
    agregarNodoANido(x, y);

    return nodo;
}

/**
 * @brief Agregar Nodo a Nido
 * Conecta el nodo a un niddo segun sus conexiones.
 * 
 * @param x 
 * @param y 
 */
void IASystem::agregarNodoANido(int x, int y) {
    auto nodo = grafo.getNodoByPos(x,y);
    grafo.integrarNodoInNido(*nodo);
}

/**
 * @brief Agregar Conexion
 * Engancha el nodo Desde al nodo Hacia y a la inversa con el coste especificado
 * 
 * @param idDesde 
 * @param idHacia 
 * @param coste 
 */
void IASystem::agregarConexion(int idDesde, int idHacia, double coste) {
    Nodo *nodoDesde = grafo.getNodoById(idDesde);
    Nodo *nodoHacia = grafo.getNodoById(idHacia);

    if (!nodoDesde || !nodoHacia)
    {
        throw std::runtime_error("Los nodos especificados no existen.");
    }

    nodoDesde->addConexion(nodoHacia, coste);
}

/**
 * @brief Obtner nodo por ID 
 * Recibe una id y devuelve el nodo en el grafo
 * @param id 
 * @return Nodo* 
 */
Nodo *IASystem::obtenerNodoPorId(int id) const {
    return grafo.getNodoById(id);
}

/**
 * @brief Inicializa y construye el árbol de comportamiento (Behavior Tree) para la IA del NPC.
 * 
 * Esta función asigna e interconecta los distintos nodos de decisión y acción que conforman
 * el comportamiento de un NPC utilizando una estructura jerárquica de nodos (Root, Composites y Leafs).
 * 
 * Se definen varios modos principales de comportamiento, incluyendo:
 * 
 * - **Huir**: cuando el NPC está en peligro.
 * - **Camino**: exploración y destrucción de bloques.
 * - **Wait**: comportamiento pasivo si no hay otras acciones útiles.
 * - **Atacar**: búsqueda activa de enemigos para colocar bombas.
 * - **RecogerPowerup**: moverse a recoger potenciadores.
 * 
 * También se configura un árbol alternativo (`rootModoFoco`) para un modo de comportamiento concreto
 * que se ejecuta cuando el NPC esta en modo Foco:
 * 
 * - **EscogerFoco**: determinar un objetivo prioritario.
 * - **MoverFoco**: desplazarse hacia el enemigo.
 * - **AtacarFoco**: paralizar al enemigo.
 * 
 * @param em Referencia al EntityManager que gestiona las entidades del juego.
 * @param entidad Referencia a la entidad (NPC) para la cual se configura la IA.
 */
void IASystem::crearUtility(EntityManager& em, Entity& entidad){
    
    auto &iaComponent = em.ia.get(entidad.ia_key);

    iaComponent.root            = std::make_shared<Root>();

    iaComponent.Huir            = std::make_shared<Huir>();
    iaComponent.Camino          = std::make_shared<Camino>();
    iaComponent.Wait            = std::make_shared<Wait>();
    iaComponent.Atacar          = std::make_shared<Atacar>();
    iaComponent.RecogerPowerup  = std::make_shared<RecogerPowerup>();

    iaComponent.moverNPCAtacar        = std::make_shared<MoverNPC>();
    iaComponent.colocarBombaAtacar    = std::make_shared<ColocarBomba>();
    iaComponent.crearCaminoAtacar     = std::make_shared<CrearCamino>();

    iaComponent.moverNPCCamino        = std::make_shared<MoverNPC>();
    iaComponent.colocarBombaCamino    = std::make_shared<ColocarBomba>();
    iaComponent.crearCaminoCamino     = std::make_shared<CrearCamino>();

    iaComponent.moverNPCHuir        = std::make_shared<MoverNPC>();
    iaComponent.crearCaminoHuir     = std::make_shared<CrearCamino>();

    iaComponent.moverNPCPw        = std::make_shared<MoverNPC>();
    iaComponent.crearCaminoPw     = std::make_shared<CrearCamino>();

    iaComponent.pararNPC     = std::make_shared<PararNPC>();

    iaComponent.root->addChild(iaComponent.Huir);
    iaComponent.root->addChild(iaComponent.Camino);
    iaComponent.root->addChild(iaComponent.Wait);
    iaComponent.root->addChild(iaComponent.Atacar);
    iaComponent.root->addChild(iaComponent.RecogerPowerup);

    iaComponent.Huir->addChild(iaComponent.crearCaminoHuir);
    iaComponent.Huir->addChild(iaComponent.moverNPCHuir);

    iaComponent.Camino->addChild(iaComponent.crearCaminoCamino);
    iaComponent.Camino->addChild(iaComponent.moverNPCCamino);
    iaComponent.Camino->addChild(iaComponent.colocarBombaCamino);

    iaComponent.Atacar->addChild(iaComponent.crearCaminoAtacar);
    iaComponent.Atacar->addChild(iaComponent.moverNPCAtacar);
    iaComponent.Atacar->addChild(iaComponent.colocarBombaAtacar);

    iaComponent.RecogerPowerup->addChild(iaComponent.crearCaminoPw);
    iaComponent.RecogerPowerup->addChild(iaComponent.moverNPCPw);

    iaComponent.Wait->addChild(iaComponent.pararNPC);

    iaComponent.rootModoFoco    = std::make_shared<Foco>();
    iaComponent.escogerFoco     = std::make_shared<EscogerFoco>();
    iaComponent.moverFoco       = std::make_shared<MoverFoco>();
    iaComponent.atacarFoco      = std::make_shared<AtacarFoco>();

    iaComponent.rootModoFoco->addChild(iaComponent.escogerFoco);
    iaComponent.rootModoFoco->addChild(iaComponent.moverFoco);
    iaComponent.rootModoFoco->addChild(iaComponent.atacarFoco);
}

/**
 * @brief Inicializa y construye el árbol de comportamiento (Behavior Tree) para la IA del NPC en el modo pintura.
 * 
 * Esta función asigna e interconecta los distintos nodos de decisión y acción que conforman
 * el comportamiento de un NPC utilizando una estructura jerárquica de nodos (Root, Composites y Leafs).
 * 
 * Se definen varios modos principales de comportamiento, incluyendo:
 * 
 * - **HuirPintura**: cuando el NPC está en peligro.
 * - **CaminoPintura**: exploración y destrucción de bloques.
 * - **WaitPintura**: comportamiento pasivo si no hay otras acciones útiles.
 * - **AtacarPintura**: búsqueda activa de enemigos para colocar bombas.
 * - **RecogerPowerupPintura**: moverse a recoger potenciadores.
 * 
 * @param em Referencia al EntityManager que gestiona las entidades del juego.
 * @param entidad Referencia a la entidad (NPC) para la cual se configura la IA.
 */
void IASystem::crearUtilityPintura(EntityManager& em, Entity& entidad){
    auto &iaComponent = em.ia.get(entidad.ia_key);

    iaComponent.RootPintura                   = std::make_shared<Root>();

    iaComponent.HuirPintura            = std::make_shared<Huir>();
    iaComponent.CaminoPintura          = std::make_shared<Camino>();
    iaComponent.WaitPintura            = std::make_shared<Wait>();
    iaComponent.AtacarPintura          = std::make_shared<AtacarPintura>();           //Nuevo nodo
    iaComponent.RecogerPowerupPintura  = std::make_shared<RecogerPowerup>();

    iaComponent.moverNPC        = std::make_shared<MoverNPC>();
    iaComponent.colocarBomba    = std::make_shared<ColocarBomba>();
    iaComponent.crearCamino     = std::make_shared<CrearCamino>();

    iaComponent.pararNPC        = std::make_shared<PararNPC>();

    iaComponent.RootPintura->addChild(iaComponent.HuirPintura);
    iaComponent.RootPintura->addChild(iaComponent.CaminoPintura);
    iaComponent.RootPintura->addChild(iaComponent.WaitPintura);
    iaComponent.RootPintura->addChild(iaComponent.AtacarPintura);                     //Nuevo nodo
    iaComponent.RootPintura->addChild(iaComponent.RecogerPowerupPintura);

    iaComponent.HuirPintura->addChild(iaComponent.crearCamino);
    iaComponent.HuirPintura->addChild(iaComponent.moverNPC);

    iaComponent.CaminoPintura->addChild(iaComponent.crearCamino);
    iaComponent.CaminoPintura->addChild(iaComponent.moverNPC);
    iaComponent.CaminoPintura->addChild(iaComponent.colocarBomba);

    iaComponent.AtacarPintura->addChild(iaComponent.crearCamino);          // Nuevo nodo
    iaComponent.AtacarPintura->addChild(iaComponent.moverNPC);             // Nuevo nodo
    iaComponent.AtacarPintura->addChild(iaComponent.colocarBomba);         // Nuevo nodo

    iaComponent.RecogerPowerupPintura->addChild(iaComponent.crearCamino);
    iaComponent.RecogerPowerupPintura->addChild(iaComponent.moverNPC);

    iaComponent.WaitPintura->addChild(iaComponent.pararNPC);
}

/**
 * @brief Ejecuta la IA del NPC en un intervalo de tiempo limitado (slice). Parte del Scheduling
 * 
 * Esta función permite ejecutar la lógica de IA en "pedazos" o slices para evitar
 * bloqueos largos en el hilo principal del juego. La ejecución se repetirá hasta que
 * se complete la actualización de la IA o se agote el tiempo máximo asignado para este slice.
 * 
 * @param em Referencia al EntityManager que gestiona las entidades.
 * @param entidad Referencia a la entidad (NPC) cuya IA se va a ejecutar.
 * @param mapa Referencia al mapa del juego (matriz de casillas).
 * @param bombsystem Referencia al sistema de bombas.
 * @param mapsystem Referencia al sistema de manejo de mapa.
 * @param tiempoMaximo Tiempo máximo en segundos para ejecutar este slice de IA.
 */
void IASystem::ejecutarIASlice(EntityManager &em, Entity &entidad,  std::vector<std::vector<int>> &mapa, BombSystem &bombsystem, MapSystem &mapsystem, float tiempoMaximo) {
    auto start = std::chrono::high_resolution_clock::now();
    bool completado = false;
    while (!completado) {
        completado = iaStep(em, entidad, mapa, bombsystem, mapsystem);
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = now - start;
        if (elapsed.count() >= tiempoMaximo) {
            break;  // Se agotó el tiempo asignado para este slice
        }
    }
}

/**
 * @brief Ejecuta un paso individual de la actualización de la IA del NPC. Parte del Scheduling
 * 
 * Esta función realiza la actualización en fases, dividida en varios pasos controlados
 * mediante `sliceStep` en el componente IA, permitiendo dividir la lógica compleja en partes.
 * 
 * El método evalúa el estado del NPC y actualiza variables internas relacionadas con la peligrosidad,
 * distancia a power-ups, enemigos cercanos, y otros parámetros que influirán en la toma de decisiones.
 * Luego ejecuta el árbol de comportamiento correspondiente, diferenciando entre estados normales y "fantasma".
 * 
 * @param em Referencia al EntityManager.
 * @param entidad Referencia a la entidad (NPC) cuya IA se actualiza.
 * @param mapa Referencia al mapa del juego.
 * @param bombsystem Referencia al sistema de bombas.
 * @param mapsystem Referencia al sistema del mapa.
 * @return `true` si el paso de IA ha terminado completamente, `false` si requiere continuar en el siguiente slice.
 */
bool IASystem::iaStep(EntityManager &em, Entity &entidad, std::vector<std::vector<int>> &mapa, BombSystem &bombsystem, MapSystem &mapsystem) {
    auto &iaComponent = em.ia.get(entidad.ia_key);
    auto &fisicaComponent = em.fisicas.get(entidad.fisica_key);


    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::milliseconds(50);

    if (iaComponent.sliceStep == 0) {
        if(!fisicaComponent.esFantasma){
            iaComponent.peligrosidad            = peligrosidadEnUnPunto(em,entidad,mapa);           // Peligrosidad de la casilla actual
            iaComponent.distanciaPW             = distanciaPowerUp(em,entidad);              // Distancia al powerup
            iaComponent.pwObtenidos             = pwObtenidos(em,entidad);
            iaComponent.areaNidos               = calcularEspacioLibre(em,entidad,mapa);             // Area de nidos
            iaComponent.bloquesTotales          = bloquesTotales(em,mapa);          // Bloques totales
            iaComponent.distanciaNPC            = enemigosCercanos(em,entidad);          // Distancia a los enemigos
            iaComponent.bombaCercana            = bombasCercanas(em,entidad,mapa);          // Distancia a las bomba
            iaComponent.puedePW                 = alcanzaUnPW(em, entidad, mapa); // Indica si el NPC puede recoger un powerup
            iaComponent.fuegoCercano            = calculoFuegoCercano(em, entidad, mapa);
        }else{
            iaComponent.rootModoFoco->run(mapa, em, entidad, *this, bombsystem, mapsystem, end);
        }
        iaComponent.sliceStep = 1;
        return false;
    }
    else if (iaComponent.sliceStep == 1) {
        if(iaComponent.root != nullptr){
            if(!fisicaComponent.esFantasma){
                iaComponent.root->evaluateAndExecute(mapa, em, entidad, *this, bombsystem, mapsystem, end);
            }else{
                iaComponent.rootModoFoco->run(mapa, em, entidad, *this, bombsystem, mapsystem, end);
            }
            iaComponent.sliceStep = 2;

            return false;
        }
    }
    else if (iaComponent.sliceStep == 2) {
        // Paso 2: Evaluar y ejecutar el selector de acciones.
        if(!fisicaComponent.esFantasma){
            iaComponent.root->evaluateAndExecute(mapa, em, entidad, *this, bombsystem, mapsystem, end);
        }else{
            iaComponent.rootModoFoco->run(mapa, em, entidad, *this, bombsystem, mapsystem, end);
        }
        iaComponent.sliceStep = 0; // Reiniciar para la siguiente actualización.
        return true;
    }
    return true;
}

/**
 * @brief Ejecuta un paso individual de la actualización de la IA del NPC. Parte del Scheduling
 * 
 * Esta función realiza la actualización en fases, dividida en varios pasos controlados
 * mediante `sliceStep` en el componente IA, permitiendo dividir la lógica compleja en partes.
 * 
 * El método evalúa el estado del NPC y actualiza variables internas relacionadas con la peligrosidad,
 * distancia a power-ups, enemigos cercanos, y otros parámetros que influirán en la toma de decisiones.
 * Luego ejecuta el árbol de comportamiento correspondiente, diferenciando entre estados normales y "fantasma".
 * 
 * @param em Referencia al EntityManager.
 * @param entidad Referencia a la entidad (NPC) cuya IA se actualiza.
 * @param mapa Referencia al mapa del juego.
 * @param bombsystem Referencia al sistema de bombas.
 * @param mapsystem Referencia al sistema del mapa.
 * @return `true` si el paso de IA ha terminado completamente, `false` si requiere continuar en el siguiente slice.
 */
bool IASystem::iaStepPintura(EntityManager &em, Entity &entidad, std::vector<std::vector<int>> &mapa, BombSystem &bombsystem, MapSystem &mapsystem) {
    auto &iaComponent = em.ia.get(entidad.ia_key);
    auto &fisicaComponent = em.fisicas.get(entidad.fisica_key);

    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::milliseconds(50); // Tiempo límite para ejecutar

    if (iaComponent.sliceStep == 0) {
        iaComponent.peligrosidad       = peligrosidadEnUnPunto(em, entidad, mapa);
        iaComponent.distanciaPW        = distanciaPowerUp(em, entidad);
        iaComponent.pwObtenidos        = pwObtenidos(em, entidad);
        iaComponent.areaNidos          = calcularEspacioLibre(em,entidad,mapa);             // Area de nidos
        iaComponent.bloquesTotales     = bloquesTotales(em, mapa);
        iaComponent.distanciaNPC       = enemigosCercanos(em, entidad);
        iaComponent.bombaCercana       = bombasCercanas(em, entidad, mapa);
        iaComponent.puedePW            = alcanzaUnPW(em, entidad, mapa); // Indica si el NPC puede recoger un powerup
        iaComponent.fuegoCercano       = calculoFuegoCercano(em, entidad, mapa);



        iaComponent.sliceStep = 1;
        return false;
    } else if (iaComponent.sliceStep == 1) {
        // Paso 1: Evaluar y ejecutar comportamiento RootPintura
        if (iaComponent.RootPintura != nullptr) {
            iaComponent.RootPintura->evaluateAndExecute(mapa, em, entidad, *this, bombsystem, mapsystem, end);
        }

        iaComponent.sliceStep = 0; 
        return true;
    }

    return true; 
}

/**
 * @brief Ejecuta la IA del NPC en un intervalo de tiempo limitado (slice). Parte del Scheduling para el modo pintura
 * 
 * Esta función permite ejecutar la lógica de IA en "pedazos" o slices para evitar
 * bloqueos largos en el hilo principal del juego. La ejecución se repetirá hasta que
 * se complete la actualización de la IA o se agote el tiempo máximo asignado para este slice.
 * 
 * @param em Referencia al EntityManager que gestiona las entidades.
 * @param entidad Referencia a la entidad (NPC) cuya IA se va a ejecutar.
 * @param mapa Referencia al mapa del juego (matriz de casillas).
 * @param bombsystem Referencia al sistema de bombas.
 * @param mapsystem Referencia al sistema de manejo de mapa.
 * @param tiempoMaximo Tiempo máximo en segundos para ejecutar este slice de IA.
 */
void IASystem::ejecutarIASlicePintura(EntityManager &em, Entity &entidad, std::vector<std::vector<int>> &mapa, BombSystem &bombsystem, MapSystem &mapsystem, float tiempoMaximo) {
    auto &fisica = em.fisicas.get(entidad.fisica_key);

    if(!fisica.estaMuerto){
        auto start = std::chrono::high_resolution_clock::now();
        bool completado = false;
        while (!completado) {
            completado = iaStepPintura(em, entidad, mapa, bombsystem, mapsystem);
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsed = now - start;
            if (elapsed.count() >= tiempoMaximo) {
                break;
            }
        }
    }
}

/**
 * @brief Actualizar Camino
 * Recoge el camino de la entidad y va actualizado su posicion respecto al siguiente nodo en el camino
 * Calcula su velocidada y la mete en el componente de fisicas del npc
 * 
 * @param em 
 * @param entidad 
 */
void IASystem::actualizarCamino(EntityManager &em, Entity &entidad) {
    auto &iaComponent = em.ia.get(entidad.ia_key);
    auto &fisica = em.fisicas.get(entidad.fisica_key);
    int posNPCTileX = fisica.tileZ;
    int posNPCTileY = fisica.tileX;

    //! Si el camino esta vacio se pone velocidad a 0 y return.
    if (iaComponent.camino.empty()){
        fisica.vx = 0;
        fisica.vz = 0;
        return;
    }

    // Extraemos los nodos del camino   
    MiVector2 nodoActual = {static_cast<float>(iaComponent.camino.at(0)->getFrom()->getPosX()),
                            static_cast<float>(iaComponent.camino.at(0)->getFrom()->getPosY())};
    MiVector2 nodoSiguiente = {static_cast<float>(iaComponent.camino.at(0)->getTo()->getPosX()),
                               static_cast<float>(iaComponent.camino.at(0)->getTo()->getPosY())};

    iaComponent.siguienteCaminoX = nodoSiguiente.x;
    iaComponent.siguienteCaminoZ = nodoSiguiente.y;

    // Si estamos en el nodo actual, avanzamos al siguiente
    if (posNPCTileX == nodoSiguiente.x && posNPCTileY == nodoSiguiente.y) {
        iaComponent.camx.clear();
        iaComponent.camz.clear();
        // Iteramos sobre los nodos restantes del camino para recoger su posicion
        for (const auto& nodo : iaComponent.camino) {
            iaComponent.camx.push_back(nodo->getFrom()->getPosX());
            iaComponent.camz.push_back(nodo->getFrom()->getPosY());
        }

        iaComponent.camino.erase(iaComponent.camino.begin()); // Eliminamos el nodo visitado
        // TODO: En caso de completar el camino ponemos poner Bomba a true
        if (iaComponent.camino.empty()) {
                iaComponent.inPositionToPlaceBomb = true;
        }
    }

    // Calculamos la dirección del movimiento hacia el nodo siguiente
    MiVector2 direccion = {nodoSiguiente.x - posNPCTileX, nodoSiguiente.y - posNPCTileY};
    // Limitamos el movimiento a los ejes cardinales
    if (direccion.x != 0) {
        if (direccion.x > 0) {direccion.x = K_speed;}
        else {direccion.x = -K_speed;}
        direccion.y = 0; // Forzar movimiento solo en X
    }
    else if (direccion.y != 0) {
        if (direccion.y > 0) { direccion.y = K_speed; }
        else{ direccion.y = -K_speed; }
        direccion.x = 0; // Forzar movimiento solo en Y
    }
    fisica.vx = direccion.x;
    fisica.vz = direccion.y;
}

/**
 * @brief Bombas cercanas 
 * Calcula la peligrosidad en cuanto a las bombas cercanas.
 * Devuelve un valor entre 0-1.
 * 
 * @param em 
 * @param entidad 
 * @param mapa 
 * @return float 0-1
 */
float IASystem::bombasCercanas(EntityManager& em, Entity& entidad, std::vector<std::vector<int>>& mapa) const {
    auto &iaComponent = em.ia.get(entidad.ia_key);
    auto &fisica = em.fisicas.get(entidad.fisica_key);

    int posX = fisica.tileX;
    int posZ = fisica.tileZ;
    int radio = 3;  //Deberia de ir con la personalidad
    double sumaBombas = 0.0f;
    double sumaFuegos = 0.0f;
    double alpha = 2.0f;
    double beta = 10.0f;

    //Recorre los bloques en forma de radio y suma segun vea fuegos o bombas
    for(int w=0;w<radio;w++){
        for(int i=posZ-w;i<=posZ+w;i++){
            for(int j=posX-w;j<=posX+w;j++){
                if(i < mapa.size() && i >= 0 && j < mapa[0].size() && j >= 0 ){
                    if(i==posZ-w || i==posZ+w || j==posX-w || j==posX+w){
                        if(mapa[i][j] == BOMBA) {//BOMBA
                            sumaBombas += 1.0 / (1 + w);
                        } else if (mapa[i][j] == FUEGO) {//FUEGO
                            sumaFuegos += 1.0 / pow(1 + w, 2);
                        }
                    }
                }
            }
        }
    }
    // Aplicamos la fórmula final
    if(sumaBombas+sumaFuegos <= 0){
        return 0.0f;
    }
    return 1 - exp(-(alpha * sumaBombas + beta * sumaFuegos));
}

/**
 * @brief Enemigos Cercanos
 * Recoge los enemigos y calcula lo peligroso en funcion de su cercania
 * 
 * @param em 
 * @param entidad 
 * @return float 
 */
float IASystem::enemigosCercanos(EntityManager& em, Entity& entidad) const {
    auto &iaComponent = em.ia.get(entidad.ia_key);
    auto &fisica = em.fisicas.get(entidad.fisica_key);

    std::vector<std::pair<int,int>> enemigosPos;

    for(auto& e : em.getEnemies(entidad)){
        if(e.type == EntityType::NPC || e.type == EntityType::PLAYER){
            if (!em.fisicas.get(e.fisica_key).estaMuerto) {
                enemigosPos.push_back({em.fisicas.get(e.fisica_key).tileX, em.fisicas.get(e.fisica_key).tileZ});
            }
        }        
    }

    //! Si no hay enemigos return 0
    if (enemigosPos.size() == 0) return 0.0f;

    float peligrosidad = 0.0f;
    float pesoMaximo = 1.0f / enemigosPos.size();  // Cada enemigo aporta hasta 1/N

    for (int i = 0; i < enemigosPos.size(); i++) {
        int ex = enemigosPos[i].first;
        int ez = enemigosPos[i].second;

        // Calcular distancia Manhattan
        int distancia = std::abs(ex - fisica.tileX) + std::abs(ez - fisica.tileZ);

        // Evitar división por cero si está en la misma posición
        if (distancia == 0) distancia = 1;

        // Sumar contribución de este enemigo
        peligrosidad += pesoMaximo / (1.0f + distancia);
    }

    return peligrosidad;
}

/**
 * @brief Peligrosidad en un Punto
 * Utiliza el calculo de bombas cercanas y enemigos cercanos para calcular la peligrosidad general en un punto
 * Devuelve un valor entre 0-1
 * 
 * @param em 
 * @param entidad 
 * @param mapa 
 * @return float 0-1
 */
float IASystem::peligrosidadEnUnPunto(EntityManager& em, Entity& entidad, std::vector<std::vector<int>>& mapa) const {
    return (0.6f * bombasCercanas(em, entidad, mapa)) + (0.4f * enemigosCercanos(em ,entidad));
}

/**
 * @brief Calcular Espacio Libre
 * 
 * 
 * @param em 
 * @param entidad 
 * @param mapa 
 * @return float 
 */
float IASystem::calcularEspacioLibre(EntityManager& em, Entity& entidad, std::vector<std::vector<int>>& mapa) const {
    auto &iaComponent = em.ia.get(entidad.ia_key);
    auto &fisica = em.fisicas.get(entidad.fisica_key);
    int posX = fisica.tileX;
    int posZ = fisica.tileZ;
    int radio = 3;
    float freeSpace = 0.0f;
    float calculatedSpace = 0.0f;

    for(int i=posX - radio; i<=posX + radio;i++) {
        for(int j=posZ - radio;j<=posZ + radio;j++) {
            if(enRangoMapa(i,j)) {
                if(mapa[i][j] != SOLIDO) {
                    calculatedSpace++;
                    if(casillaAtravesable(i,j,mapa)) {
                        freeSpace++;
                    }
                }
            }
        }
    }

    return (freeSpace/calculatedSpace);
}

/**
 * @brief Calcula una medida de influencia de los power-ups cercanos sobre una entidad.
 * 
 * Esta función evalúa la cercanía de todos los power-ups existentes en el mapa respecto
 * a una entidad concreta y devuelve un valor proporcional inversamente a la distancia.
 * 
 * - Si no hay power-ups disponibles en el mapa, retorna 0.0f.
 * - La influencia de cada power-up se calcula como `pesoMaximo / (1 + distancia)`,
 *   donde `pesoMaximo = 1 / número_de_powerups`.
 * - La distancia es la **Manhattan** (|x1 - x2| + |z1 - z2|).
 * 
 * @param em Referencia al EntityManager que contiene todas las entidades del juego.
 * @param entidad Entidad (NPC) desde la cual se mide la distancia.
 * 
 * @return float Valor de influencia acumulada basado en la cercanía de los power-ups.
 */
float IASystem::distanciaPowerUp(EntityManager& em, Entity& entidad) const {
    auto &fisica = em.fisicas.get(entidad.fisica_key);

    std::vector<std::pair<int, int>> powerUpsPos;

    for (auto& p : em.getEntitiesByType(EntityType::POWERUP)) {
        powerUpsPos.push_back({em.fisicas.get(p.fisica_key).tileX, em.fisicas.get(p.fisica_key).tileZ});
    }

    int x = fisica.tileX;
    int z = fisica.tileZ;

    // Si no hay power-ups, retorno 0.0f
    if (powerUpsPos.empty()) return 0.0f;

    // Calcular el valor basado en la distancia de los power-ups
    float influencia = 0.0f;
    float pesoMaximo = 1.0f / powerUpsPos.size();  // Cada power-up aporta hasta 1/N

    for (const auto& [px, pz] : powerUpsPos) {
        int distancia = std::abs(px - x) + std::abs(pz - z);
        if (distancia == 0) distancia = 1;  // Evitar división por cero

        influencia += pesoMaximo / (1.0f + distancia);
    }

    return influencia;
}

/**
 * @brief Bloques Totales
 * Calcula el total de bloques rompibles en el mapa
 * Devuelve un valor entre 0-1 
 * @param em 
 * @param mapa 
 * @return float 0-1
 */
float IASystem::bloquesTotales(EntityManager& em, std::vector<std::vector<int>>& mapa) const {
    int bloques = 0;
    for (int i = 0; i < mapa.size(); i++) {
        for (int j = 0; j < mapa[i].size(); j++) {
            if (bloqueRompible(i,j,mapa)) {
                bloques++;
            }
        }
    }
    return   static_cast<float>(bloques) / B_MAX;
}

/**
 * @brief Alcanza un PW
 * Busca en todo el mapa si es posible alcanzar al menos un powerup con un camino habil
 * Metodo booleano devuelve true si encuentra y false sino.
 * 
 * @param em 
 * @param entidad 
 * @param mapa 
 * @return true 
 * @return false 
 */
bool IASystem::alcanzaUnPW(EntityManager& em, Entity& entidad, std::vector<std::vector<int>>& mapa) const {
    auto &fisicaPersonaje = em.fisicas.get(entidad.fisica_key);
    auto nodoStart = grafo.getNodoByPos(fisicaPersonaje.tileZ, fisicaPersonaje.tileX);
    if(nodoStart == nullptr){
        return false;
    }

    SimpleHeuristic heuristic;
    std::vector<Conexion*> elVeldaeroCamino;
    for(int i=0;i<mapa.size(); i++){
        for(int j=0;j<mapa[0].size(); j++){
            if(mapa[i][j] > 9 && mapa[i][j] < 18) {
                const auto nodoFinish = grafo.getNodoByPos(i,j);
                elVeldaeroCamino = AStar::pathfindAStar(const_cast<Grafo&>(grafo), nodoStart, nodoFinish, heuristic);
                if(elVeldaeroCamino.size()>0) 
                    return true;
                }
        }
    }
    return false;
}

/**
 * @brief PW Obtenidos
 * Recoge el numero de powerUps que tiene un NPC.
 * Devuelve un valor entre 0-1
 *  
 * @param em 
 * @param entidad 
 * @return float 0-1
 */
float IASystem::pwObtenidos(EntityManager& em, Entity& entidad) const {
    auto &pwComponent = em.powerUps.get(entidad.powerUp_key);
    return static_cast<float>(pwComponent.collectedPowerUps.size()) / P_MAX;
}

/**
 * @brief Evalúa las casillas del mapa en función de un objetivo determinado y selecciona la mejor ruta mediante A*.
 * 
 * Esta función calcula una puntuación para cada casilla del mapa que sea atravesable, según un criterio (`id`) 
 * que representa un tipo de comportamiento o estrategia para la IA (NodoPrimerNivel). Luego, utiliza el algoritmo A* para 
 * encontrar una ruta desde la posición actual hasta la casilla con mayor puntuación alcanzable.
 * 
 * Los tipos de objetivo (`id`)(NodoPrimerNivel) que la IA puede tener son:
 * - 0: Destrucción (evaluar el potencial de destruir bloques cercanos).
 * - 1: Huida (priorizar seguridad, alejamiento de peligro).
 * - 2: Recolectar Power-Up.
 * - 3: Atacar (colocar bombas para dañar enemigos).
 * - 4: Pintar (cubrir zonas con pintura propia).
 * 
 * @param x Coordenada X actual del agente.
 * @param y Coordenada Y actual del agente.
 * @param mapa Mapa del juego (matriz de enteros con la información de celdas).
 * @param mapaColor Mapa de colores si el objetivo es pintar (modo 4).
 * @param em Referencia al EntityManager (maneja entidades del juego).
 * @param entidad Entidad controlada por IA que ejecuta esta función.
 * @param id Tipo de objetivo que se quiere cumplir.
 * 
 * @return std::vector<Conexion*> Ruta resultante encontrada con A*, ordenada desde la posición actual al destino.
 */
std::vector<Conexion*> IASystem::valorarCasillas(int x, int y,  const std::vector<std::vector<int>>& mapa, const std::vector<std::vector<int>>& mapaColor, EntityManager& em, Entity& entidad, int id) const{
    auto &fisica = em.fisicas.get(entidad.fisica_key);
    auto &iacomponentVariables = em.ia.get(entidad.ia_key);
    // Inicializa el mapa de probabilidades con -1
    if (iacomponentVariables.mapaProbabilidades.empty()) 
    iacomponentVariables.mapaProbabilidades.assign(mapa.size(), std::vector<float>(mapa[0].size(), -1.0f));

    if (iacomponentVariables.mapaProbabilidades1.empty()) 
        iacomponentVariables.mapaProbabilidades1.assign(mapa.size(), std::vector<float>(mapa[0].size(), -1.0f));

    if (iacomponentVariables.mapaProbabilidades2.empty()) 
        iacomponentVariables.mapaProbabilidades2.assign(mapa.size(), std::vector<float>(mapa[0].size(), -1.0f));

    if (iacomponentVariables.mapaProbabilidades3.empty()) 
        iacomponentVariables.mapaProbabilidades3.assign(mapa.size(), std::vector<float>(mapa[0].size(), -1.0f));

    if (iacomponentVariables.mapaProbabilidades4.empty()) 
        iacomponentVariables.mapaProbabilidades4.assign(mapa.size(), std::vector<float>(mapa[0].size(), -1.0f));
    
    float distancia = 0.0f;
    float calculo = 0.0f;
    float auxPuntuacion = 0.0f;
    std::vector<std::tuple<int,int,float>> puntuacionPorCasilla;

    //METEMOS EL RESTO DE ENEMIGOS PARA CALCULAR CON ELLOS
    std::array<std::pair<int,int>,3> enemigosPos;
    int counter = 0;
    for(auto e : em.getEntitiesByType(EntityType::NPC)){
        if(e.getId() != entidad.getId()){
            enemigosPos[counter] = {em.fisicas.get(e.fisica_key).tileX, em.fisicas.get(e.fisica_key).tileZ};
            counter++;
        }
    }

    enemigosPos[counter] = {em.fisicas.get(em.getEntitiesByType(EntityType::PLAYER)[0].fisica_key).tileX, em.fisicas.get(em.getEntitiesByType(EntityType::PLAYER)[0].fisica_key).tileZ};

    for(int i=0;i<mapa.size();i++){
        for(int j=0;j<mapa[0].size();j++){
            if(casillaAtravesable(i,j,mapa)){
                switch (id) {
                    case 0: //CAMINO
                        distancia = distanciaNormalizada(x, y, i , j);
                        calculo = puntosDestruccionCruz(i,j,mapa) * (1.0f - distancia);
                        puntuacionPorCasilla.push_back({i, j, calculo});
                        iacomponentVariables.mapaProbabilidades[i][j] = calculo;
                        break;

                    case 1: //HUIR
                    //ANOTACION PARA ESCUTIA
                    //SEGURAMENTE HAYA QUE CAMBIAR EL VALOR DE SEGURIDAD DE CASILLA PORQUE CREO QUE DA EL VALOR INVERSO
                        distancia = distanciaNormalizada(x, y, i , j);
                        auxPuntuacion = -puntosSeguridadCasilla(i, j, mapa, em) + puntosAlineamientoEscape( std::pair<int,int>{fisica.tileX, fisica.tileZ}, std::pair<int,int>{i, j}, std::pair<int,int>{fisica.vx, fisica.vz});
                        calculo = auxPuntuacion * (1.0f - distancia);
                        puntuacionPorCasilla.push_back({i, j, calculo});

                        iacomponentVariables.mapaProbabilidades1[i][j] = calculo;
                        break;

                    case 2: //POWER UP
                        if (puntosPowerUp(i, j, mapa)) {
                            distancia = distanciaNormalizada(x, y, i , j);
                            calculo = 1.0f - distancia;
                            puntuacionPorCasilla.push_back({i, j, calculo});
                            iacomponentVariables.mapaProbabilidades2[i][j] = calculo;
                        }
                        break;

                    case 3: //ATACAR
                        distancia = distanciaNormalizada(x, y, i , j);
                        calculo =  puntosAtacarCelda(std::make_pair(i,j), std::make_pair(x,y),mapa,enemigosPos) * (1.0f - distancia);
                        puntuacionPorCasilla.push_back({i, j, calculo});  

                        iacomponentVariables.mapaProbabilidades3[i][j] = calculo;                   
                        break;

                    case 4: //PINTAR
                        distancia = distanciaNormalizada(x, y, i , j);
                        calculo =  puntosColorear(i , j,mapa, mapaColor, em.getPinturaId(entidad.getId())) * (1.0f - distancia);
                        puntuacionPorCasilla.push_back({i, j, calculo});                 
                        break;

                    default:
                        distancia = distanciaNormalizada(x, y, i , j);
                        calculo = puntosDestruccionCruz(i,j,mapa) * (1.0f - distancia);
                        puntuacionPorCasilla.push_back({i, j, calculo});
                        iacomponentVariables.mapaProbabilidades[i][j] = calculo;
                        break;
                }
            }
        }
    }

    //Ordena las puntuaciones de los boques validos de mayor a menor.
    std::sort(puntuacionPorCasilla.begin(), puntuacionPorCasilla.end(), 
        [](const std::tuple<int, int, float>& a, const std::tuple<int, int, float>& b) {
            return std::get<2>(a) > std::get<2>(b);
        });

    std::vector<Conexion*> elVeldaeroCamino;

    auto nodoStart = grafo.getNodoByPos(x,y);
    SimpleHeuristic heuristic;
    
    //Prueba en cada una de las puntuaciones hasta que encuentra una buena y la utiliza
    for(int i=0; i<puntuacionPorCasilla.size(); i++){
        auto nodoFinish = grafo.getNodoByPos(std::get<0>(puntuacionPorCasilla[i]),std::get<1>(puntuacionPorCasilla[i]));
        elVeldaeroCamino = AStar::pathfindAStar(grafo, nodoStart, nodoFinish, heuristic);
        if(elVeldaeroCamino.size()>0) 
            break;
    }

    return elVeldaeroCamino;    
}

/**
 * @brief Puntos destruccion Cruz
 * Valora una casilla por la detruccion que puede ocasionar
 * Si rompe un bloque suma +0.33
 * Si abre un camino calcula cuanto de grande es la zona a desbloquear y lo suma
 * 
 * @param x 
 * @param y 
 * @param mapa 
 * @return float 
 */
float IASystem::puntosDestruccionCruz(int x, int y, const std::vector<std::vector<int>>& mapa) const {
    int rango = 4;

    std::array<int,4> Up_Right_Down_Left {0,0,0,0};
    int freeSpaceCounter = 0;

    int xx = x;
    int yy = y;

    for (int i = 0; i < rango; i++) {
        for (int j = 0; j < 4; j++) {
            switch (j) {
                case 0: xx = x-i; yy = y; break;
                case 1: xx = x; yy = y+i; break;
                case 2: xx = x+i; yy = y; break;
                case 3: xx = x; yy = y-i; break;
                default:                  break;
            }
            if (Up_Right_Down_Left[j] != 1 && Up_Right_Down_Left[j] != 3 && (enRangoMapa(xx,yy))) {
                if (mapa[xx][yy] == SOLIDO) {
                    //Bloque irrompible
                    Up_Right_Down_Left[j] = 1;
                }else if(Up_Right_Down_Left[j] == 2 && bloqueRompible(xx,yy,mapa)) {
                    Up_Right_Down_Left[j] = 3;
                }else if (Up_Right_Down_Left[j] == 2 && casillaAtravesable(xx,yy,mapa)) {
                    auto nAUX = grafo.getNodoByPos(xx, yy);
                    if(nAUX != nullptr) {
                        auto nAUX2 = grafo.buscarIdNidoPorNodo(nAUX);
                        if(nAUX2 != 999){
                            auto nAUX3 = grafo.getNido(nAUX2);
                            freeSpaceCounter += nAUX3.size();
                            Up_Right_Down_Left[j] = 3;
                        }
                    }
                } else if (Up_Right_Down_Left[j] != 3 && bloqueRompible(xx,yy,mapa)) {
                    Up_Right_Down_Left[j] = 2;
                }
            }
        }
    }

    float destruccionCruz = 0.0f;
    int maxSpaceForNido = 30;
    //Comprobar que no exceda el limite puesto
    if(freeSpaceCounter > maxSpaceForNido) {
        freeSpaceCounter = maxSpaceForNido;
    }

    //Sumamos por cada direccion 
    for(int i=0;i<Up_Right_Down_Left.size(); i++) {
        if(Up_Right_Down_Left[i] == 2 || Up_Right_Down_Left[i] == 3){
            destruccionCruz += 0.33f;
        }
    }
    return (destruccionCruz * 0.5) + ((static_cast<float>(freeSpaceCounter) / static_cast<float>(maxSpaceForNido)) * 0.5);
}

/**
 * @brief Puntos Alineamiento Escape
 * Para calcular el valor de la huida en cada posicion se valora si el tile objetivo esta en la direccion de movimiento actual
 * Parte de la logica de la huida esta en seguir una direccion natural 
 * 
 * @param actual 
 * @param vecino 
 * @param velocidad 
 * @return float 
 */
float IASystem::puntosAlineamientoEscape(std::pair<int, int> actual, std::pair<int, int> vecino, std::pair<int, int> velocidad) const {
    int dx = vecino.first - actual.first;
    int dy = vecino.second - actual.second;

    // Producto punto: mide la similitud entre los vectores (dx, dy) y (dirX, dirY)
    double dotProduct = dx * velocidad.first + dy * velocidad.second;

    // Normalizamos dividiendo entre la magnitud del movimiento del enemigo
    double magnitude = sqrt(dx * dx + dy * dy) * sqrt(velocidad.first * velocidad.first + velocidad.second * velocidad.second);
    
    return magnitude == 0 ? 0 : dotProduct / magnitude; // Coseno del ángulo entre ambos vectores   
}

/**
 * @brief Calcula el nivel de peligro o inseguridad de una casilla específica del mapa.
 * 
 * Esta función evalúa el nivel de seguridad de la posición `(x, y)` teniendo en cuenta la proximidad
 * de bombas activas y el fuego existente en el entorno. 
 * 
 * Se consideran dos tipos de peligros:
 * - Peligro de bombas (`bombDanger`): Aumenta inversamente proporcional a la distancia a una bomba activa.
 * - Peligro de fuego (`fireDanger`): Se calcula si la bomba está explotando y el fuego alcanza la casilla.
 * 
 * La explosión de bombas se analiza en las cuatro direcciones (arriba, abajo, izquierda, derecha),
 * con un rango definido por la variable `rango`. Si la casilla está dentro del rango de fuego
 * y hay fuego en ella, se incrementa el peligro según la distancia.
 * 
 * @param x Coordenada X de la casilla a evaluar.
 * @param y Coordenada Y de la casilla a evaluar.
 * @param mapa Matriz que representa el mapa del juego.
 * @param em Referencia al `EntityManager` que contiene información sobre bombas y físicas.
 * @return float Valor que representa el nivel de peligro de la casilla (a mayor valor, mayor peligro).
 */
float IASystem::puntosSeguridadCasilla(int x, int y, const std::vector<std::vector<int>>& mapa, EntityManager& em) const {

    auto bombas =  em.getBombs();

    float bombDanger = 0.5f;
    float fireDanger = 0.3f;
    float peligroTotal = 0.0f;
    float dist = 0.0f;

    std::array<int,4> Up_Right_Down_Left {0,0,0,0};
    int xx;
    int yy;
    int rango = 4;

    for(auto b : bombas){
        auto& fisicas = em.fisicas.get(b.fisica_key);
        auto& bomba = em.bombs.get(b.bomb_key);
        
        float fil = fisicas.tileZ;
        float col = fisicas.tileX;

        dist = distanciaNormalizada(x, y, fil, col);
        if(dist < 0.55f){
            peligroTotal += bombDanger / dist;
            if(bomba.isExploding){
                for (int i = 1; i <= rango; i++) {
                    for (int j = 0; j < 4; j++) {
                        switch (j) {
                            case 0: xx = x-i; yy = y; break;
                            case 1: xx = x; yy = y+i; break;
                            case 2: xx = x+i; yy = y; break;
                            case 3: xx = x; yy = y-i; break;
                            default:                  break;
                        }
                        if (Up_Right_Down_Left[j] != 1 && Up_Right_Down_Left[j] != 2 && (enRangoMapa(xx,yy))) {
                            if (mapa[xx][yy] == 1) {
                                //Me he topado con un irrompible
                                Up_Right_Down_Left[j] = 1;
                            } else if (bloqueRompible(xx,yy,mapa)) {
                                //Me he topado con un rompible
                                Up_Right_Down_Left[j] = 2;

                            } else if (mapa[xx][yy] == FUEGO) {
                                //Me he topado con fuego
                                Up_Right_Down_Left[j] = 0;
                                dist = distanciaNormalizada(x,y,xx,yy);
                                if(dist < 0.55f){
                                    peligroTotal += fireDanger * dist;
                                }   
                            }
                        }
                    }
                }
            }
        }       
    }

    return peligroTotal;
}

/**
 * @brief Puntos PowerUp
 * Comprueba si hay un power up en ese bloque
 * 
 * @param x 
 * @param y 
 * @param mapa 
 * @return true 
 * @return false 
 */
bool IASystem::puntosPowerUp(int x, int y, const std::vector<std::vector<int>>& mapa) const {
    if(mapa[x][y] > 9 && mapa[x][y] < 18) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Calcula la utilidad de colocar una bomba en una celda específica para atacar.
 * 
 * Evalúa la celda `objetivo` considerando el impacto potencial de una bomba colocada en esa posición.
 * El algoritmo analiza las cuatro direcciones de explosión (arriba, abajo, izquierda, derecha), hasta un rango
 * definido por `radioBomba`. Asigna una puntuación según los siguientes criterios:
 * 
 * - +100 puntos por cada enemigo alcanzado en la zona de explosión.
 * - +50 puntos por cada bloque destructible alcanzado.
 * - -150 puntos si el jugador (NPC) se encuentra dentro del área de explosión (autodaño).
 * 
 * Si la explosión encuentra un bloque sólido (`SOLIDO`), se detiene en esa dirección.
 * Lo mismo ocurre si encuentra un bloque destructible (`DESTRUIBLE`), aunque se suma puntuación.
 * 
 * @param objetivo Coordenadas (x, y) donde se colocaría la bomba.
 * @param player Coordenadas actuales del jugador (NPC).
 * @param mapa Mapa del entorno representado como matriz de enteros.
 * @param enemigos Array con posiciones de los enemigos actuales.
 * @return float Puntuación de utilidad para atacar desde la celda objetivo.
 */
float IASystem::puntosAtacarCelda(const std::pair<int,int>& objetivo,const std::pair<int,int>& player, const std::vector<std::vector<int>>& mapa, const std::array<std::pair<int,int>,3> enemigos) const {
    float score = 0.0;
    int radioBomba = 3;

    // Revisar todas las direcciones de explosión (arriba, abajo, izquierda, derecha)
    std::array<std::pair<int, int>, 4> direcciones = {std::make_pair(1, 0), std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(0, -1)};

    for (auto dir : direcciones) {
        for (int i = 1; i <= radioBomba; i++) {
            int nx = objetivo.first + dir.first * i;
            int ny = objetivo.second + dir.second * i;

            if (!enRangoMapa(nx, ny)) break; // Si sale del mapa, detener búsqueda

            // Si hay un bloque indestructible, la explosión no continúa
            if (mapa[nx][ny] == SOLIDO) break;

            // Si hay un bloque destructible, darle puntuación y detener explosión
            if (mapa[nx][ny] == DESTRUIBLE) {
                score += 50;
                break;
            }

            // Verificar si hay un enemigo en la zona de explosión
            for (const auto &enemigo : enemigos) {
                if (enemigo.first== nx && enemigo.second == ny) {
                    score += 100; // Golpear a un enemigo es prioritario
                }
            }
        }
    }

    // Penalización si el jugador se pone en peligro con la bomba
    for (auto dir : direcciones) {
        for (int i = 1; i <= radioBomba; i++) {
            int nx = objetivo.first + dir.first * i;
            int ny = objetivo.second + dir.second * i;

            if (!enRangoMapa(nx, ny)) break;
            if (mapa[nx][ny] == SOLIDO) break;

            if (nx == player.first && ny == player.second) {
                score -= 150; // Penalizar fuerte si el jugador se pone en peligro
            }
        }
    }

    return score;
}

/**
 * @brief Calcula la puntuación potencial por pintar en una ubicación específica del mapa.
 * 
 * Esta función evalúa el área adyacente a una posición (x, y) en cuatro direcciones (arriba, derecha, abajo, izquierda)
 * hasta un rango determinado. El objetivo es calcular una puntuación basada en la utilidad de 
 * pintar los espacios, considerando obstáculos, bloques rompibles y zonas ya pintadas.
 * 
 * La puntuación se asigna de la siguiente forma:
 * - +0.5 por cada bloque rompible en el camino.
 * - +1.0 si una casilla atravesable está sin pintar (blanca).
 * - +1.5 si una casilla atravesable está pintada por otro equipo (color diferente).
 * - +0 si la casilla ya tiene el mismo color que `idColor`.
 * 
 * El cálculo se detiene en cada dirección si se encuentra un bloque irrompible o un rompible.
 * 
 * @param x Coordenada X central desde donde se evalúa.
 * @param y Coordenada Y central desde donde se evalúa.
 * @param mapa Mapa de colisiones o tipo de bloque.
 * @param mapaColor Mapa de colores actual.
 * @param idColor ID del color propio del NPC o equipo.
 * @return float Puntuación total obtenida por pintar desde la posición dada.
 */
float IASystem::puntosColorear(int x, int y, const std::vector<std::vector<int>>& mapa, const std::vector<std::vector<int>>& mapaColor, int idColor) const {
    int rango = 3;

    std::array<int,4> Up_Right_Down_Left {0,0,0,0};
    float puntuacionPorPintar = 0.0f;

    int xx = x;
    int yy = y;

    for (int i = 1; i <= rango; i++) {
        for (int j = 0; j < 4; j++) {
            switch (j) {
                case 0: xx = x-i; yy = y; break;
                case 1: xx = x; yy = y+i; break;
                case 2: xx = x+i; yy = y; break;
                case 3: xx = x; yy = y-i; break;
                default:                  break;
            }

            if (Up_Right_Down_Left[j] != 1 && Up_Right_Down_Left[j] != 2 && (enRangoMapa(xx,yy))) {
                if (mapa[xx][yy] == 1) {
                    //Me he topado con un irrompible
                    Up_Right_Down_Left[j] = 1;
                } else if (bloqueRompible(xx,yy,mapa)) {
                    //Me he topado con un rompible
                    Up_Right_Down_Left[j] = 2;
                    puntuacionPorPintar += 0.5f;

                } else if (casillaAtravesable(xx,yy,mapa)) {
                    //Me he topado con un espacio de color
                    //Si esta sin pintar +1.0
                    //Si esta pintador por otro +1.5
                    if(mapaColor[xx][yy] == 0){
                        //COLOR BLANCO + 1
                        puntuacionPorPintar+=1.0f;
                    } else if(mapaColor[xx][yy] != idColor){
                        //OTRO COLOR
                        puntuacionPorPintar+=1.5f;
                    }
                        //MISMO COLOR + 0

                    Up_Right_Down_Left[j] = 0;
                }
            }
        }
    }

    return puntuacionPorPintar;
}

/**
 * @brief Determina si una casilla del mapa es atravesable por el NPC.
 * 
 * Esta función evalúa si el valor de una casilla en la posición (x, y) representa un terreno que
 * el NPC puede atravesar sin impedimentos. Se considera atravesable si:
 * - Su valor es 0 (espacio libre),
 * - Su valor es 9 (spawn esquinas), o
 * - Está en el rango de valores entre 10 y 17 inclusive (es decir, >9 y <18).
 * 
 * Este método es esencial para la lógica de navegación del NPC, evitando obstáculos y elementos sólidos.
 * 
 * @param x Coordenada X del mapa.
 * @param y Coordenada Y del mapa.
 * @param mapa Mapa del entorno representado como una matriz de enteros.
 * @return true Si la casilla es atravesable.
 * @return false Si la casilla no puede ser atravesada.
 */
bool IASystem::casillaAtravesable(int x, int y, const std::vector<std::vector<int>>& mapa) const {
    if(mapa[x][y] == 0 || mapa[x][y] == 9 || (mapa[x][y] > 9 && mapa[x][y] < 18)) {
        return true;
    }
    return false;
}

/**
 * @brief Verifica si una posición dada está dentro de los límites del mapa.
 * 
 * Esta función comprueba si las coordenadas (x, y) están dentro del rango válido definido
 * por las dimensiones del mapa (`filas` y `columnas`). Es útil para evitar accesos inválidos
 * a la matriz del mapa durante evaluaciones o movimientos.
 * 
 * @param x Coordenada X a verificar.
 * @param y Coordenada Y a verificar.
 * @return true Si la posición está dentro de los límites del mapa.
 * @return false Si la posición está fuera del mapa.
 */
bool IASystem::enRangoMapa(int x, int y) const {
    if( x >= 0 && x < filas && y >= 0 & y < columnas){
        return true;
    } 
    return false;
}

/**
 * @brief Determina si el bloque en la posición dada del mapa es rompible.
 * 
 * Esta función evalúa si un bloque del mapa puede ser destruido. Se considera rompible si:
 * - Su valor es exactamente 2, o
 * - Su valor está en el rango (110 a 117), ambos inclusive (es decir, mayor que 109 y menor que 118).
 * 
 * Esto permite identificar obstáculos que el NPC podría eliminar o que deben ser evitados.
 * 
 * @param x Coordenada X del mapa.
 * @param y Coordenada Y del mapa.
 * @param mapa Mapa del entorno representado como una matriz de enteros.
 * @return true Si el bloque es rompible.
 * @return false Si el bloque no es rompible.
 */
bool IASystem::bloqueRompible(int x, int y, const std::vector<std::vector<int>>& mapa) const {
    if( mapa[x][y] == 2 || (mapa[x][y] > 109 && mapa[x][y] < 118 )){
        return true;
    }
    return false;
}

/**
 * @brief Take Foco Target
 * Segun su posicion obtiene el objetivo para atacar con el foco
 * El objetivo sera siempre el enemigo mas cercano que no sea el objetivo anterior
 * 
 * @param x 
 * @param y 
 * @param em 
 * @param entidad 
 * @param pesado 
 * @return int 
 */
int IASystem::takeFocoTarget(float x, float y, EntityManager& em, Entity& entidad) const {
    auto &focoId = em.ia.get(entidad.ia_key).idFocoTarget;
    std::vector<std::pair<int,float>> distancia;
    int contador = 0;
    auto enemigos = em.getEnemies(entidad);
    
    for(auto& e : enemigos) {
        auto &fisicaComponent = em.fisicas.get(e.fisica_key);
        if(!fisicaComponent.esFantasma && e.getId() != focoId){
            distancia.push_back({contador,distanciaNormalizada(x,y,fisicaComponent.x,fisicaComponent.z)});
        }
        contador++;
    }
    
    float auxPuntos = 999.0f;
    int pos = 0;
    for(auto& d : distancia){
        if(d.second < auxPuntos){
            auxPuntos = d.second;
            pos = d.first;
        }
    }

    return enemigos[pos].getId();
}

/**
 * @brief Mover Foco NPC
 * Calcula la distancia entre el mismo y su objetivo y con el vector se lo pasa a las fisicas para poder moverse en la direccion correspondiente
 * Si el objetivo muere antes de completar la accion vuelve a buscar otro objetivo
 * 
 * @param em 
 * @param entidad 
 */
void IASystem::moverFocoNpc(EntityManager& em, Entity& entidad) const{
    auto &iaComponent = em.ia.get(entidad.ia_key);
    auto &fisicaComponent = em.fisicas.get(entidad.fisica_key);

    Entity enemigo;

    for(auto& e : em.getEnemies(entidad)) {
        if(e.getId() == iaComponent.idFocoTarget){
            enemigo = e;
            break;
        }
    }
    auto &fisicaComponentEnemigo = em.fisicas.get(enemigo.fisica_key);

    if(fisicaComponentEnemigo.esFantasma){
        iaComponent.idFocoTarget = takeFocoTarget(fisicaComponent.x, fisicaComponent.z, em, entidad);
        for(auto& e : em.getEnemies(entidad)) {
            if(e.getId() == iaComponent.idFocoTarget){
                enemigo = e;
                break;
            }
        }
        auto& fisicaComponentEnemigo = em.fisicas.get(enemigo.fisica_key);
    }

    float distanciaX = fisicaComponentEnemigo.x - fisicaComponent.x;
    float distanciaZ = fisicaComponentEnemigo.z - fisicaComponent.z;

    if(distanciaX > 0){
        fisicaComponent.moverDerecha = true;
        fisicaComponent.moverIzquierda = false;
    }else if(distanciaX < 0){
        fisicaComponent.moverIzquierda = true;
        fisicaComponent.moverDerecha = false;
    }
    if(distanciaZ > 0){
        fisicaComponent.moverAbajo = true;
        fisicaComponent.moverArriba = false;
    }else if(distanciaZ < 0){
        fisicaComponent.moverArriba = true;
        fisicaComponent.moverAbajo = false;
    }

    if(abs(distanciaX) + abs(distanciaZ) < 3.0f){
        fisicaComponent.estuneo = true;
        if(fisicaComponent.cargaCompletada){
            iaComponent.inPositionToFlash = true;
        }
    }
}

/**
 * @brief Distancia Normalizada
 * Calcula la distancia entre dos puntos+
 * Devuelve un valor entre 0-1
 * 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @return float  0-1
 */
float IASystem::distanciaNormalizada(float x1, float y1, float x2, float y2) const {
    constexpr float d_max = 17.0f;
    constexpr float inv_d_max_sq = 1.0f / (d_max * d_max); // precalcular inversa del cuadrado

    float dx = x2 - x1;
    float dy = y2 - y1;
    float dist_sq = dx * dx + dy * dy;
    float res = dist_sq * inv_d_max_sq;

    if (res > 1.0f) res = 1.0f;
    return res;
}

/**
 * @brief No Quiero Morir
 * Comprueba si la casilla en la que se encuentra es suficientemente segura
 * El objetivo es saber si en su posicion se pusiese una bomba habria algun lugar libre para escapar
 * Se pone una supuesta bomba en la posicion y se calcula la seguridad alrededor la casilla
 * 
 * @param mapa 
 * @param em 
 * @param e 
 * @return true 
 * @return false 
 */
bool IASystem::noQuieroMorir(std::vector<std::vector<int>>& mapa, EntityManager& em, Entity& e) const {
    fisicaComponent fisicas = em.fisicas.get(e.fisica_key);

    int x = fisicas.tileZ;
    int y = fisicas.tileX;
    bool exit = false;

    int auxMapa = mapa[x][y]; //*Se guarda un auxiliar que contiene lo que habia en el mapa y se pone una bomba
    mapa[x][y] = BOMBA;

    int radio = 3;
    float seguridad = 0.0f;

    for(int w=0;w<radio;w++) {
        for(int i=x-w;i<=x+w;i++) {
            for(int j=y-w;j<=y+w;j++) {
                if(enRangoMapa(i,j)){
                    if(casillaAtravesable(i,j,mapa) && (i==x-w || i==x+w || j==y-w || j==y+w)){
                        seguridad = puntosSeguridadCasilla(i, j, mapa, em);
                        if(seguridad < 3.0f && !casillaEstaRodeada(i, j, mapa)){
                            exit = true;
                            break;
                        }
                    }
                }
            }
            if(exit){break;}
        }
        if(exit){break;}
    }

    mapa[x][y] = auxMapa;
    return exit;
}

/**
 * @brief Casilla esta Rodeada
 * Comprueba si la casilla que estamos observando tiene algun bloqoue libre sus 4 direcciones
 * 
 * @param x 
 * @param y 
 * @param mapa 
 * @return true 
 * @return false 
 */
bool IASystem::casillaEstaRodeada(int x, int y, const std::vector<std::vector<int>>& mapa) const {
    if(casillaAtravesable(x+1, y, mapa) && casillaAtravesable(x-1, y, mapa) && casillaAtravesable(x, y+1, mapa) && casillaAtravesable(x, y-1, mapa)){
        return true;
    }
    return false;
}

/**
 * @brief Calcula el nivel de peligro debido a fuego cercano alrededor de una entidad.
 * 
 * Esta función evalúa un área alrededor de la posición de la entidad en el mapa para detectar
 * la presencia de fuego. La búsqueda se realiza en un radio definido alrededor de la posición actual de la entidad.
 * 
 * Si se detecta fuego en el perímetro del área, la función retorna un valor 
 * de peligro máximo (1.0f). Si no se detecta fuego, retorna 0.0f.
 * 
 * @param em Referencia al EntityManager que contiene la información de las entidades.
 * @param entidad Referencia a la entidad para la cual se calcula el peligro.
 * @param mapa Mapa representado como una matriz de enteros donde se indican elementos como fuego.
 * @return float Nivel de peligro debido al fuego cercano (1.0 si hay fuego, 0.0 si no).
 */
float IASystem::calculoFuegoCercano(EntityManager& em, Entity& entidad, const std::vector<std::vector<int>>& mapa) const {
    auto& fisicas = em.fisicas.get(entidad.fisica_key);
    int x = fisicas.tileZ;
    int y = fisicas.tileX;
    
    float peligroFuego = 0.0f;
    int radio = 2;
    bool exit = false;

    for(int w=0;w<radio;w++) {
        for(int i=x-w;i<=x+w;i++) {
            for(int j=y-w;j<=y+w;j++) {
                if(enRangoMapa(i,j)){
                    if(i==x-w || i==x+w || j==y-w || j==y+w){
                        if(mapa[i][j] == FUEGO){
                            peligroFuego = 1.0f;
                            exit = true;
                            break;
                        }
                    }
                }
            }
            if(exit){break;}
        }
        if(exit){break;}
    }
    return peligroFuego;
}