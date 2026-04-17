#include "FmodSoundEngine.h"
#include "../../utils/json.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

FmodSoundEngine::FmodSoundEngine(EntityManager &entityManager)
    : entityManager(entityManager), system(nullptr), coreSystem(nullptr) {}

FmodSoundEngine::~FmodSoundEngine()
{
    shutdown();
}

void FmodSoundEngine::init(const std::string &configFilePath)
{
    // Leer el archivo JSON de configuración
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo de configuración: " + configFilePath);
    }

    nlohmann::json config;
    configFile >> config;

    FMOD_RESULT result = FMOD_Studio_System_Create(&system, FMOD_VERSION);
    ERR_CHECK(result);
    if (result != FMOD_OK)
    {
        throw std::runtime_error("Error al crear el sistema FMOD: " + std::string(FMOD_ErrorString(result)));
    }

    result = FMOD_Studio_System_Initialize(system, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
    ERR_CHECK(result);
    if (result != FMOD_OK)
    {
        throw std::runtime_error("Error al inicializar FMOD: " + std::string(FMOD_ErrorString(result)));
    }

    FMOD_Studio_System_GetCoreSystem(system, &coreSystem);

    loadAllBanks(configFilePath);
}

void FmodSoundEngine::unloadBank(const std::string &bankPath)
{
    auto it = banks.find(bankPath);
    if (it != banks.end())
    {
        FMOD_Studio_Bank_Unload(it->second);
        banks.erase(it);
    }
}

void FmodSoundEngine::shutdown()
{

    for (auto &pair : banks)
    {
        FMOD_Studio_Bank_Unload(pair.second);
    }
    banks.clear();

    eventDescriptionsCache.clear();

    if (system)
    {
        FMOD_Studio_System_Release(system);
        system = nullptr;
    }
    std::cout << "FMOD system shut down." << std::endl;
}

void FmodSoundEngine::update()
{
    if (system)
    {
        FMOD_Studio_System_Update(system);
    }
}

FMOD_STUDIO_EVENTDESCRIPTION *FmodSoundEngine::getEventDescription(const std::string &eventPath)
{
    auto it = eventDescriptionsCache.find(eventPath);
    if (it != eventDescriptionsCache.end())
    {
        return it->second;
    }
    FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = nullptr;
    FMOD_RESULT result = FMOD_Studio_System_GetEvent(system, eventPath.c_str(), &eventDescription);
    ERR_CHECK(result);
    if (result == FMOD_OK && eventDescription)
    {
        eventDescriptionsCache[eventPath] = eventDescription;
    }
    return eventDescription;
}

void FmodSoundEngine::playSound(EntityManager &em, Entity &entity)
{
    auto &soundComp = em.sounds.get(entity.sound_key);

    auto it = eventInstances.find(entity.id);
    if (it != eventInstances.end())
    {

        for (size_t i = 0; i < soundComp.events.size(); i++)
        {
            SoundEvent &se = soundComp.events[i];

            if (!se.isPlaying && i < it->second.size() && it->second[i])
            {
                FMOD_STUDIO_PLAYBACK_STATE state;
                FMOD_Studio_EventInstance_GetPlaybackState(it->second[i], &state);
                if (state != FMOD_STUDIO_PLAYBACK_PLAYING)
                {
                    FMOD_Studio_EventInstance_Start(it->second[i]);
                    se.isPlaying = true;
                }
            }
        }
        return;
    }

    std::vector<FMOD_STUDIO_EVENTINSTANCE *> instances;
    for (auto &se : soundComp.events)
    {
        FMOD_STUDIO_EVENTDESCRIPTION *eventDesc = getEventDescription(se.eventPath);
        if (!eventDesc)
        {
            std::cerr << "Error obteniendo evento: " << se.eventPath << "\n";
            instances.push_back(nullptr);
            continue;
        }

        FMOD_STUDIO_EVENTINSTANCE *eventInstance = nullptr;
        FMOD_RESULT result = FMOD_Studio_EventDescription_CreateInstance(eventDesc, &eventInstance);
        ERR_CHECK(result);
        if (result != FMOD_OK || !eventInstance)
        {
            std::cerr << "Error creando instancia de evento: " << se.eventPath << "\n";
            instances.push_back(nullptr);
            continue;
        }

        if (!se.isPlaying)
        {
            FMOD_Studio_EventInstance_Start(eventInstance);
            se.isPlaying = true;
        }

        instances.push_back(eventInstance);
    }

    eventInstances[entity.id] = instances;
}

void FmodSoundEngine::stopAllSounds(EntityManager &em)
{
    for (auto &[entityID, instances] : eventInstances)
    {

        Entity entity;
        entity.id = entityID;

        auto &soundComp = em.sounds.get(entity.sound_key);

        for (size_t i = 0; i < instances.size(); ++i)
        {
            if (instances[i])
            {

                FMOD_Studio_EventInstance_Stop(instances[i], FMOD_STUDIO_STOP_IMMEDIATE);

                if (i < soundComp.events.size())
                {
                    soundComp.events[i].isPlaying = false;
                }
            }
        }
    }
}

