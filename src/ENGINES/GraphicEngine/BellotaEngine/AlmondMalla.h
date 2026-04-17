#ifndef ALMONDMALLA_H
#define ALMONDMALLA_H
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
#include "AlmondMaterial.h"
#include "AlmondShader.h"

//librerias assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>
class AlmondGestorRecursos;
//class AlmondNode;
#include "AlmondNode.h"
// #include "AlmondEntidad.h"
// #include "AlmondEntidadLuz.h"


/**
 * @brief Estructura que representa un vértice de una malla.
 */
//estructura de los vertices 
struct AlmondVertex {
    glm::vec3 Position; ///< Posición del vértice
    glm::vec3 Normal;   ///< Normal del vértice
    glm::vec2 TexCoords;///< Coordenadas de textura
};


/**
 * @brief Representa una submalla con su propia geometría, material y shaders.
 */
struct SubMalla {
    std::vector<AlmondVertex> vertices; ///< Lista de vértices
    std::vector<unsigned int> indices;  ///< Índices para dibujado por elementos
    unsigned int VAO, VBO, EBO;         ///< Buffers de OpenGL
    unsigned int materialIndex;         ///< Índice al material correspondiente
    GLuint shaderProgram;               ///< Programa de shader asociado
    AlmondShader* shaderVertex;         ///< Shader de vértices
    AlmondShader* shaderFragment;       ///< Shader de fragmentos

    // Cache de ubicaciones de uniformes
    GLint modelLoc = -1;
    GLint viewLoc = -1;
    GLint projLoc = -1;
    GLint viewPosLoc = -1;
    GLint texLoc = -1;
    GLint objectColorLoc = -1;
    GLint ambLoc = -1;
    GLint difLoc = -1;
    GLint speLoc = -1;
    GLint shiLoc = -1;
    //GLint u_numLightsLoc = -1;
     /**
     * @brief Constructor por defecto.
     */
    SubMalla() : VAO(0), VBO(0), EBO(0), materialIndex(0) {}

    //funcion que carga esto y evita hacerlo todo el rato
    /**
     * @brief Carga las ubicaciones de los uniformes para optimización.
     */
    void cacheUniformLocations() {
        modelLoc        = glGetUniformLocation(shaderProgram, "model");
        viewLoc         = glGetUniformLocation(shaderProgram, "view");
        projLoc         = glGetUniformLocation(shaderProgram, "projection");
        viewPosLoc      = glGetUniformLocation(shaderProgram, "viewPos");
        texLoc          = glGetUniformLocation(shaderProgram, "texture1");
        objectColorLoc  = glGetUniformLocation(shaderProgram, "objectColor");
        ambLoc          = glGetUniformLocation(shaderProgram, "amb");
        difLoc          = glGetUniformLocation(shaderProgram, "dif");
        speLoc          = glGetUniformLocation(shaderProgram, "spe");
        shiLoc          = glGetUniformLocation(shaderProgram, "shi");
        //u_numLightsLoc    = glGetUniformLocation(shaderProgram, "numLights");
    }
    /**
     * @brief Asigna los shaders a la submalla.
     */
    void cargarShaders(AlmondShader* shaderVertexPAS,AlmondShader* shaderFragmentPAS){
        shaderVertex = shaderVertexPAS;
        shaderFragment = shaderFragmentPAS;
    }
    /**
     * @brief Configura los buffers de OpenGL.
     */
    void setupMesh() {
        // Crear el programa de shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, shaderVertex->getShader());
        glAttachShader(shaderProgram, shaderFragment->getShader());
        glLinkProgram(shaderProgram);

        GLint success;
        GLchar infoLog[512];

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        }

        // Genera los identificadores para el VAO, VBO y EBO
        glGenVertexArrays(1, &VAO); // Almacena la configuración de los atributos de los vértices.
        glGenBuffers(1, &VBO); // Almacena la información de los vértices (posiciones, normales, coordenadas de textura, etc.).
        glGenBuffers(1, &EBO); // Almacena los índices de los vértices para dibujar elementos con glDrawElements.

        // Vincula el VAO (esto comienza el proceso de configuración)
        glBindVertexArray(VAO);

        // Vincula el VBO (almacena los datos de los vértices)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(AlmondVertex), &vertices[0], GL_STATIC_DRAW);  // Sube los datos de los vértices a la GPU

