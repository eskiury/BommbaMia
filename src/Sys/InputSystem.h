/**
 * @file InputSystem.h
 * @brief Sistema de entrada que gestiona el estado del teclado y mandos para múltiples jugadores.
 *
 * Este sistema mantiene mapas de entrada por jugador, gestiona la conexión de mandos y traduce las entradas
 * en eventos del juego usando el EventSystem.
 */

 #ifndef INPUTSYSTEM_H
 #define INPUTSYSTEM_H
 
 #include "../AlmondGE/EntityManager.h"
 #include "EventSystem.h"
 #include <unordered_map>
 #include <unordered_set>
 #include <string>
 #include "../ENGINES/GraphicEngine/motorGrafico.h"
 
 /**
  * @class InputSystem
  * @brief Sistema responsable de manejar la entrada de usuarios (teclado y mandos) y mapearla a acciones del juego.
  */
 class InputSystem
 {
 public:
     /**
      * @brief Constructor del sistema de entrada.
      * @param eventSystem Puntero al sistema de eventos para disparar acciones basadas en la entrada del usuario.
      */
     InputSystem(EventSystem *eventSystem = nullptr);
 
     /**
      * @brief Destructor.
      */
     ~InputSystem();
 
     /**
      * @brief Actualiza el mapa de entradas para un jugador determinado.
      * 
      * Este método consulta el estado del teclado o joystick y actualiza el mapa de acciones del jugador.
      * 
      * @param player Entidad del jugador cuyas entradas deben actualizarse.
      */
     void actualizarInputMap(Entity &player);
 
     /**
      * @brief Obtiene el mapa de entradas del jugador.
      * 
      * @param player Entidad del jugador.
      * @return Referencia constante al mapa de entradas (tecla/acción -> estado booleano).
      */
     const std::unordered_map<std::string, bool> &getInputMap(Entity &player) const;
 
     /**
      * @brief Cuenta la cantidad de mandos actualmente conectados.
      * @return Número de mandos detectados.
      */
     int contarMandosConectados() const;
 
 private:
     /**
      * @brief Mapas de entradas por jugador (ID de entidad -> mapa de entradas).
      */
     std::unordered_map<unsigned int, std::unordered_map<std::string, bool>> inputMaps;
 
     /**
      * @brief Puntero al sistema de eventos usado para generar eventos según las entradas.
      */
     EventSystem *eventSystem;
 
     /**
      * @brief Mapeo de jugadores a joysticks conectados (ID de jugador -> ID de joystick).
      */
     std::unordered_map<unsigned int, int> playerToJoystick;
 
     /**
      * @brief Conjunto de IDs de joysticks ya asignados a jugadores.
      */
     std::unordered_set<int> joystickAssigned;
 
     // === Estado del teclado (solo primer jugador) ===
 
     /// Indica si la tecla Enter está presionada.
     bool teclaEnter_presionada = false;
 
     /// Indica si la tecla Espacio está presionada.
     bool teclaEspacio_presionada = false;
 
     /// Indica si la tecla Pause está presionada.
     bool teclaPause_presionada = false;
 
     // === Estado de botones por jugador (joystick) ===
 
     /// Estado del botón A por jugador.
     std::unordered_map<unsigned int, bool> botonA_presionado;
 
     /// Estado del botón B por jugador.
     std::unordered_map<unsigned int, bool> botonB_presionado;
 
     /// Estado del botón Start por jugador.
     std::unordered_map<unsigned int, bool> botonStart_presionado;
 };
 
 #endif // INPUTSYSTEM_H
 