#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "escena.h" 
#include <string>

/**
 * @brief Declaración de la clase SceneManager, que controla las escenas del juego.
 *
 * SceneManager se encarga de cambiar entre escenas (como menú, juego, intro, etc.), así como de actualizar
 * y renderizar la escena activa en cada frame. También guarda el estado actual y el anterior.
 */
class SceneManager
{
public:
 
    /**
     * @brief Constructor por defecto. Inicializa el estado actual y anterior, y define la escena inicial como "Intro".
     */
    SceneManager();

    /**
     * @brief Destructor. Libera los recursos asociados al SceneManager.
     */
    ~SceneManager();

    /**
     * @brief Llama al método update() de la escena actualmente activa.
     * 
     * Se utiliza en el bucle principal del juego para actualizar la lógica de la escena activa.
     */
    void actualizar();
     /**
     * @brief Llama al método render() de la escena actualmente activa.
     * 
     * Se utiliza en el bucle principal del juego para dibujar la escena activa.
     */
    void renderizar();
    /**
     * @brief Devuelve el estado anterior al actual.
     * 
     * @return int Código entero que representa el estado anterior.
     */
    int GetAnterior();
     /**
     * @brief Establece el estado anterior.
     * 
     * @param estadoAR Código entero del nuevo estado anterior.
     */
    void SetAnterior(int estadoAR);
    /**
     * @brief Cambia la escena activa según el nombre recibido.
     * 
     * @param estado Nombre de la nueva escena ("Menu", "Juego", "Intro", etc.)
     */
    void cambiarEstado(std::string);
    /**
     * @brief Devuelve el estado actual.
     * 
     * @return int Código entero que representa el estado.
     */
    int GetEstado();
    Escena* ActualmenteUsada; ///< Puntero a la escena que se esta usando actualmente en el juego
    int estado;               ///< Escena actual en la que se encuentra el juego
    int estadoAnterior;       ///< Escena anterior en la que se encuentraba el juego
    
private:
    std::string est;          ///< Nombre de la escena actual del juego


};

#endif 