        // Vincula el EBO (almacena los índices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Especifica la estructura de los vértices:
        // 1. Posiciones de los vértices
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AlmondVertex), (void*)0);

        // 2. Normales de los vértices
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AlmondVertex), (void*)offsetof(AlmondVertex, Normal));

        // 3. Coordenadas de textura
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AlmondVertex), (void*)offsetof(AlmondVertex, TexCoords));

        // Desvincula el VAO (para terminar la configuración)
        glBindVertexArray(0);

        cacheUniformLocations();

    }
     /**
    * @brief Método de dibujado que hereda de la clase Entidad.
    *
    * Renderiza la luz utilizando las matrices de transformación, vista y proyección, 
    * además de la posición de la cámara y un conjunto de luces de la escena.
    *
    * @param transformacion Matriz de transformación del modelo.
    * @param view Matriz de vista.
    * @param projection Matriz de proyección.
    * @param materiales punteros a los materiales.
    * @param materialesIndex indices asignados a los materiales sobre esta malla en general.
    * @param camPos Posición actual de la cámara.
    * @param luces Vector de punteros a nodos de luces en la escena.
    */
    void draw(const glm::mat4& transformacion, const glm::mat4& view, const glm::mat4& projection, const std::vector<AlmondMaterial*>& materiales, const std::vector<int>& materialesIndex, const glm::vec3& campos, std::vector<AlmondNode*> luces) const{
            glUseProgram(shaderProgram);//aqui ira la funcion de usar del shader
             
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transformacion[0][0]);
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
            glUniform3fv(viewPosLoc, 1, glm::value_ptr(campos));

            glm::vec3 objectColor(1.0f, 0.0f, 0.0f); // Objeto rojo
            glm::vec3 amb(0.0f, 0.0f, 0.0f); // ambiente
            glm::vec3 dif(0.0f, 0.0f, 0.0f); // Objeto rojo
            glm::vec3 spe(0.0f, 0.0f, 0.0f); // Objeto rojo
            float shi = 0.0f;

            unsigned int textureIndex = 0;
            //cojo el color del material
            // Buscar la posición correcta en materialesIndex
            auto it = std::find(materialesIndex.begin(), materialesIndex.end(), materialIndex);

            if (it != materialesIndex.end()) {
                size_t pos = std::distance(materialesIndex.begin(), it); // Obtener la posición en el vector

                if (pos < materiales.size() && materiales[pos] != nullptr) {
                    objectColor = materiales[pos]->getColor(); // Obtener el color correcto
                    //obtengo los variasbles para las luces del material
                    amb = materiales[pos]->getAmbient();
                    dif = materiales[pos]->getDiffuse();
                    spe = materiales[pos]->getSpecular();
                    shi = materiales[pos]->getShininess();
                    //obtengo la textura
                    AlmondTextura* aux = materiales[pos]->getTextura();
                    if(aux != nullptr){
                        textureIndex = aux->getId();
                    }
                } else {
                   // std::cerr << "Error: Índice fuera de rango en materiales." << std::endl;
                }
            } else {
               // std::cerr << "MaterialIndex no encontrado en materialesIndex: " << materialIndex << std::endl;
            }

            glUniform3fv(objectColorLoc, 1, glm::value_ptr(objectColor));
            glUniform3fv(ambLoc, 1, glm::value_ptr(amb));
            glUniform3fv(difLoc, 1, glm::value_ptr(dif));
            glUniform3fv(speLoc, 1, glm::value_ptr(spe));
            glUniform1f(shiLoc, shi);  // Correcto para un float
            //---------------------------------------LUCES-----------------------------------------------------
            const int MAX_LIGHTS = 16;
            int numLights = std::min(static_cast<int>(luces.size()), MAX_LIGHTS);
            glUniform1i(glGetUniformLocation(shaderProgram, "numLights"), numLights);

            for (int i = 0; i < numLights; ++i) {
                AlmondNode* nodoLuz = luces[i];
                auto entidad = nodoLuz->getEntidad();

                auto* luz = dynamic_cast<AlmondEntidadLuz*>(entidad); // aseguramos que es luz
                if (luz == nullptr) {
                    //std::cerr << "Advertencia: Nodo no contiene una luz válida\n";
                    continue;
                }

                std::string base = "lights[" + std::to_string(i) + "].";

                glUniform3fv(glGetUniformLocation(shaderProgram, (base + "intensidad").c_str()), 1, glm::value_ptr(luz->getIntensidad()));
                glUniform3fv(glGetUniformLocation(shaderProgram, (base + "posicion").c_str()), 1, glm::value_ptr(luz->getPosicion()));
                glUniform3fv(glGetUniformLocation(shaderProgram, (base + "direccion").c_str()), 1, glm::value_ptr(luz->getDireccion()));

                glUniform1f(glGetUniformLocation(shaderProgram, (base + "ambientStrength").c_str()), luz->getAmbientStrength());
                glUniform1f(glGetUniformLocation(shaderProgram, (base + "diffuseStrength").c_str()), luz->getDiffuseStrength());
                glUniform1f(glGetUniformLocation(shaderProgram, (base + "specularStrength").c_str()), luz->getSpecularStrength());

                glUniform1f(glGetUniformLocation(shaderProgram, (base + "atenCte").c_str()), luz->getAtenCte());
                glUniform1f(glGetUniformLocation(shaderProgram, (base + "atenLineal").c_str()), luz->getAtenLineal());
                glUniform1f(glGetUniformLocation(shaderProgram, (base + "atenCuadrat").c_str()), luz->getAtenCuadrat());

                glUniform1f(glGetUniformLocation(shaderProgram, (base + "apertura").c_str()), luz->getApertura());
                glUniform1f(glGetUniformLocation(shaderProgram, (base + "penumbra").c_str()), luz->getPenumbra());

                glUniform1i(glGetUniformLocation(shaderProgram, (base + "tipoLuz").c_str()), luz->getTipoLuz());
            }
            // --- AHORA ENLAZAMOS LA TEXTURA ---
            if (textureIndex != 0) {  // Verifica que la textura es válida
                glUniform1i(glGetUniformLocation(shaderProgram, "hasTexture"), 1);
                glActiveTexture(GL_TEXTURE0);  // Activa la unidad de textura 0
                glBindTexture(GL_TEXTURE_2D, textureIndex);  // Vincula la textura
                //glUniform1i(texLoc, 0);  // Asigna la textura a la unidad 0 en el shader
            } else {
               // std::cerr << "Advertencia: No se encontró una textura válida. Se usará una textura vacía." << std::endl;
                glUniform1i(glGetUniformLocation(shaderProgram, "hasTexture"), 0);
                glBindTexture(GL_TEXTURE_2D, 0);  // Usa una textura vacía si no hay una válida
            }

            // Asigna la textura al shader
           glUniform1i(texLoc, 0);  // Indica que la textura está en la unidad 0

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

