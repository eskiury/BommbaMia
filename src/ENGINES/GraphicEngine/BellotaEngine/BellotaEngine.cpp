#include "BellotaEngine.h"
#include "../../../utils/Const.h"
#include "../../../../imgui/src/imgui.h"
#include "../../../../imgui/src/imgui_impl_glfw.h"
#include "../../../../imgui/src/imgui_impl_opengl3.h"

#include <thread>
#include <chrono>

//ESTA FUNCION ES PARA EL TAMAÑO DE LA PANTALLA, NO HACE FALTA QUE SEA DE LA CLASE, YA QUE ES COMO UNA FUNCION GLOBAL
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

BellotaEngine::BellotaEngine(){
    window = nullptr;
    int w = 800;
    int h = 600;
    std::string n = "Ventana del juego";
    inicializarMotor(w, h, n); //inicializamos glew opengl etc...
    //lastFrameTime = glfwGetTime();;
}

BellotaEngine::~BellotaEngine(){
    finalizarMotor();
}

BellotaEngine::BellotaEngine(int const w, int const h, const std::string &n){
    window = nullptr;
    inicializarMotor(w,h,n); //inicializamos glew opengl etc...
    //lastFrameTime = glfwGetTime();;
    CrearArbolDeJuego();
}

void BellotaEngine::inicializarMotor(int const w, int const h, const std::string &n){
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        //return -1;
    }

    //para imgui
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //para imgui

     // Crear una ventana con GLFW
    window = glfwCreateWindow(w, h, n.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        //return -1;
    }

    // Hago que la ventana sea el contexto actual de opengl(QUE SEA LA VENTANA QUE SE VE)
    glfwMakeContextCurrent(window);

    // Registrar el callback de tamaño de framebuffer, ESTO ES PARA REAJUSSTAR EL TAMAÑO DE LA PANTALLA
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicializar GLEW, para cargar las funciones de opengl(PARA PODER USAR LAS FUNCIONES DE OPENGL)
    glewExperimental = GL_TRUE; // Habilita funciones modernas de OpenGL
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW" << std::endl;
        //return -1;
    }

    //configuramos opengl con lo que nos interesa
    configurarOpenGL();

    glfwSwapInterval(0);  // Desactiva V-Sync para que el control de FPS sea tuyo
    //inicializamos los fps a 60
    SetTargetFPS(60);
    tiempoAnterior = glfwGetTime();
}

void BellotaEngine::finalizarMotor(){
    // Limpiar ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    //destruyo la ventana y cierro el glfw de opengl
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

bool BellotaEngine::motorEnFuncionamiento(){
    return !glfwWindowShouldClose(window); // Devuelve true mientras la ventana NO deba cerrarse
}


bool BellotaEngine::cerrarJuego(){
    //cerramos el juego
    glfwSetWindowShouldClose(window, true);
    return true;
}


void BellotaEngine::actualizarPantalla(){
    double tiempoDeFinal = glfwGetTime();
    deltaTime = tiempoDeFinal - tiempoDeInicio;

    // Guardamos el FPS actual para usarlo en ImGui
    if (deltaTime > 0.0) {
        fpsActual = 1.0 / deltaTime;
    } else {
        fpsActual = 0.0;
    }

    double tiempoRestante = tiempoPorFrame - deltaTime;
    if (tiempoRestante > 0.0) {
        std::this_thread::sleep_for(std::chrono::duration<double>(tiempoRestante));
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    //tiempoDeInicio = glfwGetTime();
}

void BellotaEngine::configurarOpenGL(){
    glEnable(GL_DEPTH_TEST); // Habilita la prueba de profundidad
}


void BellotaEngine::limpiarPantalla(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
    //los convertimos a floats
    float rn = static_cast<float>(r) / 255.0f;
    float gn = static_cast<float>(g) / 255.0f;
    float bn = static_cast<float>(b) / 255.0f;
    float an = static_cast<float>(a) / 255.0f;
    glClearColor(rn, gn, bn, an); // Establece el color de limpieza
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia la pantalla con el color definido
}

GLFWwindow* BellotaEngine::getWindow(){
    return window;
}


void BellotaEngine::dibujarRectanguloConColor(float x, float y, float width, float height, float r, float g, float b, float a) {
    // Forzar un estado limpio de OpenGL antes de dibujar en 2D
    glUseProgram(0);  // Desactiva shaders
    glBindVertexArray(0);  // Desvincula cualquier VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // Desvincula VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // Desvincula EBO

    // Asegurar que no haya atributos de vértices extra activados
    for (int i = 0; i < 8; i++) {
        glDisableVertexAttribArray(i);
    }

    // Restaurar la matriz de proyección ortográfica para 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenW, screenH, 0, -1, 1);  // Cambia según tu tamaño de ventana

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Desactivar cosas que pueden interferir con 2D
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);  // Si no usas texturas en el rectángulo
    glDisable(GL_BLEND);  // Si no necesitas transparencias



    // Establecer el color
    glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);

    // Dibujar el rectángulo con `GL_QUADS`
    //std::cout << "Dibujando el rectángulo..." << std::endl;
    glBegin(GL_QUADS);
        glVertex2f(x, y);                   
        glVertex2f(x + width, y);           
        glVertex2f(x + width, y + height);  
        glVertex2f(x, y + height);          
    glEnd();


    // Restaurar las matrices
    glPopMatrix(); // Restaurar la matriz de modelo
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // Restaurar la matriz de proyección
    glMatrixMode(GL_MODELVIEW);

    // Restaurar configuraciones para 3D
    //std::cout << "Restaurando configuraciones para 3D..." << std::endl;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);
    glDepthMask(GL_TRUE);

}


