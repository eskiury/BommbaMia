#ifndef UTILITY_SYSTEM_H
#define UTILITY_SYSTEM_H

#include <vector>
#include <memory>
#include <limits>
#include <chrono>
class EntityManager;
class Entity;
class IASystem;
class BombSystem;
class MapSystem;

/**
 * @enum NodoClass
 * @brief Representa los diferentes estados o acciones posibles dentro del árbol de utilidad en el comportamiento de IA.
 *
 * Este enum define todos los tipos de nodo que forman el árbol de utilidad 
 */
enum struct NodoClass {
    WAIT,           /**< El nodo ejecuta sin realizar ninguna acción. */
    ATACAR,         /**< El nodo realiza una acción ofensiva. */
    HUIR,           /**< El nodo escapa del peligro. */
    CAMINO,         /**< El nodo se abre camino colocando una bomba en un bloque destruible. */
    POWERUP,        /**< El nodo recoge un PowerUp. */
    FOCO,           /**< El nodo especifico del modo foco. */
    ATACARPINTURA   /**< El nodo especifico de ataque en modo pintura. */
};

/**
 * @enum TaskStatus
 * @brief Representa el estado actual de una tarea dentro del árbol de utilidad.
 *
 * Esta enumeración se utiliza  para indicar el resultado o progreso de una tarea dentro del árbol de utilidad.
 */
enum class TaskStatus {
    RUNNING,    /**< La tarea se está ejecutando actualmente. */
    SUCCESS,    /**< La tarea se completó con éxito. */
    FAILURE,    /**< La tarea ha fallado. */
    WAITING     /**< La tarea está en espera antes de poder ejecutarse. */
};

/**
 * @class NodoUB
 * @brief Clase base abstracta que representa un nodo de comportamiento en un árbol de decisión.
 *
 * @param TaskStatus Define el estado en la que se encuentra el nodo
 * 
 * Cada nodo mantiene un estado asociado (TaskStatus) y debe implementar el método `run`, que define su lógica principal. 
 * Tambien un invoke que mide el tiempo de ejecucion, si tiene tiempo sigue y si no sale del arbol
 * Dentro del codigo no existe ningun NodoUB, sino que varias clases heredan de NodoUB.
 * Tiene diversos seters y un get para gestionar TaskStatus
 * 
 */
class NodoUB{
    private:
        TaskStatus status;
    public:
        virtual ~NodoUB() = default;

        TaskStatus getStatus() const { return status; }
        void setStatusRunning() { status = TaskStatus::RUNNING; }
        void setStatusSuccess() { status = TaskStatus::SUCCESS; }
        void setStatusWaiting() { status = TaskStatus::WAITING; }
        void setStatusFailure() { status = TaskStatus::FAILURE; }

        TaskStatus invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end);
        virtual TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) = 0;

};

/**
 * @class NodoPrimerNivel
 * @brief Nodo del primer nivel en el árbol de comportamiento, hereda de NodoUB.
 * 
 * Esta clase representa un nodo de alto nivel que agrupa y ejecuta subnodos (childs)
 * permitiendo una estructura jerárquica. Cada NodoPrimerNivel tiene un tipo (`NodoClass`) 
 * el cual contiene la clase a la que pertenece (Huir, Atacar, Wait, Camino y PowerUp)
 * 
 * Métodos:
 * - `addChild`: Añade nodos hijos al nodo actual.
 * - `calculateUtility`: Calcula su propia utilidad
 * - `run`: Ejecuta la lógica del nodo y sus hijos si es necesario.
 */
class NodoPrimerNivel : public NodoUB {
protected:
    NodoClass nodoClass; 
    std::vector<std::shared_ptr<NodoUB>> childs;

public:
    ~NodoPrimerNivel() = default;
    NodoPrimerNivel(NodoClass type) : nodoClass(type) {}

    void addChild(std::shared_ptr<NodoUB> child) {
        childs.push_back(child);
    }

    NodoClass getNodoClass() { return nodoClass; }

    virtual float calculateUtility(EntityManager& em, Entity& e) = 0;

    TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

/**
 * @class Root
 * @brief Nodo raíz del árbol de comportamiento encargado de gestionar todos los nodos
 * 
 * La clase Root actúa como el punto de entrada principal del árbol de decisiones. 
 * Contiene una serie de hijos que son nodos de primer nivel (`NodoPrimerNivel`) y se encarga de seleccionar 
 * y ejecutar el nodo con la mayor utilidad mediante el método `evaluateAndExecute`.
 * 
 * Métodos principales:
 * - `addChild`: agrega un nodo hijo de primer nivel al árbol.
 * - `evaluateAndExecute`: evalúa todos los hijos y ejecuta aquel con la mayor utilidad.
 */
class Root {
    private:
        std::vector<std::shared_ptr<NodoPrimerNivel>> childs;
    public:
        void addChild(std::shared_ptr<NodoPrimerNivel> child) {
            childs.push_back(child);
        }
        void evaluateAndExecute(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end);
};

/**
 * @class Huir
 * @brief Nodo de comportamiento que representa una acción de huida en el árbol de decisiones.
 * 
 * Esta clase hereda de `NodoPrimerNivel` y representa un nodo específico cuyo objetivo es ejecutar
 * una acción evasiva (huir) dentro del sistema de IA. 
 * 
 * Se inicializa con el tipo `NodoClass::HUIR` y debe implementar el método `calculateUtility`
 * para evaluar si el ataque es conveniente en el contexto dado.
 */
class Huir : public NodoPrimerNivel {
    public:
        Huir() : NodoPrimerNivel(NodoClass::HUIR){}
        float calculateUtility(EntityManager& em, Entity& e) override;
};

/**
 * @class Camino
 * @brief Nodo de comportamiento que representa una acción de desplazamiento o movimiento.
 * 
 * Esta clase hereda de `NodoPrimerNivel` y define una acción centrada en el movimiento por el mapa.
 * Busca una casilla en el mapa que sea util y ahi coloca una bomba.
 * 
 * Se inicializa con el tipo `NodoClass::CAMINO` y debe implementar el método `calculateUtility` 
 * para determinar si el movimiento es la acción más adecuada según el contexto.
 */
class Camino : public NodoPrimerNivel {
    public:
        Camino() : NodoPrimerNivel(NodoClass::CAMINO){}
        float calculateUtility(EntityManager& em, Entity& e) override;
};

/**
 * @class Atacar
 * @brief Nodo de comportamiento que representa una acción ofensiva en el árbol de decisiones. 
 * 
 * Esta clase hereda de `NodoPrimerNivel` y define un comportamiento orientado al ataque.
 * Es utilizada cuando la IA considera que atacar es la acción más efectiva según la situación actual.
 * 
 * Se inicializa con el tipo `NodoClass::ATACAR` y debe implementar el método `calculateUtility`
 * para evaluar si el ataque es conveniente en el contexto dado.
 */
class Atacar : public NodoPrimerNivel {
    public:
        Atacar() : NodoPrimerNivel(NodoClass::ATACAR){}
        float calculateUtility(EntityManager& em, Entity& e) override;
};

/**
 * @class RecogerPowerUp
 * @brief Nodo de comportamiento que va a recoger un powerup. 
 * 
 * Esta clase hereda de `NodoPrimerNivel` y define un comportamiento en el que busca y va a recoger un powerup.
 * Es utilizada cuando la IA considera que buscar un powerup es la acción más efectiva según la situación actual.
 * 
 * Se inicializa con el tipo `NodoClass::POWERUP` y debe implementar el método `calculateUtility`
 * para evaluar si el ataque es conveniente en el contexto dado.
 */
class RecogerPowerup : public NodoPrimerNivel {
    public:
        RecogerPowerup() : NodoPrimerNivel(NodoClass::POWERUP){}
        float calculateUtility(EntityManager& em, Entity& e) override;
};

/**
 * @class Wait
 * @brief Nodo de comportamiento que representa una acción de espera o inactividad controlada.
 * 
 * Esta clase hereda de `NodoPrimerNivel` y modela un comportamiento en el que la IA decide no actuar 
 * inmediatamente. Se utiliza para mantener la posición.
 * 
 * Se inicializa con el tipo `NodoClass::WAIT` y debe implementar el método `calculateUtility` 
 * para determinar si la espera es la mejor decisión en el contexto actual.
 */
class Wait : public NodoPrimerNivel {
    public:
        Wait() : NodoPrimerNivel(NodoClass::WAIT) {}
        float calculateUtility(EntityManager& em, Entity& e) override;
};

/**
 * @class Foco
 * @brief Nodo de comportamiento reservado exclusivamente a ejecutar el modo Foco
 * 
 * Esta clase hereda de `NodoPrimerNivel` y modela un comportamiento en el que la IA decide a quien atacar
 * con el foco y procede a ello.
 * 
 * Se inicializa con el tipo `NodoClass::FOCO` y debe implementar el método `calculateUtility` 
 * aun que no lo utilice porque no compite con mas nodos. Forma parte de otro arbol.
 */
class Foco : public NodoPrimerNivel {
    public:
        Foco() : NodoPrimerNivel(NodoClass::FOCO) {}
        float calculateUtility(EntityManager& em, Entity& e) override;

};

/**
 * @class AtacarPintura
 * @brief Nodo de comportamiento que representa una acción ofensiva en el árbol de decisiones exlusivo para el modo pintura. 
 * 
 * Esta clase hereda de `NodoPrimerNivel` y define un comportamiento orientado al ataque exlusivo del modo pintura.
 * Es utilizada cuando la IA considera que debe pintar mas
 * 
 * Se inicializa con el tipo `NodoClass::ATACARPINTURA` y debe implementar el método `calculateUtility`
 * para evaluar si el ataque es conveniente en el contexto dado.
 */
class AtacarPintura : public NodoPrimerNivel {
    public:
        AtacarPintura() : NodoPrimerNivel(NodoClass::ATACARPINTURA){}
        float calculateUtility(EntityManager& em, Entity& e) override;
};

/**
 * @class MoverNPC
 * @brief Nodo de comportamiento que implementa el movimiento físico del NPC en el entorno.
 * 
 * Esta clase hereda de `NodoUB` y representa una acción concreta de bajo nivel que mueve al NPC 
 * dentro del mapa. Es utilizada como nodo hijo dentro de un nodo de primer nivel como `Camino`, 
 * donde la lógica general decide moverse y este nodo ejecuta la acción.
 * 
 * Debe implementar el método `run`, el cual contiene la lógica de movimiento 
 */
class MoverNPC : public NodoUB{
    public:
        MoverNPC() = default;
        TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

/**
 * @class PararNPC
 * @brief Nodo de comportamiento que detiene el movimiento del NPC o lo mantiene en su posición actual.
 * 
 * Esta clase hereda de `NodoUB` y representa una acción concreta de bajo nivel en la que el NPC 
 * no realiza desplazamientos. Se usa solo como hijo del nodo Wait
 * 
 * La lógica se implementa en el método `run`. 
 */
class PararNPC : public NodoUB{
    public:
        PararNPC() = default;
        TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

/**
 * @class ColocarBomba
 * @brief Nodo de comportamiento que representa la acción de colocar una bomba por parte del NPC.
 * 
 * Esta clase hereda de `NodoUB` y define una acción concreta de bajo nivel, utilizada para ejecutar
 * la colocación de una bomba en el entorno del juego. 
 * 
 * La lógica específica de colocación se implementa en el método `run`
 */
class ColocarBomba : public NodoUB{
    public:
        ColocarBomba() = default;
        TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

/**
 * @class CrearCamino
 * @brief Nodo de comportamiento que genera o actualiza una ruta de navegación para el NPC.
 * 
 * Esta clase hereda de `NodoUB` y representa una acción concreta de bajo nivel utilizada para 
 * calcular o ajustar el camino que seguirá el NPC dentro del entorno con astar. 
 * 
 * Normalmente se utiliza como nodo hijo de `Camino` o `Huir`, dependiendo del tipo de desplazamiento.
 * La lógica de planificación se implementa en el método `run`
 */
class CrearCamino : public NodoUB{
    public:
        CrearCamino() = default;
        TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

/**
 * @class EscogerFoco
 * @brief Nodo de comportamiento exlusivo del modo foco encargado de seleccionar el enemigo principal del NPC
 * 
 * Esta clase hereda de `NodoUB` y representa una acción concreta de bajo nivel cuyo propósito es 
 * determinar qué entidad a la que debe atacar con el foco
 * 
 * Es útil para ajustar la atención de la IA en el modo foco
 * La lógica de selección se implementa en el método `run`
 */
class EscogerFoco : public NodoUB{
    public:
        EscogerFoco() = default;
        TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

/**
 * @class MoverFoco
 * @brief Nodo de comportamiento que mueve al NPC cuando esta en modo foco
 * 
 * Esta clase hereda de `NodoUB` y representa una acción concreta de bajo nivel cuyo propósito es 
 * desplazar al NPC cuando esta en modo foco
 * 
 * La lógica de movimiento se implementa en el método `run`
 */
class MoverFoco : public NodoUB{
    public:
        MoverFoco() = default;
        TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

/**
 * @class AtacarFoco
 * @brief Nodo de comportamiento que realiza un ataque cuando se encuentra en modo foco
 * 
 * Esta clase hereda de `NodoUB` y representa una acción concreta de bajo nivel donde el NPC ejecuta
 * un ataque contra el objetivo prioritario definido previamente por el nodo `EscogerFoco`.
 * 
 * La lógica específica del ataque se implementa en el método `run`
 */
class AtacarFoco : public NodoUB{
    public:
        AtacarFoco() = default;
        TaskStatus run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) override;
};

#endif // UTILITY_SYSTEM_H