/**
 * @brief Clase AlmondMalla
 *
 * Esta clase hereda de la clase recurso y gestiona el funcionamiento y gestion de las mallas del motor grafico.
 * 
 */
class AlmondMalla : public AlmondRecurso
{
public:
    /**
    * @brief Constructor de la clase AlmondMalla.
    *
    * Inicializa una instancia de la clase malla, que hereda de la clase recurso.
    */
    AlmondMalla();
    /**
    * @brief Destructor de la clase AlmondMalla.
    *
    * Libera los recursos asociados a la instancia de la luz.
    */
    ~AlmondMalla();
    //constructor predefinido
    /**
     * @brief Constructor con nombre del recurso.
     *
     * @param nombre ruta de la malla que usara.
     */
    AlmondMalla(const std::string& nombre);
    /**
     * @brief Constructor con nombre del recurso y el gestor de recursos.
     *
     * @param nombre ruta de la malla que usara.
     * @param gestor Puntero al gestor de recursos que contiene el modelo.
     */
    AlmondMalla(const std::string& nombre,AlmondGestorRecursos* gestor);
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
    void dibujar(const glm::mat4&, const glm::mat4&, const glm::mat4&, const glm::vec3&, std::vector<AlmondNode*> );
     /**
     * @brief Sobreescribe la funcion de cargar fichero de la clase recurso, para cargar un recurso de clase malla usando Assimp.
     *
    * Esta función utiliza Assimp para leer un archivo de modelo (.obj, .fbx, etc.), y en caso de éxito,
    * comienza el procesamiento recursivo de sus nodos.
    *
     * @param nombre ruta de la malla que usara.
     */
    void cargarFichero (const std::string& nombre) override; 
     /**
     * @brief Guarda una textura em el vector de texturas de la malla
     */
    void cargarTexturaALaMalla(AlmondTextura* tex);
    /**
    * @brief Guarda una referencia al gestor de recursos.
    *
    * Se utiliza para conservar el puntero al gestor y poder acceder a él más adelante cuando se necesite cargar recursos.
    *
    * @param gestor Puntero al gestor de recursos.
    */
    void cargarGestor(AlmondGestorRecursos* gestor);
    /**
     * @brief Carga y guarda los shaders por defecto de vértices y fragmentos.Usa los mismos para todas las mallas
     */
    void cargarAlmondVertexShader();
    void cargarAlmondFragmentShader();
     /**
     * @brief Asigna los shaders a todas las submallas para que puedan usarlos.
     */
    void PasarShadersASubmallas();