MiVector2 BellotaEngine::getWH(const std::string &nombre){
    AlmondTextura* textu = gestor.getRecurso<AlmondTextura>(nombre);
    int width = textu->getWidth();
    int height = textu->getHeight();
    return {static_cast<float>(width), static_cast<float>(height)};
}


void BellotaEngine::PintarSpriteTextura(const std::string &nombre, float x, float y, float escala, float rotacion, float r, float g, float b, float a){
    // Guardar todo el estado de OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);  // Guarda todos los estados de OpenGL
    glPushMatrix();  // Guardar las matrices actuales (modelo y proyección)

    AlmondTextura* textu = gestor.getRecurso<AlmondTextura>(nombre);
    if (!textu) return;

    GLuint texturaID = textu->getId();
    int width = textu->getWidth();
    int height = textu->getHeight();

    // Si no se especifican coordenadas, centrar el sprite en la pantalla
    if (x == -1 && y == -1) {
        x = (screenW - width * escala) / 2.0f;
        y = (screenH - height * escala) / 2.0f;
    }

     // Forzar un estado limpio de OpenGL antes de dibujar en 2D
     glUseProgram(0);  // Desactiva shaders
     glBindVertexArray(0);  // Desvincula cualquier VAO
     glBindBuffer(GL_ARRAY_BUFFER, 0);  // Desvincula VBO
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // Desvincula EBO
 
     // Asegurar que no haya atributos de vértices extra activados
     for (int i = 0; i < 8; i++) {
         glDisableVertexAttribArray(i);
     }

    // Desactivar el depth test y otros estados de OpenGL que puedan afectar el renderizado de los sprites
    glDisable(GL_DEPTH_TEST);      // Desactiva el test de profundidad
    glDisable(GL_LIGHTING);        // Desactiva la iluminación
    glDisable(GL_CULL_FACE);       // Desactiva el culling
    glDisable(GL_TEXTURE_3D);      // Desactiva texturas 3D
    glDisable(GL_ALPHA_TEST);      // Desactiva el alpha test

    glDepthMask(GL_FALSE);         // Evita escribir en el depth buffer
    glEnable(GL_TEXTURE_2D);      // Habilitar texturas 2D
    glBindTexture(GL_TEXTURE_2D, texturaID);

    glEnable(GL_BLEND);            // Habilitar blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Cambiar a modo ortográfico para renderizar en 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenW, screenH, 0, -1, 1);  // Definir el área de proyección ortográfica

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Aplicar transformaciones de traslación, rotación y escala
    glTranslatef(x + (width * escala) / 2, y + (height * escala) / 2, 0);
    glRotatef(rotacion, 0, 0, 1);  // Rotación en el eje Z (2D)
    glScalef(escala, escala, 1);   // Escala del sprite
    glTranslatef(-width / 2, -height / 2, 0);  // Mover el origen a la esquina superior izquierda

    // Aplicar el color del sprite (en formato de 0 a 1)
    glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);

    // Dibujar el sprite (cuadro con textura)
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0, 0);
        glTexCoord2f(1, 0); glVertex2f(width, 0);
        glTexCoord2f(1, 1); glVertex2f(width, height);
        glTexCoord2f(0, 1); glVertex2f(0, height);
    glEnd();

    // Restaurar las matrices de OpenGL a su estado anterior
    glPopMatrix(); // Restaurar modelo
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // Restaurar proyección
    glMatrixMode(GL_MODELVIEW);

    // Restaurar el estado de OpenGL a lo que estaba antes del renderizado del sprite
    glPopAttrib();  // Restaurar todos los estados de OpenGL (como blending, depth test, etc.)
}




void BellotaEngine::CrearArbolDeJuego(){
    // Crear nodos utilizando std::make_unique y agregar al vector
    nodosJuego.push_back(std::make_unique<AlmondNode>());  // nEscena Padre RAIZ
    nodosJuego.push_back(std::make_unique<AlmondNode>());  // nLuces del juego
    nodosJuego.push_back(std::make_unique<AlmondNode>());  // nCamaras del juego
    nodosJuego.push_back(std::make_unique<AlmondNode>());  // nPersonajes del juego
    //nodosJuego.push_back(std::make_unique<AlmondNode>());  // nMapa del juego
    //nodosJuego.push_back(std::make_unique<AlmondNode>());  // nDecoracionesMapa del juego
    nodosJuego.push_back(std::make_unique<AlmondNode>());  // para la pintura, ya que no es una entidad
    nodosJuego.push_back(std::make_unique<AlmondNode>());  // bombas
    nodosJuego.push_back(std::make_unique<AlmondNode>());   //rastro bombas

    //añado hijos
    nodosJuego[0]->addHijo(nodosJuego[1].get());  // nEscena -> luces
    nodosJuego[0]->addHijo(nodosJuego[2].get());  // nEscena -> camaras
    nodosJuego[0]->addHijo(nodosJuego[3].get());  // nEscena -> personajes
    //nodosJuego[0]->addHijo(nodosJuego[4].get());  // nEscena -> mapa
    //nodosJuego[0]->addHijo(nodosJuego[5].get());  // nEscena -> decoraciones
    nodosJuego[0]->addHijo(nodosJuego[4].get());  // nEscena -> pintura
    nodosJuego[0]->addHijo(nodosJuego[5].get());  // nEscena -> bombas
    nodosJuego[0]->addHijo(nodosJuego[6].get());  // nEscena -> rastro bombas
    // Crear tiles y sus respectivos hijos (mapa + decoraciones)
    for (int i = 0; i < 4; ++i) {
        auto tile = std::make_unique<AlmondNode>();
        int fila = i / 2;
        int columna = i % 2;
        tile->setTile(fila, columna);
        tile->setSepinta(true); // o false si los desactivas de inicio
        // if(i == 1){
        //     tile->setSepinta(false); 
        // }

        auto mapa = std::make_unique<AlmondNode>();
        auto decoracion = std::make_unique<AlmondNode>();

        tile->addHijo(mapa.get());
        tile->addHijo(decoracion.get());

        nodosJuego[0]->addHijo(tile.get()); // Tile al nodo raíz

        // Guardamos los nodos si necesitas acceso después
        nodosJuego.push_back(std::move(tile));       // Tile_i
        nodosJuego.push_back(std::move(mapa));       // Mapa_i
        nodosJuego.push_back(std::move(decoracion)); // Decoracion_i
    }
}

