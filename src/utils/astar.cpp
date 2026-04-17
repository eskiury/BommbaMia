#include "astar.h"
#include "graph.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

/**
 * @struct NodoRecord
 * @brief Estructura auxiliar para guardar información de cada nodo durante la búsqueda A*
 */
struct NodoRecord {
    Nodo* nodo;               ///< Nodo actual
    Conexion* conexion;       ///< Conexión previa hacia este nodo
    double costSoFar;         ///< Costo acumulado desde el nodo inicial
    double estimatedCost;     ///< Costo estimado total (acumulado + heurístico)

    /**
     * @brief Compara dos NodoRecord por su costo estimado
     * @param otro NodoRecord a comparar
     * @return true si el costo estimado actual es menor
     */
    bool operator<(const NodoRecord& otro) const {
        return estimatedCost < otro.estimatedCost;
    }

    /**
     * @brief Verifica si dos NodoRecord son equivalentes
     * @param otro NodoRecord a comparar
     * @return true si son iguales en nodo, conexión y costo acumulado
     */
    bool operator==(const NodoRecord& otro) const {
        return (nodo == otro.nodo) && (conexion == otro.conexion) && (costSoFar == otro.costSoFar);
    }
};

/**
 * @class PathFindingList
 * @brief Contenedor personalizado de NodoRecord para listas abierta y cerrada de A*
 */
class PathFindingList {
private:
    std::vector<NodoRecord> records; ///< Lista de registros

public:
    /**
     * @brief Agrega un nuevo NodoRecord a la lista
     * @param nuevoRecord NodoRecord a agregar
     */
    void add(NodoRecord& nuevoRecord) {
        records.push_back(nuevoRecord);
    }

    /**
     * @brief Elimina un NodoRecord de la lista
     * @param borraRecord NodoRecord a eliminar
     */
    void remove(NodoRecord& borraRecord) {
        records.erase(std::remove(records.begin(), records.end(), borraRecord), records.end());
    }

    /**
     * @brief Busca un NodoRecord asociado a un nodo específico
     * @param nodo Nodo a buscar
     * @return Puntero al NodoRecord si se encuentra, nullptr si no
     */
    NodoRecord* find(Nodo* nodo) {
        auto it = std::find_if(records.begin(), records.end(), [nodo](const NodoRecord& record) {
            return record.nodo == nodo;
        });
        return it != records.end() ? &(*it) : nullptr;
    }

    /**
     * @brief Verifica si un nodo ya está en la lista
     * @param nodo Nodo a verificar
     * @return true si el nodo existe en la lista
     */
    bool contain(Nodo* nodo) const {
        return std::any_of(records.begin(), records.end(), [nodo](const NodoRecord& record) {
            return record.nodo == nodo;
        });
    }

    /**
     * @brief Obtiene el NodoRecord con menor costo estimado
     * @return NodoRecord con menor estimatedCost
     */
    NodoRecord smallestElement() const {
        return *std::min_element(records.begin(), records.end());
    }

    /**
     * @brief Verifica si la lista está vacía
     * @return true si no contiene elementos
     */
    bool empty() const {
        return records.empty();
    }
};

/**
 * @brief Ejecuta el algoritmo A* para encontrar el camino más corto entre dos nodos en un grafo
 * 
 * @param grafo Referencia al grafo sobre el que se realiza la búsqueda
 * @param start Nodo inicial
 * @param goal Nodo objetivo
 * @param heuristico Referencia a función heurística para estimar distancia restante
 * @return std::vector<Conexion*> Camino encontrado como vector de conexiones (vacío si no hay camino)
 */
std::vector<Conexion*> AStar::pathfindAStar(const Grafo& grafo, Nodo* start, Nodo* goal, const Heuristics& heuristico) {
    PathFindingList open;    ///< Lista abierta: nodos por explorar
    PathFindingList closed;  ///< Lista cerrada: nodos ya evaluados

    // Inicializamos nodo de partida
    NodoRecord startRecord;
    startRecord.nodo = start;
    startRecord.conexion = nullptr;
    startRecord.costSoFar = 0;
    startRecord.estimatedCost = heuristico.estimate(start, goal);

    open.add(startRecord);

    NodoRecord nodoActual;

    // Bucle principal del algoritmo A*
    while (!open.empty()) {
        nodoActual = open.smallestElement();

        // Si llegamos al objetivo, se termina la búsqueda
        if (nodoActual.nodo == goal) {
            break;
        }

        auto conexiones = grafo.getConexiones(nodoActual.nodo);

        for (auto conexion : conexiones) {
            Nodo* nodoFinal = conexion->getTo();
            double costeNodoFinal = nodoActual.costSoFar + conexion->getCoste();

            NodoRecord* recordNodoFinal = nullptr;
            double heuristicoNodoFinal = 0;

            if (closed.contain(nodoFinal)) {
                recordNodoFinal = closed.find(nodoFinal);
                if (recordNodoFinal->costSoFar <= costeNodoFinal) continue;

                closed.remove(*recordNodoFinal);
                heuristicoNodoFinal = recordNodoFinal->estimatedCost - recordNodoFinal->costSoFar;

            } else if (open.contain(nodoFinal)) {
                recordNodoFinal = open.find(nodoFinal);
                if (recordNodoFinal->costSoFar <= costeNodoFinal) continue;

                heuristicoNodoFinal = recordNodoFinal->estimatedCost - recordNodoFinal->costSoFar;

            } else {
                recordNodoFinal = new NodoRecord();
                recordNodoFinal->nodo = nodoFinal;
                heuristicoNodoFinal = heuristico.estimate(nodoFinal, goal);
            }

            // Actualizamos el registro del nodo conectado
            recordNodoFinal->costSoFar = costeNodoFinal;
            recordNodoFinal->conexion = conexion;
            recordNodoFinal->estimatedCost = costeNodoFinal + heuristicoNodoFinal;

            if (!open.contain(nodoFinal)) {
                open.add(*recordNodoFinal);
            }
        }

        // Movemos el nodo actual a la lista cerrada
        open.remove(nodoActual);
        closed.add(nodoActual);
    }

    // ! Si no se encontró camino al objetivo
    if (nodoActual.nodo != goal) return {};

    // Reconstrucción del camino desde el objetivo hacia el inicio
    std::vector<Conexion*> camino;
    while (nodoActual.nodo != start) {
        camino.push_back(nodoActual.conexion);
        nodoActual.nodo = nodoActual.conexion->getFrom();
        NodoRecord* anterior = closed.find(nodoActual.nodo);
        if (!anterior) return {};
        nodoActual = *anterior;
    }

    std::reverse(camino.begin(), camino.end());
    return camino;
}
