#ifndef ALMONDENTIDADANIMACION_H
#define ALMONDENTIDADANIMACION_H
//#include "AlmondEntidadModelo.h"
#include "AlmondEntidad.h"
#include "AlmondGestorRecursos.h"
#include "AlmondTextura.h"
#include <vector>
//includes de librerias de glm y glmatrix
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <string>

class AlmondNode;
/**
 * @brief Clase AlmondEntidadAnimacion
 *
 * Esta clase hereda de la clase entidad y gestiona el almacenamiento de los punteros a las animaciones ademas de su dibujado y actualizacion
 * 
 */
class AlmondEntidadAnimacion : public AlmondEntidad {
public:

    /**
    * @brief Constructor de la clase AlmondEntidadAnimacion.
    *
    * Inicializa una entidad que maneja animaciones, heredando de AlmondEntidad.
    */
    AlmondEntidadAnimacion();
    /**
    * @brief Destructor de la clase AlmondEntidadAnimacion.
    *
    * Libera los recursos utilizados por la animación.
    */
    ~AlmondEntidadAnimacion();
    /**
    * @brief Dibuja la entidad animada.
    *
    * Método de renderizado que hereda de la clase base. Utiliza las matrices de transformación
    * y la posición de la cámara junto con las luces disponibles.
    *
    * @param model Matriz de transformación del modelo.
    * @param view Matriz de vista.
    * @param projection Matriz de proyección.
    * @param camPos Posición de la cámara.
    * @param luces Vector con los punteros a nodos de luz activos.
    */
    void dibujar(const glm::mat4&, const glm::mat4&, const glm::mat4&, const glm::vec3&, std::vector<AlmondNode*>) override;
    /**
    * @brief Actualiza el estado de la animación en función del tiempo transcurrido.
    *
    * Calcula si se debe cambiar de frame basándose en el tiempo acumulado y la velocidad de animación (fps).
    *
    * @param deltaTime Tiempo transcurrido desde el último frame (en segundos).
    */
    void actualizarAnimacion(float deltaTime);
    /**
    * @brief Guarda el puntero al gestor de recursos.
    *
    * Se almacena una referencia al gestor de recursos para acceder a las mallas u otros recursos más adelante.
    *
    * @param gestor Puntero al gestor de recursos.
    */
    void guardarGestor(AlmondGestorRecursos* gestor);
    /**
    * @brief Verifica si hay una animación cargada en la entidad.
    *
    * @return true si hay al menos una animación cargada, false en caso contrario.
    */
    bool getCargado();
    /**
    * @brief Carga una animación a partir de una lista de rutas a modelos.
    *
    * Asocia un nombre de animación con un conjunto de modelos (frames) y los guarda en un mapa.
    *
    * @param nombreAnim Nombre identificador para la animación.
    * @param rutasFrames Vector con rutas a los archivos de los modelos que componen la animación.
    */
    void cargarAnimacion(const std::string& nombreAnim, const std::vector<std::string>& rutasFrames);
    /**
    * @brief Reproduce una animación si está previamente cargada.
    *
    * Busca la animación por nombre y la establece como activa para su reproducción.
    *
    * @param nombreAnim Nombre de la animación a reproducir.
    */
    void reproducirAnimacion(const std::string& nombreAnim);
    /**
    * @brief Establece los FPS (frames por segundo) a los que se reproducirá la animación.
    *
    * Define la velocidad a la que se cambia de frame dentro de la animación activa.
    *
    * @param fps Valor en frames por segundo.
    */
    void setFPS(float fps);
    /**
    * @brief Define si la animación debe reproducirse en bucle.
    *
    * Si está activado, al finalizar una animación, esta volverá a reproducirse desde el principio.
    *
    * @param loop true para bucle infinito, false para una sola reproducción.
    */
    void setLoop(bool loop);

private:

    /**
     * @brief Unordered map de animaciones.
     *
     * Almacena la clave que es el string/nombre de la animacion y los punteros a las mallas
     */
    std::unordered_map<std::string, std::vector<AlmondMalla*>> animaciones;
    /**
     * @brief String de la animacion.
     *
     * nombre de la animacion que se esta repoduciendo
     */
    std::string animacionActual;
    /**
     * @brief Float de la animacion.
     *
     * tiempo para calcular el valor del frame en el que nos encontramos
     */
    float tiempoAcumulado;
    /**
     * @brief Int de la animacion.
     *
     * frame actual de la animacion
     */
    int frameActual;
     /**
     * @brief Float de la animacion.
     *
     * velocidad a la que se ve o cambia la animacion
     */
    float fps;
    /**
     * @brief bool de la animacion.
     *
     * variable para indicar si la animacion se reproduce en bucle
     */
    bool loop;
    /**
     * @brief bool de la animacion.
     *
     * variable que indica si hay una animacion cargada
     */
    bool cargado;
    /**
     * @brief Puntero al gestor de la animacion.
     *
     * puntero al gestor de recursos para obtener los necesarios
     */
    AlmondGestorRecursos* gestorGuardado;
};

#endif