void FmodSoundEngine::stopSound(EntityManager &em, Entity &entity)
{
    auto it = eventInstances.find(entity.id);
    if (it != eventInstances.end())
    {
        for (auto instance : it->second)
        {
            if (instance)
            {
                FMOD_Studio_EventInstance_Stop(instance, FMOD_STUDIO_STOP_IMMEDIATE);
                FMOD_Studio_EventInstance_Release(instance);
            }
        }
        eventInstances.erase(it);
    }

    auto &soundComp = em.sounds.get(entity.sound_key);
    for (auto &se : soundComp.events)
    {
        se.isPlaying = false;
    }
}

void FmodSoundEngine::stopSoundById(EntityManager &em, int soundId)
{

    for (auto &[entityID, instances] : eventInstances)
    {

        auto entity = em.getEntity(entityID);
        auto &soundComp = em.sounds.get(entity.sound_key);

        for (size_t i = 0; i < soundComp.events.size(); ++i)
        {
            auto &se = soundComp.events[i];
            if (se.soundId == soundId && se.isPlaying)
            {
                FMOD_Studio_EventInstance_Stop(instances[i], FMOD_STUDIO_STOP_IMMEDIATE);
                se.isPlaying = false;
                std::cout << "Sonido " << soundId << " detenido" << std::endl;
            }
        }
    }
}

void FmodSoundEngine::pauseSound(EntityManager &em, Entity &entity, bool pause)
{
    auto it = eventInstances.find(entity.id);
    if (it != eventInstances.end())
    {
        for (auto instance : it->second)
        {
            if (instance)
            {
                FMOD_Studio_EventInstance_SetPaused(instance, pause);
            }
        }
    }
}

void FmodSoundEngine::updateSoundParameters(EntityManager &em, Entity &entity)
{
    auto &soundComp = em.sounds.get(entity.sound_key);
    auto it = eventInstances.find(entity.id);
    if (it != eventInstances.end())
    {

        for (size_t i = 0; i < soundComp.events.size(); i++)
        {
            if (i < it->second.size() && it->second[i])
            {
                for (const auto &param : soundComp.events[i].parameters)
                {
                    FMOD_RESULT result = FMOD_Studio_EventInstance_SetParameterByName(
                        it->second[i],
                        param.first.c_str(),
                        param.second,
                        false);
                    ERR_CHECK(result);
                }
            }
        }
    }
}

void FmodSoundEngine::loadBank(const std::string &bankPath)
{
    if (banks.find(bankPath) != banks.end())
        return;

    if (!system)
    {
        std::cerr << "Error: FMOD system is not initialized. Cannot load bank: " << bankPath << std::endl;
        return;
    }

    FMOD_STUDIO_BANK *bank = nullptr;
    FMOD_RESULT result = FMOD_Studio_System_LoadBankFile(system, bankPath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
    ERR_CHECK(result);
    if (result == FMOD_OK)
    {
        banks[bankPath] = bank;
    }
}

void FmodSoundEngine::setBusVolume(const std::string &busPath, float volume)
{
    FMOD_STUDIO_BUS *bus = nullptr;
    FMOD_RESULT result = FMOD_Studio_System_GetBus(system, busPath.c_str(), &bus);
    ERR_CHECK(result);
    if (result == FMOD_OK)
    {
        FMOD_Studio_Bus_SetVolume(bus, volume);
    }
}

void FmodSoundEngine::playUnaVez(const std::string &eventPath)
{
    FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = getEventDescription(eventPath);
    if (!eventDescription)
        return;

    FMOD_STUDIO_EVENTINSTANCE *eventInstance = nullptr;
    FMOD_RESULT result = FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance);
    ERR_CHECK(result);
    if (result != FMOD_OK)
        return;

    FMOD_Studio_EventInstance_Start(eventInstance);
    FMOD_Studio_EventInstance_Release(eventInstance);
}

void FmodSoundEngine::loadBanksForScene(const std::string &sceneName, const std::string &jsonPath)
{
    std::ifstream file(jsonPath);
    if (!file.is_open())
    {
        std::cerr << "No se pudo abrir " << jsonPath << std::endl;
        return;
    }
    nlohmann::json config;
    file >> config;

    if (sceneName == "global")
    {
        if (config.contains("global") && config["global"].contains("banks"))
        {
            auto banksList = config["global"]["banks"];
            for (const auto &bank : banksList)
            {
                std::string bankPath = bank.get<std::string>();
                loadBank(bankPath);
                std::cout << "Cargado banco: " << bankPath << " para la escena global" << std::endl;
            }
        }
        else
        {
            std::cerr << "No se definieron bancos globales en el JSON." << std::endl;
        }
    }
    else
    {

        if (config.contains("scenes") && config["scenes"].contains(sceneName))
        {
            auto banksList = config["scenes"][sceneName]["banks"];
            for (const auto &bank : banksList)
            {
                std::string bankPath = bank.get<std::string>();
                loadBank(bankPath);
                std::cout << "Cargado banco: " << bankPath << " para la escena " << sceneName << std::endl;
            }
        }
        else
        {
            std::cerr << "No se definieron bancos para la escena: " << sceneName << std::endl;
        }
    }
}