void BellotaEngine::CargarMapa(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
    //creo el nodo que contendra el modelo
    nodosJuego.push_back(std::make_unique<AlmondNode>());
    //lo añado al padre que es mapa
    //lo obtengo porque es el ultimo que he metido
    AlmondNode* nodoMapa = nodosJuego.back().get();
    if(x <= 12.0f && z > 10.5f){
        //va a la zona 1
        nodosJuego[8]->addHijo(nodoMapa);  // Tile1 -> mapa
    }else if(x > 12.0f && z > 10.5f){
        //va a la zona 2
        nodosJuego[11]->addHijo(nodoMapa);  // Tile2 -> mapa
    }else if(x <= 12.0f && z <= 10.5f){
        //va a la zona 3
        nodosJuego[14]->addHijo(nodoMapa);  // Tile3 -> mapa
    }else if(x > 12.0f && z <= 10.5f){
        //va a la zona 4
        nodosJuego[17]->addHijo(nodoMapa);  // Tile4 -> mapa
    }
    //nodosJuego[4]->addHijo(nodoMapa);  // nEscena -> mapa
    nodoMapa->trasladar(glm::vec3(x, y, z));

    nodoMapa->setEntidad(std::make_unique<AlmondEntidadModelo>());

    auto entidad = nodoMapa->getEntidad();
    if (entidad) {
        // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
        //         << ", Dirección de la entidad: " << entidad << std::endl;
    } else {
        std::cerr << "Entidad es nula." << std::endl;
    }

    if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad)) {
        //std::cout << "Entidad modelo correctamente asignada." << std::endl;
        entidadModelo->guardarGestor(&gestor);
        entidadModelo->cargarModelo("assets/modelos/bloqueIndestructible.obj", &gestor);
    } else {
        std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadModelo." << std::endl;
    }

}

void BellotaEngine::RecorrerArbolDeJuego() {
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 camaraposition;
    glm::vec3 camTarget; 
    float fov;

    // Necesito pasar la matriz vista y proyección, por lo que las cojo del vector de cámaras
    if(camaras.size()>0){
        auto entidad = camaras[0]->getEntidad();
        if (entidad) {
            // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
            //           << ", Dirección de la entidad: " << entidad << std::endl;
        } else {
            std::cerr << "Entidad es nula." << std::endl;
        }

        if (auto* entidadCamara = dynamic_cast<AlmondEntidadCamara*>(entidad)) {
            // Calculamos la matriz de vista usando lookAt y las variables de la cámara
            view = entidadCamara->getMatrizDeVista();
            projection = entidadCamara->getMatrizDeProjection();
            camaraposition = entidadCamara->getPosicion();
            camTarget = entidadCamara->getTarget();
            fov = entidadCamara->getFov();
        } else {
            std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadCamara." << std::endl;
        }
    }
    

   
    // Activar backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);       // Elimina caras traseras
    glFrontFace(GL_CCW);       // Caras delanteras en sentido antihorario

    // Guardar el estado actual de OpenGL (todas las configuraciones)
    glPushAttrib(GL_ALL_ATTRIB_BITS);  // Guarda todos los estados de OpenGL
    glPushMatrix();  // Guardar las matrices actuales (modelo y proyección)

    // Cambiar la proyección a ortográfica para renderizar en 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();  // Limpiar la matriz de proyección
    glOrtho(0, screenW, screenH, 0, -1, 1);  // Definir el área de proyección ortográfica

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();  // Limpiar la matriz de modelo-vista

    // Le paso la matriz de vista y la de proyección para que las usen los nodos de los modelos
    nodosJuego[0]->recorrer(glm::mat4(1.0f), view, projection,camaraposition,luces);

    // Aquí podrías realizar otras operaciones o transformaciones necesarias

    // Ahora eliminamos los nodos hijos de nodosJuego[0] (que son nodos 3 y 4)
    if (nodosJuego.size() <= 5) return; // Asegurar que nodos 3 y 4 existen

    // Limpiar hijos de nodosJuego[4], limpio la pintura
    for (AlmondNode* hijo : nodosJuego[4]->hijos) {
        auto it = std::find_if(nodosJuego.begin(), nodosJuego.end(),
            [hijo](const std::unique_ptr<AlmondNode>& nodo) { return nodo.get() == hijo; });

        if (it != nodosJuego.end()) {
            it->reset();  // Libera la memoria del nodo
        }
    }
    nodosJuego[4]->hijos.clear(); // Limpia la referencia de los hijos

    for (AlmondNode* hijo : nodosJuego[6]->hijos) {
        auto it = std::find_if(nodosJuego.begin(), nodosJuego.end(),
            [hijo](const std::unique_ptr<AlmondNode>& nodo) { return nodo.get() == hijo; });

        if (it != nodosJuego.end()) {
            it->reset();  // Libera la memoria del nodo
        }
    }
    nodosJuego[6]->hijos.clear(); // Limpia la referencia de los hijos

    // Eliminar nodos NULL de `nodosJuego`
    nodosJuego.erase(
        std::remove_if(nodosJuego.begin(), nodosJuego.end(),
            [](const std::unique_ptr<AlmondNode>& nodo) { return !nodo; }),
        nodosJuego.end()
    );

    // Restaurar las matrices de OpenGL a su estado anterior
    glPopMatrix();  // Restaurar modelo
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();  // Restaurar proyección
    glMatrixMode(GL_MODELVIEW);

    //Desactivar Backface Culling
    glDisable(GL_CULL_FACE);
    // Restaurar el estado de OpenGL a lo que estaba antes del renderizado de los sprites
    glPopAttrib();  // Restaurar todos los estados de OpenGL (como blending, depth test, etc.)

}



