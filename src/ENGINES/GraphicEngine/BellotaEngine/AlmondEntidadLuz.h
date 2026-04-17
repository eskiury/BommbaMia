#ifndef ALMONDENTIDADLUZ_H
#define ALMONDENTIDADLUZ_H
#include "AlmondEntidad.h"
#include "AlmondShader.h"
#include <vector>
//includes de librerias de glm y glmatrix
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>

//#include "AlmondNode.h"
class AlmondNode;
/**
 * @brief Clase AlmondEntidadLuz
 *
 * Esta clase hereda de la clase entidad y gestiona el funcionamiento de las luces del motor grafico
 * 
 */
class AlmondEntidadLuz : public AlmondEntidad
{
public:
    /**
    * @brief Constructor de la clase AlmondEntidadLuz.
    *
    * Inicializa una instancia de la luz, que hereda de la clase Entidad.
    */
    AlmondEntidadLuz();
    /**
    * @brief Destructor de la clase AlmondEntidadLuz.
    *
    * Libera los recursos asociados a la instancia de la luz.
    */
    ~AlmondEntidadLuz();

    /**
    * @brief Método de dibujado que hereda de la clase Entidad.
    *
    * Renderiza la luz utilizando las matrices de transformación, vista y proyección, 
    * además de la posición de la cámara y un conjunto de luces de la escena.
    *
    * @param model Matriz de transformación del modelo.
    * @param view Matriz de vista.
    * @param projection Matriz de proyección.
    * @param camPos Posición actual de la cámara.
    * @param luces Vector de punteros a nodos de luces en la escena.
    */
    void dibujar(const glm::mat4&, const glm::mat4& , const glm::mat4&, const glm::vec3&,std::vector<AlmondNode*>  ) override;
    /**
    * @brief Establece la intensidad de la luz.
    *
    * Define la intensidad de la luz como un vector RGB.
    *
    * @param intensidad Vector con los valores RGB de la intensidad.
    */
    void setIntensidad (glm::vec3);
    /**
    * @brief Obtiene la intensidad de la luz.
    *
    * @return Vector vec3 que representa la intensidad (RGB).
    */
    glm::vec3 getIntensidad ();
    /**
    * @brief Carga los shaders necesarios para la luz.
    *
    * Recibe los punteros a los shaders utilizados para representar la luz y sus efectos.
    *
    * @param SFV Shader de vértices para el cubo de luz.
    * @param SLYF Fragment Shader para la figura y la luz, el shader de la propia luz.
    * @param SF Fragment Shader específico de la figura, un cubo que se usaba de referencia para ver donde esta la luz,.
    */
    void cargarShadersLuz(AlmondShader* SFV, AlmondShader* SLYF, AlmondShader* SF);
    /**
    * @brief Inicializa el cubo que representa visualmente la luz.
    *
    * Crea los buffers necesarios (VAO y VBO) para renderizar la luz como un cubo.Esta funcion solo se usaba para depuracion de la posicion de la luz
    */
    void inicializarCuboLuz();
    /**
    * @brief Crea el programa de shaders.
    *
    * Compila y enlaza los shaders cargados en un programa para su uso en renderizado.
    */
    void crearPrograma();   //crea el programa usando los shaders

    // Configuración rápida
     // Configuradores específicos por tipo
     /**
    * @brief Configura una luz direccional.
    *
    * Define los parámetros básicos para una luz de tipo direccional.
    *
    * @param direccion Dirección de la luz.
    * @param color Color RGB de la luz.
    * @param ambStrength Fuerza de la luz ambiental.
    * @param diffStrength Fuerza de la luz difusa.
    * @param specStrength Fuerza de la luz especular.
    */
     void ConfigurarLuzDireccional(glm::vec3 direccion, glm::vec3 color, float ambStrength, float diffStrength, float specStrength);
     /**
    * @brief Configura una luz puntual.
    *
    * Define todos los parámetros de una luz puntual, incluyendo atenuación.
    *
    * @param posicion Posición de la luz.
    * @param color Color RGB de la luz.
    * @param ambStrength Fuerza de la luz ambiental.
    * @param diffStrength Fuerza de la luz difusa.
    * @param specStrength Fuerza de la luz especular.
    * @param atenCte Atenuación constante.
    * @param atenLineal Atenuación lineal.
    * @param atenCuadrat Atenuación cuadrática.
    */
     void ConfigurarLuzPuntual(glm::vec3 posicion, glm::vec3 color, float ambStrength, float diffStrength, float specStrength,
                                float atenCte, float atenLineal, float atenCuadrat);
    /**
    * @brief Configura una luz tipo spotlight (foco).
    *
    * Define todos los parámetros para un foco de luz incluyendo ángulos y atenuación.
    *
    * @param posicion Posición del foco.
    * @param direccion Dirección hacia donde apunta.
    * @param color Color RGB de la luz.
    * @param ambStrength Fuerza de la luz ambiental.
    * @param diffStrength Fuerza de la luz difusa.
    * @param specStrength Fuerza de la luz especular.
    * @param atenCte Atenuación constante.
    * @param atenLineal Atenuación lineal.
    * @param atenCuadrat Atenuación cuadrática.
    * @param apertura Ángulo de apertura del haz (coseno).
    * @param penumbra Ángulo de penumbra (coseno externo).
    */
     void ConfigurarLuzSpot(glm::vec3 posicion, glm::vec3 direccion, glm::vec3 color, float ambStrength, float diffStrength, float specStrength,
                            float atenCte, float atenLineal, float atenCuadrat, float apertura, float penumbra); 

