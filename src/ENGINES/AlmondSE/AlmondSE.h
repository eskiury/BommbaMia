#pragma once

#include "../../AlmondGE/EntityManager.h"
#include <string>

/**
 * @file AlmondSE.h
 * @brief Interfaz abstracta para el motor de sonido del juego.
 *
 * Define una fachada para la integración de sistemas de audio como FMOD,
 * permitiendo operaciones de inicialización, reproducción, control y gestión de bancos de sonido.
 */
class AlmondSE
{
public:
    /**
     * @brief Destructor virtual por defecto.
     */
    virtual ~AlmondSE() = default;

    /**
     * @brief Inicializa el motor de sonido.
     *
     * Lee la configuración desde el archivo especificado.
     *
     * @param configFilePath Ruta del archivo de configuración.
     */
    virtual void init(const std::string &configFilePath) = 0;

    /**
     * @brief Detiene el motor de sonido.
     */
    virtual void shutdown() = 0;

    /**
     * @brief Reproduce el sonido asociado a una entidad.
     *
     * Utiliza la información del componente de sonido de la entidad para reproducir el evento correspondiente.
     *
     * @param em Referencia al EntityManager.
     * @param entity Entidad que contiene el componente de sonido.
     */
    virtual void playSound(EntityManager &em, Entity &entity) = 0;

    /**
     * @brief Detiene el sonido asociado a una entidad.
     *
     * @param em Referencia al EntityManager.
     * @param entity Entidad que contiene el componente de sonido.
     */
    virtual void stopSound(EntityManager &em, Entity &entity) = 0;

    /**
     * @brief Pausa o reanuda el sonido asociado a una entidad.
     *
     * @param em Referencia al EntityManager.
     * @param entity Entidad que contiene el componente de sonido.
     * @param pause Si es true, pausa el sonido; si es false, lo reanuda.
     */
    virtual void pauseSound(EntityManager &em, Entity &entity, bool pause) = 0;

    /**
     * @brief Actualiza los parámetros del sonido asociado a una entidad.
     *
     * Por ejemplo, puede actualizar la posición o la intensidad de un sonido.
     *
     * @param em Referencia al EntityManager.
     * @param entity Entidad que contiene el componente de sonido.
     */
    virtual void updateSoundParameters(EntityManager &em, Entity &entity) = 0;

    /**
     * @brief Establece el volumen de un bus de audio.
     *
     * @param busPath Ruta del bus.
     * @param volume Volumen a establecer (por ejemplo, de 0.0f a 1.0f).
     */
    virtual void setBusVolume(const std::string &busPath, float volume) = 0;

    /**
     * @brief Reproduce un evento de sonido de forma única.
     *
     * Este método se utiliza para reproducir un sonido que no se mantendrá activo,
     * sino que se reproducirá una única vez.
     *
     * @param eventPath Ruta del evento de FMOD.
     */
    virtual void playUnaVez(const std::string &eventPath) = 0;

    /**
     * @brief Carga los bancos de audio para una escena específica.
     *
     * Lee la configuración del JSON y carga los bancos definidos para la escena.
     *
     * @param sceneName Nombre de la escena.
     * @param jsonPath Ruta del archivo JSON con la configuración de bancos.
     */
    virtual void loadBanksForScene(const std::string &sceneName, const std::string &jsonPath) = 0;

    /**
     * @brief Descarga los bancos de audio para una escena específica.
     *
     * Lee la configuración del JSON y descarga los bancos definidos para la escena.
     *
     * @param sceneName Nombre de la escena.
     * @param jsonPath Ruta del archivo JSON con la configuración de bancos.
     */
    virtual void unloadBanksForScene(const std::string &sceneName, const std::string &jsonPath) = 0;

    /**
     * @brief Carga todos los bancos de audio definidos en el archivo JSON.
     *
     * @param jsonPath Ruta del archivo JSON con la configuración de bancos.
     */
    virtual void loadAllBanks(const std::string &jsonPath) = 0;

    /**
     * @brief Descarga todos los bancos de audio cargados.
     */
    virtual void unloadAllBanks() = 0;

    /**
     * @brief Actualiza el motor de sonido.
     *
     * Debe llamarse de forma regular para procesar los eventos y actualizaciones internas.
     */
    virtual void update() = 0;

    /**
     * @brief Detiene todos los sonidos.
     *
     * @param em Referencia al EntityManager.
     * @param entity Entidad que contiene el componente de sonido.
     */
    virtual void stopAllSounds(EntityManager &em) = 0;

    /**
     * @brief Detiene un sonido específico por su ID.
     *
     * @param em Referencia al EntityManager.
     * @param entityId ID de la entidad cuyo sonido se desea detener.
     */
    virtual void stopSoundById(EntityManager &em, int entityId) = 0;

    /**
     * @brief Reproduce la música de fondo asociada a una entidad.
     *
     * @param em Referencia al EntityManager.
     * @param entity Entidad que contiene el componente de sonido.
     */
    virtual void playMusicaFondo(EntityManager &em, Entity &entity) = 0;

    /**
     * @brief Reproduce un efecto de sonido asociado a una entidad.
     *
     * @param em Referencia al EntityManager.
     * @param entity Entidad que contiene el componente de sonido.
     */
    virtual void playSoundEffect(EntityManager &em, Entity &entity) = 0;
};
