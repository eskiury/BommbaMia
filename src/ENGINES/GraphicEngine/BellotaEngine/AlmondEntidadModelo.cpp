#include "AlmondEntidadModelo.h"
#include "AlmondMalla.h"


    AlmondEntidadModelo::AlmondEntidadModelo(){
        cargado = false;
        gestorGuardado = nullptr;
    }
    AlmondEntidadModelo::~AlmondEntidadModelo(){
        mallas.clear();
    }
    void AlmondEntidadModelo::dibujar(const glm::mat4& transformacion, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& campos,std::vector<AlmondNode*> luces){
        //de momento no uso la matriz pasada
        for (auto& malla : mallas) {
            malla->dibujar(transformacion, view, projection, campos, luces);
        } 
    }


   void AlmondEntidadModelo::cargarModelo(const std::string& ruta, AlmondGestorRecursos* gestor){
        //std::cout << "Cargando modelo 3D desde el archivo: " << ruta << std::endl;

        // Aquí pides las mallas al gestor
        AlmondMalla* mallaCargada = gestorGuardado->getRecurso<AlmondMalla>(ruta);  // Pide el recurso por nombre, ESTOY USANDO EL GESTOR GUARDADO
        if (mallaCargada != nullptr) {
            // Si la malla existe, la agregamos al vector mallas
            mallas.push_back(mallaCargada);
            cargado = true;  // Marca como cargado si el proceso es exitoso

        } else {
            std::cerr << "No se pudo cargar la malla desde el gestor" << std::endl;
        }
    }

void AlmondEntidadModelo::cargarTextura(const std::string& ruta, AlmondGestorRecursos* gestor){
    AlmondTextura* texturaCargada = gestorGuardado->getRecurso<AlmondTextura>(ruta);  // Pide el recurso por nombre,  ESTOY USANDO EL GESTOR GUARDADO
    for (AlmondMalla* malla : mallas) {  // Recorremos cada malla en el vector
    
    if (texturaCargada != nullptr) {
        // Si la textura se cargó correctamente, la agregamos a la malla
        malla->cargarTexturaALaMalla(texturaCargada);
    } else {
        std::cerr << "No se pudo cargar la textura al modelo desde el gestor" << std::endl;
    }
}
}

 


bool AlmondEntidadModelo::getCargado(){
    return cargado;
}


void AlmondEntidadModelo::guardarGestor(AlmondGestorRecursos* gestor){
    gestorGuardado = gestor;
}

// Getter para el centro del bounding box
glm::vec3 AlmondEntidadModelo::getCentroBoundingBox() const {
    return mallas[0]->getCentroBoundingBox();
}

// Getter para el tamaño del bounding box (lado del cubo)
float AlmondEntidadModelo::getTamanyoBoundingBox() const {
    return mallas[0]->getTamanyoBoundingBox();
}

// Getter para el vértice mínimo
glm::vec3 AlmondEntidadModelo::getMinVertex() const {
    return mallas[0]->getMinVertex();
}

// Getter para el vértice máximo
glm::vec3 AlmondEntidadModelo::getMaxVertex() const {
    return mallas[0]->getMaxVertex();
}
