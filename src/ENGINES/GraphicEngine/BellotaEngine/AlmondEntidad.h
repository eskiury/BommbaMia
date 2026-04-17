#ifndef ALMONDENTIDAD_H
#define ALMONDENTIDAD_H

#include <vector>
//includes de librerias de glm y glmatrix
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>

class AlmondNode;

//Clase virtual de la que pueden heredar los distintos tipos de entidades del proyecto
/**
 * @brief Clase AlmondEntidad
 *
 * Esta clase actúa como una interfaz o fachada para las distintas clases de tipo entidad
 * 
 */
class AlmondEntidad {
public:
    
    /*
        Método: ~AlmondEntidad
        ---------------------------
        Destructor de la clase
    */
    virtual ~AlmondEntidad() = default;
    
    /**
    * @brief Dibuja la entidad.
    *
    * Método de renderizado que sirve como fachada/interfaz para las entidades que hereden de esta clase. Utiliza las matrices de transformación
    * y la posición de la cámara junto con las luces disponibles.
    *
    * @param model Matriz de transformación del modelo.
    * @param view Matriz de vista.
    * @param projection Matriz de proyección.
    * @param camPos Posición de la cámara.
    * @param luces Vector con los punteros a nodos de luz activos.
    */
    virtual void dibujar(const glm::mat4&, const glm::mat4&, const glm::mat4&, const glm::vec3&, std::vector<AlmondNode*> ) = 0;   // Se ejecuta al cargar la escena

   
    


private:

protected: // Para que las subclases puedan acceder a estas cosas, si queremos que sean globales
    
};

#endif