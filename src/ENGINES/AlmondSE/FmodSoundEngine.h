#pragma once

#include "AlmondSE.h"
#include "../../AlmondGE/EntityManager.h"
#include <fmod_studio.h>
#include <fmod_errors.h>
#include <unordered_map>
#include <string>
#include <iostream>

/**
 * @brief Macro para verificar errores de FMOD y reportarlos por consola.
 */
#define ERR_CHECK(result) \
    if ((result) != FMOD_OK) { \
        std::cerr << "FMOD Error: " << FMOD_ErrorString(result) << std::endl; \
    }

/**
 * @brief Implementación del motor de sonido usando FMOD.
 *
 * Esta clase implementa la interfaz AlmondSE, integrando FMOD Studio para la gestión de
 * eventos de audio, bancos de sonido, reproducción, control y actualización dinámica.
 */
class FmodSoundEngine : public AlmondSE {
public:
    /**
     * @brief Constructor.
     * 
     * @param entityManager Referencia al administrador de entidades.
     */
    explicit FmodSoundEngine(EntityManager &entityManager);

    /**
     * @brief Destructor.
     * 
     * Libera los recursos asociados a FMOD.
     */
    ~FmodSoundEngine() override;

    /**
     * @brief Inicializa el sistema FMOD y carga configuración.
     * 
     * @param configFilePath Ruta al archivo de configuración.
     */
    void init(const std::string &configFilePath) override;

    /**
     * @brief Finaliza el sistema FMOD y libera recursos.
     */
    void shutdown() override;

    /**
     * @brief Reproduce el sonido asociado a una entidad.
     * 
     * @param em EntityManager que contiene la entidad.
     * @param entity Entidad cuyo sonido será reproducido.
     */
    void playSound(EntityManager &em, Entity &entity) override;

    /**
     * @brief Detiene el sonido asociado a una entidad.
     * 
     * @param em EntityManager que contiene la entidad.
     * @param entity Entidad cuyo sonido será detenido.
     */
    void stopSound(EntityManager &em, Entity &entity) override;

    /**
     * @brief Pausa o reanuda el sonido asociado a una entidad.
     * 
     * @param em EntityManager que contiene la entidad.
     * @param entity Entidad cuyo sonido será pausado o reanudado.
     * @param pause True para pausar, false para reanudar.
     */
    void pauseSound(EntityManager &em, Entity &entity, bool pause) override;

    /**
     * @brief Actualiza parámetros dinámicos de sonido para una entidad.
     * 
     * @param em EntityManager que contiene la entidad.
     * @param entity Entidad cuyos parámetros de sonido serán actualizados.
     */
    void updateSoundParameters(EntityManager &em, Entity &entity) override;

    /**
     * @brief Actualiza el sistema FMOD (debe llamarse periódicamente).
     */
    void update() override;

    /**
     * @brief Carga un banco de sonido desde un archivo.
     * 
     * @param bankPath Ruta del banco de sonido a cargar.
     */
    void loadBank(const std::string &bankPath);

    /**
     * @brief Establece el volumen de un bus de audio.
     * 
     * @param busPath Ruta del bus en FMOD.
     * @param volume Volumen (0.0f a 1.0f).
     */
    void setBusVolume(const std::string &busPath, float volume);

    /**
     * @brief Reproduce un evento de sonido único (sin seguimiento posterior).
     * 
     * @param eventPath Ruta del evento de FMOD.
     */
    void playUnaVez(const std::string &eventPath) override;

    /**
     * @brief Descarga un banco de sonido.
     * 
     * @param bankPath Ruta del banco a descargar.
     */
    void unloadBank(const std::string &bankPath);

    /**
     * @brief Carga bancos de sonido específicos para una escena leyendo un archivo JSON.
     * 
     * @param sceneName Nombre de la escena.
     * @param jsonPath Ruta al JSON con la configuración de bancos.
     */
    void loadBanksForScene(const std::string &sceneName, const std::string &jsonPath) override;

    /**
     * @brief Descarga bancos de sonido de una escena leyendo un archivo JSON.
     * 
     * @param sceneName Nombre de la escena.
     * @param jsonPath Ruta al JSON con la configuración de bancos.
     */
    void unloadBanksForScene(const std::string &sceneName, const std::string &jsonPath ) override;

    /**
     * @brief Carga todos los bancos definidos en un archivo JSON.
     * 
     * @param jsonPath Ruta al archivo JSON.
     */
    void loadAllBanks(const std::string &jsonPath);

    /**
     * @brief Descarga todos los bancos actualmente cargados.
     */
    void unloadAllBanks();

    /**
     * @brief Detiene todos los sonidos activos.
     * 
     * @param em EntityManager que contiene las entidades.
     */
    void stopAllSounds(EntityManager &em);

    /**
     * @brief Detiene el sonido de una entidad específica por su ID.
     * 
     * @param em EntityManager que contiene las entidades.
     * @param entityId ID de la entidad cuyo sonido será detenido.
     */
    void stopSoundById(EntityManager &em, int entityId);

    /**
     * @brief Reproduce la música de fondo asociada a una entidad.
     * 
     * @param em EntityManager que contiene la entidad.
     * @param entity Entidad con música de fondo.
     */
    void playMusicaFondo(EntityManager &em, Entity &entity);

    /**
     * @brief Reproduce un efecto de sonido asociado a una entidad.
     * 
     * @param em EntityManager que contiene la entidad.
     * @param entity Entidad con efecto de sonido.
     */
    void playSoundEffect(EntityManager &em, Entity &entity);

private:
    EntityManager &entityManager; ///< Referencia al EntityManager.

    FMOD_STUDIO_SYSTEM *system{nullptr}; ///< Puntero al sistema FMOD Studio.
    FMOD_SYSTEM *coreSystem{nullptr};    ///< Puntero al sistema FMOD Core.

    std::unordered_map<std::string, FMOD_STUDIO_BANK *> banks; ///< Bancos de sonido cargados.
    std::unordered_map<uint16_t, std::vector<FMOD_STUDIO_EVENTINSTANCE*>> eventInstances; ///< Instancias de eventos por entidad.
    std::unordered_map<std::string, FMOD_STUDIO_EVENTDESCRIPTION*> eventDescriptionsCache; ///< Cache de descripciones de eventos.

    /**
     * @brief Obtiene la descripción de un evento FMOD, con cacheo para evitar recargas.
     * 
     * @param eventPath Ruta del evento.
     * @return Puntero a la descripción del evento.
     */
    FMOD_STUDIO_EVENTDESCRIPTION* getEventDescription(const std::string &eventPath);
};
