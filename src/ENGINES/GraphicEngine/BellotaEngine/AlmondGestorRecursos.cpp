#include "AlmondGestorRecursos.h"
 #include <filesystem> // Para cambiar la ruta

    AlmondGestorRecursos::AlmondGestorRecursos(){
        
    }
    AlmondGestorRecursos::~AlmondGestorRecursos(){
        
    }


AlmondMaterial* AlmondGestorRecursos::getMaterial(const std::string& nombre, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, const std::string& textura) {
    // Buscar si el recurso ya está almacenado
    auto it = recursos.find(nombre);
    if (it != recursos.end()) {
        //std::cout << "Recurso encontrado en caché: " << nombre << std::endl;
        return dynamic_cast<AlmondMaterial*>(it->second.get()); 
    }

    //std::cout << "Cargando nuevo recurso: " << nombre << std::endl;

    // Crear el recurso según la extensión
    std::unique_ptr<AlmondMaterial> nuevoRecurso;
    int miNumero = 0;
    nuevoRecurso = std::make_unique<AlmondMaterial>(nombre, miNumero);

    AlmondTextura* tex = nullptr;
    std::string nuevaRutaTextura = "NOHAY";

    if (textura != "NOHAY") {
        // Extraer solo el nombre del archivo sin la ruta completa
        std::filesystem::path rutaOriginal(textura);
        std::string nombreArchivo = rutaOriginal.filename().string(); // Obtiene "boceto7.png"

        // Construir la nueva ruta
        nuevaRutaTextura = "assets/sprites/" + nombreArchivo; //cambiar esta ruta
        tex = this->getRecurso<AlmondTextura>(nuevaRutaTextura);
    }

    // Cargar el material con la nueva ruta de textura
    nuevoRecurso->cargarMaterial(nombre, ambient, diffuse, specular, shininess, tex);

    // Guardarlo en la caché
    AlmondMaterial* ptrRecurso = dynamic_cast<AlmondMaterial*>(nuevoRecurso.get());  
    recursos[nombre] = std::move(nuevoRecurso);

    return ptrRecurso;
}

