#ifndef MOTORRAYLIB_H
#define MOTORRAYLIB_H

//#include "raylib.h"                       // Biblioteca gráfica Raylib
#include <stdexcept>                      // Manejo de excepciones
#include <string>                         // Uso de cadenas de texto
#include "../../AlmondGE/EntityManager.h" // Gestión de entidades del juego
#include "../../utils/miVector3.h"
#include "../../utils/CronoManager.h"
#include "../../Sys/MapSystem.h"
#include "../../Sys/FisicaSystem.h"
#include <vector>
#include "BellotaEngine/BellotaEngine.h"

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "AlmondParticula.h"

extern bool enableMoveOrDie;
extern bool PausarJuego;

/**
 * @brief ElMotorRaylib es la clase fachada entre el motor grafico y nuestro juego
 *
 * Clase singleton que encapsula las funciones principales del motor gráfico.
 *  Se encarga de inicializar lel motor grafico
 *
 */
class ElMotorRaylib
{
public:
    
    /**
    * @brief Método estático para inicializar una instancia única de la fachada del motor gráfico.
    * 
    * @param w Ancho de la ventana.
    * @param h Alto de la ventana.
    * @param n Nombre de la ventana.
    * @return Referencia a la instancia única de la fachada del motor gráfico.
    * 
    * @throws std::runtime_error Si ya existe una instancia del motor.
    */
    static ElMotorRaylib &CrearMotor(int const w, int const h, const std::string &n);

    /**
    * @brief Destructor que cierra la ventana y libera los recursos asociados al motor gráfico.
    */
    ~ElMotorRaylib();

   /**
    * @brief Verifica si la ventana está abierta.
    * 
    * @return true Si la ventana sigue abierta.
    * @return false Si la ventana ha sido cerrada.
    */
    bool run();

    /**
    * @brief Cierra el motor gráfico y libera los recursos asociados.
    */
    void closeMotor();

    /**
    * @brief Inicia el proceso de renderizado de una escena y limpia el fondo.
    */
    void empezarEscena();
    /**
    * @brief Finaliza el proceso de renderizado de una escena.
    */
    void acabarEscena();
    /**
    * @brief Inicializa y añade las entidades a dibujar al motor grafico (jugador, bomba, bloque destructible etc...).
    * 
    * @param em EntityManager que gestiona las entidades.
    * @param e Referencia constante a la entidad a renderizar.
    * @param physics Referencia al sistema de físicas utilizado para posicionamiento.
    */
    void dibujaEntidad(EntityManager &em, Entity const &e, const FisicaSystem &physics);
    /**
    * @brief Carga el mapa, bloques no destruibles al arbol de escenas
    * 
    * @param mapa Matriz bidimensional que representa el estado del mapa.
    */
    void dibujaMapa(const std::vector<std::vector<int>> &);
    /**
    * @brief Carga al arbol de escenas los bloques destruibles
    * 
    * @param em EntityManager que gestiona las entidades de los bloques destruibles.
    * @param mapa Matriz del mapa que contiene la información de bloques destruibles.
    */
    void dibujaBloquesDestruibles(EntityManager &em, const std::vector<std::vector<int>> &);
    /**
    * @brief Renderiza el HUD del juego, incluyendo un contador de tiempo y otros elementos gráficos.
    */
    void dibujaHud(float r, EntityManager &em,int contP1,int contP2,int contP3,int contP4);
    /**
    * @brief Renderiza el HUD del juego en modo pintura, incluyendo un contador de tiempo y otros elementos gráficos.
    */
    void dibujaHudPintura(float r, EntityManager &em,int contP1,int contP2,int contP3,int contP4, int bloquesTotales);
    /**
    * @brief Inicializa todos los elementos del juego, incluyendo mapa, entidades, HUD y efectos visuales. Actualiza los que ya estan cargados y recorrer el arbol de escenas.
    * 
    * @param em EntityManager con las entidades del juego.
    * @param seleccionActual Elemento seleccionado en el menú de power-ups.
    * @param tiempo Tiempo total de juego.
    * @param mapMatrix Referencia al sistema del mapa.
    * @param contador Contador para los 3 segundos iniciales.
    * @param physics Sistema de físicas utilizado.
    * @param perdido Indica si se ha perdido la partida.
    * @param animacion Referencia al estado de la animación de entrada/salida.
    * @param pintura Indica si está activo el modo pintura.
    * @param contP1 Contador de victorias del jugador 1.
    * @param contP2 Contador de victorias del jugador 2.
    * @param contP3 Contador de victorias del jugador 3.
    * @param contP4 Contador de victorias del jugador 4.
    */
    void gameRender(EntityManager &em, int seleccionActual, float, MapSystem &mapMatrix, int, const FisicaSystem &physics, bool perdido, bool &animacion, bool pintura,int contP1,int contP2,int contP3,int contP4);