    // Setters
    /**
    * @brief Establece la posición de la luz.
    *
    * @param luzPos Nueva posición.
    */
    void setPosicion(glm::vec3 luzPos);
    /**
    * @brief Establece la dirección de la luz.
    *
    * @param dir Vector dirección.
    */
    void setDireccion(glm::vec3 dir);
    /**
    * @brief Establece la fuerza de la luz ambiental.
    *
    * @param ambStrength Valor de intensidad ambiental.
    */
    void setAmbientStrength(float ambStrength);
    /**
    * @brief Establece la fuerza de la luz difusa.
    *
    * @param diffStrength Valor de intensidad difusa.
    */
    void setDiffuseStrength(float diffStrength);
    /**
    * @brief Establece la fuerza de la luz especular.
    *
    * @param specStrength Valor de intensidad especular.
    */
    void setSpecularStrength(float specStrength);
    /**
    * @brief Establece la atenuación constante.
    *
    * @param value Valor de atenuación constante.
    */
    void setAtenCte(float value);
    /**
    * @brief Establece la atenuación lineal.
    *
    * @param value Valor de atenuación lineal.
    */
    void setAtenLineal(float value);
    /**
    * @brief Establece la atenuación cuadrática.
    *
    * @param value Valor de atenuación cuadrática.
    */
    void setAtenCuadrat(float value);
    /**
    * @brief Establece el ángulo de apertura del foco.
    *
    * @param angle Ángulo.
    */
    void setApertura(float angle);
    /**
    * @brief Establece el ángulo de apertura del foco.
    *
    * @param angle Ángulo en radianes o coseno.
    */
    void setPenumbra(float angle);
    /**
    * @brief Define el tipo de luz.
    *
    * 0 = direccional, 1 = puntual, 2 = spotlight.
    *
    * @param tipo Valor entero del tipo de luz.
    */
    void setTipoLuz(int tipo);

    // Getters
    /**
    * @brief Obtiene la posición actual de la luz.
    *
    * @return Vector vec3 con la posición.
    */
    glm::vec3 getPosicion();
    /**
    * @brief Obtiene la dirección actual de la luz.
    *
    * @return Vector vec3 con la dirección.
    */
    glm::vec3 getDireccion();
    /**
    * @brief Obtiene la intensidad ambiental.
    *
    * @return Valor float de intensidad ambiental.
    */
    float getAmbientStrength();
    /**
    * @brief Obtiene la intensidad difusa.
    *
    * @return Valor float de intensidad difusa.
    */
    float getDiffuseStrength();
    /**
    * @brief Obtiene la intensidad especular.
    *
    * @return Valor float de intensidad especular.
    */
    float getSpecularStrength();
    /**
    * @brief Obtiene la atenuación constante.
    *
    * @return Valor float.
    */
    float getAtenCte();
    /**
    * @brief Obtiene la atenuación lineal.
    *
    * @return Valor float.
    */
    float getAtenLineal();
    /**
    * @brief Obtiene la atenuación cuadrática.
    *
    * @return Valor float.
    */
    float getAtenCuadrat();
    /**
    * @brief Obtiene el valor de apertura del spotlight.
    *
    * @return Valor float del ángulo.
    */
    float getApertura();
    /**
    * @brief Obtiene el valor de penumbra del spotlight.
    *
    * @return Valor float del ángulo.
    */
    float getPenumbra();
    /**
    * @brief Obtiene el tipo actual de luz.
    *
    * @return 0 = direccional, 1 = puntual, 2 = spotlight.
    */
    int getTipoLuz();

    

    //shaders que tendra la luz
    /// @brief Shader de vértices para el cubo de luz.
    AlmondShader* shaderFiguraVertex; //este es un VERTEX SHADER para poder pintar el cubo
    /// @brief Shader de fragmento este shader sera tanto para la propia luz como para la figura FRAGMENT SHADER.
    AlmondShader* shaderLuzYFigura; // este shader sera tanto para la propia luz como para la figura FRAGMENT SHADER
    /// @brief Shader de fragmento exclusivo para el cubo (figura).
    AlmondShader* shaderFigura; //este shader sera el que tenga el cubo que veremos en la pantalla, representando la luz FRAGMENT SHADER
    /// @brief Programa de shaders para renderizado general.
    GLuint shaderProgram; //es el programa creado con los shaders
    /// @brief Programa de shaders específico para el cubo de luz.
    GLuint shaderProgramCUBO; //es el programa creado con los shaders

    /// @brief Vertex Array Object para el cubo de luz y Vertex Buffer Object para el cubo de luz.
    unsigned int lightVAO, lightVBO;
    
    

private:
    /// @brief Intensidad de la luz en RGB.
    glm::vec3 intensidad; // es un vec3 porue tiene RGB
    /// @brief Posición de la luz en el mundo.
    glm::vec3 posicion; //posicion donde se encuentra la luz
    /// @brief Intensidad de la componente ambiental.
    float ambientStrength;
    /// @brief Intensidad de la componente difusa.
    float diffuseStrength;
    /// @brief Intensidad de la componente especular.
    float specularStrength;
    /// @brief Dirección en la que apunta la luz.
    glm::vec3 direccion;

    // Atenuación (para punto y foco)
    /// @brief Atenuación constante.
    float atenCte;
    /// @brief Atenuación lineal.
    float atenLineal;
    /// @brief Atenuación cuadrática.
    float atenCuadrat;

    // Spotlight
    /// @brief Ángulo interno del haz de luz.
    float apertura;      // cos(inner angle)
    /// @brief Ángulo externo de la penumbra.
    float penumbra;      // cos(outer angle)

    /// @brief Tipo de luz: 0 = direccional, 1 = puntual, 2 = spotlight.
    int tipoLuz; // 0 = direccional, 1 = punto, 2 = spot
    
};

#endif 