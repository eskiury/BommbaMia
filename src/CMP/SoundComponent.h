#pragma once

#include <unordered_map>
#include <vector>
#include <string>

/**
 * @file SoundComponent.h
 * @brief Define los eventos y el componente de sonido para entidades en el sistema ECS.
 */

/**
 * @struct SoundEvent
 * @brief Representa un evento de sonido individual reproducido mediante FMOD.
 *
 * Esta estructura almacena los datos necesarios para controlar un sonido,
 * incluyendo su ruta, configuración de reproducción y parámetros dinámicos.
 */
struct SoundEvent
{
    std::string eventPath;                             ///< Ruta del evento en FMOD (por ejemplo: "event:/Explosion").
    bool playOnce{false};                              ///< Si es verdadero, el sonido se reproduce una única vez.
    bool looping{false};                               ///< Si es verdadero, el sonido se reproduce en bucle.
    bool isPlaying{false};                             ///< Estado actual del evento (reproduciéndose o no).
    float volume{1.0f};                                ///< Volumen del sonido (de 0.0 a 1.0).
    std::unordered_map<std::string, float> parameters; ///< Parámetros personalizados para el evento (posición, intensidad, etc.).
    int soundId{0};                                    ///< Identificador único del sonido en el sistema de audio.

    /**
     * @brief Establece un valor para un parámetro del evento.
     *
     * @param name Nombre del parámetro.
     * @param value Valor que se desea asignar.
     */
    void setParameter(const std::string &name, float value)
    {
        parameters[name] = value;
    }

    /**
     * @brief Recupera el valor de un parámetro del evento.
     *
     * @param name Nombre del parámetro a consultar.
     * @return Valor del parámetro si existe, o 0.0f si no se encuentra.
     */
    float getParameter(const std::string &name) const
    {
        auto it = parameters.find(name);
        return (it != parameters.end()) ? it->second : 0.0f;
    }
};

/**
 * @struct soundComponent
 * @brief Componente que gestiona eventos de sonido asociados a una entidad.
 *
 * Permite reproducir, pausar y actualizar múltiples sonidos a través de una lista de eventos.
 * Ideal para juegos con efectos de sonido dinámicos y personalizados.
 */
struct soundComponent
{
    std::vector<SoundEvent> events; ///< Lista de eventos de sonido activos para la entidad.

    /**
     * @brief Agrega un nuevo evento de sonido al componente.
     *
     * @param event Evento que se desea agregar.
     */
    void addSoundEvent(const SoundEvent &event)
    {
        events.push_back(event);
    }

    /**
     * @brief Recupera un puntero a un evento de sonido existente por su ruta.
     *
     * @param eventPath Ruta del evento (coincide con la ruta definida en FMOD).
     * @return Puntero al evento si se encuentra, o nullptr si no existe.
     */
    SoundEvent *getSoundEvent(const std::string &eventPath)
    {
        for (auto &event : events)
        {
            if (event.eventPath == eventPath)
                return &event;
        }
        return nullptr;
    }
};