    /**
    * @brief Renderiza el menú principal del juego.
    * 
    * @param em EntityManager con las entidades del menú.
    * @param physics Sistema de físicas para posicionamiento de elementos.
    */
    void menuRender(EntityManager &em,  const FisicaSystem &physics);
    /**
    * @brief Inicializa el motor gráfico, configurando la cámara y los FPS.
    */
    void iniciarMotor();
    /**
    * @brief Configura los parámetros de la cámara (posición, objetivo, proyección), que son pasados al motor grafico y este se ocupa de crearla.
    */
    void configurarCamara();

    /**
    * @brief Dibuja un cubo en la escena.Esta funcion se usa para el debugger de fisicas
    * 
    * @param position Posición del cubo.
    * @param width Ancho del cubo.
    * @param height Altura del cubo.
    * @param length Longitud del cubo.
    * @param color Color del cubo.
    */
    void DibujarCubo(MiVector3 position, double width, double height, double length, MiColor color);
    /**
    * @brief Dibuja texto en pantalla.Esta funcion se usaba con raylib
    * 
    * @param texto Texto a mostrar.
    * @param x Posición horizontal.
    * @param y Posición vertical.
    * @param tamanyoFuente Tamaño de fuente.
    * @param color Color del texto.
    */
    void DibujarTextos(std::string texto, int x, int y, int tamanyoFuente, MiColor color);
    /**
    * @brief Dibuja un rectángulo en pantalla.
    * 
    * @param posX Posición X.
    * @param posY Posición Y.
    * @param width Ancho del rectángulo.
    * @param height Alto del rectángulo.
    * @param color Color del rectángulo.
    */
    void DibujarRectangulo(int posX, int posY, int width, int height, MiColor color);
    /**
    * @brief Renderiza el tiempo restante del juego en pantalla.
    * 
    * @param tiempo Tiempo restante a mostrar.
    */
    void PintaTiempoJuego(float);
    /**
    * @brief Obtiene la altura de la pantalla.
    * 
    * @return int Altura en píxeles.
    */
    int GetAlturaPantalla();
    /**
    * @brief Obtiene el ancho de la pantalla.
    * 
    * @return int Ancho en píxeles.
    */
    int GetAnchuraPantalla();
    /**
    * @brief Renderiza la pantalla de introducción del juego.
    * 
    * @param alpha Transparencia para efectos visuales.
    * @param mostrarPantallasCarga Indica si se deben mostrar las pantallas de carga.
    */
    void introRender(float, bool);
    /**
    * @brief Renderiza el menú de opciones.
    * 
    * @param em EntityManager con elementos interactivos.
    * @param physics Sistema de físicas para posicionamiento.
    */
    void opcionesRender(EntityManager &em, const FisicaSystem &physics);
    /**
    * @brief Controla y dibuja la cuenta regresiva inicial (3, 2, 1...).
    * 
    * @param tiempo Tiempo transcurrido desde el inicio.
    * @param animacionAcabada Indica si la animación ha finalizado.
    */
    void dibujaTiempoInicial(int, bool);
    /**
    * @brief Dibuja la barra del modo Move or Die para el jugador.
    * 
    * @param em EntityManager para acceder al jugador.
    */
    void dibujaBarraMoveOrDie(EntityManager &em);
    /**
    * @brief Activa o desactiva el pintado del debugger (se activa al presionar la tecla Q).
    */
    void ActivaPintadoDebbuger();
    /**
    * @brief Renderiza los ajustes de música.
    * 
    * @param value Valor actual del control deslizante de volumen.
    * @return float Nuevo valor del volumen ajustado.
    */
    float ajustesRenderMusica(float value);
    /**
    * @brief Renderiza los ajustes de efectos de sonido (SFX).
    * 
    * @param value Valor actual del control deslizante de SFX.
    * @return float Nuevo valor ajustado.
    */
    float ajustesRenderSFX(float value);
    /**
    * @brief Renderiza la pantalla de game over.
    * 
    * @param perdido Indica si el jugador ha perdido.
    */
    void dibujaGameover(bool perdido);
    /**
    * @brief Ejecuta la animación de inicio del juego.Modifica los valores de la camara
    * 
    * @param animacion Referencia al estado de animación en curso.
    */
    void animacionInicio(bool &animacion);
    /**
    * @brief Obtiene la posición actual del mouse y hace calculos segun el tamaño de la pantalla.
    * 
    * @return MiVector2 Posición del mouse en coordenadas de pantalla.
    */
    MiVector2 ObtenerPosicionMouse();
    /**
    * @brief Verifica si un botón del mouse está siendo presionado.
    * 
    * @param boton Código del botón del mouse.
    * @return true Si el botón está presionado.
    * @return false En caso contrario.
    */
    bool BotonMousePresionado(int boton);
    /**
    * @brief Verifica si un botón del mouse fue soltado.
    * 
    * @param boton Código del botón del mouse.
    * @return true Si el botón fue soltado.
    * @return false En caso contrario.
    */
    bool BotonMouseSoltado(int boton);
    /**
    * @brief Dibuja una textura 2D con nombre específico.Pinta sprites como en el hud
    * 
    * @param nombre Nombre del recurso de la textura.
    * @param x Posición X.
    * @param y Posición Y.
    * @param escala Escala de la textura (por defecto 1.0).
    * @param rotacion Ángulo de rotación (por defecto 0.0).
    * @param color Color de la textura.
    */
    void DibujarTextura(const std::string &nombre, float x, float y, float escala = 1.0f, float rotacion = 0.0f, MiColor color = {255, 255, 255, 255});
    /**
    * @brief Pinta los tiles del mapa en el modo pintura, carga los modelos al arbol dependiendo del mapa.
    * 
    * @param mapMatrix Sistema de mapa con la información de tiles.
    */
    void pintarCuadrados(MapSystem &mapMatrix);
    /**
    * @brief Devuelve una referencia al motor de juego (BellotaEngine).
    */
    BellotaEngine& getMotorDeJuego();
    /**
    * @brief Carga las decoraciones del entorno de juego de modo pintura al arbol de escenas del motor.
    */
    void pintarDecoraciones();
    /**
    * @brief Carga las decoraciones del entorno de juego de modo clasico al arbol de escenas del motor.
    */
    void pintarDecoracionesClasico();
    /**
    * @brief Verifica si el mouse está sobre una región rectangular.
    * 
    * @param x Posición X del área.
    * @param y Posición Y del área.
    * @param width Ancho del área.
    * @param height Alto del área.
    * @param nombre Nombre del elemento asociado (por ejemplo, botón).
    * @return true Si el mouse está sobre el área.
    */
    bool IsMouseOver(float x, float y, float width, float height, const std::string &nombre);
    /**
    * @brief Renderiza la pantalla de victoria.
    */
    void winRender(EntityManager &em, const FisicaSystem &physics);
    /**
    * @brief Renderiza la pantalla de derrota.
    */
    void gameOverRender(EntityManager &em, const FisicaSystem &physics);
    /**
    * @brief Renderiza el menú de controles del juego.
    */
    void controlesRender(EntityManager &em, const FisicaSystem &physics);
    /**
    * @brief Muestra las instrucciones con animación.
    * 
    * @param ani Estado de animación.
    * @param imagen Índice de la imagen de instrucciones.
    */
    void mostrarInstrucciones(bool& ani, const int imagen);
    /**
    * @brief Reinicia la animación de inicio de partida
    */
    void resetAnimacion();
    /**
    * @brief Reinicia la variable que indica si hay que volver a cargar el mapa al arbol de escenas del motor
    */
    void ResetDibujadoDeMapa();

//------------------------METODOS SELECCION--------------------------

