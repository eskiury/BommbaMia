#ifndef ALMONDPARTICULA_H
#define ALMONDPARTICULA_H

#include "AlmondShader.h"
#include "AlmondTextura.h"

#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief AlmondParticula Representa una partícula en el sistema de partículas, incluyendo su comportamiento visual y físico.
 *
 * Una partícula tiene posición, color, tamaño, vida útil, velocidad, tipo y se renderiza mediante shaders y una textura.
 * Puede representar humo, explosión, chispa, etc.
 */
//para cargar materiales podemos hacer un archivo .json, guardar los valores y el nombre para el material(cargariamos todo al inicio) y luego asignar esos materiales a los modelos que queramos
class AlmondParticula
{
public:
    /** @brief Constructor por defecto */
    AlmondParticula();
    //constructor propio
    /**
     * @brief Constructor parametrizado
     * @param posi Posición inicial
     * @param col Color
     * @param tam Tamaño
     * @param vida Vida útil
     * @param velo Velocidad
     * @param verti Vértices para formar la partícula
     * @param shaderVertexPAS Shader de vértices
     * @param shaderFragmentPAS Shader de fragmentos
     * @param ang Ángulo de rotación
     * @param tex Textura
     * @param tip Tipo de partícula
     */
    AlmondParticula(glm::vec3 posi,glm::vec3 col,float tam, float vida, glm::vec3 velo, glm::vec3 verti[4],AlmondShader* shaderVertexPAS,AlmondShader* shaderFragmentPAS,float ang, AlmondTextura* tex, int tip);
    /** @brief Destructor */
    ~AlmondParticula();
    //---------------------------GETTERS----------------------
    /** @brief Obtiene la posición de la partícula */
    glm::vec3 getPosicion() const;
    /** @brief Obtiene el color de la partícula */
    glm::vec3 getColor() const;
    /** @brief Obtiene el tamaño de la partícula */
    float getTamanyo() const;
    /** @brief Obtiene la vida restante */
    float getVida() const;
    /** @brief Obtiene la velocidad */
    glm::vec3 getVelocidad() const;
    /** @brief Obtiene los vértices de la partícula */
    const glm::vec3* getVertices() const; // Getter para vertices
    /** @brief Obtiene el tipo de partícula (1: humo, 2: explosión, 3: chispa) */
    int getTipoDeParticula();
    //---------------------------SETTERS----------------------
    /** @brief Asigna una nueva posición */
    void setPosicion(glm::vec3 posi);
    /** @brief Asigna un nuevo color */
    void setColor(glm::vec3 col);
    /** @brief Asigna un nuevo tamaño */
    void setTamanyo(float tam);
    /** @brief Asigna una nueva vida */
    void setVida(float vid);
    /** @brief Asigna una nueva velocidad */
    void setVelocidad(glm::vec3 vel);
    /** @brief Asigna nuevos vértices */
    void setVertices(const glm::vec3 verts[4]); // Setter para vertices
    /** @brief Asigna el tipo de partícula */
    void setTipoDeParticula(int tip);
    //----------------------FUNCIONES PROPIAS----------------
    /**
     * @brief Configura la partícula: buffers, atributos, shaders.
     *
     * Inicializa los VAO, VBO, EBO y VBOinstancia, y vincula los shaders si están disponibles.
     */
    void configurar();
    /**
     * @brief Dibuja la partícula con los shaders y parámetros proporcionados
     * @param view Matriz de vista
     * @param projection Matriz de proyección
     * @param campos Posición de la cámara
     */
    void dibujar(const glm::mat4& view, const glm::mat4& projection,const glm::vec3& campos);
    /**
     * @brief Actualiza la lógica según el tipo de partícula
     * @param dt Tiempo delta desde el último frame
     */
    void actualizarParticulas(float dt);
    /** @brief Actualiza una partícula de humo */
    void actualizarHumo(float dt);
    /** @brief Actualiza una partícula de explosión */
    void actualizarExplosion(float dt);
    /** @brief Actualiza una partícula tipo chispa */
    void actualizarChispa(float dt);

    /** @brief Asigna el ángulo de rotación */
    void setAngulo(float a);
    /** @brief Obtiene el ángulo de rotación */
    float getAngulo() const;
    /** @brief Asigna la textura */
    void setTextura(AlmondTextura* tex);
    /** @brief Obtiene el puntero a la textura */
    AlmondTextura* getTextura() const;

private:
    glm::vec3 posicion; ///< Posición actual
    glm::vec3 color;    ///< Color base
    float tamanyo;      ///< Tamaño visual
    float vida; // <= 0: muerta ///< Vida restante
    glm::vec3 velocidad;    ///< Vector de velocidad
    glm::vec3 vertices[4]; // usando GLM //tendra 4 ///< Vértices para el quad
    float vidaInicial;   ///< Vida al momento de crearse
    float angulo;        ///< Ángulo de rotación
    int tipoDeParticula;    ///< Tipo de partícula

    unsigned int VAO, VBO, EBO, VBOinstancia;   ///< Vertex Array Object y vertex buffers
    GLuint shaderProgram; // Programa de shaders    ///< Programa de shaders compilado
    AlmondShader* shaderVertex; ///< Shader de vértices
    AlmondShader* shaderFragment;   ///< Shader de fragmentos
    AlmondTextura* textura; ///< Textura aplicada a la partícula
    
};

#endif 