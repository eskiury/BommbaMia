#ifndef ALMONDENTIDADCAMARA_H
#define ALMONDENTIDADCAMARA_H
#include "AlmondEntidad.h"
#include <vector>
//includes de librerias de glm y glmatrix
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>

//#include "AlmondNode.h"
class AlmondNode;
/**
 * @brief Clase AlmondEntidadCamara
 *
 * Esta clase hereda de la clase entidad y gestiona el funcionamiento de las camaras del motor grafico
 * 
 */
class AlmondEntidadCamara : public AlmondEntidad
{
public:
    /**
    * @brief Constructor de la clase AlmondEntidadCamara.
    *
    * Inicializa una instancia de la cámara, que hereda de la clase Entidad.
    */
    AlmondEntidadCamara();
    /**
    * @brief Destructor de la clase AlmondEntidadCamara.
    *
    * Libera los recursos asociados a la instancia de la cámara.
    */
    ~AlmondEntidadCamara();
    /**
    * @brief Método de dibujado que hereda de la clase Entidad.
    *
    * Renderiza la cámara utilizando las matrices de transformación, vista y proyección, 
    * además de la posición de la cámara y un conjunto de luces de la escena.
    *
    * @param model Matriz de transformación del modelo.
    * @param view Matriz de vista.
    * @param projection Matriz de proyección.
    * @param camPos Posición actual de la cámara.
    * @param luces Vector de punteros a nodos de luces en la escena.
    */
    void dibujar(const glm::mat4&, const glm::mat4& , const glm::mat4& , const glm::vec3&, std::vector<AlmondNode*> ) override;
    // Métodos de configuración
    /**
    * @brief Establece el campo de visión vertical de la cámara.
    * 
    * @param fov Nuevo valor del campo de visión en grados.
    */
    void setFov(float fov);
    /**
    * @brief Define si la cámara usará proyección en perspectiva u ortográfica.
    * 
    * @param perspectiva true para perspectiva, false para ortográfica.
    */
    void setModoPerspectiva(bool perspectiva);
    /**
    * @brief Establece el punto al que la cámara apunta.
    * 
    * @param targetn Nuevo vector objetivo.
    */
    void setTarget(glm::vec3 targetn);
    /**
    * @brief Define la orientación vertical de la cámara.
    * 
    * @param upn Nuevo vector "up" de la cámara.
    */
    void setUp(glm::vec3 upn);
    /**
    * @brief Establece la posición de la cámara en el mundo.
    * 
    * @param campos Nueva posición de la cámara.
    */
    void setPosicion(glm::vec3 campos);
    /**
    * @brief Devuelve el punto al que la cámara está mirando.
    * 
    * @return Vector objetivo actual.
    */
    glm::vec3 getTarget();
    /**
    * @brief Devuelve el vector que indica la orientación vertical de la cámara.
    * 
    * @return Vector up actual.
    */
    glm::vec3 getUp();
    /**
    * @brief Devuelve la posición actual de la cámara.
    * 
    * @return Vector de posición.
    */
    glm::vec3 getPosicion();
    /**
    * @brief Devuelve el campo de visión vertical de la cámara.
    * 
    * @return Valor del FOV en grados.
    */
    float getFov();
    /**
    * @brief Calcula y devuelve la matriz de vista de la cámara.
    * 
    * @return Matriz de vista.
    */
    glm::mat4 getMatrizDeVista();
    /**
    * @brief Calcula y devuelve la matriz de vista de la cámara.
    * 
    * @return Matriz de vista.
    */
    glm::mat4 getMatrizDeProjection();
    /**
    * @brief Configura completamente la cámara con sus parámetros básicos.
    * 
    * @param targetn Nuevo objetivo.
    * @param upn Vector hacia arriba.
    * @param fov Campo de visión en grados.
    * @param pers Modo de proyección (true = perspectiva).
    * @param campos Posición de la cámara.
    */
    void ConfigurarCamara(glm::vec3 targetn, glm::vec3 upn, float fov, bool pers, glm::vec3 campos);
    //variables de la clase camara
    /**
     * @brief float camara.
     *
     * Campo de visión
     */
    float fovy;              // Campo de visión
    /**
     * @brief bool camara.
     *
     * Modo de proyección (perspectiva/ortográfica)
     */
    bool esPerspectiva;      // Modo de proyección (perspectiva/ortográfica)
    /**
     * @brief glm::vec3 camara.
     *
     * lugar donde apunta la camara
     */
    glm::vec3 target;        // lugar donde apunta la camara
    /**
     * @brief glm::vec3 camara.
     *
     * orientación vertical de la camara
     */
    glm::vec3 up;            // orientación vertical de la camara
    /**
     * @brief glm::vec3 camara.
     *
     * posicion de la camara
     */
    glm::vec3 posicion;      //posicion de la camara
    //meter la position tambien, porque sera mas comodo--------------------------------------------------------------

private:
    
    
};

#endif 