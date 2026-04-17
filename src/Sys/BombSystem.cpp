#include "BombSystem.h"
#include <cmath>
#include <iostream>
#include "../utils/miColor.h"
#include "../utils/Const.h"
#include "../ENGINES/GraphicEngine/motorGrafico.h"


BombSystem::BombSystem() {
}

BombSystem::~BombSystem() {
}


void BombSystem::crearBomba(const Entity &jugador, EntityManager &em, MapSystem &mapSystem)
{
    auto &fisica = em.fisicas.get(jugador.fisica_key);
    if(!fisica.estaMuerto){
        auto &fisica = em.fisicas.get(jugador.fisica_key);
        auto &bombComp = em.bombs.get(jugador.bomb_key);
        auto &render = em.renders.get(jugador.render_key);
        if(render.animacion != 2){
            render.animacion = 2;
            render.animacionYaIniciada = false;
            render.colocandoBombaAnimacion = true;
        }

        if (bombComp.bombsPlaced < bombComp.maxBombs)
        {
           
            int gridX = static_cast<int>(round(fisica.x / tileSize));
            int gridZ = static_cast<int>(round(fisica.z / tileSize));

       
            if (mapSystem.getTile(gridX, gridZ) == 1)
            {
                buscarTileLibre(fisica, mapSystem);
            }

        
            gridX = static_cast<int>(round(fisica.x / tileSize));
            gridZ = static_cast<int>(round(fisica.z / tileSize));
            float bombaX = gridX * tileSize;
            float bombaZ = gridZ * tileSize;

            Entity &bomba = em.createEntity(EntityType::BOMBA);
            fisicaComponent fisicaBomba{bombaX, TileY, bombaZ};
            MiColor colorPersonalizado(230, 41, 55, 255);
            renderComponent renderBomba{sizeBomba, colorPersonalizado};
            powerUpComponent powerBloque{};
            soundComponent soundMusica;
            SoundEvent se;
            se.eventPath = "event:/Efectos/Explosion"; 
            se.playOnce = false;                          
            se.looping = false;                           
            se.isPlaying = false;                         
            se.volume = 1.0f;                             

    
            soundMusica.addSoundEvent(se);

            bombaComponent bombaCMP{false, explosionDuration, bombComp.expRange, fisica.maxBombs, bombComp.bombsPlaced, jugador.id, 0,lifetime, false};
            iaComponent bombatieneBT{false};
            buttonComponent bombaButton{};

            em.createEntityComponents(bomba, fisicaBomba, renderBomba, powerBloque, soundMusica, bombaCMP, bombatieneBT, bombaButton);

            bombComp.bombsPlaced++;


            iaSystemPtr->getGrafo().actualizarZonaInfluencia(gridZ,gridX,mapSystem.getMap(), true);
            mapSystem.setTile(gridX, gridZ, BOMBA);
            bombaCMP.explosionRastros.push_back({bombaCMP.explosionDuration, fisicaBomba.tileX, fisicaBomba.tileZ});
        }
    }
}

void BombSystem:: updateBombs(EntityManager &em, float deltaTime, MapSystem &mapSystem)
{
    std::vector<int> bombsToRemove;
    auto bombs = em.getEntitiesByType(EntityType::BOMBA);

    for (auto &b : bombs)
    {
        auto &fisica = em.fisicas.get(b.fisica_key);
        auto &soundComp = em.sounds.get(b.sound_key);
        auto &bombComp = em.bombs.get(b.bomb_key);

        auto entidad = em.getEntity(bombComp.ownerId);
        auto &entidadBomb = em.bombs.get(entidad.bomb_key);


        if (!bombComp.isExploding)
        {
            bombComp.lifetime -= deltaTime;
            if (bombComp.lifetime <= 0.0f)
            {
                bombComp.isExploding = true;
                bombComp.lifetime = bombComp.explosionDuration;
            }
        }
        else
        {
            bombComp.explosionDuration -= deltaTime;
            if (bombComp.explosionDuration <= 0.0f)
            {
                bombsToRemove.push_back(b.id);

                iaSystemPtr->getGrafo().actualizarZonaInfluencia(fisica.z/2,fisica.x/2,mapSystem.getMap(), false);

                mapSystem.setTile(fisica.x / 2, fisica.z / 2, 0);
                mapSystem.setTileColor(fisica.x / 2, fisica.z / 2, entidadBomb.pinturaId);
            }
        }
    }

    removeBombs(em, bombsToRemove);
}

void BombSystem::removeBombs(EntityManager &em, const std::vector<int> &ids)
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
        
    for (int id : ids)
    {
        Entity &entidad = em.getEntity(id);
        auto &render = em.renders.get(entidad.render_key);
        motorRaylib.getMotorDeJuego().EliminarPersonaje(render.SusDatosEnElArbol);
        em.eraseEntity(id);
    }
}

void BombSystem::buscarTileLibre(fisicaComponent &fisica, MapSystem &mapSystem)
{
    int gridX = static_cast<int>(round(fisica.x / tileSize));
    int gridZ = static_cast<int>(round(fisica.z / tileSize));
    bool encontrado = false;
    int radioBusqueda = 1;

    while (!encontrado)
    {
        for (int dx = -radioBusqueda; dx <= radioBusqueda; dx++)
        {
            for (int dz = -radioBusqueda; dz <= radioBusqueda; dz++)
            {
                int candidatoX = gridX + dx;
                int candidatoZ = gridZ + dz;
                if (mapSystem.getTile(candidatoX, candidatoZ) != 1)
                {
                    fisica.x = candidatoX * tileSize;
                    fisica.z = candidatoZ * tileSize;
                    encontrado = true;
                    break;
                }
            }
            if (encontrado)
                break;
        }
        radioBusqueda++;
    }
}