#include "AlmondShader.h"
#include <fstream>
#include <sstream>
#include <iostream>
//SEPARAR FRAGMENT SHADERS Y VERTEX SHADERS
    AlmondShader::AlmondShader(){
        
    }
    AlmondShader::~AlmondShader(){
        glDeleteShader(shaderProgram);
    }

    AlmondShader::AlmondShader(const std::string& nombre){
        this->nombre = nombre;  // Asignar el nombre al atributo protegido
        cargarFichero(nombre);
    }

    void AlmondShader::cargarFichero(const std::string& ruta) {
        std::string stringShaderCode;   //variables para guardar el codigo que leeamos del archivo
    
        // Llama a la función para leer el archivo y extraer los shaders
        cargarShadersDesdeArchivo(ruta, stringShaderCode);
        
        // Compila los shaders y crea el shader program
        shaderProgram = compilarShaders(stringShaderCode, tipo);
   
    }

    // Método para activar el shader
    void AlmondShader::usar() {
        glUseProgram(shaderProgram);
    }


    void AlmondShader::cargarShadersDesdeArchivo(const std::string& rutaArchivo, std::string& stringShaderCode) {
        std::ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) { //sino se puede abrir el archivo devuelvo error
            std::cerr << "Error al abrir el archivo de shaders: " << rutaArchivo << std::endl;
            return;
        }

        std::string linea;
        std::string* shaderActual = nullptr;    //para saber si estoy en el vertexShader o en el fragmentShader

        while (std::getline(archivo, linea)) {
            if (linea == "[VERTEX]") {
                tipo = AlmondShaderType::VERTEX;
                shaderActual = &stringShaderCode; // A partir de ahora, guarda en vertexShaderCode
            } else if (linea == "[FRAGMENT]") {
                tipo = AlmondShaderType::FRAGMENT;
                shaderActual = &stringShaderCode; // Ahora guarda en fragmentShaderCode
            } else if (shaderActual) {
                *shaderActual += linea + "\n"; // Guarda la línea en el shader correspondiente
            }
        }

        archivo.close();    //cierro el archivo
    }


    // Método para compilar y enlazar los shaders
    GLuint AlmondShader::compilarShaders(const std::string& shaderCode, AlmondShaderType tipo) {
        if(tipo == AlmondShaderType::VERTEX){
            shaderProgram = compilarShader(shaderCode, GL_VERTEX_SHADER); //le paso la cadena que hay guardada en el vertexcode
        }
        if(tipo == AlmondShaderType::FRAGMENT){
            shaderProgram = compilarShader(shaderCode, GL_FRAGMENT_SHADER);   //le paso la cadena que hay guardada en el fragmentCode
        }

        return shaderProgram;
    }


    GLuint AlmondShader::compilarShader(const std::string& codigoFuente, GLenum tipoShader) { //esta funcion crea los shaders
        GLuint shader = glCreateShader(tipoShader);
        const char* codigoCStr = codigoFuente.c_str();  // Convertir a formato C-string
        glShaderSource(shader, 1, &codigoCStr, NULL);
        glCompileShader(shader);

        // Verificar errores de compilación
        GLint exito;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &exito);
        if (!exito) {
            GLchar logInfo[512];
            glGetShaderInfoLog(shader, 512, NULL, logInfo);
            std::cerr << "Error compilando el " 
                    << (tipoShader == GL_VERTEX_SHADER ? "Vertex" : "Fragment") 
                    << " Shader: " << logInfo << std::endl;
        }

        return shader;
    }

    GLuint AlmondShader::getShader(){
        return shaderProgram;
    }
