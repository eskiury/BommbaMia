/**
 * @file GameplaySystem.h
 * @brief Sistema encargado de gestionar la lógica principal del juego.
 *
 * Este sistema administra efectos de power-ups, control de rondas, modo fantasma, y distribución de elementos del mapa.
 */

 #ifndef GAMEPLAYSYSTEM_H
 #define GAMEPLAYSYSTEM_H
 
 #include "../AlmondGE/EntityManager.h"
 #include "../CMP/PowerupComponent.h" ///< Incluye la definición de PowerUpType.
 #include "../Sys/MapSystem.h"
 
 /**
  * @class GameplaySystem
  * @brief Sistema responsable de la lógica de juego relacionada con power-ups, rondas y eventos especiales.
  */
 class GameplaySystem
 {
 public:
     /**
      * @brief Constructor que recibe una referencia al EntityManager.
      * @param em Referencia al gestor de entidades.
      */
     GameplaySystem(EntityManager &em) : em(em) {}
 
     /**
      * @brief Aplica un power-up a una entidad jugador.
      * @param jugador Entidad del jugador que recibe el power-up.
      * @param tipo Tipo de power-up a aplicar.
      */
     void aplicarPowerUp(Entity &jugador, PowerUpType tipo);
 
     /**
      * @brief Rellena aleatoriamente el mapa con bloques destruibles según un porcentaje.
      * @param mapSystem Sistema de mapas que administra las celdas del nivel.
      * @param porcentaje Porcentaje de celdas que se llenarán con bloques destruibles (valor entre 0 y 100).
      */
     void rellenarBloquesDestruibles(MapSystem &mapSystem, float porcentaje);
 
     /**
      * @brief Recoloca los power-ups previamente recogidos en nuevas posiciones del mapa.
      * @param mapSystem Sistema de mapas que controla las ubicaciones disponibles.
      * @param collectedPowerUps Vector de tipos de power-ups recogidos por los jugadores.
      */
     void recolocarPowerUps(MapSystem &mapSystem, std::vector<PowerUpType> collectedPowerUps);
 
     /**
      * @brief Establece la cantidad total de rondas a jugar.
      * @param rondas Número de rondas totales.
      */
     void setRondas(int rondas);
 
     /**
      * @brief Determina si un NPC ha ganado según la cantidad de rondas.
      * @param rondasNPC Rondas ganadas por el NPC.
      * @return 1 si gana el NPC, 0 si no.
      */
     int determinarGanadorNPC(int rondasNPC);
 
     /**
      * @brief Obtiene el número total de rondas configuradas.
      * @return Número total de rondas.
      */
     int getRondasTotales() { return rondasTotales; }
 
     /**
      * @brief Aplica el modo fantasma a una entidad, permitiéndole atravesar obstáculos.
      * @param em Gestor de entidades.
      * @param entidad Entidad que activará el modo fantasma.
      */
     void aplicarModoFantasma(EntityManager &em, Entity &entidad);
 
 private:
     /// Referencia al gestor de entidades del juego.
     EntityManager &em;
 
     /// Número total de rondas configuradas para la partida.
     int rondasTotales = 3;
 };
 
 #endif // GAMEPLAYSYSTEM_H
 