void BellotaEngine::CrearCamara(glm::vec3 targetn, glm::vec3 upn, float fov, bool pers, glm::vec3 campos){
    //creo el nodo que contendra el modelo
    nodosJuego.push_back(std::make_unique<AlmondNode>());
    //lo añado al padre que es mapa
    //lo obtengo porque es el ultimo que he metido
    AlmondNode* nodoCamara = nodosJuego.back().get();
    nodosJuego[2]->addHijo(nodoCamara);  // nEscena -> camaras
    //nodoCamara->trasladar(glm::vec3(x, y, z));

    nodoCamara->setEntidad(std::make_unique<AlmondEntidadCamara>());

    auto entidad = nodoCamara->getEntidad();
    if (entidad) {
        // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
        //         << ", Dirección de la entidad: " << entidad << std::endl;
    } else {
        std::cerr << "Entidad es nula." << std::endl;
    }

    if (auto* entidadCamara = dynamic_cast<AlmondEntidadCamara*>(entidad)) {
        //std::cout << "Entidad modelo correctamente asignada." << std::endl;
        entidadCamara->ConfigurarCamara(targetn, upn, fov, pers, campos);
        
    } else {
        std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadCamara." << std::endl;
    }

    //meto la camara al vector de camaras
    camaras.push_back(nodosJuego.back().get());
}

double BellotaEngine::getTiempoAcumulado() {
    // static double lastFrameTime = glfwGetTime();  // Inicialización estática para la primera llamada
    // double currentTime = glfwGetTime();
    // double deltaTime = currentTime - lastFrameTime;
    // lastFrameTime = currentTime;
    // return deltaTime;
    return deltaTime;
}



void BellotaEngine::CargarPW(float x, float y, float z,float rota, const std::string &nombre){
    //creo el nodo que contendra el modelo
    nodosJuego.push_back(std::make_unique<AlmondNode>());
    //lo añado al padre que es personajes
    //lo obtengo porque es el ultimo que he metido
    AlmondNode* nodoPersonaje = nodosJuego.back().get();
    nodosJuego[3]->addHijo(nodoPersonaje);  // nEscena -> personajes
    nodoPersonaje->trasladar(glm::vec3(x, y, z));
    nodoPersonaje->setRotacion(glm::vec3(0.0f, rota, 0.0f));

    nodoPersonaje->setEntidad(std::make_unique<AlmondEntidadModelo>());

    auto entidad = nodoPersonaje->getEntidad();
    if (entidad) {
        // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
        //         << ", Dirección de la entidad: " << entidad << std::endl;
    } else {
        std::cerr << "Entidad es nula." << std::endl;
    }

    if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad)) {
        //std::cout << "Entidad modelo correctamente asignada." << std::endl;
        entidadModelo->guardarGestor(&gestor);
        entidadModelo->cargarModelo(nombre, &gestor);
    } else {
        std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadModelo." << std::endl;
    }

}

void BellotaEngine::SetTargetFPS(int fps) {
    // fpsTarget = fps;                            // Establecer FPS objetivo
    // tiempoPorFrame = 1.0f / fpsTarget;          // Calcular el tiempo por frame
    // tiempoDeInicio = glfwGetTime();              // Inicializar el tiempo de inicio para el primer frame
    if (fps < 1) fps = 1;
    tiempoPorFrame = 1.0 / static_cast<double>(fps);
}

int BellotaEngine::GetFPS() {   //esto no va bien
    return static_cast<int>(fpsActual + 0.5); // Redondeado
}

