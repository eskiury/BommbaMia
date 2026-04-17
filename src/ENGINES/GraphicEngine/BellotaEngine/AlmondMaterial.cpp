#include "AlmondMaterial.h"
#include <fstream>
#include <sstream>
#include <iostream>


    AlmondMaterial::AlmondMaterial(){
        
    }
    AlmondMaterial::~AlmondMaterial(){
        
    }

    AlmondMaterial::AlmondMaterial(const std::string& nombre){
        this->nombre = nombre;  // Asignar el nombre al atributo protegido
        //cargarFichero(nombre);
    }

    AlmondMaterial::AlmondMaterial(const std::string& nombre, int num){
        this->nombre = nombre;  // Asignar el nombre al atributo protegido
        //cargarMaterial(nombre);
    }

    void AlmondMaterial::cargarFichero(const std::string& ruta) {
        std::ifstream archivo(ruta);
        if (!archivo.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo MTL en " << ruta << std::endl;
            return;
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
        std::istringstream iss(linea);
        std::string palabraClave;
        iss >> palabraClave;

        if (palabraClave == "newmtl") {
            iss >> nombre;  // Guardar el nombre del material
        } else if (palabraClave == "Ns") {
            iss >> shininess;
        } else if (palabraClave == "Ka") {
            iss >> ambient.x >> ambient.y >> ambient.z;
        } else if (palabraClave == "Kd") {
            iss >> diffuse.x >> diffuse.y >> diffuse.z;
        } else if (palabraClave == "Ks") {
            iss >> specular.x >> specular.y >> specular.z;
        }
    }

    //std::cout << "Material cargado correctamente: " << nombre << std::endl;
    archivo.close();

    }

    void AlmondMaterial::cargarMaterial(const std::string& ruta, glm::vec3 ambiente,glm::vec3 diffusea,glm::vec3 speculary, float shininessy, AlmondTextura* text) {
        ambient = ambiente;
        diffuse = diffusea;
        specular = speculary;
        shininess = shininessy;
        textura = text;

        // Imprimir los valores recibidos
        // std::cout << "Cargando material desde: " << ruta << std::endl;
        // std::cout << "Ambiente: (" << ambient.x << ", " << ambient.y << ", " << ambient.z << ")" << std::endl;
        // std::cout << "Diffuse: (" << diffuse.x << ", " << diffuse.y << ", " << diffuse.z << ")" << std::endl;
        // std::cout << "Specular: (" << specular.x << ", " << specular.y << ", " << specular.z << ")" << std::endl;
        // std::cout << "Shininess: " << shininess << std::endl;
    
        if (textura) {
            //std::cout << "Textura asignada correctamente." << std::endl;
        } else {
            //std::cout << "No se asignó textura (textura es nullptr)." << std::endl;
        }
    }

    void AlmondMaterial::guardarSuTextura(AlmondTextura* te){
        textura = te;
    }

    glm::vec3 AlmondMaterial::getColor(){
        return diffuse;
    }

    AlmondTextura* AlmondMaterial::getTextura(){
        return textura;
    }

    glm::vec3 AlmondMaterial::getAmbient(){
        return ambient;
    }
    glm::vec3 AlmondMaterial::getDiffuse(){
        return diffuse;
    }
    glm::vec3 AlmondMaterial::getSpecular(){
        return specular;
    }
    float AlmondMaterial::getShininess(){
        return shininess;
    }