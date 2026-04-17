/**
 * @file SoundSystem.h
 * @brief Sistema de sonido basado en el patrón Singleton que encapsula la lógica de audio del juego.
 *
 * Utiliza una implementación concreta del motor de sonido (`AlmondSE`, como FMOD) para reproducir sonidos,
 * gestionar bancos de audio y actualizar el oyente 3D.
 */

 #pragma once

 #include <memory>
 #include "../../AlmondGE/EntityManager.h"
 #include "../ENGINES/AlmondSE/AlmondSE.h"
 #include "../ENGINES/AlmondSE/FmodSoundEngine.h"
 #include "../../utils/miVector3.h"
 
 /**
  * @class SoundSystem
  * @brief Sistema centralizado de gestión de sonido en el juego (Singleton).
  *
  * Se encarga de la reproducción, pausa, parada y control del volumen de sonidos, además de cargar
  * bancos de sonidos para escenas específicas.
  */
 class SoundSystem {
 public:
 
     /**
      * @brief Obtiene la instancia única del sistema de sonido.
      * 
      * Si es la primera vez que se accede, se debe proporcionar un puntero a un EntityManager para inicializar el motor.
      *
      * @param entityManager Puntero al gestor de entidades. Solo se usa en la primera llamada.
      * @return Referencia a la instancia única de SoundSystem.
      */
     static SoundSystem& getInstance(EntityManager* entityManager = nullptr) {
         static SoundSystem instance(
             std::make_unique<FmodSoundEngine>(*entityManager), entityManager
         );
         return instance;
     }
 
     /// Elimina el constructor de copia.
     SoundSystem(const SoundSystem&) = delete;
     /// Elimina el operador de asignación por copia.
     SoundSystem& operator=(const SoundSystem&) = delete;
     /// Elimina el constructor por movimiento.
     SoundSystem(SoundSystem&&) = delete;
     /// Elimina el operador de asignación por movimiento.
     SoundSystem& operator=(SoundSystem&&) = delete;
 
     /**
      * @brief Inicializa el sistema de sonido con un archivo de configuración.
      * @param configFilePath Ruta al archivo JSON de configuración.
      */
     void init(const std::string& configFilePath);
 
     /**
      * @brief Actualiza el estado interno del sistema de sonido. Debe llamarse cada frame.
      */
     void update();
 
     /**
      * @brief Reproduce un sonido asociado a una entidad.
      * @param entity Referencia a la entidad que reproduce el sonido.
      */
     void playSound(Entity& entity);
 
     /**
      * @brief Detiene el sonido asociado a una entidad.
      * @param em Referencia al EntityManager.
      * @param entity Entidad cuyo sonido debe detenerse.
      */
     void stopSound(EntityManager& em, Entity& entity);
 
     /**
      * @brief Pausa o reanuda el sonido de una entidad.
      * @param entity Entidad objetivo.
      * @param pause `true` para pausar, `false` para reanudar.
      */
     void pauseSound(Entity& entity, bool pause);
 
     /**
      * @brief Actualiza la posición y orientación del oyente 3D.
      * @param position Posición del oyente.
      * @param forward Vector de dirección hacia adelante.
      * @param up Vector hacia arriba.
      */
     void updateListener(const MiVector3& position, const MiVector3& forward, const MiVector3& up);
 
     /**
      * @brief Carga un banco de sonidos desde un archivo .bank.
      * @param bankPath Ruta al archivo de banco de sonidos.
      */
     void loadBank(const std::string& bankPath);
 
     /**
      * @brief Descarga un banco de sonidos.
      * @param bankPath Ruta del archivo .bank a descargar.
      */
     void unloadBank(const std::string& bankPath);
 
     /**
      * @brief Carga todos los bancos asociados a una escena desde un archivo JSON.
      * @param sceneName Nombre de la escena.
      * @param jsonPath Ruta al archivo JSON con los bancos por escena.
      */
     void loadBanksForScene(const std::string& sceneName, const std::string& jsonPath);
 
     /**
      * @brief Descarga todos los bancos asociados a una escena.
      * @param sceneName Nombre de la escena.
      * @param jsonPath Ruta al archivo JSON.
      */
     void unloadBanksForScene(const std::string& sceneName, const std::string& jsonPath);
 
     /**
      * @brief Carga todos los bancos definidos en un archivo JSON.
      * @param jsonPath Ruta al archivo JSON.
      */
     void loadAllBanks(const std::string& jsonPath);
 
     /**
      * @brief Descarga todos los bancos cargados.
      */
     void unloadAllBanks();
 
     /**
      * @brief Detiene todos los sonidos activos en todas las entidades.
      * @param em Referencia al EntityManager.
      */
     void stopAllSounds(EntityManager& em);
 
     /**
      * @brief Detiene el sonido de una entidad específica mediante su ID.
      * @param em Referencia al EntityManager.
      * @param entityId ID numérico de la entidad.
      */
     void stopSoundById(EntityManager& em, int entityId);
 
     /**
      * @brief Reproduce música de fondo asociada a una entidad.
      * @param em Referencia al EntityManager.
      * @param entity Entidad a la que se le asocia la música.
      */
     void playMusicaFondo(EntityManager& em, Entity& entity);
 
     /**
      * @brief Reproduce un efecto de sonido específico (distinto de música).
      * @param em Referencia al EntityManager.
      * @param entity Entidad que ejecuta el efecto.
      */
     void playSoundEffect(EntityManager& em, Entity& entity);
 
     /**
      * @brief Ajusta el volumen de un bus de sonido.
      * @param busName Nombre del bus.
      * @param volume Valor de volumen entre 0.0 (silencio) y 1.0 (máximo).
      */
     void setBusVolume(const std::string& busName, float volume);
 
 private:
     /**
      * @brief Constructor privado (Singleton).
      * @param engine Implementación concreta del motor de sonido (por ejemplo, FmodSoundEngine).
      * @param entityManager Puntero al gestor de entidades.
      */
     SoundSystem(std::unique_ptr<AlmondSE> engine, EntityManager* entityManager);
 
     /**
      * @brief Destructor.
      */
     ~SoundSystem();
 
     /// Motor de sonido utilizado (FMOD, Wwise, etc.).
     std::unique_ptr<AlmondSE> engine;
 
     /// Referencia al gestor de entidades.
     EntityManager* entityManager;
 };
 