    /**
    * @brief Renderiza la pantalla de selección de personajes.
    */
   void SeleccionRender(EntityManager &em, const FisicaSystem &physics);
    /**
    * @brief Inicializa la cámara y la crea en el motor grafico.
    */
    void initCamara();
    /**
    * @brief Método que cambia una variable de un nodo del arbol de escenas para indicarle que no pinte todos los players y npcs y sus hijos
    */
    void noDibujaEntidad(EntityManager &em, Entity const &e, const FisicaSystem &physics);
    /**
    * @brief Método que cambia una variable de un nodo del arbol de escenas para indicarle que pinte todos los players y sus hijos
    */
    void pintaALosPersonajes(EntityManager &em, const FisicaSystem &physics,int nu);
     /**
    * @brief Método que cambia una variable de un nodo del arbol de escenas para indicarle que pinte esa entidad y sus hijos
    */
    void activaDibujado(EntityManager &em, Entity const &e, const FisicaSystem &physics);
     /**
    * @brief Método que cambia una variable de un nodo del arbol de escenas para indicarle que no pinte esa entidad y sus hijos
    */
    void desactivaDibujado(EntityManager &em, Entity const &e, const FisicaSystem &physics);
    /**
    * @brief Método que cambia una variable de un nodo del arbol de escenas para indicarle que no pinte todos los players y sus hijos
    */
    void despintaALosPersonajes(EntityManager &em, const FisicaSystem &physics, int nu);

//-------------------------------------------------------------------


//----------------------CRONOMANAGER-------------------//
    /**
    * @brief Inicia el cronómetro.Para controlar el bucle de juego y fps
    */
    void iniciarCrono();
    /**
    * @brief Verifica si el sistema puede actualizarse.
    */
    bool canUpdate();
    /**
    * @brief Verifica si el sistema puede renderizarse.
    */
    bool canRender();
    /**
    * @brief Devuelve el tiempo transcurrido entre frames.
    */
    double getDeltaTime();
    /**
    * @brief Devuelve los FPS actuales.
    */
    int getCurrentFPS();
    /**
    * @brief Actualiza el cronómetro interno.
    */
    void tick();
    /**
    * @brief Devuelve el deltaTime fijo para cálculos constantes.
    */
    double getFixedDeltaTime() const;
//-----------------------------------------------------//