void FmodSoundEngine::unloadBanksForScene(const std::string &sceneName, const std::string &jsonPath)
{
    std::ifstream file(jsonPath);
    if (!file.is_open())
    {
        std::cerr << "No se pudo abrir " << jsonPath << std::endl;
        return;
    }
    nlohmann::json config;
    file >> config;

    if (sceneName == "global")
    {
        if (config.contains("global") && config["global"].contains("banks"))
        {
            auto banksList = config["global"]["banks"];
            for (const auto &bank : banksList)
            {
                std::string bankPath = bank.get<std::string>();
                unloadBank(bankPath);
                std::cout << "Descargado banco: " << bankPath << " para la escena global" << std::endl;
            }
        }
        else
        {
            std::cerr << "No se definieron bancos globales en el JSON." << std::endl;
        }
    }
    else
    {
        if (config.contains("scenes") && config["scenes"].contains(sceneName))
        {
            auto banksList = config["scenes"][sceneName]["banks"];
            for (const auto &bank : banksList)
            {
                std::string bankPath = bank.get<std::string>();
                unloadBank(bankPath);
                std::cout << "Descargado banco: " << bankPath << " para la escena " << sceneName << std::endl;
            }
        }
        else
        {
            std::cerr << "No se definieron bancos para la escena: " << sceneName << std::endl;
        }
    }
}

void FmodSoundEngine::loadAllBanks(const std::string &jsonPath)
{
    std::ifstream file(jsonPath);
    if (!file.is_open())
    {
        std::cerr << "No se pudo abrir " << jsonPath << std::endl;
        return;
    }
    nlohmann::json config;
    file >> config;

    if (config.contains("global") && config["global"].contains("banks"))
    {
        for (const auto &bankPath : config["global"]["banks"])
        {
            loadBank(bankPath.get<std::string>());
            std::cout << "Banco global cargado: " << bankPath << std::endl;
        }
    }

    if (config.contains("scenes"))
    {
        for (auto &scene : config["scenes"].items())
        {
            auto banksList = scene.value()["banks"];
            for (const auto &bank : banksList)
            {
                loadBank(bank.get<std::string>());
                std::cout << "Banco de escena (" << scene.key() << ") cargado: " << bank << std::endl;
            }
        }
    }
}

void FmodSoundEngine::unloadAllBanks()
{
    for (auto &pair : banks)
    {
        FMOD_Studio_Bank_Unload(pair.second);
    }
    banks.clear();
    std::cout << "Todos los bancos descargados." << std::endl;
}

void FmodSoundEngine::playMusicaFondo(EntityManager &em, Entity &entity)
{
    auto &soundComp = em.sounds.get(entity.sound_key);

    for (size_t i = 0; i < soundComp.events.size(); ++i)
    {
        SoundEvent &se = soundComp.events[i];
        if (se.isPlaying || se.playOnce)
            continue;

        FMOD_STUDIO_EVENTDESCRIPTION *eventDesc = getEventDescription(se.eventPath);
        if (!eventDesc)
            continue;

        FMOD_STUDIO_EVENTINSTANCE *eventInstance = nullptr;
        FMOD_RESULT result = FMOD_Studio_EventDescription_CreateInstance(eventDesc, &eventInstance);
        ERR_CHECK(result);
        if (result != FMOD_OK || !eventInstance)
            continue;

        FMOD_Studio_EventInstance_Start(eventInstance);
        se.isPlaying = true;

        eventInstances[entity.id].push_back(eventInstance);
    }
}

void FmodSoundEngine::playSoundEffect(EntityManager &em, Entity &entity)
{
    auto &soundComp = em.sounds.get(entity.sound_key);

    for (auto &se : soundComp.events)
    {
        FMOD_STUDIO_EVENTDESCRIPTION *eventDesc = getEventDescription(se.eventPath);
        if (!eventDesc)
            continue;

        FMOD_STUDIO_EVENTINSTANCE *eventInstance = nullptr;
        FMOD_RESULT result = FMOD_Studio_EventDescription_CreateInstance(eventDesc, &eventInstance);
        ERR_CHECK(result);
        if (result != FMOD_OK || !eventInstance)
            continue;

        FMOD_Studio_EventInstance_Start(eventInstance);

        FMOD_Studio_EventInstance_Release(eventInstance);
    }
}