void BellotaEngine::ModificarCamara(glm::vec3 targetn, glm::vec3 upn, float fov, bool pers, glm::vec3 campos){
    if(camaras.size()>0){
    auto entidad = camaras[0]->getEntidad();
    if (entidad) {
       // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
        //        << ", Dirección de la entidad: " << entidad << std::endl;
    } else {
        std::cerr << "Entidad es nula." << std::endl;
    }

    if (auto* entidadCamara = dynamic_cast<AlmondEntidadCamara*>(entidad)) {
        //std::cout << "Entidad modelo correctamente asignada." << std::endl;
        entidadCamara->ConfigurarCamara(targetn, upn, fov, pers, campos);
        
    } else {
        std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadCamara." << std::endl;
    }
    }
}

void BellotaEngine::DibujarCubos(float x, float y, float z, double width, double height, double length, float r, float g, float b, float a) {
    // Inicialización del cubo-----------------
    float w = static_cast<float>(width) / 2.0f;
    float h = static_cast<float>(height) / 2.0f;
    float l = static_cast<float>(length) / 2.0f;

    float rn = r / 255.0f;
    float gn = g / 255.0f;
    float bn = b / 255.0f;
    float an = a / 255.0f;

    float vertices[] = {
        // Cara trasera
        -w, -h, -l,  w, -h, -l,  w,  h, -l,
        w,  h, -l, -w,  h, -l, -w, -h, -l,

        // Cara delantera
        -w, -h,  l,  w, -h,  l,  w,  h,  l,
        w,  h,  l, -w,  h,  l, -w, -h,  l,

        // Cara izquierda
        -w, -h, -l, -w,  h, -l, -w,  h,  l,
        -w,  h,  l, -w, -h,  l, -w, -h, -l,

        // Cara derecha
        w, -h, -l,  w,  h, -l,  w,  h,  l,
        w,  h,  l,  w, -h,  l,  w, -h, -l,

        // Cara inferior
        -w, -h, -l,  w, -h, -l,  w, -h,  l,
        w, -h,  l, -w, -h,  l, -w, -h, -l,

        // Cara superior
        -w,  h, -l,  w,  h, -l,  w,  h,  l,
        w,  h,  l, -w,  h,  l, -w,  h, -l
    };

    // Guardar los vértices
    unsigned int lightVAO, lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Crear programa con los shaders----------
    const std::string& nombre = "assets/shaders/shaderCubo.AlmondVertex";
    const std::string& nombre2 = "assets/shaders/shaderCubo.AlmondFragment";
    AlmondShader* shaderFiguraVertex = gestor.getRecurso<AlmondShader>(nombre); //este es un VERTEX SHADER para poder pintar el cubo
    AlmondShader* shaderLuzYFigura = gestor.getRecurso<AlmondShader>(nombre2);

    GLuint shaderProgramCUBO = glCreateProgram();
    glAttachShader(shaderProgramCUBO, shaderFiguraVertex->getShader());
    glAttachShader(shaderProgramCUBO, shaderLuzYFigura->getShader());
    glLinkProgram(shaderProgramCUBO);

    GLint success;
    GLchar infoLog[512];

    glGetProgramiv(shaderProgramCUBO, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramCUBO, 512, NULL, infoLog);
        std::cerr << "Error linkeando el Shader Program: " << infoLog << std::endl;
    }

    // Desactivar el culling de caras (si lo deseas)
    glDisable(GL_CULL_FACE); // Desactiva el culling si quieres ver todas las caras
     // Habilitar el blending (transparencia)
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Establece el modo de mezcla para la transparencia
 

    // Pintado del cubo----------------
    glm::mat4 view;
    glm::mat4 projection;

    if(camaras.size()>0){
    // Obtener las matrices de la cámara
    auto entidad = camaras[0]->getEntidad();
    if (auto* entidadCamara = dynamic_cast<AlmondEntidadCamara*>(entidad)) {
        view = entidadCamara->getMatrizDeVista();
        projection = entidadCamara->getMatrizDeProjection();
    } else {
        std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadCamara." << std::endl;
    }

    glUseProgram(shaderProgramCUBO);

    GLuint modelLoc = glGetUniformLocation(shaderProgramCUBO, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgramCUBO, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgramCUBO, "projection");
    GLuint colorLoc = glGetUniformLocation(shaderProgramCUBO, "objectColor");
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(x, y, z));

    glUniform4f(colorLoc, rn, gn, bn, an);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Vincular el VAO del cubo y dibujarlo
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    // Pintado del cubo----------------
    }
}

void BellotaEngine::CargarDecoraciones(float x, float y, float z, const std::string &nombre, float rota, int zona){
    //creo el nodo que contendra el modelo
    nodosJuego.push_back(std::make_unique<AlmondNode>());
    //lo añado al padre que es personajes
    //lo obtengo porque es el ultimo que he metido
    AlmondNode* nodoDecoracion = nodosJuego.back().get();
    //obtengo el centro original del objeto
    if(zona == 0){
        //va a la zona 1
        nodosJuego[8]->addHijo(nodoDecoracion);  // Tile1 -> mapa
    }else if(zona == 1){
        //va a la zona 2
        nodosJuego[11]->addHijo(nodoDecoracion);  // Tile2 -> mapa
    }else if(zona == 2){
        //va a la zona 3
        nodosJuego[14]->addHijo(nodoDecoracion);  // Tile3 -> mapa
    }else if(zona == 3){
        //va a la zona 4
        nodosJuego[17]->addHijo(nodoDecoracion);  // Tile4 -> mapa
    }else{
        nodosJuego[8]->addHijo(nodoDecoracion);  // Tile1 -> mapa
    }
    //nodosJuego[5]->addHijo(nodoDecoracion);  // nEscena -> personajes
    nodoDecoracion->trasladar(glm::vec3(x,y,z));
    //nodoDecoracion->escalar(glm::vec3(2.0f, 2.0f, 2.0f));
    if(rota != 0.0f){
    nodoDecoracion->setRotacion(glm::vec3(0.0f, rota, 0.0f));
    }

    nodoDecoracion->setEntidad(std::make_unique<AlmondEntidadModelo>());

    auto entidad = nodoDecoracion->getEntidad();
    if (entidad) {
        // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
        //         << ", Dirección de la entidad: " << entidad << std::endl;
    } else {
        std::cerr << "Entidad es nula." << std::endl;
    }

    if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad)) {
        //std::cout << "Entidad modelo correctamente asignada." << std::endl;
        entidadModelo->guardarGestor(&gestor);
        entidadModelo->cargarModelo(nombre, &gestor);
    } else {
        std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadModelo." << std::endl;
    }

}

