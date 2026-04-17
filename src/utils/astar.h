#ifndef ASTAR_H
#define ASTAR_H

#include "graph.h"
// #include <vector>
#include <unordered_map>
#include <algorithm>
#include <math.h>

class Heuristics {
    public:
        virtual float estimate (Nodo* current, Nodo* goal) const = 0;  
};

// Heurística simple
class SimpleHeuristic : public Heuristics {
public:
    float estimate(Nodo* current, Nodo* goal) const override{
        float d_max = 18.0;
        float distancia = std::sqrt(std::pow(goal->getPosX() - current->getPosX(), 2) + std::pow(goal->getPosY() - current->getPosY(), 2));
        float res = distancia / d_max;
        if(res>1) {res = 1;}
        return res;
    }
};

class AStar {
    public:
        static std::vector<Conexion*> pathfindAStar(const Grafo& grafo, Nodo* current, Nodo* goal, const Heuristics& heuristico);
};

#endif //ASTAR_H