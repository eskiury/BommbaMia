#include "AlmondNode.h"
#include <algorithm>
#include <iostream>
#include "AlmondEntidadModelo.h"
#include "AlmondEntidadAnimacion.h"



    AlmondNode::AlmondNode(){
        //inicializamos las transformaciones por defecto
        traslacion = glm::vec3(0.0f); // Sin traslación
        rotacion = glm::vec3(0.0f);   // Sin rotación
        escalado = glm::vec3(1.0f);   // Escala inicial en 1.0 (sin cambios)
        //inicializamos la matriz como la matriz identidad, que significa que todavia no se le han hecho transformaciones
        matrixTranf = glm::mat4(1.0f);
        actualizarMatriz = false;
        padre = nullptr;
        entidad = nullptr;
        Sepinta = true;
        EsunTile = false;
        posTileX = -1;
        posTileY = -1;
    }

    AlmondNode::AlmondNode(int idnu){
        id = idnu;
        //inicializamos las transformaciones por defecto
        traslacion = glm::vec3(0.0f); // Sin traslación
        rotacion = glm::vec3(0.0f);   // Sin rotación
        escalado = glm::vec3(1.0f);   // Escala inicial en 1.0 (sin cambios)
        //inicializamos la matriz como la matriz identidad, que significa que todavia no se le han hecho transformaciones
        matrixTranf = glm::mat4(1.0f);
        actualizarMatriz = false;
        padre = nullptr;
        entidad = nullptr;
        Sepinta = true;
        EsunTile = false;
        posTileX = -1;
        posTileY = -1;
    }

    AlmondNode::~AlmondNode() {
        // Liberar la entidad si existe
        if (entidad) {
            // delete entidad;  // Descoméntalo si la entidad necesita ser eliminada
            entidad = nullptr;
        }
    
        // No eliminamos manualmente los hijos, ya que `nodosJuego` maneja la memoria.
        hijos.clear(); // Solo limpiamos el vector
    }
    
    void AlmondNode::addHijo(AlmondNode* retaco) {
        hijos.push_back(retaco);
        retaco->padre = this;
    }
    
    void AlmondNode::quitarHijo(AlmondNode* retaco) {
        // Solo lo removemos de la lista, sin hacer `delete` (lo maneja `nodosJuego`)
        hijos.erase(std::remove(hijos.begin(), hijos.end(), retaco), hijos.end());
        retaco->padre = nullptr;
    }
    
    bool AlmondNode::setEntidad (std::unique_ptr<AlmondEntidad> laEntidad){
        entidad = std::move(laEntidad);
        return true;
    }
    AlmondEntidad * AlmondNode::getEntidad (){
        return entidad.get();;
    }
    AlmondNode * AlmondNode::getPadre (){
        return padre;
    }

    void AlmondNode::setTraslacion (glm::vec3 laTraslacion){
        traslacion = laTraslacion;
        actualizarMatriz = true;
    }
    void AlmondNode::setRotacion (glm::vec3 laRotacion){
        rotacion = laRotacion;
    }
    void AlmondNode::setEscalado (glm::vec3 laEscalacion){
        escalado = laEscalacion;
    }
    void AlmondNode::trasladar (glm::vec3 delta){
        traslacion += delta; // Añadir el desplazamiento
        actualizarMatriz = true;

         // Propagar la actualización a los hijos recursivamente
        for (AlmondNode* hijo : hijos) {
            if (hijo) {
            hijo->propagarActualizacion();
            }
        }
    }
    void AlmondNode::rotar (glm::vec3 delta){
        rotacion += delta; // Añadir la rotación
        actualizarMatriz = true;

         // Propagar la actualización a los hijos recursivamente
        for (AlmondNode* hijo : hijos) {
            if (hijo) {
            hijo->propagarActualizacion();
            }
        }
    }
    void AlmondNode::escalar (glm::vec3 factor){
        escalado *= factor; // Añadir el escalado
        actualizarMatriz = true;

         // Propagar la actualización a los hijos recursivamente
        for (AlmondNode* hijo : hijos) {
            if (hijo) {
            hijo->propagarActualizacion();
            }
        }
    }
    glm::vec3 AlmondNode::getTraslacion (){
        return traslacion;
    }
    glm::vec3 AlmondNode::getRotacion (){
        return rotacion;
    }
    glm::vec3 AlmondNode::getEscalado (){
        return escalado;
    }
    void AlmondNode::setMatrizTransf (glm::mat4 laMatrixTranf){
        matrixTranf = laMatrixTranf;
    }
    glm::mat4 AlmondNode::getMatrizTransf (){
        return matrixTranf;
    }

    glm::mat4 AlmondNode::calcularMatriz (){
        // Matriz identidad, para calcular la matriz desde cero
        glm::mat4 matrizAux = glm::mat4(1.0f);

        // Aplicar transformaciones en orden: Traslación -> Rotación -> Escalado
        matrizAux = glm::translate(matrizAux, traslacion);
        matrizAux = glm::rotate(matrizAux, glm::radians(rotacion.x), glm::vec3(1, 0, 0));//redondear la rotacion a 5 o 6 decimales
        matrizAux = glm::rotate(matrizAux, glm::radians(rotacion.y), glm::vec3(0, 1, 0));
        matrizAux = glm::rotate(matrizAux, glm::radians(rotacion.z), glm::vec3(0, 0, 1));
        matrizAux = glm::scale(matrizAux, escalado);

        return matrizAux;
    }

    //en la primera llamada a esta funcion se le pasara la matriz identidad glm::mat4(1.0f) ya que con esta no se acumulan transformaciones que no queremos al principio
    void AlmondNode::recorrer(glm::mat4 matrizAcum, glm::mat4 view, glm::mat4 projection, const glm::vec3& campos, std::vector<AlmondNode*> luces) {
    // Si es un tile y NO se pinta → corta aquí y no pinta nada
    if (EsunTile && !Sepinta) {
        return; // ni este nodo ni sus hijos se procesan
    }

    if (actualizarMatriz) {
        matrixTranf = matrizAcum * calcularMatriz();

        // Redondear los valores de matrixTranf a 5 decimales
        float factor = std::pow(10.0f, 5);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                matrixTranf[i][j] = std::round(matrixTranf[i][j] * factor) / factor;
            }
        }
        //pongo el bool a false, para que no vuelve a hacerlo hasta que haya cambios en las transformaciones
        actualizarMatriz = false;
    }

    // std::cout << "Matriz de Transformación:" << std::endl;
    // printMatrix(matrixTranf);

    if (entidad) {
        if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad.get())) {
        // Si el cast es exitoso
             if(entidadModelo->getCargado()== true){
                if(Sepinta == true){
                    entidad->dibujar(matrixTranf,view,projection,campos,luces);  //dibujo la entidad
                }
            }
        }
        else if(auto* entidadLuz = dynamic_cast<AlmondEntidadLuz*>(entidad.get())){
            entidad->dibujar(matrixTranf,view,projection,campos,luces);
        }else if(auto* entidadAnimacion = dynamic_cast<AlmondEntidadAnimacion *>(entidad.get())){
            if(entidadAnimacion->getCargado()== true){
                if(Sepinta == true){
                    entidad->dibujar(matrixTranf,view,projection,campos,luces);  //dibujo la entidad
                }
            }
        }
        
    }

    // Recorrer recursivamente los hijos, pasandole la matriz con las transformaciones del padre
    for (AlmondNode* hijo : hijos) {
        if (hijo) {
            hijo->recorrer(matrixTranf,view,projection,campos, luces);
        }
    }
}

