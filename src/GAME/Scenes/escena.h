#ifndef ESCENA_H
#define ESCENA_H


class SceneManager;

/**
 * @brief Clase base abstracta para una escena del juego.
 *
 * Define la interfaz que todas las escenas deben implementar,
 * incluyendo la inicialización, actualización lógica y renderizado.
 */
class Escena
{
public:
    /**
     * @brief Destructor virtual por defecto.
     */
    virtual ~Escena() = default;

    /**
     * @brief Inicializa la escena.
     *
     * Se llama una vez al cargar la escena para configurar sus variables y recursos.
     */
    virtual void init() = 0;

    /**
     * @brief Actualiza la lógica de la escena.
     *
     * Se llama cada frame para manejar la lógica del juego.
     */
    virtual void update() = 0;

    /**
     * @brief Renderiza la escena.
     *
     * Se llama cada frame para dibujar en pantalla.
     */
    virtual void render() = 0;

    /**
     * @brief Asigna el puntero al gestor de escenas.
     *
     * Este método permite a la escena conocer el gestor para
     * cambiar entre escenas o consultar su estado.
     *
     * @param manager Puntero al SceneManager.
     */
    virtual void inicializarEscena(SceneManager* manager) {
        esm = manager;
    }

protected:
    SceneManager* esm = nullptr; ///< Puntero al gestor de escenas.

    /**
     * @brief Obtiene el puntero al gestor de escenas.
     *
     * @return Puntero al SceneManager.
     */
    SceneManager* getESM() {
        return esm;
    }
};

#endif // ESCENA_H
