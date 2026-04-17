#ifndef ALMONDENTIDADMODELO_H
#define ALMONDENTIDADMODELO_H
#include "AlmondEntidad.h"
//#include "AlmondMalla.h"
#include "AlmondTextura.h"
#include "AlmondGestorRecursos.h"
#include <vector>
//includes de librerias de glm y glmatrix
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>

//librerias assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#include "AlmondNode.h"
class AlmondNode;
class AlmondMalla;
/**
 * @brief Clase AlmondEntidadModelo
 *
 * Esta clase hereda de la clase entidad y gestiona el funcionamiento y gestion de las mallas del motor grafico.
 * 
 */
class AlmondEntidadModelo : public AlmondEntidad
{
public:
    /**
    * @brief Constructor de la clase AlmondEntidadModelo.
    *
    * Inicializa una instancia de la clase modelo, que hereda de la clase Entidad.
    */
    AlmondEntidadModelo();
    /**
    * @brief Destructor de la clase AlmondEntidadModelo.
    *
    * Libera los recursos asociados a la instancia de la luz.
    */
    ~AlmondEntidadModelo();
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
    void dibujar(const glm::mat4&, const glm::mat4&, const glm::mat4&, const glm::vec3&, std::vector<AlmondNode*>) override;
    /**
    * @brief Carga el modelo 3D desde el gestor de recursos.
    *
    * Bussca el modelo atraves del gestor de recursos y guarda el puntero a las mallas que pueda contener ese modelo, en la mayoria de casos es solo una malla
    *
    * @param nombreModel Nombre o ruta del modelo.
    * @param gestor Puntero al gestor de recursos que contiene el modelo.
    */
    void cargarModelo(const std::string&, AlmondGestorRecursos* gestor);
    /**
    * @brief Carga una textura asociada al modelo desde el gestor de recursos.
    *
    * Asocia texturas a las mallas del modelo utilizando el gestor de recursos.
    *
    * @param nombreTextura Nombre o ruta de la textura.
    * @param gestor Puntero al gestor de recursos que contiene la textura.
    */
    void cargarTextura(const std::string&, AlmondGestorRecursos* gestor);
    /**
    * @brief Guarda una referencia al gestor de recursos.
    *
    * Se utiliza para conservar el puntero al gestor y poder acceder a él más adelante cuando se necesite cargar recursos.
    *
    * @param gestor Puntero al gestor de recursos.
    */
    void guardarGestor(AlmondGestorRecursos* gestor);// guardo el gestor para poder usarlo luego
    /**
    * @brief Indica si hay un modelo cargado correctamente.
    *
    * @return `true` si hay un modelo cargado, `false` si no.
    */
    bool getCargado();

     //getters bounding box
     /**
    * @brief Obtiene el centro del bounding box del modelo.
    *
    * Devuelve el centro del modelo recibido de la malla.
    *
    * @return Vector con la posición del centro.
    */
     glm::vec3 getCentroBoundingBox() const;
     /**
    * @brief Devuelve el tamaño (dimensión máxima, tamaño mas grande del cubo) del bounding box.
    *
    * @return Valor float con el tamaño.
    */
     float getTamanyoBoundingBox() const;
     /**
    * @brief Devuelve el vértice mínimo del bounding box (mínima coordenada).
    *
    * @return Vector vec3 con el vértice mínimo.
    */
     glm::vec3 getMinVertex() const;
     /**
    * @brief Devuelve el vértice máximo del bounding box (máxima coordenada).
    *
    * @return Vector vec3 con el vértice máximo.
    */
     glm::vec3 getMaxVertex() const;

    /// @brief Contenedor de punteros a las mallas del modelo.
    std::vector<AlmondMalla*> mallas;  // Ahora contiene punteros a AlmondMalla
    /// @brief Puntero al gestor de recursos utilizado para cargar modelos o texturas.
    AlmondGestorRecursos* gestorGuardado;
    /// @brief Indica si el modelo fue cargado correctamente.
    bool cargado;

private:

    
    
};

#endif 