#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <unordered_set>
#include <array>
#include <algorithm>
#include <stdexcept>
#include <iomanip>

// Constantes para la zona de influencia
constexpr int K_radio_Bomba = 3;
constexpr int K_radio_Fuego = 1;
constexpr int K_radio_PowerUp = 5;
constexpr float K_valor_Bomba = 0.7f;
constexpr float K_valor_Fuego = 0.9f;
constexpr float K_valor_PowerUp = 0.8f;
constexpr float K_multiplicador_Bomba = 0.6f;
constexpr float K_multiplicador_Fuego = 0.5f;
constexpr float K_multiplicador_PowerUp = 0.4f;

class Conexion; ///< Declaración adelantada de la clase Conexion

/**
 * @brief Clase Nodo
 *
 * Representa un nodo dentro de un grafo, con posición y conexiones salientes.
 */
class Nodo {
private:
    int id; ///< Identificador único del nodo
    int posX; ///< Posición X del nodo
    int posY; ///< Posición Y del nodo
    std::vector<Conexion*> conexiones; ///< Conexiones salientes del nodo

public:
    /**
     * @brief Constructor de Nodo
     * 
     * @param id Identificador del nodo
     * @param pX Posición X
     * @param pY Posición Y
     */
    Nodo(int id, int pX, int pY);

    /**
     * @brief Destructor de Nodo
     * 
     * Elimina todas las conexiones salientes.
     */
    ~Nodo();

    /**
     * @brief Obtiene el ID del nodo
     * 
     * @return ID del nodo
     */
    int getId() const;

    /**
     * @brief Obtiene la posición X del nodo
     * 
     * @return Posición X
     */
    int getPosX() const;

    /**
     * @brief Obtiene la posición Y del nodo
     * 
     * @return Posición Y
     */
    int getPosY() const;

    /**
     * @brief Añade una conexión saliente
     * 
     * @param toNode Nodo destino
     * @param cost Coste de la conexión
     */
    void addConexion(Nodo* toNode, double cost);

    /**
     * @brief Devuelve todas las conexiones salientes
     * 
     * @return Vector de punteros a conexiones
     */
    const std::vector<Conexion*>& getConexiones() const;
};

/**
 * @brief Clase Conexion
 * 
 * Representa una conexión entre dos nodos del grafo.
 */
class Conexion {
private:
    Nodo* fromNodo; ///< Nodo origen
    Nodo* toNodo;   ///< Nodo destino
    double coste;   ///< Coste de la conexión

public:
    /**
     * @brief Constructor de Conexion
     * 
     * @param from Nodo origen
     * @param to Nodo destino
     * @param cost Coste de la conexión
     */
    Conexion(Nodo* from, Nodo* to, double cost);

    /**
     * @brief Obtiene el nodo origen
     * 
     * @return Puntero al nodo origen
     */
    Nodo* getFrom() const;

    /**
     * @brief Obtiene el nodo destino
     * 
     * @return Puntero al nodo destino
     */
    Nodo* getTo() const;

    /**
     * @brief Obtiene el coste de la conexión
     * 
     * @return Coste de la conexión
     */
    double getCoste() const;

    /**
     * @brief Establece un nuevo coste para la conexión
     * 
     * @param newCoste Nuevo valor de coste
     */
    void setCoste(double newCoste) { coste = newCoste; }
};

/**
 * @brief Clase Grafo
 * 
 * Representa un grafo de nodos y conexiones.
 */
class Grafo {
private:
    std::vector<Nodo*> nodos; ///< Conjunto de nodos del grafo
    std::vector<std::vector<Nodo*>> matrizIdNodos; ///< Matriz de nodos por ID
    std::array<std::vector<Nodo*>, 32> nidos; ///< Conjunto de nidos (grupos de nodos)

public:
    /**
     * @brief Destructor de Grafo
     * 
     * Libera la memoria de todos los nodos y conexiones.
     */
    ~Grafo();

