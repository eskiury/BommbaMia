#ifndef ALMONDRECURSO_H
#define ALMONDRECURSO_H

#include <string>
#include <vector>
//includes de librerias de glm y glmatrix
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief AlmondRecurso Clase base abstracta para todos los recursos del sistema Almond.
 *
 * Esta clase proporciona una interfaz uniforme para cargar recursos (como texturas, shaders, modelos, etc.).
 * Las clases derivadas deben implementar el método `cargarFichero`. 
 * También gestiona el nombre del recurso, útil para sistemas de gestión o carga por nombre.
 */
class AlmondRecurso {
public:
    
 /**
     * @brief Destructor virtual por defecto.
     */
    virtual ~AlmondRecurso() = default;

    // Métodos virtuales
    /**
     * @brief Carga el recurso desde un fichero.
     * 
     * Este método debe ser implementado por todas las clases que hereden de AlmondRecurso.
     * 
     * @param nombre Nombre del archivo del que se cargará el recurso.
     */
    virtual void cargarFichero(const std::string& nombre) = 0; // Método puro a implementar

    // Getters y Setters para el nombre del recurso
    /**
     * @brief Obtiene el nombre del recurso.
     * @return Nombre del recurso como string.
     */
    std::string getNombre() const { return nombre; }
    /**
     * @brief Establece el nombre del recurso.
     * @param nuevoNombre El nuevo nombre a asignar al recurso.
     */
    void setNombre(const std::string& nuevoNombre) { nombre = nuevoNombre; }

private:

protected: // Para que las subclases puedan acceder a estas cosas, si queremos que sean globales
    ///< Nombre del recurso, variable de tipo protected (usado como identificador o referencia en el sistema)
    std::string nombre; // Nombre del recurso (textura, modelo, etc.)
    
};

#endif