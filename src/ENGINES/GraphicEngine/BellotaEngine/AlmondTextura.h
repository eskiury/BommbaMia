#ifndef ALMONDTEXTURA_H
#define ALMONDTEXTURA_H
#include <vector>
#include <string>      // Para std::string
#include <iostream>
//includes de librerias de glm y glmatrix
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "AlmondRecurso.h"

//librerias assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


/**
 * @brief Declaración de la clase AlmondTextura, que representa una textura cargada en OpenGL.
 *
 * Esta clase hereda de AlmondRecurso y encapsula la lógica de carga, configuración y acceso a una textura.
 * Utiliza la librería stb_image para cargar archivos de imagen (.JPG y compatibles).
 */
class AlmondTextura : public AlmondRecurso
{
public:
/**
     * @brief Constructor por defecto.
     */
    AlmondTextura();
    /**
     * @brief Destructor.
     */
    ~AlmondTextura();
    //constructor predefinido
    /**
     * @brief Constructor que permite cargar la textura directamente desde un archivo.
     * @param nombre Ruta al archivo de textura.
     */
    AlmondTextura(const std::string& nombre);
    /**
     * @brief Carga una textura desde un archivo sobrescribiendo la funcion de la clase padre.
     * @param nombre Ruta al archivo de imagen.
     */
    void cargarFichero (const std::string& nombre) override; 
    /**
     * @brief Obtiene el ID OpenGL de la textura.
     * @return ID de la textura (glGenTextures).
     */
    unsigned int getId();
    /**
     * @brief Obtiene el ancho de la textura.
     * @return Ancho en píxeles.
     */
    int getWidth();
    /**
     * @brief Obtiene la altura de la textura.
     * @return Alto en píxeles.
     */
    int getHeight();

    ///< ID de la textura generada por OpenGL (glGenTextures)
    unsigned int id; //(glGenTextures)
    ///< Ancho de la textura
    int width;
    ///< Altura de la textura
    int height;
    ///< Número de canales de color (por ejemplo, 3 para RGB, 4 para RGBA)
    int nrChannels; //number of color channels

private:

    
};

#endif 