    /**
    * @brief Configura las luces que usamos y las crea en el motor grafico en la escena.
    */
    void ConfigurarLuces();
    /**
    * @brief Inicializa el sistema de partículas.Crea todas las particulas y las almacena en un vector de particulas
    */
    void crearParticulasGame();
    /**
    * @brief Inicializa las partículas que indiquemos como particulas de humo.
    * @param origen Posición donde se generaran las particulas.
    * @param cantidad Cantidad de particulas a generar.
    */
    void generarHumo(glm::vec3 origen, int cantidad);
    /**
    * @brief Inicializa las partículas que indiquemos como particulas de explosion.
    * @param origen Posición donde se generaran las particulas.
    * @param cantidad Cantidad de particulas a generar.
    */
    void generarExplosion(glm::vec3 origen, int cantidad);
    /**
    * @brief Inicializa las partículas que indiquemos como particulas de chispas.
    * @param origen Posición donde se generaran las particulas.
    * @param cantidad Cantidad de particulas a generar.
    */
    void generarChispas(glm::vec3 origen, int cantidad);
    /**
    * @brief Establece el estado de selección del menú de personajes.
    */
    void setSeleccionbool(bool);
    
private:

    BellotaEngine MotorDeJuegoPropio;   ///< Motor grafico
    /**
    * @brief Constructor privado de la fachada del motor gráfico para uso interno (Singleton).
    * 
    * @param w Ancho de la ventana.
    * @param h Alto de la ventana.
    * @param n Nombre de la ventana.
    */
    ElMotorRaylib(int const w, int const h, const std::string &n);

    // Dimensiones de la ventana
    int width_{}, height_{};    ///< Ancho y alto de la ventana de juego

    // Nombre de la ventana
    std::string const nombre_{};    ///< Nombre de la ventana de opengl

    // Instancia única del motor (Singleton)
    static ElMotorRaylib *instancia_;   ///< Instancia única del motor (Singleton)

    CronoManager cronoManager;  ///< sistema que gestiona el tiempo para el bucle de juego

    bool cargado = false;   ///< Indicaba si las texturas estan cargadas en el tiempo de inicio
    bool seleccionBool = false; ///< Indicaba si se han seleccionado personajes
    // para pintar o no el debugger
    bool pintaDebugger; ///< Indicaba si el debugger esta activo o no
    bool show_debugger_window = true;   ///< Indica si pintar la pantalla inicial del debugger de  imgui

    /**
    * @brief Dibuja el UI del debugger con ImGui.
    * 
    * @param em EntityManager con las entidades.
    * @param mapMatrix Referencia a la matriz del mapa.
    * @param fisicasys Sistema de físicas del juego.
    */
    void ImGuiRendeUI(EntityManager &em, MapSystem &mapMatrix,  FisicaSystem  const &fisicasys);

    bool teclaQ_presionada = false; ///< Indica si se ha presionado la q
    EntityManager em;   ///< El entity manager
    bool animacion2 = false;    ///< Indica si pintar la pantalla de instruciones

    //variable para cargar el mapa al arbol una unica vez
    bool mapaPintado = false;   ///< Indica si se ha iniciado el mapa
    bool iniciadoYa = false;    ///< Indica si se ha iniciado el motor grafico

    std::vector<AlmondParticula> particulas;    ///< Vector de particulas

    //Pantalla de carga
    int tiempoPantallaDeCargaPuntos = 0;    ///< Se utiliza para cambiar el numero de puntos en pantalla al cargar en la pantalla de carga
};

#endif // MOTORRAYLIB_H
