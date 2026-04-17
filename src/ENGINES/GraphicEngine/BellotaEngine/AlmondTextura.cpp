#include "AlmondTextura.h"
//usamos stb image loader C++ para cargar las texturas, segun OpenGL admite solo los .JPG
//patastb solo se hace una vez
#define STB_IMAGE_IMPLEMENTATION
#include "Librerias/stb_image.h"


    AlmondTextura::AlmondTextura(){
        
    }
    AlmondTextura::~AlmondTextura(){
        
    }

    AlmondTextura::AlmondTextura(const std::string& nombre){
        this->nombre = nombre;  // Asignar el nombre al atributo protegido
        cargarFichero(nombre);
    }

    void AlmondTextura::cargarFichero(const std::string& ruta) {
        //carga la imagen y guarda el with, height y el numero de colores del canal
        

        //genera el id que se refiere a la textura y lo guardo en id
        //primero toma como entrada cuántas texturas queremos generar y las almacena en una matriz int sin signo dada como segundo argumento, en nuestro caso usamos solo 1, por lo que no usamos matriz
        glGenTextures(1, &id);
        //lo vinculamos a opengl
        glBindTexture(GL_TEXTURE_2D, id);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Controlan cómo OpenGL maneja las coordenadas de la textura fuera del rango [0,1].
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //La textura se repetirá cuando las coordenadas sean mayores a 1.0 o menores a 0.0.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Usa interpolación lineal con mipmaps al reducir la textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Usa interpolación lineal al aumentar la textura.
        // load and generate the texture
        unsigned char *data = stbi_load(ruta.c_str(), &width, &height, &nrChannels, 0); //Usamos ruta.c_str() para convertir std::string a const char*, que es lo que stbi_load() espera.
        if (data)
        {
            GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB; // Detectar el formato automáticamente
            //Envía la textura a la GPU.
            glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
            //Genera mipmaps automáticamente.
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            //std::cout << "No se pudo cargar la textura" << std::endl;
        }
        //Libera la memoria usada por stbi_load(), ya que la imagen ya está en la GPU.
        stbi_image_free(data);
    }

    unsigned int AlmondTextura::getId(){
        return id;
    }

    int AlmondTextura::getWidth(){
        return width;
    }

    int AlmondTextura::getHeight(){
        return height;
    }