void BellotaEngine::DibujarTextosPropios(std::string texto, int x, int y){
    int espacio = 15;  // Espacio entre caracteres
    int ancho_digito = 20;  // Asumiendo que cada imagen de número tiene 20px de ancho

    for (size_t i = 0; i < texto.size(); i++) {
        char caracter = texto[i];
        std::string nombre_textura;

        if (caracter >= '0' && caracter <= '9') {
            nombre_textura = "assets/sprites/Tiempo/" + std::to_string(caracter - '0') + "Numeros.png";
        } else if (caracter == ':') {
            nombre_textura = "assets/sprites/Tiempo/puntosNumeros.png";  // Imagen especial para ":"
        }

        int pos_x = x + i * (ancho_digito + espacio);

        // Pintar cada carácter desde su imagen correspondiente
        PintarSpriteTextura(nombre_textura, pos_x, y, 1.0f, 0.0f, 255.0f, 255.0f, 255.0f, 255.0f);
    }
}

void BellotaEngine::PrecargarRecursosMalla(const std::string &nombre){
    AlmondMalla* textu = gestor.getRecurso<AlmondMalla>(nombre);
}


void BellotaEngine::SetVSync(bool enabled) {
    vsyncActivo = enabled;
    glfwSwapInterval(vsyncActivo ? 1 : 0);
}

bool BellotaEngine::IsVSyncActivo() const {
    return vsyncActivo;
}

// void BellotaEngine::GuardarInicioDeFrame(){
//     tiempoDeInicio = glfwGetTime();
// }


AlmondNode* BellotaEngine::CargarPersonajesYDevuelve(float x, float y, float z, const std::string &nombre){
    //creo el nodo que contendra el modelo
    nodosJuego.push_back(std::make_unique<AlmondNode>());
    //lo añado al padre que es personajes
    //lo obtengo porque es el ultimo que he metido
    AlmondNode* nodoPersonaje = nodosJuego.back().get();
    nodosJuego[3]->addHijo(nodoPersonaje);  // nEscena -> personajes
    nodoPersonaje->trasladar(glm::vec3(x, y, z));
    
    nodoPersonaje->setEntidad(std::make_unique<AlmondEntidadModelo>());

    auto entidad = nodoPersonaje->getEntidad();
    if (entidad) {
        // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
        //         << ", Dirección de la entidad: " << entidad << std::endl;
    } else {
        std::cerr << "Entidad es nula." << std::endl;
    }

    if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad)) {
        //std::cout << "Entidad modelo correctamente asignada." << std::endl;
        entidadModelo->guardarGestor(&gestor);
        entidadModelo->cargarModelo(nombre, &gestor);
    } else {
        std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadModelo." << std::endl;
    }

    //Devuelvo la direccion de memoria al nodo,para poder usar este nodo, pero el control de su destruccion lo mantiene el vector de esta clase
    return nodoPersonaje;
}

void BellotaEngine::EliminarPersonaje(AlmondNode* aborrar){
    if (!aborrar) return;

    // Quitar del padre
    AlmondNode* padre = aborrar->getPadre();
    if (padre) {
        padre->quitarHijo(aborrar);
    }

    // Recolectar nodos a eliminar
    std::vector<AlmondNode*> aEliminar;

    auto recolectar = [&](AlmondNode* nodo, auto&& self) -> void {
        aEliminar.push_back(nodo);
        for (AlmondNode* hijo : nodo->hijos) {
            self(hijo, self);  // Recursión explícita
        }
    };

    recolectar(aborrar, recolectar);

    // Eliminar del vector nodosJuego
    for (AlmondNode* nodo : aEliminar) {
        auto it = std::find_if(nodosJuego.begin(), nodosJuego.end(),
            [nodo](const std::unique_ptr<AlmondNode>& ptr) {
                return ptr.get() == nodo;
            });
        if (it != nodosJuego.end()) {
            it->reset(); // Libera la memoria
        }
    }
}

void BellotaEngine::Reset(){
    for (auto& nodo : nodosJuego) {
        nodo.reset();
    }
    nodosJuego.clear();

    camaras.clear();
    luces.clear();
    //restauro los de por defecto
    CrearArbolDeJuego();
    tilesActivos = false;
}

