#include "SoundSystem.h"

SoundSystem::SoundSystem(std::unique_ptr<AlmondSE> engine, EntityManager* entityManager)
    : engine(std::move(engine)), entityManager(entityManager) {}

SoundSystem::~SoundSystem()
{
    if (engine) {
        engine->shutdown();
    }
}

void SoundSystem::init(const std::string& configFilePath)
{
    engine->init(configFilePath);
}

void SoundSystem::update()
{
    auto entities = entityManager->getEntities();

    for (auto& entity : entities)
    {
        if (entity.sound_key.id != -1)
        {
            engine->updateSoundParameters(*entityManager, entity);
        }
    }

    engine->update();
}

void SoundSystem::playSound(Entity& entity)
{
    engine->playSound(*entityManager, entity);
}

void SoundSystem::stopSound(EntityManager& em, Entity& entity)
{
    engine->stopSound(em, entity);
}

void SoundSystem::pauseSound(Entity& entity, bool pause)
{
    engine->pauseSound(*entityManager, entity, pause);
}

void SoundSystem::updateListener(const MiVector3& position, const MiVector3& forward, const MiVector3& up)
{
    
}

void SoundSystem::loadBanksForScene(const std::string& sceneName, const std::string& jsonPath)
{
    engine->loadBanksForScene(sceneName, jsonPath);
}

void SoundSystem::unloadBanksForScene(const std::string& sceneName, const std::string& jsonPath)
{
    engine->unloadBanksForScene(sceneName, jsonPath);
}

void SoundSystem::loadAllBanks(const std::string& jsonPath)
{
    engine->loadAllBanks(jsonPath);
}

void SoundSystem::unloadAllBanks()
{
    engine->unloadAllBanks();
}

void SoundSystem::stopAllSounds(EntityManager& em)
{
    engine->stopAllSounds(em);
}

void SoundSystem::stopSoundById(EntityManager& em, int entityId)
{
    engine->stopSoundById(em, entityId);
}

void SoundSystem::playMusicaFondo(EntityManager& em, Entity& entity)
{
    engine->playMusicaFondo(em, entity);
}

void SoundSystem::playSoundEffect(EntityManager& em, Entity& entity)
{
    engine->playSoundEffect(em, entity);
}

void SoundSystem::setBusVolume(const std::string& busName, float volume)
{
    engine->setBusVolume(busName, volume);
}