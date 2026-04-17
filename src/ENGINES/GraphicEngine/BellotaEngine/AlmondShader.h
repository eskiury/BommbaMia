#ifndef ALMONDSHADER_H
#define ALMONDSHADER_H
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
 * @enum AlmondShaderType
 * @brief Tipos de shaders compatibles.
 */
enum AlmondShaderType{
    VERTEX,     ///< Shader de tipo Vertex
    FRAGMENT    ///< Shader de tipo Fragment
};

/** 
 * @brief AlmondShader Clase encargada de gestionar shaders en OpenGL.
 * 
 * Hereda de AlmondRecurso. Permite cargar, compilar y usar shaders desde un archivo.
 * Los archivos deben contener secciones `[VERTEX]` y `[FRAGMENT]` al inicio del archivo dependiendo del tipo
 */
class AlmondShader : public AlmondRecurso
{
public:
/**
     * @brief Constructor por defecto.
     */
    AlmondShader();
     /**
     * @brief Destructor que libera el shader.
     */
    ~AlmondShader();
    //constructor predefinido
    /**
     * @brief Constructor que permite inicializar directamente con el nombre del shader.
     * @param nombre Ruta del archivo de shader.
     */
    AlmondShader(const std::string& nombre);
    /**
     * @brief Sobrescribe la funcion de la clase recurso.Carga y compila un shader desde el archivo dado.
     * @param nombre Ruta del archivo que contiene el código del shader.
     */
    void cargarFichero (const std::string& nombre) override; 
    /**
     * @brief Activa el shader para su uso con OpenGL.
     */
    void usar();
     /**
     * @brief Lee el archivo fuente y extrae el código del shader.
     * @param rutaArchivo Ruta al archivo de shader.
     * @param stringShaderCode Referencia donde se almacenará el código leído.
     */
    void cargarShadersDesdeArchivo(const std::string& rutaArchivo, std::string& stringShaderCode);
     /**
     * @brief Lee el archivo fuente y extrae el código del shader, luego se usara la funcion de compilarShader para compilarlo segun el tipo.
     * @param rutaArchivo Ruta al archivo de shader.
     * @param stringShaderCode Referencia donde se almacenará el código leído.
     */
    GLuint compilarShaders(const std::string& shaderCode, AlmondShaderType tipo); 
     /**
     * @brief Compila un shader individual.
     * @param codigoFuente Código fuente en GLSL.
     * @param tipoShader Tipo del shader (GL_VERTEX_SHADER o GL_FRAGMENT_SHADER).
     * @return ID del shader compilado.
     */
    GLuint compilarShader(const std::string& codigoFuente, GLenum tipoShader);
    /**
     * @brief Devuelve el ID del shader compilado.
     * @return GLuint del shader (no el programa completo).
     */
    GLuint getShader();
   
    ///< Tipo actual del shader cargado (VERTEX o FRAGMENT)
    AlmondShaderType tipo;
    ///< ID del programa de shader generado con OpenGL
    GLuint shaderProgram; // Programa de shaders // ID del programa de shaders
    

private:

    
};

#endif 