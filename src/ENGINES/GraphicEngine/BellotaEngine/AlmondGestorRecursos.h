#ifndef ALMONDGESTORRECURSOS_H
#define ALMONDGESTORRECURSOS_H

#include <vector>
#include <iostream>
#include <string> 
#include <unordered_map>
#include <memory>
//includes de librerias de glm y glmatrix
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>

#include "AlmondRecurso.h"
#include "AlmondMalla.h" 
#include "AlmondTextura.h" 
#include "AlmondMaterial.h"
#include "AlmondShader.h"
/**
 * @brief AlmondGestorRecursos es el gestor centralizado de recursos para el motor grafico.
 *
 * Esta clase administra la carga, almacenamiento y reutilización de distintos tipos de recursos
 * (como modelos, texturas, shaders o materiales) mediante una caché interna. También incluye
 * una función especial para la creación/carga de materiales al leer archivos .obj/.fbx mediante Assimp.
 */
class AlmondGestorRecursos {
    public:
        /**
        * @brief Constructor por defecto.
        *
        * Inicializa el gestor de recursos.
        */
        AlmondGestorRecursos();
        /**
        * @brief Destructor.
        *
        * Libera los recursos al eliminar el gestor.
        */
        ~AlmondGestorRecursos();

    /**
     * @brief Devuelve un recurso del tipo especificado, reutilizándolo si ya fue cargado.
     *
     * Esta es una función plantilla que permite solicitar cualquier tipo de recurso
     * derivado de AlmondRecurso (como AlmondMalla, AlmondTextura, etc.). Si el recurso
     * ya ha sido cargado previamente (y está almacenado en la caché), se devuelve.
     * Si no, se determina el tipo por la extensión del archivo y se carga automáticamente, los tipos de los shaders son creados por nosotrosS.
     *
     * @tparam T Tipo de recurso deseado (debe derivar de AlmondRecurso).
     * @param nombre Nombre o ruta del archivo del recurso.
     * @return Puntero al recurso solicitado, o nullptr si la extensión es desconocida.
     */

        //plantilla para que reconozca diferentes tipos de recursos
        template <typename T>
        T* getRecurso(const std::string& nombre) {
            // Buscar si el recurso ya está almacenado
            auto it = recursos.find(nombre);
            if (it != recursos.end()) {
                //std::cout << "Recurso encontrado en caché: " << nombre << std::endl;
                return dynamic_cast<T*>(it->second.get()); 
            }

           // std::cout << "Cargando nuevo recurso: " << nombre << std::endl;

            // Crear el recurso según la extensión
            std::unique_ptr<AlmondRecurso> nuevoRecurso;
            if (nombre.ends_with(".obj") || nombre.ends_with(".fbx")) {
                nuevoRecurso = std::make_unique<AlmondMalla>(nombre, this);
            }else if(nombre.ends_with(".jpg") || nombre.ends_with(".png") || nombre.ends_with(".bmp")){ //formatos que admite texturas:(.jpg, .jpeg)(.png)(.bmp)(.gif)(.psd)(.tga)(.hdr)(.pic)(.ppm, .pgm)
                nuevoRecurso = std::make_unique<AlmondTextura>(nombre);
            }else if(nombre.ends_with(".AlmondVertex") || nombre.ends_with(".AlmondFragment")){ //formatos que admite shaders:(.AlmondShader es creado por nosotros, este tipo no existe en realidad)
                nuevoRecurso = std::make_unique<AlmondShader>(nombre);
            }else if(nombre.ends_with(".mtl")){ //formatos que admite materiales:(.mtl)
                nuevoRecurso = std::make_unique<AlmondMaterial>(nombre);
            } else {
                std::cerr << "Error: Tipo de recurso desconocido para " << nombre << std::endl;
                return nullptr;
            }

            // Cargar el recurso
            nuevoRecurso->cargarFichero(nombre);

            // Guardarlo en la caché
            T* ptrRecurso = dynamic_cast<T*>(nuevoRecurso.get());  
            recursos[nombre] = std::move(nuevoRecurso);

            return ptrRecurso;
        }
        /**
        * @note Esta función plantilla permite escribir una única lógica para todos los tipos de recursos.
        *       Usa `dynamic_cast` para convertir correctamente el recurso cargado al tipo solicitado.
        *       Admite formatos:
        *       - Modelos: .obj, .fbx
        *       - Texturas: .jpg, .png, .bmp
        *       - Shaders: .AlmondVertex, .AlmondFragment FORMATOS PROPIOS
        *       - Materiales: .mtl
        */

        //variables
        /// @brief Mapa de recursos cacheados indexados por nombre o ruta.
        std::unordered_map<std::string, std::unique_ptr<AlmondRecurso>> recursos;  

        /**
        * @brief Crea o devuelve un material con parámetros personalizados.
        *
        * Esta función fue añadida para facilitar la carga directa de materiales al procesar modelos
        * con Assimp, permitiendo definir sus propiedades directamente despues de leer el archivo.
        *
        * @param nombre Nombre identificador del material.
        * @param ambient Componente ambiental del material.
        * @param diffuse Componente difusa del material.
        * @param specular Componente especular del material.
        * @param shininess Brillo especular.
        * @param textura Ruta de la textura asociada (si la hay).
        * @return Puntero al material creado o recuperado.
        */
        AlmondMaterial* getMaterial(const std::string& nombre, glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular, float shininess, const std::string& textura); // esta es especial y se usa para cargarlos directamente desde la malla con assimp

        private:

            
    };

#endif