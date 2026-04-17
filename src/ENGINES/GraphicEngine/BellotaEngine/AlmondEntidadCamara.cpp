#include "AlmondEntidadCamara.h"
#include "../../../utils/Const.h"


    AlmondEntidadCamara::AlmondEntidadCamara(){

    }
    AlmondEntidadCamara::~AlmondEntidadCamara(){

    }
    void AlmondEntidadCamara::dibujar(const glm::mat4& tranf, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& campos,std::vector<AlmondNode*> luces){
        
    }

    // Métodos de configuración
    void AlmondEntidadCamara::setFov(float fov){
        fovy = fov;
    }

    void AlmondEntidadCamara::setModoPerspectiva(bool perspectiva){
        esPerspectiva = perspectiva;
    }

    void AlmondEntidadCamara::setTarget(glm::vec3 targetn){
        target = targetn;
    }

    void AlmondEntidadCamara::setUp(glm::vec3 upn){
        up = upn;
    }

    void AlmondEntidadCamara::setPosicion(glm::vec3 campos){
        posicion = campos;
    }
   
    // glm::mat4 AlmondEntidadCamara::getViewMatrix(const glm::mat4& tranf) const {
    //     // Obtener la posición de la cámara a partir de la matriz de transformación del nodo
    
    //     // Generar la matriz de vista
    //     return glm::lookAt(position, target, up);
    // }

    void AlmondEntidadCamara::ConfigurarCamara(glm::vec3 targetn, glm::vec3 upn, float fov, bool pers, glm::vec3 campos){
        setFov(fov);
        setModoPerspectiva(pers);
        setTarget(targetn);
        setUp(upn);
        setPosicion(campos);
    }

    glm::vec3 AlmondEntidadCamara::getTarget(){
        return target;
    }

    glm::vec3 AlmondEntidadCamara::getUp(){
        return up;
    }

    glm::vec3 AlmondEntidadCamara::getPosicion(){
        return posicion;
    }

    float AlmondEntidadCamara::getFov(){
        return fovy;
    }

    glm::mat4 AlmondEntidadCamara::getMatrizDeVista(){
        glm::mat4 view = glm::lookAt(posicion, target, up);
        return view;
    }

    glm::mat4 AlmondEntidadCamara::getMatrizDeProjection(){
        float aspectRatio =  static_cast<float>(screenW) /  static_cast<float>(screenH);  // Ajustar esto dependiendo de la resolución de la ventana
        glm::mat4 projection = glm::perspective(glm::radians(fovy), aspectRatio, 0.1f, 100.0f);
        return projection;
    }