void BellotaEngine::CargarPintura(float x, float y, float z, const std::string &nombre){
    //creo el nodo que contendra el modelo
    nodosJuego.push_back(std::make_unique<AlmondNode>());
    //lo añado al padre que es personajes
    //lo obtengo porque es el ultimo que he metido
    AlmondNode* nodoPersonaje = nodosJuego.back().get();
    nodosJuego[4]->addHijo(nodoPersonaje);  // nEscena -> personajes
    nodoPersonaje->trasladar(glm::vec3(x, y, z));

    nodoPersonaje->setEntidad(std::make_unique<AlmondEntidadModelo>());
   
    auto entidad = nodoPersonaje->getEntidad();
    if (entidad) {
        // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
        //         << ", Dirección de la entidad: " << entidad << std::endl;
    } else {
        std::cerr << "Entidad es nula." << std::endl;
    }

    if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad)) {
        //std::cout << "Entidad modelo correctamente asignada." << std::endl;
        entidadModelo->guardarGestor(&gestor);
        entidadModelo->cargarModelo(nombre, &gestor);
    } else {
        std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadModelo." << std::endl;
    }
}

AlmondNode* BellotaEngine::CargarBombasYDevuelve(float x, float y, float z, const std::string &nombre){
    //creo el nodo que contendra el modelo
    nodosJuego.push_back(std::make_unique<AlmondNode>());
    //lo añado al padre que es personajes
    //lo obtengo porque es el ultimo que he metido
    AlmondNode* nodoPersonaje = nodosJuego.back().get();
    nodosJuego[5]->addHijo(nodoPersonaje);  // nEscena -> personajes
    nodoPersonaje->trasladar(glm::vec3(x, y, z));
    
    nodoPersonaje->setEntidad(std::make_unique<AlmondEntidadModelo>());

    auto entidad = nodoPersonaje->getEntidad();
    if (entidad) {
        // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
        //         << ", Dirección de la entidad: " << entidad << std::endl;
    } else {
        std::cerr << "Entidad es nula." << std::endl;
    }

    if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad)) {
        //std::cout << "Entidad modelo correctamente asignada." << std::endl;
        entidadModelo->guardarGestor(&gestor);
        entidadModelo->cargarModelo(nombre, &gestor);
    } else {
        std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadModelo." << std::endl;
    }

    //Devuelvo la direccion de memoria al nodo,para poder usar este nodo, pero el control de su destruccion lo mantiene el vector de esta clase
    return nodoPersonaje;
}

void BellotaEngine::AnyadirBombaRastro(AlmondNode* padreBomba, float x, float y, float z, const std::string &nombre){
     //creo el nodo que contendra el modelo
     nodosJuego.push_back(std::make_unique<AlmondNode>());
     //lo añado al padre que es personajes
     //lo obtengo porque es el ultimo que he metido
     AlmondNode* nodoPersonaje = nodosJuego.back().get();
     nodosJuego[6]->addHijo(nodoPersonaje);  // nEscena -> personajes
     nodoPersonaje->trasladar(glm::vec3(x, y, z));
 
     nodoPersonaje->setEntidad(std::make_unique<AlmondEntidadModelo>());
    
     auto entidad = nodoPersonaje->getEntidad();
     if (entidad) {
         // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
         //         << ", Dirección de la entidad: " << entidad << std::endl;
     } else {
         std::cerr << "Entidad es nula." << std::endl;
     }
 
     if (auto* entidadModelo = dynamic_cast<AlmondEntidadModelo*>(entidad)) {
         //std::cout << "Entidad modelo correctamente asignada." << std::endl;
         entidadModelo->guardarGestor(&gestor);
         entidadModelo->cargarModelo(nombre, &gestor);
     } else {
         std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadModelo." << std::endl;
     }

}

void BellotaEngine::GuardarInicioDeFrame() {
    tiempoDeInicio = glfwGetTime(); // Marca el comienzo del frame
    deltaTime = tiempoDeInicio - tiempoAnterior;
    tiempoAnterior = tiempoDeInicio;

    // Limita deltaTime para evitar saltos enormes si el PC va muy mal (por ejemplo al alt-tabear)
    if (deltaTime > 0.25) {
        deltaTime = 0.25;
    }
    fpsReal = 1.0 / deltaTime; // Calcula los FPS actuales
    acumulador += deltaTime;
}

void BellotaEngine::CrearLuz(
    int tipoLuz,                      // 0 = direccional, 1 = puntual, 2 = spot
    glm::vec3 posicion,
    glm::vec3 direccion,
    glm::vec3 color,
    float ambStrength,
    float diffStrength,
    float specStrength,
    float atenCte,
    float atenLineal,
    float atenCuadrat,
    float apertura,     // solo para spot
    float penumbra      // solo para spot
) {
    // creo el nodo que contendrá la entidad luz
    nodosJuego.push_back(std::make_unique<AlmondNode>());
    AlmondNode* nodoLuz = nodosJuego.back().get();
    nodosJuego[1]->addHijo(nodoLuz);  // nEscena -> luces

    nodoLuz->setEntidad(std::make_unique<AlmondEntidadLuz>());
    auto entidad = nodoLuz->getEntidad();

    if (auto* entidadLuz = dynamic_cast<AlmondEntidadLuz*>(entidad)) {
        switch (tipoLuz) {
            case 0: // Direccional
                entidadLuz->ConfigurarLuzDireccional(
                    direccion, color, ambStrength, diffStrength, specStrength
                );
                break;

            case 1: // Puntual
                entidadLuz->ConfigurarLuzPuntual(
                    posicion, color, ambStrength, diffStrength, specStrength,
                    atenCte, atenLineal, atenCuadrat
                );
                break;

            case 2: // Spot
                entidadLuz->ConfigurarLuzSpot(
                    posicion, direccion, color, ambStrength, diffStrength, specStrength,
                    atenCte, atenLineal, atenCuadrat, apertura, penumbra
                );
                break;

            default:
                std::cerr << "Error: Tipo de luz no válido." << std::endl;
                return;
        }
    } else {
        std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadLuz." << std::endl;
        return;
    }

    // agrego al vector de luces
    luces.push_back(nodosJuego.back().get());
}

