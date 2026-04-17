#ifndef ALMONDNODE_H
#define ALMONDNODE_H

#include <vector>
#include <memory>
//includes de librerias de glm y glmatrix
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>

#include "AlmondEntidad.h"
#include "AlmondEntidadLuz.h"
#include "AlmondEntidadCamara.h"
//#include "AlmondEntidadModelo.h"
class AlmondEntidadModelo;
class AlmondEntidadAnimacion;

/** 
 * @brief AlmondNode Representa un nodo en una jerarquía de escena con transformaciones y una entidad asociada.
 */
class AlmondNode {
public:
    //AlmondEntidad* entidad;
    std::unique_ptr<AlmondEntidad> entidad; /// Entidad asociada al nodo (modelo, luz, cámara, etc.)
    std::vector<AlmondNode*> hijos;         /// Lista de hijos del nodo
    AlmondNode* padre;                      /// Puntero al nodo padre
    int id;                                 /// ID único del nodo, se usaba para depuracion de codigo

    //variables para las transformaciones
    glm::vec3 traslacion;   ///< Vector de traslación
    glm::vec3 rotacion;     ///< Vector de rotación en grados
    glm::vec3 escalado;     ///< Vector de escalado
    glm::mat4 matrixTranf;  ///< Matriz de transformación final
    bool actualizarMatriz;  ///< Bandera para saber si recalcular la matriz por cambios en las transformaciones y si no hay cambios nos ahorramos calculos y mejoramos eficiencia

    //funciones clasicas
    /** @brief Constructor por defecto */
    AlmondNode();
    /** @brief Destructor */
    ~AlmondNode();
     /**
     * @brief Constructor con ID, se usaba para depuracion
     * @param id ID del nodo
     */
    AlmondNode(int );

    //funciones
    /** @brief Añade un hijo al nodo */
    void addHijo(AlmondNode *);
    /** @brief Quita un hijo del nodo */
    void quitarHijo(AlmondNode *);
    /** @brief Asigna una nueva entidad al nodo puede ser de diferentes tipos, camara,luz,modelo,animacion etc... 
    * @param puntero puntero a la entidad
    */
    bool setEntidad (std::unique_ptr<AlmondEntidad>);
    /** @brief Obtiene la entidad asociada */
    AlmondEntidad *getEntidad ();
    /** @brief Obtiene el nodo padre */
    AlmondNode *getPadre ();
    /** @brief Calcula la matriz de transformación local */
    glm::mat4 calcularMatriz();  // Calcular la matriz de transformación
    /**
     * @brief Recorre el árbol de nodos aplicando transformaciones y dibujando entidades
     * @param matrizAcum Matriz acumulada desde el padre
     * @param view Matriz de vista
     * @param projection Matriz de proyección
     * @param campos Posición de la cámara
     * @param luces Lista de luces activas
     */
    void recorrer(glm::mat4 matrizAcum, glm::mat4 view, glm::mat4 projection, const glm::vec3& campos, std::vector<AlmondNode*> luces);  // Recorrer y aplicar transformaciones
    
    /** @brief Asigna el valor de la traslacion "manualmente" */
    void setTraslacion (glm::vec3);
    /** @brief Asigna el valor de la rotacion "manualmente" */
    void setRotacion (glm::vec3);
    /** @brief Asigna el valor del escalado "manualmente" */
    void setEscalado (glm::vec3);
    /** @brief Suma el valor recibido a la traslacion que ya tenia el modelo */
    void trasladar (glm::vec3);
    /** @brief Suma el valor recibido a la rotacion que ya tenia el modelo */
    void rotar (glm::vec3);
    /** @brief Suma el valor recibido al escalado que ya tenia el modelo */
    void escalar (glm::vec3);
    /** @brief Devuelve la traslacion */
    glm::vec3 getTraslacion ();
    /** @brief Devuelve la rotacion */
    glm::vec3 getRotacion ();
    /** @brief Devuelve el escalado */
    glm::vec3 getEscalado ();
    /** @brief Asigna el valor de la matriz de transformacion "manualmente" */
    void setMatrizTransf (glm::mat4);
    /** @brief Devuelve la matriz de transformacion */
    glm::mat4 getMatrizTransf ();
    /** @brief Marca como desactualizada la matriz y propaga a los hijos */
    void propagarActualizacion();

    void printMatrix(const glm::mat4& matrix);

    //getters bounding box
    /** @brief Devuelve el centro del bounding box */
    glm::vec3 getCentroBoundingBox() const;
    /** @brief Devuelve el tamaño del bounding box */
    float getTamanyoBoundingBox() const;
    /** @brief Devuelve el vértice mínimo del bounding box */
    glm::vec3 getMinVertex() const;
    /** @brief Devuelve el vértice máximo del bounding box */
    glm::vec3 getMaxVertex() const;

    // Control visual
    /** @brief Establece si el nodo y sus hijos deben pintarse */
    void setSepinta(bool ajuste);
    /** @brief Indica si el nodo se debe pintar */
    bool getSepinta();
    /**
     * @brief Marca este nodo como un tile y almacena su posición
     * @param x Coordenada X del tile
     * @param y Coordenada Y del tile
     */
    void setTile(int x, int y);
    /** @brief Indica si este nodo es un tile */
    bool getTile();

private:
    //variable para pintar o no uno si no queremos
    bool Sepinta;   ///< Indica si se debe pintar el nodo
    bool EsunTile;  ///< Indica si el nodo es un tile
    int posTileX, posTileY; ///< Posición X e Y del tile, No se usan
};

#endif