    /**
     * @brief Crea un nuevo nodo
     * 
     * @param id Identificador del nodo
     * @param posX Posición X
     * @param posY Posición Y
     * @return Puntero al nodo creado
     */
    Nodo* crearNodo(int id, int posX, int posY);

    /**
     * @brief Obtiene las conexiones de un nodo
     * 
     * @param fromNodo Nodo desde el que se desean las conexiones
     * @return Vector de punteros a conexiones
     */
    std::vector<Conexion*> getConexiones(Nodo* fromNodo) const;

    /**
     * @brief Inicializa el grafo desde una matriz
     * 
     * @param mapaMatriz Matriz base del mapa
     */
    void inicializar(const std::vector<std::vector<int>>& mapaMatriz);

    /**
     * @brief Devuelve un nodo por ID
     * 
     * @param id ID del nodo
     * @return Puntero al nodo, o nullptr si no existe
     */
    Nodo* getNodoById(int id) const;

    /**
     * @brief Devuelve un nodo por su posición en la matriz
     * 
     * @param getX Posición X
     * @param getY Posición Y
     * @return Puntero al nodo correspondiente
     */
    Nodo* getNodoByPos(int getX, int getY) const;

    /**
     * @brief Obtiene el ID del último nodo creado
     * 
     * @return ID del último nodo
     */
    int getLastNodoId() const;

    /**
     * @brief Instala un nodo en una posición específica
     * 
     * @param x Posición X
     * @param y Posición Y
     */
    void instalarNodo(int x, int y);

    /**
     * @brief Imprime la matriz de nodos
     */
    void printMatrizNodos() const;

    /**
     * @brief Imprime las conexiones de cada nodo
     */
    void printConexionesNodos() const;

    /**
     * @brief Limpia todos los nodos del grafo
     * 
     * Se utiliza en el reinicio del juego para evitar nodos antiguos.
     */
    void limpiarNodos();

    /**
     * @brief Actualiza la zona de influencia del grafo para el estrategico tactico
     * 
     * @param x Posición X
     * @param y Posición Y
     * @param mapa Mapa de la partida
     * @param poner_quitar Si se añade o elimina la influencia
     */
    void actualizarZonaInfluencia(int x, int y, const std::vector<std::vector<int>>& mapa, bool poner_quitar);

    /**
     * @brief Inicializa los nidos del grafo
     */
    void inicializarNidos();

    /**
     * @brief Comprueba si un nodo pertenece a un nido
     * 
     * @param nodo Nodo a verificar
     * @return true si pertenece a un nido, false en caso contrario
     */
    bool nodoInNido(Nodo& nodo) const;

    /**
     * @brief Incluye recursivamente nodos en un nido
     * 
     * @param nodo Nodo raíz
     * @param idNido ID del nido al que se añadirá
     */
    void recursivaIncluirNodosEnNidos(Nodo& nodo, int idNido);

    /**
     * @brief Busca el nido al que pertenece un nodo
     * 
     * @param nodo Nodo a buscar
     * @return Vector de nodos del nido
     */
    std::vector<Nodo*> buscarNidoPorNodo(Nodo* nodo) const;

    /**
     * @brief Busca el ID del nido al que pertenece un nodo
     * 
     * @param nodo Nodo a buscar
     * @return ID del nido
     */
    int buscarIdNidoPorNodo(Nodo* nodo) const;

    /**
     * @brief Busca el ID del nido por posición
     * 
     * @param x Posición X
     * @param y Posición Y
     * @return ID del nido
     */
    int buscarIdNidoPorPos(int x, int y) const;

    /**
     * @brief Integra un nodo dentro de un nido existente
     * 
     * @param nodo Nodo a integrar
     */
    void integrarNodoInNido(Nodo& nodo);

    /**
     * @brief Devuelve todos los nodos de un nido
     * 
     * @param id ID del nido
     * @return Vector de nodos del nido o vacío si no existen
     */
    std::vector<Nodo*> getNido(int id) const {
        if (nidos[id][0] != nullptr) return nidos[id];
        else return std::vector<Nodo*>();
    }
};

#endif // GRAPH_H
