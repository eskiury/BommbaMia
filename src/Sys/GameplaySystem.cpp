#include "GameplaySystem.h"
#include <algorithm>
#include <iostream>
#include <random>
#include "../utils/Const.h"



void GameplaySystem::aplicarPowerUp(Entity &jugador, PowerUpType tipo)
{
    auto &fisica = em.fisicas.get(jugador.fisica_key);
    auto &bombComp = em.bombs.get(jugador.bomb_key);

    switch (tipo)
    {
    case PowerUpType::BOMB_UP:
        if(bombComp.maxBombs < 6)
            bombComp.maxBombs += 1;
        break;
    case PowerUpType::BOMB_DOWN:
        if (bombComp.maxBombs > 1)
            bombComp.maxBombs -= 1;
        break;
    case PowerUpType::EXP_RANGE_UP:
        if (bombComp.expRange < 8)
            bombComp.expRange += 1;
        break;
    case PowerUpType::EXP_RANGE_DOWN:
        if (bombComp.expRange > 1)
            bombComp.expRange -= 1;
        break;
    case PowerUpType::EXP_RANGE_MAX:
        bombComp.expRange = 9;
        break;
    case PowerUpType::SPEED_UP:
        if(fisica.maxSpeed < 0.15f){
            fisica.maxSpeed += 0.0125f;
            fisica.ax = +0.1f;
            fisica.az = +0.1f;
        }
        break;
    case PowerUpType::SPEED_DOWN:
        if(fisica.maxSpeed > 0.095f){
            fisica.maxSpeed -= 0.0125f;
        }
        break;
    case PowerUpType::PUSH_BOMB:
        fisica.canPushBombs = true;
        break;

    default:
        break;
    }
}



void GameplaySystem::rellenarBloquesDestruibles(MapSystem &mapSystem, float porcentaje)
{
    int filas = mapSystem.getHeight();
    int columnas = mapSystem.getWidth();

   
    std::vector<PowerUpType> listaPowerUps = {
        // 9 BOMB_UP
        PowerUpType::BOMB_UP, PowerUpType::BOMB_UP, PowerUpType::BOMB_UP,
        PowerUpType::BOMB_UP,

        // 9 BOMB_DOWN
        PowerUpType::BOMB_DOWN, PowerUpType::BOMB_DOWN, PowerUpType::BOMB_DOWN,
        PowerUpType::BOMB_DOWN,

        // 6 EXP_RANGE_UP
        PowerUpType::EXP_RANGE_UP, PowerUpType::EXP_RANGE_UP, PowerUpType::EXP_RANGE_UP,
        PowerUpType::EXP_RANGE_UP,

        // 6 EXP_RANGE_DOWN
        PowerUpType::EXP_RANGE_DOWN, PowerUpType::EXP_RANGE_DOWN, PowerUpType::EXP_RANGE_DOWN,
        PowerUpType::EXP_RANGE_DOWN,

        // 4 SPEED_UP
        PowerUpType::SPEED_UP, PowerUpType::SPEED_UP, PowerUpType::SPEED_UP, PowerUpType::SPEED_UP,
        PowerUpType::SPEED_UP,

        // 4 SPEED_DOWN
        PowerUpType::SPEED_DOWN, PowerUpType::SPEED_DOWN, PowerUpType::SPEED_DOWN, PowerUpType::SPEED_DOWN,

        // 2 PUSH_BOMB
        PowerUpType::PUSH_BOMB,

        // 1 EXP_RANGE_MAX
        PowerUpType::EXP_RANGE_MAX};

  
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(listaPowerUps.begin(), listaPowerUps.end(), gen);

    std::vector<std::pair<int, int>> celdasVacias;
    for (int y = 0; y < filas; y++)
    {
        for (int x = 0; x < columnas; x++)
        {
            if (mapSystem.getTile(x, y) == 0)
            {
                celdasVacias.emplace_back(x, y);
            }
        }
    }


    int totalCeldas = celdasVacias.size();
    int bloquesDestruibles = static_cast<int>(totalCeldas * porcentaje);

    std::shuffle(celdasVacias.begin(), celdasVacias.end(), gen);


    for (int i = 0; i < bloquesDestruibles && i < totalCeldas; i++)
    {
        int x = celdasVacias[i].first;
        int y = celdasVacias[i].second;

        if (!listaPowerUps.empty())
        {
            PowerUpType powerUp = listaPowerUps.back();
            listaPowerUps.pop_back();
            mapSystem.setTile(x, y, POWERUP + static_cast<int>(powerUp));
        }
        else
        {
            mapSystem.setTile(x, y, DESTRUIBLE);
        }
    }
}

void GameplaySystem::recolocarPowerUps(MapSystem &mapSystem, std::vector<PowerUpType> collectedPowerUps)
{

    std::vector<std::pair<int, int>> celdasVacias;
    for (int y = 0; y < mapSystem.getHeight(); y++)
    {
        for (int x = 0; x < mapSystem.getWidth(); x++)
        {
            if (mapSystem.getTile(x, y) == 0)
            {
                celdasVacias.emplace_back(x, y);
            }
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(celdasVacias.begin(), celdasVacias.end(), gen);

    for (auto powerUp : collectedPowerUps)
    {
        if (celdasVacias.empty())
        {
            break;
        }

        int x = celdasVacias.back().first;
        int y = celdasVacias.back().second;
        celdasVacias.pop_back();

        mapSystem.setTile(x, y, static_cast<int>(powerUp));
    }
}

void GameplaySystem::setRondas(int rondas)
{
    if (rondas > 0 && rondas <= 5)
    {
        rondasTotales = rondas;
    }
}

int GameplaySystem::determinarGanadorNPC(int rondasNPC)
{
    int ganadorID = 0;

    auto NPCs = em.getEntitiesByType(EntityType::NPC);

    for (auto &npc : NPCs)
    {

        if (rondasNPC == rondasTotales)
        {
            ganadorID = npc.getId();
            break;
        }
    }
    return ganadorID;
}


void GameplaySystem::aplicarModoFantasma(EntityManager &em, Entity &entidad)
{
    auto &fisica = em.fisicas.get(entidad.fisica_key);

    fisica.esFantasma = true;
    auto &render = em.renders.get(entidad.render_key);
    render.color.r = 253;
    render.color.g = 249;
    render.color.b = 0;
    render.color.a = 127;

}