    //getters bounding box
    /**
     * @brief Devuelve el centro del modelo cargado.
     */
    glm::vec3 getCentroBoundingBox() const;
    /**
     * @brief Devuelve el tamaño/lado/dimension mas grande de un modelo, para poder carcular una boundingbox cuadrada
     */
    float getTamanyoBoundingBox() const;
    /**
     * @brief Devuelve la dimension(x,y,z) de la malla mas pequeña, la distancia entre el punto mas lejano y cercano de cada coordenada
     */
    glm::vec3 getMinVertex() const;
    /**
     * @brief Devuelve la dimension(x,y,z) de la malla mas grande, la distancia entre el punto mas lejano y cercano de cada coordenada
     */
    glm::vec3 getMaxVertex() const;

    /// @brief Puntero al gestor de recursos utilizado para cargar modelos o texturas.
    AlmondGestorRecursos* gestorGuardado;
    ///< Submallas que componen la malla
    std::vector<SubMalla> subMallas;
    ///< Texturas usadas,seran de la clase AlmondTextura y sera un vector de punteros obtenidos con el get del gestor de recursos
    std::vector<AlmondTextura*> texturas; // seran de la clase AlmondTextura y sera un vector de punteros obtenidos con el get del gestor de recursos
    //tendremos materiales y los materiales estan compuestos por las texturas(y mas cosas pero puede que solo sea una textura)
    //almacenar los material index que indican a la malla que material usa cada malla
    ///< Materiales usados
    std::vector<AlmondMaterial*> materiales;
    ///< Índices de todos los materiales usados por esta malla
    std::vector<int> materialesIndex;
    //shaders
    ///< Shader de vértices
    AlmondShader* shaderVertex; 
    ///< Shader de fragmentos
    AlmondShader* shaderFragment;

    
    
private:
    /**
    * @brief Procesa recursivamente un nodo de la escena cargada por Assimp.
    *
    * Recorre cada malla asociada al nodo y luego procesa todos sus nodos hijos.
    * También se encarga de calcular el centro y tamaño del bounding box global
    * basado en los vértices de todas las submallas.
    *
    * @param nodo Puntero al nodo de Assimp que se va a procesar.
    * @param scene Puntero a la escena completa cargada por Assimp.
    */
    void procesarNodo(aiNode* nodo, const aiScene* scene);
    /**
    * @brief Procesa una malla individual y crea una SubMalla a partir de sus datos.
    *
    * Esta función extrae vértices, normales, coordenadas de textura e índices de la malla.
    * También gestiona materiales y texturas, y configura la submalla con shaders y buffers de OpenGL.
    * 
    * @param mesh Puntero a la malla de Assimp que se va a procesar.
    * @param scene Puntero a la escena de Assimp, necesario para acceder a materiales y texturas.
    */
    void procesarMalla(aiMesh* mesh, const aiScene* scene);
    ///< Centro del modelo cargado con assimp
    glm::vec3 centroBoundingBox;
    ///< Lado del cubo, coge el tamaño de la cara mas grande, para que no quede nada fuera del bounding box
    float tamanyoBoundingBox;  // Lado del cubo, //coge el tamaño de la cara mas grande, para que no quede nada fuera del bounding box
    ///< minimo tamaño de vertices del modelo entre las tres dimensiones x,y,z
    glm::vec3 minVertex;
    ///< maximo tamaño de vertices del modelo entre las tres dimensiones x,y,z
    glm::vec3 maxVertex;
    
    
};

#endif 