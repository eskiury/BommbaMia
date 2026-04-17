#ifndef IASYSTEM_H
#define IASYSTEM_H

#include <random> // Para std::random_device, std::mt19937, std::uniform_int_distribution
// #include "../utils/graph.h"
#include "../utils/astar.h"
#include <vector>
#include <unordered_map>
#include <cmath>
#include <tuple>
#include "../AlmondGE/EntityManager.h"
#include "../CMP/Entity.h"

class IASystem
{
private:
    Grafo grafo; // Instancia del grafo usado por el sistema de IA
    int filas = 0;
    int columnas = 0;
public:
    // Inicialización del sistema
    void init(const std::vector<std::vector<int>>& mapSystem); // Configura los nodos y conexiones del grafo

    // Métodos para manejar el grafo
    Nodo *agregarNodo(int x, int y);
    void agregarNodoANido(int x, int y);
    void agregarConexion(int idDesde, int idHacia, double coste);

    // Métodos auxiliares
    Nodo *obtenerNodoPorId(int id) const;

    Grafo &getGrafo() { return grafo; }

    // void crearBT(EntityManager&, Entity&);
    // void ejecutarBT(EntityManager& em, Entity& entidad, std::vector<std::vector<int>> mapa, MapSystem& mapsys, BombSystem& bombsys);

    void crearUtility(EntityManager &, Entity &);
    void crearUtilityPintura(EntityManager &, Entity &);

    // Funciones Utility
    bool casillaAtravesable(int x, int y, const std::vector<std::vector<int>>& mapa) const;
    bool enRangoMapa(int x, int y) const;
    bool bloqueRompible(int x, int y, const std::vector<std::vector<int>>& mapa) const;  

    void actualizarCamino(EntityManager &em, Entity &entidad);
    void update(EntityManager &em);

    //!---NO USE
    bool isOnTileEnemy(float position, float tileSize, float velocity);

    // METODOS PARA EL SCHDULER
    void ejecutarIASlice(EntityManager &em, Entity &entidad,  std::vector<std::vector<int>> &mapa,
                         BombSystem &bombsystem, MapSystem &mapsystem, float tiempoMaximo);
    bool iaStep(EntityManager &em, Entity &entidad,  std::vector<std::vector<int>> &mapa,
                BombSystem &bombsystem, MapSystem &mapsystem);

    bool iaStepPintura(EntityManager &em, Entity &entidad,  std::vector<std::vector<int>> &mapa,
                BombSystem &bombsystem, MapSystem &mapsystem);
    void ejecutarIASlicePintura(EntityManager &em, Entity &entidad,  std::vector<std::vector<int>> &mapa,
                         BombSystem &bombsystem, MapSystem &mapsystem, float tiempoMaximo);


    // METODOS CALCULO IA ESCUTIA

    float enemigosCercanos(EntityManager &em, Entity &entidad) const;
    float bombasCercanas(EntityManager &em, Entity &entidad, std::vector<std::vector<int>>& mapa) const;
    float peligrosidadEnUnPunto(EntityManager& em, Entity& entidad, std::vector<std::vector<int>>& mapa) const;
    float calcularEspacioLibre(EntityManager& em, Entity& entidad, std::vector<std::vector<int>>& mapa) const;
    bool alcanzaUnPW(EntityManager& em, Entity& entidad, std::vector<std::vector<int>>& mapa) const;
    // METODOS CALCULO IA MANU

    float distanciaPowerUp(EntityManager &em, Entity &pw) const;
    float bloquesTotales(EntityManager &em, std::vector<std::vector<int>>& mapa)const;
    float pwObtenidos(EntityManager &em, Entity &entidad) const;


    //METODOS CALCULO OBJETIVO CAMINO
    float puntosDestruccionCruz(int x, int y, const std::vector<std::vector<int>>& mapa) const;
    float puntosAlineamientoEscape(std::pair<int, int> actual, std::pair<int, int> vecino, std::pair<int, int> velocidad) const;
    float puntosSeguridadCasilla(int x, int y, const std::vector<std::vector<int>>& mapa, EntityManager& em) const;
    bool puntosPowerUp(int x, int y, const std::vector<std::vector<int>>& mapa) const;
    float puntosAtacarCelda(const std::pair<int,int>& objetivo,const std::pair<int,int>& player, const std::vector<std::vector<int>>& mapa, const std::array<std::pair<int,int>,3> enemigos) const;
    int takeFocoTarget(float x, float y, EntityManager& em, Entity& entidad) const;
    float distanciaNormalizada(float x1, float y1, float x2, float y2) const;
    void moverFocoNpc(EntityManager& em, Entity& entidad) const;
    float puntosColorear(int x, int y, const std::vector<std::vector<int>>& mapa, const std::vector<std::vector<int>>& mapaColor, int idColor) const;
    std::vector<Conexion*> valorarCasillas(int x, int y, const std::vector<std::vector<int>>& mapa, const std::vector<std::vector<int>>& mapaColores, EntityManager& em, Entity& entidad, int id) const;
    bool noQuieroMorir(std::vector<std::vector<int>>& mapa, EntityManager& em, Entity& e) const;
    float calculoFuegoCercano(EntityManager& em, Entity& entidad, const std::vector<std::vector<int>>& mapa) const;
    bool casillaEstaRodeada(int x, int y, const std::vector<std::vector<int>>& mapa) const;




};

// VARIABLES

#endif // IASYSTEM_H