// Función auxiliar para marcar la matriz de transformación como desactualizada en toda la jerarquía
void AlmondNode::propagarActualizacion() {
    actualizarMatriz = true;

    // Propagar a los hijos de manera recursiva
    for (AlmondNode* hijo : hijos) {
        if (hijo) {
        hijo->propagarActualizacion();
        }
    }
}



void AlmondNode::printMatrix(const glm::mat4& matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //std::cout << matrix[i][j] << " ";
        }
        //std::cout << std::endl;
    }
}

glm::vec3 AlmondNode::getCentroBoundingBox() const {
    if (entidad) {
        if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad.get())) {
        // Si el cast es exitoso
             if(entidadModelo->getCargado()== true){
                return entidadModelo->getCentroBoundingBox();
            }else{
                return glm::vec3(0.0f);
            }
        }
        else{
            return glm::vec3(0.0f);
        }
        
    }else{
        return glm::vec3(0.0f);
    }
}

// Getter para el tamaño del bounding box (lado del cubo)
float AlmondNode::getTamanyoBoundingBox() const {
    if (entidad) {
        if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad.get())) {
        // Si el cast es exitoso
             if(entidadModelo->getCargado()== true){
                return entidadModelo->getTamanyoBoundingBox();
            }else{
                return 0.0f;
            }
        }
        else{
            return 0.0f;
        }
        
    }else{
        return 0.0f;
    }
}

// Getter para el vértice mínimo
glm::vec3 AlmondNode::getMinVertex() const {
    if (entidad) {
        if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad.get())) {
        // Si el cast es exitoso
             if(entidadModelo->getCargado()== true){
                return entidadModelo->getMinVertex();
            }else{
                return glm::vec3(0.0f);
            }
        }
        else{
            return glm::vec3(0.0f);
        }
        
    }else{
        return glm::vec3(0.0f);
    }
}

// Getter para el vértice máximo
glm::vec3 AlmondNode::getMaxVertex() const {
    if (entidad) {
        if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad.get())) {
        // Si el cast es exitoso
             if(entidadModelo->getCargado()== true){
                return entidadModelo->getMaxVertex();
            }else{
                return glm::vec3(0.0f);
            }
        }
        else{
            return glm::vec3(0.0f);
        }
        
    }else{
        return glm::vec3(0.0f);
    }
}

void AlmondNode::setSepinta(bool ajuste){
    Sepinta = ajuste;
}
bool AlmondNode::getSepinta(){
    return Sepinta;
}

void AlmondNode::setTile(int x, int y){
    EsunTile = true;
    posTileX = x;
    posTileY = y;
}
bool AlmondNode::getTile(){
    return EsunTile;
}