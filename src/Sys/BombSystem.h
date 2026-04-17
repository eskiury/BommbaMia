/**
 * @file BombSystem.h
 * @brief Declaración del sistema de gestión de bombas para el motor AlmondGE.
 *
 * Este sistema se encarga de crear, actualizar y eliminar bombas dentro del juego
 */

#ifndef BOMBSYSTEM_H
#define BOMBSYSTEM_H

#include "../AlmondGE/EntityManager.h"
#include "../ENGINES/AlmondSE/FmodSoundEngine.h"
#include "../../Sys/SoundSystem.h"
#include "../utils/Const.h"
#include "../Sys/MapSystem.h"
#include "IAsystem.h"

/**
 * @class BombSystem
 * @brief Sistema encargado de la lógica relacionada con las bombas en el juego.
 */
class BombSystem
{
private:
    /// Puntero al sistema de inteligencia artificial.
    IASystem *iaSystemPtr;

    /// Gestor de entidades local utilizado internamente.
    EntityManager em;

public:
    /**
     * @brief Constructor por defecto.
     */
    BombSystem();

    /**
     * @brief Destructor.
     */
    ~BombSystem();

    /**
     * @brief Crea una bomba asociada a una entidad jugador.
     *
     * @param jugador Entidad del jugador que lanza la bomba.
     * @param em Referencia al gestor de entidades principal.
     * @param mapSystem Referencia al sistema de mapas para colocar la bomba correctamente.
     */
    void crearBomba(const Entity &jugador, EntityManager &em, MapSystem &mapSystem);

    /**
     * @brief Actualiza todas las bombas en el sistema.
     *
     * Esta función reduce el tiempo de vida, verifica explosiones, y gestiona efectos colaterales.
     *
     * @param em Referencia al gestor de entidades.
     * @param deltaTime Tiempo transcurrido desde la última actualización.
     * @param mapSystem Referencia al sistema de mapas.
     */
    void updateBombs(EntityManager &em, float deltaTime, MapSystem &mapSystem);

    /**
     * @brief Elimina bombas que ya han explotado.
     *
     * @param em Referencia al gestor de entidades.
     * @param ids Vector con los identificadores de entidades de bombas a eliminar.
     */
    void removeBombs(EntityManager &em, const std::vector<int> &ids);

    /**
     * @brief Busca una celda libre en el mapa para posicionar una bomba.
     *
     * @param fisica Componente de física de la entidad que lanza la bomba.
     * @param mapSystem Referencia al sistema de mapas.
     */
    void buscarTileLibre(fisicaComponent &fisica, MapSystem &mapSystem);

    /**
     * @brief Guarda una referencia al sistema de IA.
     *
     * Este sistema puede ser utilizado para coordinar comportamientos inteligentes ante explosiones, etc.
     *
     * @param iaSystem Referencia al sistema de inteligencia artificial.
     */
    void saveIaSystem(IASystem &iaSystem)
    {
        iaSystemPtr = &iaSystem;
    }
};

#endif // BOMBSYSTEM_H