void BellotaEngine::setTilesActivos(bool var){
    tilesActivos = var;
}

AlmondNode*  BellotaEngine::CrearLuzFocoPlayer(
    int tipoLuz,                      // 0 = direccional, 1 = puntual, 2 = spot
    glm::vec3 posicion,
    glm::vec3 direccion,
    glm::vec3 color,
    float ambStrength,
    float diffStrength,
    float specStrength,
    float atenCte,
    float atenLineal,
    float atenCuadrat,
    float apertura,     // solo para spot
    float penumbra      // solo para spot
) {
    // creo el nodo que contendrá la entidad luz
    nodosJuego.push_back(std::make_unique<AlmondNode>());
    AlmondNode* nodoLuz = nodosJuego.back().get();
    nodosJuego[1]->addHijo(nodoLuz);  // nEscena -> luces

    nodoLuz->setEntidad(std::make_unique<AlmondEntidadLuz>());
    auto entidad = nodoLuz->getEntidad();

    if (auto* entidadLuz = dynamic_cast<AlmondEntidadLuz*>(entidad)) {
        switch (tipoLuz) {
            case 0: // Direccional
                entidadLuz->ConfigurarLuzDireccional(
                    direccion, color, ambStrength, diffStrength, specStrength
                );
                break;

            case 1: // Puntual
                entidadLuz->ConfigurarLuzPuntual(
                    posicion, color, ambStrength, diffStrength, specStrength,
                    atenCte, atenLineal, atenCuadrat
                );
                break;

            case 2: // Spot
                entidadLuz->ConfigurarLuzSpot(
                    posicion, direccion, color, ambStrength, diffStrength, specStrength,
                    atenCte, atenLineal, atenCuadrat, apertura, penumbra
                );
                break;
        }
    }

    // agrego al vector de luces
    luces.push_back(nodosJuego.back().get());
    //devuelvo el puntero a la luz
    return nodosJuego.back().get();
}

void BellotaEngine::pintarParticula(AlmondParticula& pasada){
    glDisable(GL_DEPTH_TEST); // Desactiva el test de profundidad
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 camaraposition;
    if(camaras.size()>0){
    auto entidad2 = camaras[0]->getEntidad();
        if (entidad2) {
            // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
            //           << ", Dirección de la entidad: " << entidad << std::endl;
        } else {
            std::cerr << "Entidad es nula." << std::endl;
        }
    
        if (auto* entidadCamara = dynamic_cast<AlmondEntidadCamara*>(entidad2)) {
            // Calculamos la matriz de vista usando lookAt y las variables de la cámara
            view = entidadCamara->getMatrizDeVista();
            projection = entidadCamara->getMatrizDeProjection();
            camaraposition = entidadCamara->getPosicion();
            pasada.dibujar(view,projection,camaraposition);
    }
    }
    glDisable(GL_BLEND);  // Si no necesitas transparencias
    glEnable(GL_DEPTH_TEST); // Vuelve a habilitar el test de profundidad
}

AlmondNode* BellotaEngine::CargarAnimacionesYDevuelve(float x, float y, float z,const std::string &nombreAnimacion, const std::vector<std::string> &rutasAnimacion){
    //creo el nodo que contendra el modelo
    nodosJuego.push_back(std::make_unique<AlmondNode>());
    //lo añado al padre que es personajes
    //lo obtengo porque es el ultimo que he metido
    AlmondNode* nodoPersonaje = nodosJuego.back().get();
    nodosJuego[3]->addHijo(nodoPersonaje);  // nEscena -> personajes
    nodoPersonaje->trasladar(glm::vec3(x, y, z));
    
    nodoPersonaje->setEntidad(std::make_unique<AlmondEntidadAnimacion>());

    auto entidad = nodoPersonaje->getEntidad();
    if (entidad) {
        // std::cout << "Entidad antes del cast: " << typeid(*entidad).name() 
        //         << ", Dirección de la entidad: " << entidad << std::endl;
    } else {
        std::cerr << "Entidad es nula." << std::endl;
    }

    if (auto* entidadAnim = dynamic_cast<AlmondEntidadAnimacion*>(entidad)) {
        //std::cout << "Entidad modelo correctamente asignada." << std::endl;
        entidadAnim->guardarGestor(&gestor);
        entidadAnim->cargarAnimacion(nombreAnimacion, rutasAnimacion);
        entidadAnim->reproducirAnimacion(nombreAnimacion);
        entidadAnim->setFPS(3.0f);
        entidadAnim->setLoop(true);
    } else {
        std::cerr << "Error: La entidad no es un modelo de tipo AlmondEntidadAnimacion." << std::endl;
    }

    //Devuelvo la direccion de memoria al nodo,para poder usar este nodo, pero el control de su destruccion lo mantiene el vector de esta clase
    return nodoPersonaje;
}