/**
 * @file EventSystem.h
 * @brief Sistema de eventos para la gestión de acciones en el juego.
 *
 * Este sistema permite suscribirse y reaccionar a eventos específicos como movimientos,
 * acciones de los jugadores y eventos del sistema (pausa, entrada, etc.).
 */

#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <unordered_map>
#include <vector>
#include <functional>
#include "../AlmondGE/EntityManager.h"

/**
 * @enum EventType
 * @brief Enumeración de tipos de eventos disponibles en el juego.
 */
enum class EventType
{
    MOVE_RIGHT, ///< Movimiento hacia la derecha.
    MOVE_LEFT,  ///< Movimiento hacia la izquierda.
    MOVE_UP,    ///< Movimiento hacia arriba.
    MOVE_DOWN,  ///< Movimiento hacia abajo.
    PLACE_BOMB, ///< Acción de colocar una bomba.
    PAUSE,      ///< Evento de pausa del juego.
    ENTER       ///< Evento de entrada (por ejemplo, menú o confirmación).
};

/**
 * @class EventSystem
 * @brief Sistema encargado de la gestión y propagación de eventos.
 *
 * Permite suscribirse a tipos de eventos y ejecutar funciones asociadas cuando dichos eventos son disparados.
 */
class EventSystem
{
public:
    /**
     * @brief Tipo de función callback que maneja eventos.
     *
     * Cada callback recibe una referencia a una entidad afectada por el evento.
     */
    using EventCallback = std::function<void(Entity &)>;

    /**
     * @brief Suscribe una función callback a un tipo de evento.
     *
     * Cuando el evento especificado ocurra, la función callback será invocada.
     *
     * @param eventType Tipo de evento al que se quiere suscribir.
     * @param callback Función que se ejecutará cuando el evento ocurra.
     */
    void subscribe(EventType eventType, EventCallback callback);

    /**
     * @brief Dispara un evento de forma inmediata.
     *
     * Ejecuta todas las funciones callback registradas para el tipo de evento especificado.
     *
     * @param eventType Tipo de evento a disparar.
     * @param entity Referencia a la entidad afectada por el evento.
     */
    void triggerEvent(EventType eventType, Entity &entity);

    /**
     * @brief Limpia todas las suscripciones registradas.
     *
     * Elimina todos los callbacks asociados a los eventos. Útil para reiniciar o limpiar el sistema.
     */
    void clear();

private:
    /**
     * @brief Mapa que relaciona cada tipo de evento con su lista de callbacks.
     */
    std::unordered_map<EventType, std::vector<EventCallback>> listeners;
};

#endif // EVENTSYSTEM_H
