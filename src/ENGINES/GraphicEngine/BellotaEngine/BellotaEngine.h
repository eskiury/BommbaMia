#ifndef BELLOTAENGINE_H
#define BELLOTAENGINE_H
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include "AlmondNode.h"
#include "AlmondEntidad.h"
#include "AlmondEntidadLuz.h"
#include "AlmondEntidadCamara.h"
#include "AlmondEntidadModelo.h"
#include "AlmondEntidadAnimacion.h"
#include "AlmondGestorRecursos.h"
#include "../../../utils/miVector2.h"
#include "AlmondParticula.h"

//LIBRERIAS DE OPENGL
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

/**
 * @brief BellotaEngine es la clase principal del motor de juego.
 *
 * Esta clase actúa como el núcleo central del motor, gestionando el ciclo de vida general de la aplicación.
 * Contiene y administra el árbol de escenas, permitiendo la organización jerárquica de los elementos del juego.
 * Además, integra un sistema de gestión de recursos eficiente que facilita la carga, almacenamiento y reutilización
 * de activos como texturas, shaders, materiales y modelos.
 *
 */
class BellotaEngine {
public:

    //funciones clasicas
    /** @brief Constructor por defecto */
    BellotaEngine();
    /** @brief Destructor */
    ~BellotaEngine();
    /**
    * @brief Constructor de BellotaEngine.
    * 
    * Inicializa el motor de juego, creando la ventana, configurando OpenGL, e inicializando el árbol de escenas.
    * 
    * @param w Ancho de la ventana en píxeles.
    * @param h Alto de la ventana en píxeles.
    * @param n Nombre de la ventana (título).
    */
    BellotaEngine(int const w, int const h, const std::string &n);
    /**
    * @brief Inicializa todos los componentes del motor.
    * 
    * Configura GLFW, crea la ventana, inicializa GLEW, registra callbacks y configura parámetros de OpenGL.
    * 
    * @param w Ancho de la ventana.
    * @param h Alto de la ventana.
    * @param n Nombre de la ventana.
    */
    void inicializarMotor(int const w, int const h, const std::string &n);
    /**
    * @brief Libera todos los recursos utilizados por el motor.
    * 
    * Destruye la ventana, limpia ImGui y finaliza GLFW.
    */
    void finalizarMotor();
    /**
    * @brief Cierra el ciclo de juego.
    *
    */
    bool cerrarJuego();
    /**
    * @brief Comprueba si el motor sigue en ejecución.
    * 
    * @return true si la ventana aún no ha sido cerrada, false en caso contrario.
    */
    bool motorEnFuncionamiento();
    /**
    * @brief Actualiza la pantalla y gestiona el tiempo de frame.
    * 
    * Realiza el swap de buffers, calcula el deltaTime, actualiza el framerate, y gestiona el control de FPS.
    */
    void actualizarPantalla();
    /**
    * @brief Configura parámetros básicos de OpenGL.
    * 
    * Por ejemplo, habilita el test de profundidad.
    */
    void configurarOpenGL();
    /**
    * @brief Limpia la pantalla con un color específico.
    * 
    * Establece el color de limpieza y limpia tanto el buffer de color como el de profundidad.
    * 
    * @param r Componente roja del color (0-255).
    * @param g Componente verde del color (0-255).
    * @param b Componente azul del color (0-255).
    * @param a Componente alfa del color (0-255).
    */
    void limpiarPantalla(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    /**
    * @brief Obtiene el puntero a la ventana GLFW.
    * 
    * @return GLFWwindow* Puntero a la ventana creada con GLFW.
    */
    GLFWwindow* getWindow();
    /**
    * @brief Dibuja un rectángulo básico en la pantalla con un color específico y con la camara ortográfica.
    * 
    * @param x Coordenada X de la esquina superior izquierda del rectángulo.
    * @param y Coordenada Y de la esquina superior izquierda del rectángulo.
    * @param width Ancho del rectángulo.
    * @param height Alto del rectángulo.
    * @param r Componente roja del color (0–255).
    * @param g Componente verde del color (0–255).
    * @param b Componente azul del color (0–255).
    * @param a Componente alfa del color (0–255).
    */
    void dibujarRectanguloConColor(float x, float y, float width, float height, float r, float g, float b, float a);//dibuja un rectangulo de la forma mas basica
    /**
    * @brief Pinta un sprite en pantalla con textura, posición, escala, rotación y color, en esta funcion hago el calculo de proyeccion de la camara ortográfica.
    * 
    * @param nombre Nombre o identificador de la textura en el gestor.
    * @param x Posición X del sprite.
    * @param y Posición Y del sprite.
    * @param escala Escala aplicada al sprite.
    * @param rotacion Rotación en grados (en el eje Z).
    * @param r Componente roja del color (0–255).
    * @param g Componente verde del color (0–255).
    * @param b Componente azul del color (0–255).
    * @param a Componente alfa del color (0–255).
    */
    void PintarSpriteTextura(const std::string &nombre, float x, float y, float escala, float rotacion,float r, float g, float b, float a);
    /**
    * @brief Crea el árbol jerárquico de nodos de juego inicial.
    * 
    * Esta funcion crea la estructura basica del arbol de escenas, el nodo raiz del que salen los nodos para los objetos dinamicos(bombas,personajes,objetos destruibles,etc...)
    * tambien tiene los nodos de los tiles que contienen los objetos estaticos como el mapa o las decoraciones, hay cuatro nodos de tiles, por lo que el mapa se divide en 4 zonas
    */
    void CrearArbolDeJuego();
    /**
    * @brief Carga un bloque del mapa en una zona específica basado en su posición.Ademas el nodo se añade al tile en el que este segun su posicion.
    * 
    * @param x Posición X del bloque.
    * @param y Posición Y del bloque.
    * @param z Posición Z del bloque.
    * @param r Componente roja del color del bloque.
    * @param g Componente verde del color del bloque.
    * @param b Componente azul del color del bloque.
    * @param a Componente alfa del color del bloque.
    */
    void CargarMapa(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    /**
    * @brief Recorre el árbol de nodos del juego, aplicando renderizado y lógica.
    * 
    * Este método también se encarga de limpiar nodos de pintura y rastros antiguos tras el frame.
    */
    void RecorrerArbolDeJuego();
    /**
    * @brief Crea una cámara en el juego con los parámetros dados.
    * 
    * @param targetn Punto al que la cámara mira.
    * @param upn Vector "up" de la cámara.
    * @param fov Ángulo de campo de visión.
    * @param pers Define si es una cámara perspectiva (true) u ortográfica (false).
    * @param campos Posición inicial de la cámara.
    */
    void CrearCamara(glm::vec3 targetn, glm::vec3 upn, float fov, bool pers, glm::vec3 campos);
    /**
    * @brief Obtiene el tiempo acumulado desde el inicio del juego o motor.
    * 
    * @return Tiempo acumulado en segundos como número de punto flotante.
    */
    double getTiempoAcumulado();
    /**
    * @brief Establece el número de FPS objetivo.
    * 
    * @param fps La cantidad de fotogramas por segundo deseada.
    */
    void SetTargetFPS(int fps); // Función para establecer el FPS objetivo
    /**
    * @brief Devuelve los fps a los que corre el juego
    * 
    * @return los fps a los que funciona el juego
    */
    int GetFPS();
    /**
    * @brief Modifica los parámetros de la cámara principal.
    * 
    * Esta función configura la cámara de la escena, incluyendo la posición del objetivo,
    * el vector de orientación hacia arriba, el campo de visión y el modo de proyección.
    * 
    * @param targetn Vector que indica hacia dónde apunta la cámara.
    * @param upn Vector que indica la orientación "arriba" de la cámara.
    * @param fov Campo de visión en grados.
    * @param pers Si es true, usa proyección en perspectiva; si es false, ortográfica.
    * @param campos Posición de la cámara en el espacio.
    */
    void ModificarCamara(glm::vec3 targetn, glm::vec3 upn, float fov, bool pers, glm::vec3 campos);
    /**
    * @brief Dibuja un cubo con las dimensiones, posición y color especificados DEBUGGER.
    * 
    * Esta función crea un cubo 3D en la escena, utilizando shaders y buffers OpenGL.Esta funcion se utiliza exclusivamente para el debugger de fisicas
    * El color admite canal alfa (transparencia).
    * 
    * @param x Posición X del cubo.
    * @param y Posición Y del cubo.
    * @param z Posición Z del cubo.
    * @param width Ancho del cubo.
    * @param height Alto del cubo.
    * @param length Largo del cubo.
    * @param r Componente roja del color (0-255).
    * @param g Componente verde del color (0-255).
    * @param b Componente azul del color (0-255).
    * @param a Componente alfa del color (0-255, para transparencia).
    */
    void DibujarCubos(float x, float y, float z, double width, double height, double length, float r, float g, float b, float a);
    /**
    * @brief Obtiene el ancho y alto de la textura que le pasemos por nombre.
    * 
    * @return dos floats que van dentro de la clase MiVector2 creada por nosotros.
    */
    MiVector2 getWH(const std::string &nombre);
    /**
    * @brief Carga y posiciona una decoración 3D en el mapa del juego.Las carga en el tile dependiendo de la zona que le marquemos, al haber 4 tiles hay cuatro zonas posibles
    * 
    * La decoración se carga como una entidad de modelo y se inserta en una zona específica del mapa.
    * 
    * @param x Posición X donde se colocará la decoración.
    * @param y Posición Y donde se colocará la decoración.
    * @param z Posición Z donde se colocará la decoración.
    * @param nombre Ruta o identificador del modelo a cargar.
    * @param rota Rotación en grados sobre el eje Y.
    * @param zona Zona del mapa donde se colocará (0 a 3; otros valores irán por defecto a la zona 1).Con zona se refiere a tiles
    */
    void CargarDecoraciones(float x, float y, float z, const std::string &nombre, float rota, int zona);
    /**
    * @brief Funcion para el dibujado de textos
    * 
    *Esta funcion en principio era para pintar varios textos, pero por falta de tiempo no se hizo y se reutilizo para pintar el tiempo del juego recibiendo el numero como texto
    *
    * @param texto texto de numero a pintar del tiempo
    * @param x Posición X donde se colocará.
    * @param y Posición Y donde se colocará.
    * @return dos floats que van dentro de la clase MiVector2 creada por nosotros.
    */
    void DibujarTextosPropios(std::string texto, int x, int y);
    /**
    * @brief Carga y posiciona un powerup 3D en el mapa del juego.Las carga como objeto dinamico en la rama de powerups
    * 
    * @param x Posición X donde se colocará la decoración.
    * @param y Posición Y donde se colocará la decoración.
    * @param z Posición Z donde se colocará la decoración.
    * @param rota Rotación del modelo.
    * @param nombre Ruta o identificador del modelo a cargar.
    */
    void CargarPW(float x, float y, float z,float rota, const std::string &nombre);
    /**
    * @brief Precarga los recursos de una malla 3D, esta funcion solicita los recursos al gestor para que este los cargue en memoria y asi poder usarlo para la pantalla de carga.
    * 
    * @param nombre Nombre o ruta del recurso de la malla a precargar.
    */
    void PrecargarRecursosMalla(const std::string &nombre);
    /**
    * @brief Activa o desactiva el VSync (sincronización vertical).
    * 
    * @param enabled true para activar VSync, false para desactivarlo.
    */
    void SetVSync(bool enabled);
    /**
    * @brief Consulta si el VSync está activo.
    * 
    * @return true si VSync está activado, false en caso contrario.
    */
    bool IsVSyncActivo() const;
    /**
    * @brief Carga un personaje en el arbol de escenas con su entidadModelo que pertenece al nodo y devuelve el nodo que lo contiene para poder hacer operaciones como rotacion o traslacion desde la fachada del motor.
    * 
    * Este método crea un nuevo nodo, lo posiciona en el espacio y carga el modelo 3D
    * 
    * @param x Posición X donde se ubicará el personaje.
    * @param y Posición Y donde se ubicará el personaje.
    * @param z Posición Z donde se ubicará el personaje.
    * @param nombre Ruta o identificador del modelo del personaje a cargar.
    * @return AlmondNode* Puntero al nodo que contiene el personaje cargado.
    */
    AlmondNode* CargarPersonajesYDevuelve(float x, float y, float z, const std::string &nombre);
    /**
    * @brief Elimina un nodo de la escena ademas usa las funciones de la clase nodo y elimina los hijos de este nodo tambien.
    * 
    * Este método remueve el nodo del árbol de escena y libera la memoria asociada ademas de la de sus hijos
    * 
    * @param aborrar Puntero al nodo del personaje que se desea eliminar.
    */
    void EliminarPersonaje(AlmondNode* aborrar);
    /**
    * @brief Elimina las camaras, luces, nodos y despues inicializa otra vez el arbol con la estructura basica
    */
    void Reset();
    /**
    * @brief Carga el modelo de pintura ,a la rama de pintura del arbol
    * 
    * Este método crea un nuevo nodo, lo posiciona en el espacio y carga el modelo 3D
    * 
    * @param x Posición X donde se ubicará el personaje.
    * @param y Posición Y donde se ubicará el personaje.
    * @param z Posición Z donde se ubicará el personaje.
    * @param nombre Ruta o identificador del modelo del personaje a cargar.
    */
    void CargarPintura(float x, float y, float z, const std::string &nombre);
    /**
    * @brief Carga una bomba en el arbol de escenas y devuelve el nodo que lo contiene para poder hacer operaciones como rotacion o traslacion desde la fachada del motor.
    * 
    * Este método crea un nuevo nodo, lo posiciona en el espacio y carga el modelo 3D
    * 
    * @param x Posición X donde se ubicará el personaje.
    * @param y Posición Y donde se ubicará el personaje.
    * @param z Posición Z donde se ubicará el personaje.
    * @param nombre Ruta o identificador del modelo de bomba a cargar.
    * @return AlmondNode* Puntero al nodo que contiene la bomba cargada.
    */
    AlmondNode* CargarBombasYDevuelve(float x, float y, float z, const std::string &nombre);
     /**
    * @brief Carga el modelo de explosion ,a la rama de rastroexplosiones del arbol
    * 
    * Este método crea un nuevo nodo, lo posiciona en el espacio y carga el modelo 3D.
    * 
    * @param padreBomba Nodo del padre de la bomba.
    * @param x Posición X donde se ubicará el personaje.
    * @param y Posición Y donde se ubicará el personaje.
    * @param z Posición Z donde se ubicará el personaje.
    * @param nombre Ruta o identificador del modelo del personaje a cargar.
    */
    void AnyadirBombaRastro(AlmondNode* padreBomba, float x, float y, float z, const std::string &nombre);
    //funciones para frames
    /**
    * @brief Guarda el tiempo de inicio del frame actual y calcula deltaTime y FPS.
    */
    void GuardarInicioDeFrame();
    /**
    * @brief Crea una luz en la escena con las propiedades indicadas.
    * 
    * @param tipoLuz Tipo de luz: 0 = direccional, 1 = puntual, 2 = spot.
    * @param posicion Posición de la luz en el espacio.
    * @param direccion Dirección hacia la que apunta la luz.
    * @param color Color de la luz.
    * @param ambStrength Intensidad de la luz ambiental.
    * @param diffStrength Intensidad de la luz difusa.
    * @param specStrength Intensidad de la luz especular.
    * @param atenCte Atenuación constante.
    * @param atenLineal Atenuación lineal.
    * @param atenCuadrat Atenuación cuadrática.
    * @param apertura Ángulo de apertura para luces spot.
    * @param penumbra Ángulo de penumbra para luces spot.
    */
    void CrearLuz(int tipoLuz,glm::vec3 posicion,glm::vec3 direccion,glm::vec3 color,float ambStrength,float diffStrength,float specStrength,
        float atenCte,float atenLineal,float atenCuadrat,float apertura,float penumbra);
    /**
    * @brief Activa o desactiva el uso de tiles en la escena.Esto se hace porque cuando el mapa del juego se ve completamente nos podemos ahorrar el calculo de comprobacion de si el tile esta activo
    * 
    * @param activo true para activar tiles, false para desactivar.
    */
    void setTilesActivos(bool);
    /**
    * @brief Crea una luz tipo foco para el jugador y devuelve el nodo creado.Es igual a la funcion de CrearLuz, lo unico que esta devuelve el nodo de la luz, para modificar la posicion de esta desde la fachada
    * 
    * @param tipoLuz Tipo de luz: 0 = direccional, 1 = puntual, 2 = spot.
    * @param posicion Posición de la luz.
    * @param direccion Dirección hacia la que apunta la luz.
    * @param color Color de la luz.
    * @param ambStrength Intensidad ambiental.
    * @param diffStrength Intensidad difusa.
    * @param specStrength Intensidad especular.
    * @param atenCte Atenuación constante.
    * @param atenLineal Atenuación lineal.
    * @param atenCuadrat Atenuación cuadrática.
    * @param apertura Ángulo apertura para spot.
    * @param penumbra Ángulo penumbra para spot.
    * @return AlmondNode* Nodo que contiene la luz creada.
    */
    AlmondNode* CrearLuzFocoPlayer(int tipoLuz,glm::vec3 posicion,glm::vec3 direccion,glm::vec3 color,float ambStrength,float diffStrength,float specStrength,
        float atenCte,float atenLineal,float atenCuadrat,float apertura,float penumbra);
    /**
    * @brief Dibuja una partícula en pantalla.Llama a la funcion de dibujar de la clase particula.
    * 
    * @param pasada Referencia a la partícula que se debe pintar.
    */
    void pintarParticula(AlmondParticula& pasada);

    //----------------------------------animaciones---------------------------------
    /**
    * @brief Carga animaciones en un nodo nuevo y lo devuelve para usarlo.Esta funcion es igual a la de CargarPersonajesYDevuelve, lo unico que esta almacena una entidad de tipo animacion y no de tipo modelo
    * 
    * Esta función crea un nodo en la escena, lo posiciona, le asigna una entidad
    * de animación, carga las animaciones indicadas y comienza a reproducirlas.
    * 
    * @param x Posición X donde colocar el nodo.
    * @param y Posición Y donde colocar el nodo.
    * @param z Posición Z donde colocar el nodo.
    * @param nombreAnimacion Nombre identificador de la animación principal.
    * @param rutasAnimacion Vector con las rutas a los archivos de animación.
    * @return AlmondNode* Puntero al nodo creado que contiene la animación.
    */
    AlmondNode* CargarAnimacionesYDevuelve(float x, float y, float z,const std::string &nombreAnimacion, const std::vector<std::string> &rutasAnimacion);
    //----------------------------------animaciones---------------------------------

    //variables
    /**
    * @brief Puntero a la ventana GLFW que muestra el juego.
    */
    GLFWwindow* window; //puntero a la ventana que sera la que muestre el juego
    //creo el gestor de recursos
    /**
    * @brief Gestor de recursos para cargar y manejar assets como modelos, texturas, etc.
    */
    AlmondGestorRecursos gestor;  // Crear el gestor de recursos
    /**
    * @brief Árbol de nodos que representa la jerarquía de objetos del juego.
    */
    std::vector<std::unique_ptr<AlmondNode>> nodosJuego;    //arbol del juego
    /**
    * @brief Lista de nodos que representan las cámaras activas en la escena.
    */
    std::vector<AlmondNode*> camaras;
    /**
    * @brief Lista de nodos que representan las luces activas en la escena.
    */
    std::vector<AlmondNode*> luces;

    //para los fotogramas
    /**
    * @brief Tiempo que debe durar cada frame (por ejemplo, para 60 FPS es 1/60).
    */
    double tiempoPorFrame = 1.0 / 60.0; // se actualiza con SetTargetFPS
    /**
    * @brief Marca el inicio del frame actual.
    */
    double tiempoDeInicio = 0.0;        // marca el inicio del frame
    /**
    * @brief Duración del frame actual (delta time).
    */
    double deltaTime = 0.0;             // duración del frame actual
    /**
    * @brief FPS calculado para el frame actual.
    */
    double fpsActual = 0.0;             // FPS calculado real
    /**
    * @brief Tiempo acumulado desde el último update.
    */
    double acumulador = 0.0;      // tiempo acumulado desde el último update
    /**
    * @brief Tiempo del frame anterior.
    */
    double tiempoAnterior = 0.0;  // tiempo del frame anterior
    /**
    * @brief FPS real calculado (puede diferir del objetivo).
    */
    double fpsReal = 0.0; // FPS real


private:

/**
 * @brief Indica si el VSync está activo o no.
 */
bool vsyncActivo = false;
/**
 * @brief Indica si los tiles están activos o no.
 */
bool tilesActivos = false;
    
};

#endif