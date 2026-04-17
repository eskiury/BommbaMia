#include "EntityManager.h"
#include <algorithm>
#include <iostream>
#include <cmath>

EntityManager::EntityManager(std::size_t defaultSize) : nextId(0)
{
    entities_.reserve(defaultSize);
}

Entity &EntityManager::createEntity(EntityType type)
{
    auto &entity = entities_.emplace_back();
    entity.id = nextId++;
    entity.type = type;
    return entity;
}

void EntityManager::createEntityComponents(Entity &entity, const fisicaComponent &fisica, const renderComponent &render, const powerUpComponent &powerUp, const soundComponent &sound, const bombaComponent &bomba, const iaComponent &bt, const buttonComponent &button)
{
    entity.fisica_key = fisicas.push_back(fisica);
    entity.render_key = renders.push_back(render);
    entity.powerUp_key = powerUps.push_back(powerUp);
    entity.sound_key = sounds.push_back(sound);
    entity.bomb_key = bombs.push_back(bomba);
    entity.ia_key = ia.push_back(bt);
    entity.button_key = buttons.push_back(button);
}

void EntityManager::eraseEntity(int entityId)
{
    auto it = std::find_if(entities_.begin(), entities_.end(),
                           [entityId](const Entity &e)
                           { return e.id == entityId; });

    if (it != entities_.end())
    {

        if (it->type == EntityType::BOMBA)
        {
            auto &bombFisica = bombs.get(it->bomb_key);

            Entity &owner = getEntity(bombFisica.ownerId);
            auto &ownerFisica = bombs.get(owner.bomb_key);

            if (ownerFisica.bombsPlaced > 0)
            {
                ownerFisica.bombsPlaced--;
            }
        }

        if (fisicas.is_valid(it->fisica_key))
        {
            fisicas.erase(it->fisica_key);
        }

        if (renders.is_valid(it->render_key))
        {
            renders.erase(it->render_key);
        }

        if (powerUps.is_valid(it->powerUp_key))
        {
            powerUps.erase(it->powerUp_key);
        }

        if (sounds.is_valid(it->sound_key))
        {
            sounds.erase(it->sound_key);
        }
        if (bombs.is_valid(it->bomb_key))
        {
            bombs.erase(it->bomb_key);
        }
        if (ia.is_valid(it->ia_key))
        {
            ia.erase(it->ia_key);
        }
        if (buttons.is_valid(it->button_key))
        {
            buttons.erase(it->button_key);
        }

        entities_.erase(it);
    }
}

std::vector<Entity> EntityManager::getEntities()
{
    return entities_;
}

void EntityManager::forEach(EntityType tipo, std::function<void(Entity &)> func)
{
    for (auto &e : entities_)
    {
        if (e.type == tipo)
        {
            func(e);
        }
    }
}

Entity &EntityManager::getEntityOfType(EntityType tipo)
{
    auto it = std::find_if(entities_.begin(), entities_.end(),
                           [tipo](const Entity &e)
                           { return e.type == tipo; });

    if (it != entities_.end())
    {
        return *it;
    }
    else
    {
        throw std::runtime_error("Entity of type not found!");
    }
}

Entity &EntityManager::getEntity(int entityId)
{
    auto it = std::find_if(entities_.begin(), entities_.end(),
                           [entityId](const Entity &e)
                           { return e.id == entityId; });
    if (it != entities_.end())
    {
        return *it;
    }
    else
    {
        throw std::runtime_error("Entity with specified ID not found!");
    }
}

std::vector<Entity> EntityManager::getEntitiesByType(EntityType tipo)
{
    std::vector<Entity> result;
    for (const auto &entity : entities_)
    {
        if (entity.type == tipo)
        {
            result.push_back(entity);
        }
    }
    return result;
}

std::vector<Entity *> EntityManager::getEntitiesByTypePunteros(EntityType tipo)
{
    std::vector<Entity *> result;
    for (auto &entity : entities_)
    {
        if (entity.type == tipo)
        {
            result.push_back(&entity);
        }
    }
    return result;
}

MiVector2 EntityManager::getTilePosition(MiVector3 position)
{
    MiVector2 tilePos;
    tilePos.x = (position.x / TILE_SIZE);
    tilePos.y = (position.z / TILE_SIZE);
    return tilePos;
}

void EntityManager::clear()
{
    for (auto &e : entities_)
    {
        if (fisicas.is_valid(e.fisica_key))
        {
            fisicas.erase(e.fisica_key);
        }

        if (renders.is_valid(e.render_key))
        {
            renders.erase(e.render_key);
        }

        if (powerUps.is_valid(e.powerUp_key))
        {
            powerUps.erase(e.powerUp_key);
        }

        if (sounds.is_valid(e.sound_key))
        {
            sounds.erase(e.sound_key);
        }

        if (bombs.is_valid(e.bomb_key))
        {
            bombs.erase(e.bomb_key);
        }

        if (ia.is_valid(e.ia_key))
        {
            ia.erase(e.ia_key);
        }
        if (buttons.is_valid(e.button_key))
        {
            buttons.erase(e.button_key);
        }
    }

    entities_.clear();
    nextId = 0;
}

std::vector<Entity> EntityManager::getEnemies(Entity &entidad)
{
    std::vector<Entity> enemigos;
    for (auto &e : getEntitiesByType(EntityType::NPC))
    {
        if (e.getId() != entidad.getId())
        {
            enemigos.push_back(e);
        }
    }

    for (auto &e : getEntitiesByType(EntityType::PLAYER))
    {
        if (e.getId() != entidad.getId())
        {
            enemigos.push_back(e);
        }
    }

    return enemigos;
}

const std::vector<Entity> EntityManager::getBombs()
{
    std::vector<Entity> bombas;
    for (auto &e : getEntitiesByType(EntityType::BOMBA))
    {
        bombas.push_back(e);
    }
    return bombas;
}

int EntityManager::getPinturaId(int entityId)
{
    auto Entidad = getEntity(entityId);

    auto &bombFisica = bombs.get(Entidad.bomb_key);

    return bombFisica.pinturaId;
}

void EntityManager::forEachErase(EntityType tipo)
{
    for (auto it = entities_.begin(); it != entities_.end();)
    {
        if (it->type == tipo)
        {
            eraseEntity(it->id);
        }
        else
        {
            ++it;
        }
    }
}

std::vector<Entity> EntityManager::getAllEntitiesInGame()
{
    std::vector<Entity> aux;
    for (auto &e : entities_)
    {
        if (e.type == EntityType::PLAYER || e.type == EntityType::NPC)
        {
            aux.push_back(e);
        }
    }
    return aux;
}