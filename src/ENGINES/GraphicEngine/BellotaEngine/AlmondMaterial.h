#ifndef ALMONDMATERIAL_H
#define ALMONDMATERIAL_H
#include <vector>
#include <string>      // Para std::string
#include <iostream>
//includes de librerias de glm y glmatrix
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "AlmondRecurso.h"
#include "AlmondTextura.h"

//librerias assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/**
 * @brief Representa un material en el sistema de renderizado, incluyendo colores y texturas.
 * 
 * Esta clase gestiona los parámetros del material como componentes ambientales, difusas, especulares,
 * el brillo y la textura asociada. Se puede cargar desde archivo o configurar manualmente.
 */
//para cargar materiales podemos hacer un archivo .json, guardar los valores y el nombre para el material(cargariamos todo al inicio) y luego asignar esos materiales a los modelos que queramos
class AlmondMaterial : public AlmondRecurso
{
public:
    /**
     * @brief Constructor por defecto.
     */
    AlmondMaterial();
    /**
     * @brief Destructor.
     */
    ~AlmondMaterial();
    //constructor predefinido
    /**
     * @brief Constructor que inicializa el material con un nombre.
     * @param nombre Nombre del material.
     */
    AlmondMaterial(const std::string& nombre);
    /**
     * @brief Constructor que inicializa el material con un nombre y un valor numérico adicional, se uso esta funcion para depuracion de la gestion de memoria.
     * @param nombre Nombre del material.
     * @param num se usaba para asignar un id.
     */
    AlmondMaterial(const std::string& nombre, int num);
    /**
     * @brief Sobrescribe la funcion de cargarFichero de la clase AlmondRecurso y carga los parámetros del material desde un archivo MTL.
     * @param nombre Ruta del archivo desde el que se cargará el material.
     */
    void cargarFichero (const std::string& nombre) override;
    /**
     * @brief Asigna manualmente los valores del material.Se puede usar para crear materiales directamente sin necesidad de leerlos desde un fichero mtl
     * 
     * @param ruta Ruta opcional del recurso.
     * @param ambiente Color ambiental.
     * @param diffusea Color difuso.
     * @param speculary Color especular.
     * @param shininessy Brillo (shininess).
     * @param textura Puntero a la textura asociada al material.
     */ 
    void cargarMaterial(const std::string& ruta, glm::vec3 ambiente,glm::vec3 diffusea,glm::vec3 speculary, float shininessy, AlmondTextura* textura);
    /**
     * @brief Asigna una textura al material.
     * @param te Puntero a la textura.
     */
    void guardarSuTextura(AlmondTextura* te);
     /**
     * @brief Devulve el color difuso del material.
     * @return Color difuso (glm::vec3).
     */
    glm::vec3 getColor();
    /**
     * @brief Devuelve la textura asociada al material.
     * @return Puntero a AlmondTextura.
     */
    AlmondTextura* getTextura();
    /**
     * @brief Devuelve el valor ambiental.
     * @return ambiental (glm::vec3).
     */
    glm::vec3 getAmbient();
     /**
     * @brief Devuelve el valor difuso.
     * @return difuso (glm::vec3).
     */
    glm::vec3 getDiffuse();
    /**
     * @brief Devuelve el valor especular.
     * @return especular (glm::vec3).
     */
    glm::vec3 getSpecular();
     /**
     * @brief Devuelve el brillo del material.
     * @return Valor de shininess (float).
     */
    float getShininess();

    //variables
    //Coeficientes luz
    //Recursos de textura
    glm::vec3 ambient;  // Ka - Color ambiental ///< Ka - Componente ambiental
    glm::vec3 diffuse;  // Kd - Color difuso    ///< Kd - Componente difusa
    glm::vec3 specular; // Ks - Color especular ///< Ks - Componente especular
    float shininess;    // Ns - Brillo          ///< Ns - Brillo
    AlmondTextura* textura; //textura del material  ///< Textura asociada al material

    //mas variables pero del archivo mtl, no se si seran necesarias
    glm::vec3 emissive;   // Ke - Color emisivo ///< Ke - Emisión
    float opacity;        // d - Opacidad       ///< d - Opacidad
    int illumModel;       // illum - Modelo de iluminación  ///< illum - Modelo de iluminación


private:

    
};

#endif 