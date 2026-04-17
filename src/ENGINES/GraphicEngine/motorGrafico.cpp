#include <sstream> // Para std::ostringstream
#include <vector>
#include <string>
#include <iostream>
#include <iomanip> // Para std::setw
#include "motorGrafico.h"
#include "../../CMP/Entity.h"
#include "../../AlmondGE/EntityManager.h"
#include "../../utils/miColor.h"
#include "../../utils/miVector2.h"
#include "../../utils/Const.h"
#include "../../utils/iaManager.h"

#include "../../../imgui/src/imgui.h"
#include "../../../imgui/src/imgui_impl_glfw.h"
#include "../../../imgui/src/imgui_impl_opengl3.h"
#include "../../../imgui/src/imgui_demo.cpp"


bool enableMoveOrDie = true;
bool PausarJuego = false;
// Colores predeterminados o mas usados
#define MY_LIGHTGRAY \
    (MiColor){200, 200, 200, 255} // Light Gray
#define MY_GRAY \
    (MiColor){130, 130, 130, 255} // Gray
#define MY_DARKGRAY \
    (MiColor){80, 80, 80, 255} // Dark Gray
#define MY_YELLOW \
    (MiColor){253, 249, 0, 255} // Yellow
#define MY_GOLD \
    (MiColor){255, 203, 0, 255} // Gold
#define MY_ORANGE \
    (MiColor){255, 161, 0, 255} // Orange
#define MY_PINK \
    (MiColor){255, 109, 194, 255} // Pink
#define MY_RED \
    (MiColor){220, 20, 60, 255} // Red
#define MY_MAROON \
    (MiColor){190, 33, 55, 255} // Maroon
#define MY_GREEN \
    (MiColor){0, 228, 48, 255} // Green
#define MY_LIME \
    (MiColor){0, 158, 47, 255} // Lime
#define MY_DARKGREEN \
    (MiColor){0, 117, 44, 255} // Dark Green
#define MY_SKYBLUE \
    (MiColor){102, 191, 255, 50} // Sky Blue
#define MY_BLUE \
    (MiColor){0, 121, 241, 255} // Blue
#define MY_DARKBLUE \
    (MiColor){0, 82, 172, 255} // Dark Blue
#define MY_PURPLE \
    (MiColor){200, 122, 255, 255} // Purple
#define MY_VIOLET \
    (MiColor){135, 60, 190, 255} // Violet
#define MY_DARKPURPLE \
    (MiColor){112, 31, 126, 255} // Dark Purple
#define MY_BEIGE \
    (MiColor){211, 176, 131, 255} // Beige
#define MY_BROWN \
    (MiColor){127, 106, 79, 255} // Brown
#define MY_DARKBROWN \
    (MiColor){76, 63, 47, 255} // Dark Brown
#define MY_WHITE \
    (MiColor){255, 255, 255, 255} // White
#define MY_BLACK \
    (MiColor){0, 0, 0, 255} // Black
#define MY_BLANK \
    (MiColor){0, 0, 0, 0} // Blank (Transparent)
#define MY_MAGENTA \
    (MiColor){255, 0, 255, 255} // Magenta
#define MY_RAYWHITE \
    (MiColor){245, 245, 245, 255} // My own White (raylib logo)
#define MY_TRANSPARENTE \
    (MiColor){0, 121, 241, 130} // My own White (raylib logo)

// Inicialización de miembros estáticos
ElMotorRaylib *ElMotorRaylib::instancia_ = nullptr;

ElMotorRaylib &ElMotorRaylib::CrearMotor(int const w, int const h, const std::string &n)
{
    if (!instancia_)
    { // Si no existe una instancia, creamos una
        instancia_ = new ElMotorRaylib(w, h, n);
    }
    return *instancia_;
}

ElMotorRaylib::ElMotorRaylib(int const w, int const h, const std::string &n) : width_(w), height_(h), nombre_(n), MotorDeJuegoPropio(w, h, n), cronoManager(60,60)
{
    if (!MotorDeJuegoPropio.motorEnFuncionamiento())
    {
        throw std::runtime_error("No se pudo iniciar el motor Raylib");
    }
    
    // Configurar ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(MotorDeJuegoPropio.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

ElMotorRaylib::~ElMotorRaylib()
{
    instancia_ = nullptr;
}

bool ElMotorRaylib::run()
{
    return MotorDeJuegoPropio.motorEnFuncionamiento();
}

void ElMotorRaylib::empezarEscena()
{
    MiColor colorPersonalizado(MY_RAYWHITE);
    MotorDeJuegoPropio.limpiarPantalla(colorPersonalizado.r, colorPersonalizado.g, colorPersonalizado.b, colorPersonalizado.a);
}

void ElMotorRaylib::acabarEscena()
{
    MotorDeJuegoPropio.actualizarPantalla();
}

void ElMotorRaylib::dibujaEntidad(EntityManager &em, Entity const &e, const FisicaSystem &physics)
{
    auto &fisica = em.fisicas.get(e.fisica_key);
    auto &bomb = em.bombs.get(e.bomb_key);
    auto &render = em.renders.get(e.render_key);
    auto &button = em.buttons.get(e.button_key);

    if (e.type == EntityType::PLAYER)
    {
        // sino esta cargado en el arbol, lo cargo
        if (render.cargadoEnElArbol == false)
        {
            const std::string &nombre = render.modeloPersonaje;

            std::string keyword = "personaje";
            size_t pos = nombre.find(keyword);
                // Extraemos el número justo después de "personaje"
                pos += keyword.length();
                std::string numeroPersonaje;

                while (pos < nombre.length() && std::isdigit(nombre[pos])) {
                    numeroPersonaje += nombre[pos];
                    ++pos;
                }
            

            std::vector<std::string> rutasPersonaje;

            // Generamos rutas de animaciones IDLE
            for (int i = 1; i <= 5; ++i) {
                rutasPersonaje.push_back("assets/modelos/animaciones/idle/player" + numeroPersonaje + "Idle" + std::to_string(i) + ".obj");
            }
            render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarAnimacionesYDevuelve(fisica.x, fisica.y- 1, fisica.z, "idle", rutasPersonaje);

            //guardo mas animaciones
            auto entidad = render.SusDatosEnElArbol->getEntidad();
            render.animacion = 1;

            std::vector<std::string> rutasPersonaje2;
            for (int i = 1; i <= 5; ++i) {
                rutasPersonaje2.push_back("assets/modelos/animaciones/correr/player" + numeroPersonaje + "Correr" + std::to_string(i) + ".obj");
            }
            if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
            {
                
                entidadAnim->cargarAnimacion("correr", rutasPersonaje2);
            }
            std::vector<std::string> rutasPersonaje3;
            for (int i = 1; i <= 4; ++i) {
                rutasPersonaje3.push_back("assets/modelos/animaciones/colocarBomba/player" + numeroPersonaje + "bomba" + std::to_string(i) + ".obj");
            }
            if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
            {
                
                entidadAnim->cargarAnimacion("colocarBombas", rutasPersonaje3);
            }

            render.cargadoEnElArbol = true;
        }
        else
        {
            // aplico cambios deposiciono de lo que quiera   PROVISIONAL
            render.SusDatosEnElArbol->setTraslacion(glm::vec3(fisica.x, fisica.y -1, fisica.z));
            render.SusDatosEnElArbol->setRotacion(glm::vec3(0.0f, render.rotacion, 0.0f));
            auto entidad = render.SusDatosEnElArbol->getEntidad();

            if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
            {
                float deltatimeanim = getFixedDeltaTime();
                entidadAnim->actualizarAnimacion(deltatimeanim);
            }
            if (render.animacion == 1 && render.animacionYaIniciada == false)
            {
                if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
                {
                    entidadAnim->reproducirAnimacion("idle");
                    entidadAnim->setFPS(6.0f);
                }
                render.animacionYaIniciada = true;
            }
            if (render.animacion == 2 && render.animacionYaIniciada == false)
            {
                if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
                {
                    entidadAnim->reproducirAnimacion("colocarBombas");
                    entidadAnim->setFPS(12.0f);
                }
                render.animacionYaIniciada = true;
            }
            if (render.animacion == 3 && render.animacionYaIniciada == false)
            {
                if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
                {
                    entidadAnim->reproducirAnimacion("correr");
                    entidadAnim->setFPS(12.0f);
                }
                render.animacionYaIniciada = true;
            }
        }

        if (fisica.esFantasma)
        {
            if (render.cargadoEnElArbolFoco == false)
            {
                glm::vec3 posicion2 = glm::vec3(fisica.x, 10.0f, fisica.z); // Posición del foco
                glm::vec3 direccion2 = glm::vec3(0.0f, -1.0f, 0.0f);        // Dirección hacia donde apunta
                glm::vec3 color2 = glm::vec3(1.0f, 1.0f, 0.3f);             // Amarillo cálido potente

                float ambientStrength2 = 1.2f;  // Más fuerte que lo normal
                float diffuseStrength2 = 2.5f;  // Mucho más brillo directo
                float specularStrength2 = 1.2f; // Reflejos más intensos

                float atenuacionConstante2 = 1.0f;
                float atenuacionLineal2 = 0.045f;      // Menor caída con la distancia
                float atenuacionCuadratica2 = 0.0075f; // Luz alcanza más lejos

                float apertura2 = cos(glm::radians(12.5f)); // Borde interior (coseno del ángulo)
                float penumbra2 = cos(glm::radians(17.5f)); // Suavizado de borde exterior (penumbra)

                render.TieneFoco = MotorDeJuegoPropio.CrearLuzFocoPlayer(
                    2, // tipoLuz: 2 = spotlight
                    posicion2,
                    direccion2,
                    color2,
                    ambientStrength2,
                    diffuseStrength2,
                    specularStrength2,
                    atenuacionConstante2,
                    atenuacionLineal2,
                    atenuacionCuadratica2,
                    apertura2,
                    penumbra2);
                render.cargadoEnElArbolFoco = true;
                render.SusDatosEnElArbol->setSepinta(false);
            }
            else
            {

                auto entidad = render.TieneFoco->getEntidad();

                if (auto *entidadLuz = dynamic_cast<AlmondEntidadLuz *>(entidad))
                {
                    // Calculamos la matriz de vista usando lookAt y las variables de la cámara
                    glm::vec3 posicionAlMoverse = glm::vec3(fisica.x, 10.0f, fisica.z); // Posición del foco
                    entidadLuz->setPosicion(posicionAlMoverse);
                }
            }
        } 
    }
    else if (e.type == EntityType::BOMBA)
    {
        if (!bomb.isExploding)
        {

            // FUNCIONA FALTA BORRAR LAS BOMBAS CUANDO S ELIMINAN-----------------------
            // sino esta cargado en el arbol, lo cargo
            if (render.cargadoEnElArbol == false)
            {
                const std::string &nombre = "assets/modelos/bomba.obj";
                render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarBombasYDevuelve(fisica.x, fisica.y - 2, fisica.z, nombre);
                render.cargadoEnElArbol = true;
            }
            //voy generando humo por cada frame mientras sigue la bomba sin explotar
            generarHumo(glm::vec3(fisica.x + 0.5f, 2.0f, fisica.z), 3); // emite 10 partículas desde el origen
            render.SusDatosEnElArbol->setTraslacion(glm::vec3(fisica.x, fisica.y -2, fisica.z));

        }
        else
        {
            if (!render.explosionaelrender)
            {
                // Apagamos el modelo original de la bomba
                if(render.SusDatosEnElArbol != nullptr)
                {  
                    render.SusDatosEnElArbol->setSepinta(false);
                }
                

                // Modelo de explosión
                const std::string &nombreExplosion = "assets/modelos/explosionProvisional.obj";

                // Añadir el centro de la explosión (justo encima)
                MotorDeJuegoPropio.AnyadirBombaRastro(render.SusDatosEnElArbol, fisica.x, fisica.y, fisica.z, nombreExplosion);

                // Obtener rastros de explosión en cruz desde el sistema de física
                auto &nonConstPhysics = const_cast<FisicaSystem &>(physics);
                const auto &explosionRastros = nonConstPhysics.getExplosionRastros(em);

                // Calcular el desplazamiento relativo y añadir cada rastro
                for (const auto &rastro : explosionRastros)
                {
                    float dx = static_cast<float>(rastro.tileX) * 2.0f;
                    float dy = fisica.y; // misma altura que el centro
                    float dz = static_cast<float>(rastro.tileZ) * 2.0f;

                    MotorDeJuegoPropio.AnyadirBombaRastro(render.SusDatosEnElArbol, dx, dy, dz, nombreExplosion);
                }

                // Marcar que ya explotó
                // render.explosionaelrender = true;
            }
        }
    }
    else if (e.type == EntityType::DESTRUIBLE)
    {
        // sino esta cargado en el arbol, lo cargo
        if (render.cargadoEnElArbol == false)
        {
            const std::string &nombre = "assets/modelos/barril.obj";
            render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarPersonajesYDevuelve(fisica.x, fisica.y, fisica.z, nombre);
            render.cargadoEnElArbol = true;
        }
    }
    else if (e.type == EntityType::NPC)
    {
        auto &npc = em.fisicas.get(e.fisica_key);
        auto &render = em.renders.get(e.render_key);
        if (npc.vida != 0)
        {
            if (render.cargadoEnElArbol == false)
            {
                const std::string &nombre = render.modeloPersonaje;

            std::string keyword = "personaje";
            size_t pos = nombre.find(keyword);
                // Extraemos el número justo después de "personaje"
                pos += keyword.length();
                std::string numeroPersonaje;

                while (pos < nombre.length() && std::isdigit(nombre[pos])) {
                    numeroPersonaje += nombre[pos];
                    ++pos;
                }
            

            std::vector<std::string> rutasPersonaje;

            // Generamos rutas de animaciones IDLE
            for (int i = 1; i <= 5; ++i) {
                rutasPersonaje.push_back("assets/modelos/animaciones/idle/player" + numeroPersonaje + "Idle" + std::to_string(i) + ".obj");
            }
            render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarAnimacionesYDevuelve(fisica.x, fisica.y- 1, fisica.z, "idle", rutasPersonaje);

            //guardo mas animaciones
            auto entidad = render.SusDatosEnElArbol->getEntidad();
            render.animacion = 1;

            std::vector<std::string> rutasPersonaje2;
            for (int i = 1; i <= 5; ++i) {
                rutasPersonaje2.push_back("assets/modelos/animaciones/correr/player" + numeroPersonaje + "Correr" + std::to_string(i) + ".obj");
            }
            if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
            {
                
                entidadAnim->cargarAnimacion("correr", rutasPersonaje2);
            }
            std::vector<std::string> rutasPersonaje3;
            for (int i = 1; i <= 4; ++i) {
                rutasPersonaje3.push_back("assets/modelos/animaciones/colocarBomba/player" + numeroPersonaje + "bomba" + std::to_string(i) + ".obj");
            }
            if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
            {
                
                entidadAnim->cargarAnimacion("colocarBombas", rutasPersonaje3);
            }

                render.cargadoEnElArbol = true;
            }
            else
            {
                render.SusDatosEnElArbol->setTraslacion(glm::vec3(fisica.x, fisica.y-1, fisica.z));
                render.SusDatosEnElArbol->setRotacion(glm::vec3(0.0f, render.rotacion, 0.0f));
                auto entidad = render.SusDatosEnElArbol->getEntidad();

            if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
            {
                float deltatimeanim = getFixedDeltaTime();
                entidadAnim->actualizarAnimacion(deltatimeanim);
            }
            if (render.animacion == 1 && render.animacionYaIniciada == false)
            {
                if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
                {
                    entidadAnim->reproducirAnimacion("idle");
                    entidadAnim->setFPS(6.0f);
                }
                render.animacionYaIniciada = true;
            }
            if (render.animacion == 2 && render.animacionYaIniciada == false)
            {
                if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
                {
                    entidadAnim->reproducirAnimacion("colocarBombas");
                    entidadAnim->setFPS(12.0f);
                }
                render.animacionYaIniciada = true;
            }
            if (render.animacion == 3 && render.animacionYaIniciada == false)
            {
                if (auto *entidadAnim = dynamic_cast<AlmondEntidadAnimacion *>(entidad))
                {
                    entidadAnim->reproducirAnimacion("correr");
                    entidadAnim->setFPS(12.0f);
                }
                render.animacionYaIniciada = true;
            }

                    if (fisica.vx > 0)
                    {
                       render.rotacion = 0.0f;
                    }
                    if (fisica.vx < 0)
                    {
                        render.rotacion = 180.0f;
                    }
                    if (fisica.vz > 0)
                    {
                        render.rotacion = 90.0f;
                    }
                    if (fisica.vz < 0)
                    {
                        render.rotacion = 270.0f;
                    }

            }
        }
         if (fisica.esFantasma)
        {
            if (render.cargadoEnElArbolFoco == false)
            {
                glm::vec3 posicion2 = glm::vec3(fisica.x, 10.0f, fisica.z); // Posición del foco
                glm::vec3 direccion2 = glm::vec3(0.0f, -1.0f, 0.0f);        // Dirección hacia donde apunta
                glm::vec3 color2 = glm::vec3(1.0f, 1.0f, 0.3f);             // Amarillo cálido potente

                float ambientStrength2 = 1.2f;  // Más fuerte que lo normal
                float diffuseStrength2 = 2.5f;  // Mucho más brillo directo
                float specularStrength2 = 1.2f; // Reflejos más intensos

                float atenuacionConstante2 = 1.0f;
                float atenuacionLineal2 = 0.045f;      // Menor caída con la distancia
                float atenuacionCuadratica2 = 0.0075f; // Luz alcanza más lejos

                float apertura2 = cos(glm::radians(12.5f)); // Borde interior (coseno del ángulo)
                float penumbra2 = cos(glm::radians(17.5f)); // Suavizado de borde exterior (penumbra)

                render.TieneFoco = MotorDeJuegoPropio.CrearLuzFocoPlayer(
                    2, // tipoLuz: 2 = spotlight
                    posicion2,
                    direccion2,
                    color2,
                    ambientStrength2,
                    diffuseStrength2,
                    specularStrength2,
                    atenuacionConstante2,
                    atenuacionLineal2,
                    atenuacionCuadratica2,
                    apertura2,
                    penumbra2);
                render.cargadoEnElArbolFoco = true;
                render.SusDatosEnElArbol->setSepinta(false);
            }
            else
            {

                auto entidad = render.TieneFoco->getEntidad();

                if (auto *entidadLuz = dynamic_cast<AlmondEntidadLuz *>(entidad))
                {
                    // Calculamos la matriz de vista usando lookAt y las variables de la cámara
                    glm::vec3 posicionAlMoverse = glm::vec3(fisica.x, 10.0f, fisica.z); // Posición del foco
                    entidadLuz->setPosicion(posicionAlMoverse);
                }
            }
        }
    }
    else if (e.type == EntityType::POWERUP)
    {
        auto pw = em.powerUps.get(e.powerUp_key);
        if (pw.type == PowerUpType::BOMB_DOWN)
        {
            if (render.cargadoEnElArbol == false)
            {
                const std::string &nombre = "assets/modelos/bombDOWN.obj";
                render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarPersonajesYDevuelve(fisica.x, fisica.y, fisica.z, nombre);
                render.cargadoEnElArbol = true;
                render.SusDatosEnElArbol->setRotacion(glm::vec3(0.0f, 180.0f, 0.0f));
            }
        }
        else if (pw.type == PowerUpType::BOMB_UP)
        {
            if (render.cargadoEnElArbol == false)
            {
                const std::string &nombre = "assets/modelos/bombUP.obj";
                render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarPersonajesYDevuelve(fisica.x, fisica.y, fisica.z, nombre);
                render.cargadoEnElArbol = true;
                render.SusDatosEnElArbol->setRotacion(glm::vec3(0.0f, 180.0f, 0.0f));
            }
        }
        else if (pw.type == PowerUpType::EXP_RANGE_UP)
        {
            if (render.cargadoEnElArbol == false)
            {
                const std::string &nombre = "assets/modelos/rangeUP.obj";
                render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarPersonajesYDevuelve(fisica.x, fisica.y, fisica.z, nombre);
                render.cargadoEnElArbol = true;
                render.SusDatosEnElArbol->setRotacion(glm::vec3(0.0f, 180.0f, 0.0f));
            }
        }
        else if (pw.type == PowerUpType::EXP_RANGE_DOWN)
        {
            if (render.cargadoEnElArbol == false)
            {
                const std::string &nombre = "assets/modelos/rangeDOWN.obj";
                render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarPersonajesYDevuelve(fisica.x, fisica.y, fisica.z, nombre);
                render.cargadoEnElArbol = true;
                render.SusDatosEnElArbol->setRotacion(glm::vec3(0.0f, 180.0f, 0.0f));
            }
        }
        else if (pw.type == PowerUpType::EXP_RANGE_MAX)
        {
            if (render.cargadoEnElArbol == false)
            {
                const std::string &nombre = "assets/modelos/rangeMAX.obj";
                render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarPersonajesYDevuelve(fisica.x, fisica.y, fisica.z, nombre);
                render.cargadoEnElArbol = true;
                render.SusDatosEnElArbol->setRotacion(glm::vec3(0.0f, 180.0f, 0.0f));
            }
        }
        else if (pw.type == PowerUpType::SPEED_UP)
        {
            if (render.cargadoEnElArbol == false)
            {
                const std::string &nombre = "assets/modelos/speedUP.obj";
                render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarPersonajesYDevuelve(fisica.x, fisica.y, fisica.z, nombre);
                render.cargadoEnElArbol = true;
                render.SusDatosEnElArbol->setRotacion(glm::vec3(0.0f, 180.0f, 0.0f));
            }
        }
        else if (pw.type == PowerUpType::SPEED_DOWN)
        {
            if (render.cargadoEnElArbol == false)
            {
                const std::string &nombre = "assets/modelos/speedDOWN.obj";
                render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarPersonajesYDevuelve(fisica.x, fisica.y, fisica.z, nombre);
                render.cargadoEnElArbol = true;
                render.SusDatosEnElArbol->setRotacion(glm::vec3(0.0f, 180.0f, 0.0f));
            }
        }
        else if (pw.type == PowerUpType::PUSH_BOMB)
        {
            if (render.cargadoEnElArbol == false)
            {
                const std::string &nombre = "assets/modelos/pushBOMBS.obj";
                render.SusDatosEnElArbol = MotorDeJuegoPropio.CargarPersonajesYDevuelve(fisica.x, fisica.y, fisica.z, nombre);
                render.cargadoEnElArbol = true;
                render.SusDatosEnElArbol->setRotacion(glm::vec3(0.0f, 180.0f, 0.0f));
            }
        }
    }
    else if (e.type == EntityType::BUTTON)
    {
        MotorDeJuegoPropio.PintarSpriteTextura(button.pathSprite, button.x, button.y, 1.0f, 0.0f, 255, 255, 255, 255);
    }
}

void ElMotorRaylib::dibujaMapa(const std::vector<std::vector<int>> &mapMatrix)
{
    MiVector3 aux{size, size, size};
    MiVector3 aux2{0.0f, 0.0f, 0.0f};

    for (int i = 0; i < mapMatrix.size(); i++) // Filas
    {
        for (int j = 0; j < mapMatrix[i].size(); j++) // Columnas
        {
            if (mapMatrix[i][j] == 1)
            {
                MotorDeJuegoPropio.CargarMapa(aux2.x + (2.0f * j), aux2.y, aux2.z + (2.0f * i), MY_BLACK.r, MY_BLACK.g, MY_BLACK.b, MY_BLACK.a);
            }
        }
    }
    mapaPintado = true; // lo pongo a true para que no lo vuelva a pintar
}

void ElMotorRaylib::dibujaBloquesDestruibles(EntityManager &em, const std::vector<std::vector<int>> &mapMatrix)
{
    MiVector3 aux{size, size, size};
    MiVector3 aux2{0.0f, 0.0f, 0.0f};

    // Obtengo el mapa del sistema
    for (int i = 0; i < mapMatrix.size(); i++) // Filas
    {
        for (int j = 0; j < mapMatrix[i].size(); j++) // Columnas
        {
            if (mapMatrix[i][j] == 2 || mapMatrix[i][j] >= 100)
            {
                // Crea la entidad de bloque destruible
                Entity &bloque = em.createEntity(EntityType::DESTRUIBLE);

                fisicaComponent fisicaBloque{static_cast<float>(j * 2), 0, static_cast<float>(i * 2)};
                MiColor blockColor = (mapMatrix[i][j] >= 100) ? MY_GOLD : MY_BROWN;
                renderComponent renderBloque{size, blockColor};
                powerUpComponent powerBloque{};
                soundComponent soundBloque{};
                bombaComponent bombaBloque{};
                iaComponent bombatieneBT{false};
                buttonComponent buttonBloque{};

                em.createEntityComponents(bloque, fisicaBloque, renderBloque, powerBloque, soundBloque, bombaBloque, bombatieneBT, buttonBloque);
            }
        }
    }
}

void ElMotorRaylib::dibujaHud(float r, EntityManager &em,int contP1,int contP2,int contP3,int contP4) {
    int player = 0;
    std::string fotoBanner = "assets/sprites/hud/banner.png";
    
    for(auto& e : em.getAllEntitiesInGame()){

        std::string fotoPlayer = "assets/sprites/hud/player" + std::to_string(player) + ".png";

        //Pintar Bombas
        auto &fisica = em.fisicas.get(e.fisica_key);
        auto &bomb = em.bombs.get(e.bomb_key); 

        int bombita = bomb.maxBombs - bomb.bombsPlaced;
        if(bombita < 0){
            bombita = 0;
        }

        float logoRotation = 0.0f;             // Sin rotación
        MiColor fondo(255, 255, 255, 255);
        int logoX = 0;
        int logoY = 0;



        int b_Up = 27;
        int b_Down = 525;
        int b_Left = -285;
        int b_Right = 970;
        int win = 0;

        switch (player){
            case 0:
                //Arriba IZQ
                DibujarTextura(fotoBanner,  b_Left, b_Up, 0.3, logoRotation, fondo); 
                DibujarTextura(fotoPlayer,  5, 50, 0.4, logoRotation, fondo); 
                win = contP1;

                break;
            case 1:
                //Abajo DCHA
                DibujarTextura(fotoBanner,  b_Right, b_Down, 0.3, 180, fondo); 
                DibujarTextura(fotoPlayer,  1170, 550, 0.4, logoRotation, fondo); 
                win = contP2;
                break;
            case 2:
                //Abajo IZQ
                DibujarTextura(fotoBanner,  b_Left, b_Down, 0.3, logoRotation, fondo); 
                DibujarTextura(fotoPlayer,  5, 550, 0.4, logoRotation, fondo);
                win = contP3;
                break;
            case 3:
                //Arriba DHCA
                DibujarTextura(fotoBanner,  b_Right, b_Up, 0.3, 180, fondo); 
                DibujarTextura(fotoPlayer,  1170, 50, 0.4, logoRotation, fondo);
                win = contP4;
                break;
        }

        //Pinta bombas----------------------
        int arriba = 42;
        int abajo = 541;
        int izquierda = 45;
        int derecha = 1040;
        int espacio = 60;
        float logoScale = 0.052f;                // Escala predeterminada

        for(int i = win; i<3; i++){
            std::string nombreNumero = "assets/sprites/hud/bombaOFF.png";

            switch (player){
                case 0:
                    logoX = izquierda + (espacio * i);
                    logoY = arriba;
                    break;
                case 1:
                    logoX = derecha - (espacio * i);
                    logoY = abajo;
                    break;
                case 2:
                    logoX = izquierda + (espacio * i);
                    logoY = abajo;
                    break;
                case 3:
                    logoX = derecha - (espacio * i);
                    logoY = arriba;
                    break;
            }
            DibujarTextura(nombreNumero,  logoX, logoY, logoScale, logoRotation, fondo); 
        }

        //Pinta papeles
        for(int i = 0; i<win; i++){
            std::string nombreNumero = "assets/sprites/hud/bombaON.png";

            switch (player){
                case 0:
                    logoX = izquierda + (espacio * i);
                    logoY = arriba;
                    break;
                case 1:
                    logoX = derecha - (espacio * i);
                    logoY = abajo;
                    break;
                case 2:
                    logoX = izquierda + (espacio * i);
                    logoY = abajo;
                    break;
                case 3:
                    logoX = derecha - (espacio * i);
                    logoY = arriba;
                    break;
            }

            DibujarTextura(nombreNumero,  logoX, logoY, logoScale, logoRotation, fondo); 
        }
        player++;
    }

    PintaTiempoJuego(r);
}

void ElMotorRaylib::dibujaHudPintura(float r, EntityManager &em,int contP1,int contP2,int contP3,int contP4, int bloquesTotales) {
    int player = 0;
    std::string fotoBanner = "assets/sprites/hud/banner.png";
    
    for(auto& e : em.getAllEntitiesInGame()){

        std::string fotoPlayer = "assets/sprites/hud/player" + std::to_string(player) + ".png";

        //Pintar Bombas
        auto &fisica = em.fisicas.get(e.fisica_key);
        auto &bomb = em.bombs.get(e.bomb_key); 

        int bombita = bomb.maxBombs - bomb.bombsPlaced;
        if(bombita < 0){
            bombita = 0;
        }

        float logoRotation = 0.0f;             // Sin rotación
        MiColor fondo(255, 255, 255, 255);
        int logoX = 0;
        int logoY = 0;

        int b_Up = 27;
        int b_Down = 525;
        int b_Left = -370;
        int b_Right = 1055;
        int porcentaje = 0;
        char textoNumero[32];

        

        switch (player){
            case 0:
                //Arriba IZQ
                DibujarTextura(fotoBanner,  b_Left, b_Up, 0.3, logoRotation, fondo); 
                DibujarTextura(fotoPlayer,  5, 50, 0.4, logoRotation, fondo); 
                porcentaje = contP1 * 100 / bloquesTotales;

                // Convierte el entero a texto
                snprintf(textoNumero, sizeof(textoNumero), "%d", porcentaje);
                DibujarTextos(textoNumero, 115, 77, 30, MY_MAROON);

                break;
            case 1:
                //Abajo DCHA
                DibujarTextura(fotoBanner,  b_Right, b_Down, 0.3, 180, fondo); 
                DibujarTextura(fotoPlayer,  1170, 550, 0.4, logoRotation, fondo); 
                porcentaje = contP2 * 100 / bloquesTotales;

                // Convierte el entero a texto
                snprintf(textoNumero, sizeof(textoNumero), "%d", porcentaje);
                DibujarTextos(textoNumero, 1085, 575, 30, MY_MAROON);
                break;
            case 2:
                //Abajo IZQ
                DibujarTextura(fotoBanner,  b_Left, b_Down, 0.3, logoRotation, fondo); 
                DibujarTextura(fotoPlayer,  5, 550, 0.4, logoRotation, fondo);
                porcentaje = contP3 * 100 / bloquesTotales;

                // Convierte el entero a texto
                snprintf(textoNumero, sizeof(textoNumero), "%d", porcentaje);
                DibujarTextos(textoNumero, 115, 575, 30, MY_MAROON);
                break;
            case 3:
                //Arriba DHCA
                DibujarTextura(fotoBanner,  b_Right, b_Up, 0.3, 180, fondo); 
                DibujarTextura(fotoPlayer,  1170, 50, 0.4, logoRotation, fondo);
                porcentaje = contP4 * 100 / bloquesTotales;

                // Convierte el entero a texto
                snprintf(textoNumero, sizeof(textoNumero), "%d", porcentaje);
                DibujarTextos(textoNumero, 1085, 77, 30, MY_MAROON);
                break;
        }
        player++;
    }

    PintaTiempoJuego(r);
}

void ElMotorRaylib::gameRender(EntityManager &em,
                               int seleccionActual,
                               float r,
                               MapSystem &mapMatrix,
                               int contadorCuadrosSegundosIniciales,
                               const FisicaSystem &physics,
                               bool perdido,
                               bool &animacion,
                               bool pintura,
                               int contP1,
                               int contP2,
                               int contP3,
                               int contP4
                               )
{
    seleccionBool = false;
    // Renderizar el contenido del juego con Raylib
    ActivaPintadoDebbuger();
    if (!mapaPintado)
    {
        if(pintura){
            dibujaMapa(mapMatrix.getMap());
            pintarDecoraciones();
        }
        else{
            dibujaMapa(mapMatrix.getMap());
            pintarDecoracionesClasico();
        }

    }

    // 1) Obtenemos el vector de todos los players
    auto players = em.getEntitiesByType(EntityType::PLAYER);

    // 2) Para cada jugador: renderizado diferenciado según esté vivo o muerto
    for (auto &player : players)
    {
        auto &fisicaPlayer = em.fisicas.get(player.fisica_key);
        auto &renderPlayer = em.renders.get(player.render_key);

        if (!fisicaPlayer.estaMuerto)
        {
            // Dibujar la entidad normalmente y gestionar stun
            this->dibujaEntidad(em, player, physics);

            if (fisicaPlayer.estuneo)
            {
                fisicaPlayer.tiempoCarga += getFixedDeltaTime();
                float progreso = std::clamp(fisicaPlayer.tiempoCarga / 1.0f, 0.0f, 1.0f);
                auto entidad2 = renderPlayer.TieneFoco->getEntidad();

                        if (auto *entidadLuz2 = dynamic_cast<AlmondEntidadLuz *>(entidad2))
                        {
                            glm::vec3 colorInicio = glm::vec3(1.0f, 1.0f, 0.3f); // amarillo suave
                            glm::vec3 colorFinal  = glm::vec3(1.0f, 0.3f, 0.0f); // rojo-anaranjado

                            // Interpolación entre colorInicio y colorFinal según progreso
                            glm::vec3 colorInterpolado = glm::mix(colorInicio, colorFinal, progreso);

                            entidadLuz2->setIntensidad(colorInterpolado);
                        }

                if (fisicaPlayer.tiempoCarga >= 1.0f)
                    fisicaPlayer.cargaCompletada = true;

                if (fisicaPlayer.cargaCompletada)
                {
                    if (fisicaPlayer.tiempoCarga >= 1.5f)
                    {
                        fisicaPlayer.tiempoCarga = 0.0f;
                        fisicaPlayer.cargaCompletada = false;
                        fisicaPlayer.estuneo = false;
                        auto entidad = renderPlayer.TieneFoco->getEntidad();

                        if (auto *entidadLuz = dynamic_cast<AlmondEntidadLuz *>(entidad))
                        {
                            // Calculamos la matriz de vista usando lookAt y las variables de la cámara
                            glm::vec3 colorEstuneo = glm::vec3(1.0f, 1.0f, 0.3f); // Posición del foco
                            entidadLuz->setIntensidad(colorEstuneo);
                        }

                    }
                }
            }
        }
        else
        {
            // Si está muerto, quitarlo del árbol y limpiar render
            renderPlayer.rotacion = 0.0f;
            renderPlayer.cargadoEnElArbol = false;
            MotorDeJuegoPropio.EliminarPersonaje(renderPlayer.SusDatosEnElArbol);
            renderPlayer.SusDatosEnElArbol = nullptr;
        }
    }

    // 3) Renderizar bombas, destructibles, NPCs y powerups
    em.forEach(EntityType::BOMBA, [this, &em, &physics](const Entity &e)
               { this->dibujaEntidad(em, e, physics); });

    em.forEach(EntityType::DESTRUIBLE, [this, &em, &physics](const Entity &e)
               { this->dibujaEntidad(em, e, physics); });

    em.forEach(EntityType::NPC, [this, &em, &physics](const Entity &e)
               {
    auto &fisicaNPC = em.fisicas.get(e.fisica_key);
    auto &renderNPC = em.renders.get(e.render_key);

    if (!fisicaNPC.estaMuerto)
    {
    this->dibujaEntidad(em, e, physics);

     if (fisicaNPC.estuneo)
            {
                float progreso = std::clamp(fisicaNPC.tiempoCarga / 1.0f, 0.0f, 1.0f);
                auto entidad2 = renderNPC.TieneFoco->getEntidad();

                        if (auto *entidadLuz2 = dynamic_cast<AlmondEntidadLuz *>(entidad2))
                        {
                            glm::vec3 colorInicio = glm::vec3(1.0f, 1.0f, 0.3f); // amarillo suave
                            glm::vec3 colorFinal  = glm::vec3(1.0f, 0.3f, 0.0f); // rojo-anaranjado

                            // Interpolación entre colorInicio y colorFinal según progreso
                            glm::vec3 colorInterpolado = glm::mix(colorInicio, colorFinal, progreso);

                            entidadLuz2->setIntensidad(colorInterpolado);
                        }

                if (fisicaNPC.cargaCompletada)
                {
                    if (fisicaNPC.tiempoCarga >= 1.5f)
                    {
                        auto entidad = renderNPC.TieneFoco->getEntidad();

                        if (auto *entidadLuz = dynamic_cast<AlmondEntidadLuz *>(entidad))
                        {
                            // Calculamos la matriz de vista usando lookAt y las variables de la cámara
                            glm::vec3 colorEstuneo = glm::vec3(1.0f, 1.0f, 0.3f); // Posición del foco
                            entidadLuz->setIntensidad(colorEstuneo);
                        }

                    }
                }
            }
    }
    else
    {
    renderNPC.rotacion = 0.0f;
    renderNPC.cargadoEnElArbol = false;
    MotorDeJuegoPropio.EliminarPersonaje(renderNPC.SusDatosEnElArbol);
    renderNPC.SusDatosEnElArbol = nullptr;
    } });

    em.forEach(EntityType::POWERUP, [this, &em, &physics](const Entity &e)
               { this->dibujaEntidad(em, e, physics); });

    // 4) Opcional: pintar cuadrícula de depuración
    if (pintura)
        pintarCuadrados(mapMatrix);

    float dt = getFixedDeltaTime(); // tu lógica de tiempo entre frames
    for (auto& parti : particulas) {
            parti.actualizarParticulas(dt);
    }
    // 5) Recorrer y renderizar el árbol de juego
    MotorDeJuegoPropio.RecorrerArbolDeJuego();
    for(auto &parti : particulas ){
        MotorDeJuegoPropio.pintarParticula(parti);
    }

    // 6) UI y HUD
    ImGuiRendeUI(em, mapMatrix, physics);
    //dibujaBarraMoveOrDie(em);
    dibujaTiempoInicial(contadorCuadrosSegundosIniciales, animacion);
    if(!pintura){
        dibujaHud(r, em,contP1,contP2,contP3,contP4);
        mostrarInstrucciones(animacion, 0); //0 es la imagen de las explicacion del modo clásico
    }
    else{
        dibujaHudPintura(r,em,contP1,contP2,contP3,contP4,113);//MANU
        mostrarInstrucciones(animacion, 1); //1 es la imagen de las explicacion del modo pintura

    }
}

void ElMotorRaylib::pintarCuadrados(MapSystem &mapMatrix)
{
    auto mapaPintar = mapMatrix.getColorMap();
    MiVector3 aux{size, size, size};
    MiVector3 aux2{0.0f, 0.0f, 0.0f};

    for (int i = 0; i < mapaPintar.size(); i++)
    {
        for (int j = 0; j < mapaPintar[i].size(); j++)
        {
            if (mapaPintar[i][j] == 1)
            {
                const std::string &nombre = "assets/modelos/pinturaProvisional3.obj";
                MotorDeJuegoPropio.CargarPintura(aux2.x + (2.0f * j), aux2.y - 1, aux2.z + (2.0f * i), nombre);
            }
            else if (mapaPintar[i][j] == 2)
            {
                const std::string &nombre = "assets/modelos/pinturaProvisional2.obj";
                MotorDeJuegoPropio.CargarPintura(aux2.x + (2.0f * j), aux2.y - 1, aux2.z + (2.0f * i), nombre);
            }
            else if (mapaPintar[i][j] == 3)
            {
                const std::string &nombre = "assets/modelos/pinturaProvisional4.obj";
                MotorDeJuegoPropio.CargarPintura(aux2.x + (2.0f * j), aux2.y - 1, aux2.z + (2.0f * i), nombre);
            }
            else if (mapaPintar[i][j] == 4)
            {
                const std::string &nombre = "assets/modelos/pinturaProvisional.obj";
                MotorDeJuegoPropio.CargarPintura(aux2.x + (2.0f * j), aux2.y - 1, aux2.z + (2.0f * i), nombre);
            }
        }
    }
}

void ElMotorRaylib::configurarCamara()
{
    glm::vec3 camPosition = glm::vec3(2.0f, 10.0f, 10.0f);
    glm::vec3 camtarget = glm::vec3(2.0f, 0.0f, 2.0f);
    glm::vec3 camup = glm::vec3(0.0f, 1.0f, 0.0f);
    MotorDeJuegoPropio.CrearCamara(camtarget, camup, 65.0f, true, camPosition);
}

void ElMotorRaylib::iniciarMotor()
{
    if (iniciadoYa == false)
    {
        iniciarCrono();
        iniciadoYa = true;
        MotorDeJuegoPropio.SetTargetFPS(60);
        configurarCamara();
        ConfigurarLuces();
        // para pintar debugger
        pintaDebugger = true;

        //particulas
        crearParticulasGame();
    }
}

void ElMotorRaylib::menuRender(EntityManager &em, const FisicaSystem &physics)
{
    const std::string &nombre = "assets/sprites/fondoMenu.png"; // Nombre de la textura de fondo
    MiColor fondo(255, 255, 255, 255);
    DibujarTextura(nombre, 0, 0, 1.0f, 0.0f, fondo); // Dibuja la textura de fondo
    em.forEach(EntityType::BUTTON, [this, &em, &physics](const Entity &e)
               { this->dibujaEntidad(em, e, physics); });
}

void ElMotorRaylib::opcionesRender(EntityManager &em, const FisicaSystem &physics)
{
    const std::string &nombre = "assets/sprites/opcionesFondo.png"; // Nombre de la textura de fondo
    MiColor fondo(255, 255, 255, 255);
    DibujarTextura(nombre, 0, 0, 1.0f, 0.0f, fondo); // Dibuja la textura de fondo
    em.forEach(EntityType::BUTTON, [this, &em, &physics](const Entity &e)
               { this->dibujaEntidad(em, e, physics); });
}


void ElMotorRaylib::winRender(EntityManager &em, const FisicaSystem &physics)
{
    const std::string &nombre = "assets/sprites/winFondo.png"; // Nombre de la textura de fondo
    MiColor fondo(255, 255, 255, 255);
    DibujarTextura(nombre, 0, 0, 1.0f, 0.0f, fondo); // Dibuja la textura de fondo
    em.forEach(EntityType::BUTTON, [this, &em, &physics](const Entity &e)
               { this->dibujaEntidad(em, e, physics); });
}

void ElMotorRaylib::controlesRender(EntityManager &em, const FisicaSystem &physics)
{
    const std::string &nombre = "assets/sprites/controlesFondo2.png"; // Nombre de la textura de fondo
    MiColor fondo(255, 255, 255, 255);
    DibujarTextura(nombre, 0, 0, 1.0f, 0.0f, fondo); // Dibuja la textura de fondo
    em.forEach(EntityType::BUTTON, [this, &em, &physics](const Entity &e)
               { this->dibujaEntidad(em, e, physics); });
}

void ElMotorRaylib::gameOverRender(EntityManager &em, const FisicaSystem &physics)
{
    const std::string &nombre = "assets/sprites/gameOverFondo.png"; // Nombre de la textura de fondo
    MiColor fondo(255, 255, 255, 255);
    DibujarTextura(nombre, 0, 0, 1.0f, 0.0f, fondo); // Dibuja la textura de fondo
    em.forEach(EntityType::BUTTON, [this, &em, &physics](const Entity &e)
               { this->dibujaEntidad(em, e, physics); });
}


void ElMotorRaylib::PintaTiempoJuego(float r)
{
    const std::string &nombre = "assets/sprites/hud/timer.png";
    MiColor fondo(255, 255, 255, 255);


    // Convierte el tiempo restante en minutos y segundos
    int minutosRestantes = (int)r / 60;
    int segundosRestantes = (int)r % 60;

    // Formatea el texto del tiempo
    char textoTiempo[64];
    // snprintf(textoTiempo, sizeof(textoTiempo), "Tiempo de JUEGO: %02d:%02d", minutosRestantes, segundosRestantes);
    snprintf(textoTiempo, sizeof(textoTiempo), "%02d:%02d", minutosRestantes, segundosRestantes);

    // Dibuja el texto en pantalla
    DibujarTextura(nombre,540, -75, 0.8, 0, fondo); 
    DibujarTextos(textoTiempo, 550, 10, 20, MY_RED);
}

void ElMotorRaylib::DibujarCubo(MiVector3 position, double width, double height, double length, MiColor color)
{
    MotorDeJuegoPropio.DibujarCubos(position.x, position.y, position.z, width, height, length, color.r, color.g, color.b, color.a);
}

void ElMotorRaylib::DibujarTextos(std::string texto, int x, int y, int TamanyoFuente, MiColor color)
{
    MotorDeJuegoPropio.DibujarTextosPropios(texto, x, y);
}

void ElMotorRaylib::DibujarRectangulo(int posX, int posY, int width, int height, MiColor color)
{
    MotorDeJuegoPropio.dibujarRectanguloConColor(static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(width), static_cast<float>(height), color.r, color.g, color.b, color.a);
}

int ElMotorRaylib::GetAlturaPantalla()
{
    return height_;
}

int ElMotorRaylib::GetAnchuraPantalla()
{
    return width_;
}

void ElMotorRaylib::introRender(float alpha, bool pantallasDeCarga)
{
    if(pantallasDeCarga == false){
        MiColor colorFondo(217, 181, 137, 255);
        MotorDeJuegoPropio.limpiarPantalla(colorFondo.r, colorFondo.g, colorFondo.b, colorFondo.a);

        const std::string &nombre = "assets/sprites/almondLogo.png";
        int logoX = -1;            // Centrado horizontalmente
        int logoY = -1;            // Posición vertical
        float logoScale = 1.0f;    // Escala predeterminada
        float logoRotation = 0.0f; // Sin rotación

        // // Dibujar la textura con transparencia controlada por alpha
        MiColor colorLogo(255, 255, 255, static_cast<int>(alpha * 255)); // Transparencia basada en alpha

        DibujarTextura(nombre, logoX, logoY, logoScale, logoRotation, colorLogo);
    }
    if(pantallasDeCarga == true){
        MiColor colorFondo2(43,14,0,255);
        MotorDeJuegoPropio.limpiarPantalla(colorFondo2.r, colorFondo2.g, colorFondo2.b, colorFondo2.a);
        const std::string &nombre2 = "assets/sprites/PantallasDeCarga/pantallaDeCarga1.png";
        int logoX = -1;            // Centrado horizontalmente
        int logoY = -1;            // Posición vertical
        float logoScale = 1.0f;    // Escala predeterminada
        float logoRotation = 0.0f; // Sin rotación
        MiColor fondo(255, 255, 255, 255);
        DibujarTextura(nombre2, logoX, logoY, logoScale, logoRotation, fondo);
        const std::string &nombre3 = "assets/sprites/PantallasDeCarga/cargandoPantalla.png";
        DibujarTextura(nombre3, 100, 20, logoScale, logoRotation, fondo);
        const std::string &nombre4 = "assets/sprites/PantallasDeCarga/cargandoPantallaPunto.png";
        tiempoPantallaDeCargaPuntos += 1;
        if(tiempoPantallaDeCargaPuntos >= 1){
            DibujarTextura(nombre4, 500, 50, logoScale, logoRotation, fondo);
        }
        if(tiempoPantallaDeCargaPuntos >= 2){
            DibujarTextura(nombre4, 530, 50, logoScale, logoRotation, fondo);
        }
        if(tiempoPantallaDeCargaPuntos >= 3){
            DibujarTextura(nombre4, 560, 50, logoScale, logoRotation, fondo);
        } 
        if(tiempoPantallaDeCargaPuntos >= 4){
            tiempoPantallaDeCargaPuntos = 0;
        }
    }
}

void ElMotorRaylib::dibujaTiempoInicial(int tiempo, bool animacion)
{
    if (cargado == false)
    {
        cargado = true;
    }
    MiColor colorPersonalizado(217, 181, 137, 255);

    if (animacion == false)
    {
        if (tiempo <= 60)
        {

            const std::string &nombre = "assets/sprites/tres.png";
            int logoX = -1;            // Centrado horizontalmente
            int logoY = -1;            // Posición vertical
            float logoScale = 1.0f;    // Escala predeterminada
            float logoRotation = 0.0f; // Sin rotación

            DibujarTextura(nombre, logoX, logoY, logoScale, logoRotation, colorPersonalizado);
        }
        else if (tiempo <= 120 && tiempo >= 61)
        {
            const std::string &nombre = "assets/sprites/dos.png";
            int logoX = -1;            // Centrado horizontalmente
            int logoY = -1;            // Posición vertical
            float logoScale = 1.0f;    // Escala predeterminada
            float logoRotation = 0.0f; // Sin rotación

            DibujarTextura(nombre, logoX, logoY, logoScale, logoRotation, colorPersonalizado);
        }
        else if (tiempo <= 179 && tiempo >= 121)
        {
            const std::string &nombre = "assets/sprites/uno.png";
            int logoX = -1;            // Centrado horizontalmente
            int logoY = -1;            // Posición vertical
            float logoScale = 1.0f;    // Escala predeterminada
            float logoRotation = 0.0f; // Sin rotación

            DibujarTextura(nombre, logoX, logoY, logoScale, logoRotation, colorPersonalizado);
        }
    }
}

void ElMotorRaylib::dibujaBarraMoveOrDie(EntityManager &em)
{
    em.forEach(EntityType::PLAYER, [this, &em](const Entity &e)
               {
                   auto &fisica = em.fisicas.get(e.fisica_key);
                   auto &bomb = em.bombs.get(e.bomb_key); 

                   int bombita = bomb.maxBombs - bomb.bombsPlaced;
                   if(bombita < 0){
                    bombita = 0;
                   }
                   // Ruta de la imagen con el número de bombas restantes
                   std::string nombreNumero = "assets/sprites/Tiempo/" + std::to_string(bombita) + "Numeros.png";
                   int logoX = 50;             // Centrado horizontalmente
                   int logoY = 90;                       // Posición vertical
                   float logoScale = 1.0f;                // Escala predeterminada
                   float logoRotation = 0.0f;             // Sin rotación
                   MiColor fondo(255, 255, 255, 255);
       
                   DibujarTextura(nombreNumero,  logoX, logoY, logoScale, logoRotation, fondo); });
}

void ElMotorRaylib::ActivaPintadoDebbuger()
{
    if (glfwGetKey(MotorDeJuegoPropio.getWindow(), GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (!teclaQ_presionada)
        {
            teclaQ_presionada = true; // Marcar que la tecla está presionada

            // Alternar el estado de pintaDebugger y show_debugger_window
            pintaDebugger = !pintaDebugger;
            show_debugger_window = !show_debugger_window;
        }
    }
    else if (glfwGetKey(MotorDeJuegoPropio.getWindow(), GLFW_KEY_Q) == GLFW_RELEASE)
    {
        teclaQ_presionada = false; // Restablecer el estado cuando la tecla se suelte
    }
}

void ElMotorRaylib::mostrarInstrucciones(bool &ani, const int imagen)
{

    if (animacion2 == false)
    {
        if(imagen == 0){
            DibujarTextura("assets/sprites/explicacionClasico.png", -1, -1, 1.0f, 0.0f, MY_WHITE);
        }
        else if(imagen == 1){
            DibujarTextura("assets/sprites/explicacionPintura.png", -1, -1, 1.0f, 0.0f, MY_WHITE);
        }

    }

    if (glfwGetKey(MotorDeJuegoPropio.getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        animacion2 = true;
    }
    if (animacion2 == true)
    {
        animacionInicio(ani);
    }
}

void ElMotorRaylib::resetAnimacion()
{
    animacion2 = false;
    MotorDeJuegoPropio.setTilesActivos(false);
}

void ElMotorRaylib::animacionInicio(bool &animacion)
{
    static float tiempoTranscurrido = 0.0f;      // Tiempo acumulado para la animación
    static const float duracionAnimacion = 2.0f; // Duración total de la animación en segundos

    if (animacion)
    {
        // Posiciones iniciales y finales de la cámara
        MiVector3 posicionInicial = {2.0f, 10.0f, 10.0f};
        MiVector3 posicionFinal = {12.0f, 30.0f, 15.0f};
        MiVector3 objetivoInicial = {2.0f, 0.0f, 2.0f};
        MiVector3 objetivoFinal = {12.0f, 0.0f, 8.5f};

        // Incrementar el tiempo transcurrido
        tiempoTranscurrido += getFixedDeltaTime();
        if (tiempoTranscurrido > duracionAnimacion)
        {
            tiempoTranscurrido = duracionAnimacion;
        }

        // Calcular el progreso de la animación (de 0.0 a 1.0)
        float progreso = tiempoTranscurrido / duracionAnimacion;

        // Interpolación lineal para la posición de la cámara
        float xc = posicionInicial.x + progreso * (posicionFinal.x - posicionInicial.x);
        float yc = posicionInicial.y + progreso * (posicionFinal.y - posicionInicial.y);
        float zc = posicionInicial.z + progreso * (posicionFinal.z - posicionInicial.z);

        // Interpolación lineal para el objetivo de la cámara
        float txc = objetivoInicial.x + progreso * (objetivoFinal.x - objetivoInicial.x);
        float tyc = objetivoInicial.y + progreso * (objetivoFinal.y - objetivoInicial.y);
        float tzc = objetivoInicial.z + progreso * (objetivoFinal.z - objetivoInicial.z);

        glm::vec3 camPosition = glm::vec3(xc, yc, zc);
        glm::vec3 camtarget = glm::vec3(txc, tyc, tzc);
        glm::vec3 camup = glm::vec3(0.0f, 1.0f, 0.0f);
        MotorDeJuegoPropio.ModificarCamara(camtarget, camup, 65.0f, true, camPosition);

        // Si la animación ha terminado
        if (tiempoTranscurrido >= duracionAnimacion)
        {
            animacion = false;         // Detener la animación
            tiempoTranscurrido = 0.0f; // Reiniciar el tiempo para futuras animaciones

            glm::vec3 camPosition = glm::vec3(12.0f, 30.0f, 15.0f); // Más alto y un poco más lejos
            glm::vec3 camtarget = glm::vec3(12.0f, 0.0f, 8.5f);     // Mantener el foco en el escenario
            glm::vec3 camup = glm::vec3(0.0f, 1.0f, 0.0f);

            MotorDeJuegoPropio.ModificarCamara(camtarget, camup, 65.0f, true, camPosition);
            // hago que los tiles ya no se calculen, porque se ve todo el mapa
            MotorDeJuegoPropio.setTilesActivos(true);
        }
    }
}

// Devuelve la posición del mouse
MiVector2 ElMotorRaylib::ObtenerPosicionMouse()
{
    double x, y;
    glfwGetCursorPos(MotorDeJuegoPropio.getWindow(), &x, &y); // 'window' es el puntero a tu ventana de GLFW

    //obtengo el tamaño actual de la pantalla
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(MotorDeJuegoPropio.getWindow(), &windowWidth, &windowHeight);

    float logicalWidth = GetAnchuraPantalla();
    float logicalHeight = GetAlturaPantalla();

    float scaleX = logicalWidth / windowWidth;
    float scaleY = logicalHeight / windowHeight;

    float logicalMouseX = x * scaleX;
    float logicalMouseY = y * scaleY;

    return {logicalMouseX, logicalMouseY};    // Convierte a MiVector2
}

// Verifica si un botón del mouse está siendo presionado
bool ElMotorRaylib::BotonMousePresionado(int boton)
{
    return glfwGetMouseButton(MotorDeJuegoPropio.getWindow(), boton) == GLFW_PRESS;
}

// Verifica si un botón del mouse está siendo soltado
bool ElMotorRaylib::BotonMouseSoltado(int boton)
{
    return glfwGetMouseButton(MotorDeJuegoPropio.getWindow(), boton) == GLFW_RELEASE;
}

void ElMotorRaylib::DibujarTextura(const std::string &nombre, float x, float y, float escala, float rotacion, MiColor color)
{
    MotorDeJuegoPropio.PintarSpriteTextura(nombre, x, y, escala, rotacion, color.r, color.g, color.b, color.a);
}

void ElMotorRaylib::ImGuiRendeUI(EntityManager &em, MapSystem &map, FisicaSystem const &fisicasys)
{
    static int fps_i{60};
    static bool show_fps_window = false;
    static bool show_entities_window = false;
    static bool show_map_window = false;
    static bool show_fisicas = false;
    static bool show_paint_map_window = false;
    static bool show_ia = false;

    const std::vector<std::vector<int>> &mapMatrix = map.getMap();
    const std::vector<std::vector<int>> &paintMatrix = map.getColorMap();

    // **Iniciar un nuevo frame de ImGui (debes llamarlo aquí)**
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (show_debugger_window)
    {
        // Ventana principal
        ImGui::SetNextWindowPos(ImVec2(GetAnchuraPantalla() + 940, 10), ImGuiCond_Once);
        // Aumentamos el tamaño de la ventana para incluir la información del gamepad
        ImGui::SetNextWindowSize(ImVec2(340, 250), ImGuiCond_Once);

        if (ImGui::Begin("Debugger", &show_debugger_window))
        {
            // --- Sección de depuración del gamepad ---
            if (glfwJoystickPresent(GLFW_JOYSTICK_1))
            {
                const char *joyName = glfwGetJoystickName(GLFW_JOYSTICK_1);
                if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
                    ImGui::Text("Gamepad detectado: %s", joyName);
                else
                    ImGui::Text("Joystick detectado: %s (No es gamepad)", joyName);
            }
            else
            {
                ImGui::Text("No se detecta ningún joystick.");
            }

            // Opciones para activar/desactivar ventanas adicionales
            ImGui::Checkbox("Mostrar FPS", &show_fps_window);
            ImGui::Checkbox("Mostrar Entidades", &show_entities_window);
            ImGui::Checkbox("Mostrar Mapa", &show_map_window);
            ImGui::Checkbox("Mostrar Mapa de Pintura", &show_paint_map_window);
            ImGui::Checkbox("Mostrar Fisicas", &show_fisicas);
            ImGui::Checkbox("Activar MoveOrDie", &enableMoveOrDie);
            ImGui::Checkbox("Pausar juego", &PausarJuego);
            ImGui::Checkbox("Mostrar IA", &show_ia);
        }
        ImGui::End();


        std::ifstream archivo("assets/ia.json");
        nlohmann::json datos;
        archivo >> datos;
        archivo.close();

        if (show_ia)
        {
            iaManager managerdeia;

            static int current_entity_index = 0;
            auto entities = em.getEntitiesByType(EntityType::NPC); // Obtener las entidades desde el EntityManager.

            // Validar que el índice actual esté dentro del rango de las entidades disponibles.
            if (current_entity_index >= static_cast<int>(entities.size()))
            {
                current_entity_index = static_cast<int>(entities.size()) - 1;
            }

            if (!entities.empty() && current_entity_index >= 0)
            {
                ImGui::SetNextWindowPos(ImVec2(GetAnchuraPantalla() - 350, 330), ImGuiCond_Once); // Posición de la ventana.
                ImGui::SetNextWindowSize(ImVec2(340, 350), ImGuiCond_Once);                   // Tamaño de la ventana.

                if (ImGui::Begin("Ventana de Editor IA", &show_ia))
                {
                    // Navegación entre entidades
                    if (ImGui::Button("Anterior"))
                    {
                        if (current_entity_index > 0)
                        {
                            current_entity_index--; // Ir a la entidad anterior.
                        }
                        else
                        {
                            current_entity_index = static_cast<int>(entities.size()) - 1; // Ir al último.
                        }
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Siguiente"))
                    {
                        if (current_entity_index < static_cast<int>(entities.size()) - 1)
                        {
                            current_entity_index++; // Ir a la siguiente entidad.
                        }
                        else
                        {
                            current_entity_index = 0; // Ir al primero.
                        }
                    }

                    ImGui::Separator();
                    // Obtener la entidad actual
                    const auto &entity = entities[current_entity_index];
                    ImGui::Text("Mostrando entidad %d de %d", current_entity_index + 1, static_cast<int>(entities.size()));
                    ImGui::Separator();

                    // Mostrar detalles basados en el tipo
                    ImGui::BulletText("ID: %d", entity.id);

                    static bool new_tieneBT = false;          
                    ImGui::Checkbox("Tiene BT", &new_tieneBT);
                    ImGui::Text("Tiene BT vale %d", new_tieneBT);

                    static float new_rasgo1 = 0.0f;         
                    ImGui::InputFloat("Rasgo1", &new_rasgo1, 0.1f, 1000.0f, "%.2f"); 
                    ImGui::Text("Rasgo1 %d", new_rasgo1);

                    static float new_rasgo2 = 0.0f;         
                    ImGui::InputFloat("Rasgo2", &new_rasgo2, 0.1f, 1000.0f, "%.2f"); 
                    ImGui::Text("Rasgo2 %d", new_rasgo2);

                    static float new_rasgo3 = 0.0f;         
                    ImGui::InputFloat("Rasgo3", &new_rasgo3, 0.1f, 1000.0f, "%.2f"); 
                    ImGui::Text("Rasgo3 %d", new_rasgo3);

                    static float new_tozudez = 0.0f;         
                    ImGui::InputFloat("Tozudez", &new_tozudez, 0.1f, 1.0f, "%.2f"); 
                    ImGui::Text("Todzudez %d", new_tozudez);


                    if (ImGui::Button("Guardar json y reiniciar"))
                    {
                        auto &iaComponent = em.ia.get(entities[current_entity_index].ia_key);
                        iaComponent.tieneBT = new_tieneBT;
                        iaComponent.rasgo1 = new_rasgo1;
                        iaComponent.rasgo2 = new_rasgo2;
                        iaComponent.rasgo3 = new_rasgo3;
                        iaComponent.tozudez = new_tozudez;

                        if (datos.contains("npcs") && datos["npcs"].is_array() && !datos["npcs"].empty())
                        {
                            datos["npcs"][current_entity_index]["tieneBT"] = iaComponent.tieneBT;
                            datos["npcs"][current_entity_index]["rasgo1"] = iaComponent.rasgo1;
                            datos["npcs"][current_entity_index]["rasgo2"] = iaComponent.rasgo2;
                            datos["npcs"][current_entity_index]["rasgo3"] = iaComponent.rasgo3;
                            datos["npcs"][current_entity_index]["tozudez"] = iaComponent.tozudez;
                        }
                        managerdeia.GuardarJSON(datos, "assets/ia.json");
                        managerdeia.cargarNPC1("assets/ia.json", em, entities[0]/*, entities[1], entities[2]*/);
                    }
                }
                ImGui::End();
            }
            else
            {
                // Si no hay entidades o el índice es inválido, mostrar un mensaje.
                ImGui::SetNextWindowPos(ImVec2(GetAnchuraPantalla() - 350, 330), ImGuiCond_Once); // Posición de la ventana.
                ImGui::SetNextWindowSize(ImVec2(340, 100), ImGuiCond_Once);                   // Tamaño de la ventana.

                if (ImGui::Begin("Ventana de Entidades", &show_entities_window))
                {
                    ImGui::Text("No hay entidades disponibles.");
                }
                ImGui::End();
            }  
        }
        // Si la ventana del mapa está activada, dibujar la matriz
        if (show_map_window)
        {
            // Crear una ventana más grande para el mapa
            ImGui::SetNextWindowPos(ImVec2(10, GetAlturaPantalla() + 410), ImGuiCond_Once); // Posición: lado izquierdo abajo
            ImGui::SetNextWindowSize(ImVec2(420, 400), ImGuiCond_Once);                   // Tamaño más grande de la ventana

            ImGui::Begin("Mapa");

            // Dibujar la matriz como una tabla editable
            ImGui::Text("Mapa del juego:");

            static int selectedX = -1, selectedY = -1; // Coordenadas de la celda seleccionada
            static int newValue = 0;                   // Nuevo valor a asignar

            // Usar BeginTable para crear una tabla
            if (ImGui::BeginTable("MapTable", mapMatrix[0].size(), ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY))
            {
                for (size_t i = 0; i < mapMatrix.size(); ++i)
                {
                    ImGui::TableNextRow(); // Mover al siguiente renglón
                    for (size_t j = 0; j < mapMatrix[i].size(); ++j)
                    {
                        ImGui::TableSetColumnIndex(j); // Colocar el índice de la columna

                        // Crear un identificador único para cada celda
                        std::string cellLabel = "Cell[" + std::to_string(i) + "][" + std::to_string(j) + "]";

                        // Mostrar el valor actual como texto clicable
                        if (ImGui::Selectable((std::to_string(mapMatrix[i][j]) + "##" + cellLabel).c_str(), selectedX == j && selectedY == i))
                        {
                            // Al hacer clic, guardar la posición seleccionada y el valor actual
                            selectedX = j;
                            selectedY = i;
                            newValue = mapMatrix[i][j];
                        }
                    }
                }
                ImGui::EndTable(); // Finalizar la tabla
            }

            // Si hay una celda seleccionada, mostrar un popup para cambiar su valor
            if (selectedX != -1 && selectedY != -1)
            {
                ImGui::OpenPopup("Editar Celda"); // Abrir el popup

                if (ImGui::BeginPopup("Editar Celda"))
                {
                    ImGui::Text("Editar posición [%d, %d]:", selectedY, selectedX);
                    ImGui::InputInt("Nuevo Valor", &newValue);

                    if (ImGui::Button("Aplicar"))
                    {
                        // Usar setTile para actualizar la matriz
                        map.setTile(selectedX, selectedY, newValue);
                        selectedX = -1; // Reiniciar selección
                        selectedY = -1;
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Cancelar"))
                    {
                        selectedX = -1; // Reiniciar selección
                        selectedY = -1;
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }
            }

            ImGui::End(); // Finalizar la ventana del mapa
        }
        if (show_paint_map_window)
        {
            // Obtener la matriz del mapa de pintura

            ImGui::SetNextWindowPos(ImVec2(10, GetAlturaPantalla() + 410), ImGuiCond_Once); // Ubicación a la derecha del mapa normal
            ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);

            ImGui::Begin("Mapa de Pintura");

            ImGui::Text("Mapa de Pintura:");

            // Coordenadas de selección de celda
            static int selectedX = -1, selectedY = -1;
            static int newValue = 0;

            if (ImGui::BeginTable("PaintTable", paintMatrix[0].size(), ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY))
            {
                for (size_t i = 0; i < paintMatrix.size(); ++i)
                {
                    ImGui::TableNextRow();
                    for (size_t j = 0; j < paintMatrix[i].size(); ++j)
                    {
                        ImGui::TableSetColumnIndex(j);

                        std::string cellLabel = "PaintCell[" + std::to_string(i) + "][" + std::to_string(j) + "]";

                        if (ImGui::Selectable((std::to_string(paintMatrix[i][j]) + "##" + cellLabel).c_str(), selectedX == j && selectedY == i))
                        {
                            selectedX = j;
                            selectedY = i;
                            newValue = paintMatrix[i][j];
                        }
                    }
                }
                ImGui::EndTable();
            }

            // Si hay una celda seleccionada, permitir edición
            if (selectedX != -1 && selectedY != -1)
            {
                ImGui::OpenPopup("Editar Celda Pintura");

                if (ImGui::BeginPopup("Editar Celda Pintura"))
                {
                    ImGui::Text("Editar posición [%d, %d]:", selectedY, selectedX);
                    ImGui::InputInt("Nuevo Valor", &newValue);

                    if (ImGui::Button("Aplicar"))
                    {
                        map.setTileColor(selectedX, selectedY, newValue); // Función que debes implementar en MapSystem
                        selectedX = -1;
                        selectedY = -1;
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Cancelar"))
                    {
                        selectedX = -1;
                        selectedY = -1;
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }
            }

            ImGui::End();
        }

        if (show_fps_window)
        {
            ImGui::SetNextWindowPos(ImVec2(GetAnchuraPantalla() + 940, 220), ImGuiCond_Once); // Debajo de la ventana principal
            ImGui::SetNextWindowSize(ImVec2(340, 150), ImGuiCond_Once);
            ImGui::Begin("Ventana de FPS", &show_fps_window);           // Pasamos el puntero para que cierre la ventana con el botón de cerrar
            ImGui::Text("FPS Actual: %.0f", MotorDeJuegoPropio.fpsReal); 
            ImGui::Text("DeltaTime: %.4f s", MotorDeJuegoPropio.deltaTime);
            ImGui::Text("Objetivo FPS: %d", fps_i);

            // Control del target FPS
            ImGui::SliderInt("FPS", &fps_i, 1, 100);
            MotorDeJuegoPropio.SetTargetFPS(fps_i);
            ImGui::Separator();
            bool vsync = MotorDeJuegoPropio.IsVSyncActivo();
            if (ImGui::Checkbox("V-Sync Activado", &vsync))
            {
                MotorDeJuegoPropio.SetVSync(vsync);
            }
            ImGui::End();
        }

        // Ventana de Entidades
        if (show_entities_window)
        {
            static int current_entity_index = 0;
            auto entities = em.getEntities(); // Obtener las entidades desde el EntityManager.

            // Validar que el índice actual esté dentro del rango de las entidades disponibles.
            if (current_entity_index >= static_cast<int>(entities.size()))
            {
                current_entity_index = static_cast<int>(entities.size()) - 1;
            }

            if (!entities.empty() && current_entity_index >= 0)
            {
                ImGui::SetNextWindowPos(ImVec2(GetAnchuraPantalla() - 350, 330), ImGuiCond_Once); // Posición de la ventana.
                ImGui::SetNextWindowSize(ImVec2(340, 350), ImGuiCond_Once);                   // Tamaño de la ventana.

                if (ImGui::Begin("Ventana de Entidades", &show_entities_window))
                {
                    // Navegación entre entidades
                    if (ImGui::Button("Anterior"))
                    {
                        if (current_entity_index > 0)
                        {
                            current_entity_index--; // Ir a la entidad anterior.
                        }
                        else
                        {
                            current_entity_index = static_cast<int>(entities.size()) - 1; // Ir al último.
                        }
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Siguiente"))
                    {
                        if (current_entity_index < static_cast<int>(entities.size()) - 1)
                        {
                            current_entity_index++; // Ir a la siguiente entidad.
                        }
                        else
                        {
                            current_entity_index = 0; // Ir al primero.
                        }
                    }

                    ImGui::Separator();
                    // Obtener la entidad actual
                    const auto &entity = entities[current_entity_index];
                    ImGui::Text("Mostrando entidad %d de %d", current_entity_index + 1, static_cast<int>(entities.size()));
                    ImGui::Separator();

                    // Mostrar detalles basados en el tipo
                    ImGui::BulletText("ID: %d", entity.id);
                    if (entity.type == EntityType::PLAYER)
                    {
                        ImGui::BulletText("Tipo: Jugador");
                        auto &fisica = em.fisicas.get(entity.fisica_key);
                        ImGui::BulletText("Vida: %.2f", fisica.vida);
                        auto &bomb = em.bombs.get(entity.bomb_key);
                        ImGui::BulletText("Rango: %d", bomb.expRange);
                        ImGui::BulletText("Bombas: %d", bomb.maxBombs);
                        ImGui::BulletText("Invulnerabilidad: %.2f", fisica.invulnerabilidad);
                        
                        // Mostrar las posiciones X y Z
                        ImGui::Separator();
                        ImGui::BulletText("PosX: %.2f, PosZ: %.2f", fisica.x, fisica.z);
                    }
                    else if (entity.type == EntityType::NPC)
                    {
                        ImGui::BulletText("Tipo: NPC");
                        auto &fisica = em.fisicas.get(entity.fisica_key);
                        ImGui::BulletText("Vida: %.2f", fisica.vida);
                        auto &bomb = em.bombs.get(entity.bomb_key);
                        ImGui::BulletText("Rango: %d", bomb.expRange);
                        ImGui::BulletText("Bombas: %d", bomb.maxBombs);
                        ImGui::BulletText("Invulnerabilidad: %.2f", fisica.invulnerabilidad);
                        // Mostrar las posiciones X y Z
                        ImGui::Separator();
                        ImGui::BulletText("PosX: %.2f, PosZ: %.2f", fisica.x, fisica.z);
                    }
                    else if (entity.type == EntityType::DESTRUIBLE)
                    {
                        ImGui::BulletText("Tipo: Destruible");
                    }
                    else if (entity.type == EntityType::BOMBA)
                    {
                        ImGui::BulletText("Tipo: Bomba");
                        auto &fisica = em.fisicas.get(entity.fisica_key);
                        auto &bomb = em.bombs.get(entity.bomb_key);
                        ImGui::BulletText("Rango: %d", bomb.expRange);
                        ImGui::BulletText("Propietario: %d", bomb.ownerId);
                        ImGui::Separator();
                        float deltaTime = getFixedDeltaTime();
                        float aux = bomb.lifetime - deltaTime;
                        ImGui::BulletText("Tiempo para que explote: %.2f", aux);
                        // Mostrar las posiciones X y Z
                        ImGui::BulletText("PosX: %.2f, PosZ: %.2f", fisica.x, fisica.z);
                    }
                    else if (entity.type == EntityType::POWERUP)
                    {
                        ImGui::BulletText("Tipo: PowerUp");
                    }
                    else if (entity.type == EntityType::SOUND)
                    {
                        ImGui::BulletText("Tipo: Sonido");
                    }
                    else
                    {
                        ImGui::BulletText("Tipo: Desconocido");
                    }

                    static float new_positionx = 0.0f;                                // Variable para almacenar la nueva posición
                    static float new_positionz = 0.0f;                                // Variable para almacenar la nueva posición
                    ImGui::InputFloat("PosX", &new_positionx, 0.1f, 1000.0f, "%.2f"); // Campo de entrada para la posición
                    ImGui::InputFloat("PosZ", &new_positionz, 0.1f, 1000.0f, "%.2f"); // Campo de entrada para la posición
                    if (ImGui::Button("Actualizar posición"))
                    {
                        auto &fisica = em.fisicas.get(entities[current_entity_index].fisica_key);
                        fisica.x = new_positionx;
                        fisica.z = new_positionz;
                        fisica.tileX = static_cast<int>(fisica.x / 2);
                        fisica.tileZ = static_cast<int>(fisica.z / 2);
                    }

                    ImGui::Separator(); // Línea divisoria para mayor claridad

                    // Desplegable BombComponent
                    if (ImGui::CollapsingHeader("BombComponent"))
                    {
                        // Aquí puedes agregar cualquier funcionalidad que desees para este componente.
                        ImGui::Text("Opciones de BombComponent:");
                        auto &bomb = em.bombs.get(entities[current_entity_index].bomb_key);
                        static int cantidad = 0;
                        ImGui::InputInt("Bombas", &cantidad, 1, 10); // Campo de entrada para la posición X
                        if (ImGui::Button("Actualizar Bomba"))
                        {
                            // Limitamos el rango entre 1 y 9
                            if (cantidad < 1)
                                cantidad = 1;
                            if (cantidad > 9)
                                cantidad = 9;
                            bomb.maxBombs = cantidad;
                        }
                        static int Rango = 1;
                        ImGui::InputInt("Rango", &Rango, 1, 10); // Campo de entrada para la posición X
                        if (ImGui::Button("Actualizar Rango"))
                        {
                            if (Rango < 1)
                                Rango = 1;
                            if (Rango > 9)
                                Rango = 9;
                            bomb.expRange = Rango;
                        }
                    }
                    // Desplegable IAComponent
                    if (ImGui::CollapsingHeader("IAComponent"))
                    {
                        auto &iacomponent = em.ia.get(entities[current_entity_index].ia_key);
                        // Mostrar si tiene BT usando ImGui
                        ImGui::Text("Tiene BT: %s", iacomponent.tieneBT ? "true" : "false");
                        if (iacomponent.tieneBT == true)
                        {
                            ImGui::Text("Árbol de Decisión:");

                            // Obtener lista de dibujo para la ventana actual
                            ImDrawList *drawList = ImGui::GetWindowDrawList();
                            ImVec2 windowPos = ImGui::GetWindowPos();
                            ImVec2 cursorPos = ImGui::GetCursorScreenPos();

                            // Coordenadas base (ajusta según el espacio que desees)
                            ImVec2 root = ImVec2(cursorPos.x + 170, cursorPos.y + 50); // Nodo raíz
                            ImVec2 leftChild = ImVec2(root.x - 100, root.y + 100);     // Hijo izquierdo
                            ImVec2 rightChild = ImVec2(root.x + 50, root.y + 100);     // Hijo derecho

                            // Subnodos del hijo izquierdo
                            ImVec2 leftSub1 = ImVec2(leftChild.x - 50, leftChild.y + 100);
                            ImVec2 leftSub2 = ImVec2(leftChild.x, leftChild.y + 100);
                            ImVec2 leftSub3 = ImVec2(leftChild.x + 50, leftChild.y + 100);

                            // Subnodos del hijo derecho
                            ImVec2 rightSub1 = ImVec2(rightChild.x - 50, rightChild.y + 100);
                            ImVec2 rightSub2 = ImVec2(rightChild.x, rightChild.y + 100);
                            ImVec2 rightSub3 = ImVec2(rightChild.x + 50, rightChild.y + 100);

                            // Función para dibujar nodos
                            auto DrawNode = [&](ImVec2 position, const char *label, const char *tooltipText, bool isActive, float additionalInfo)
                            {
                                float radius = 20.0f;

                                // Cambiar el color del nodo según si está activo
                                ImU32 color = isActive ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 200, 0, 255); // Verde si activo, amarillo si no
                                drawList->AddCircleFilled(position, radius, color);
                                drawList->AddText(ImVec2(position.x - 10, position.y - 10), IM_COL32(0, 0, 0, 255), label);

                                // Detectar si el ratón está encima
                                ImVec2 mousePos = ImGui::GetMousePos();
                                float dist = sqrtf((mousePos.x - position.x) * (mousePos.x - position.x) +
                                                   (mousePos.y - position.y) * (mousePos.y - position.y));

                                if (dist <= radius)
                                {
                                    ImGui::BeginTooltip();
                                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", tooltipText); // Texto en amarillo
                                    ImGui::Text("Calculo: %.2f", additionalInfo);
                                    ImGui::EndTooltip();
                                }
                            };

                            // Función para dibujar líneas entre nodos
                            auto DrawConnection = [&](ImVec2 from, ImVec2 to)
                            {
                                drawList->AddLine(from, to, IM_COL32(255, 255, 0, 255), 2.0f); // Línea amarilla
                            };

                            // Dibujar conexiones principales
                            DrawConnection(root, leftChild);  // Conexión al hijo izquierdo
                            DrawConnection(root, rightChild); // Conexión al hijo derecho

                            // Dibujar conexiones de subnodos (hijo izquierdo)
                            DrawConnection(leftChild, leftSub1);
                            DrawConnection(leftChild, leftSub2);
                            DrawConnection(leftChild, leftSub3);

                            // Dibujar conexiones de subnodos (hijo derecho)
                            DrawConnection(rightChild, rightSub1);
                            DrawConnection(rightChild, rightSub2);
                            DrawConnection(rightChild, rightSub3);

                            // Dibujar nodos principales
                            DrawNode(root, "->", "Estoy a salvo", iacomponent.estadoDebugger == 0, iacomponent.waitsUsados);
                            DrawNode(leftChild, "SI", "Respuesta: Sí", false, iacomponent.waitsUsados);
                            DrawNode(rightChild, "NO", "Respuesta: No", false, iacomponent.waitsUsados);

                            // Dibujar subnodos (hijo izquierdo)
                            DrawNode(leftSub1, "H", "Generar un camino de huida", iacomponent.estadoDebugger == 1, iacomponent.dangerLevel * (1 - iacomponent.hasHuidaPath));
                            DrawNode(leftSub2, "I", "Generar un camino de ida", iacomponent.estadoDebugger == 2, (1 - iacomponent.dangerLevel) * (1 - iacomponent.hasPath));
                            DrawNode(leftSub3, "W", "Esperar para decidir", iacomponent.estadoDebugger == 5, iacomponent.waitsUsados);

                            // Dibujar subnodos (hijo derecho)
                            DrawNode(rightSub1, "NPC", "Mover al NPC a una nueva posición", iacomponent.estadoDebugger == 3, iacomponent.hasPath * (1 - iacomponent.inPositionToPlaceBomb));
                            DrawNode(rightSub2, "B", "Poner una bomba en la posición actual", iacomponent.estadoDebugger == 4, iacomponent.inPositionToPlaceBomb * iacomponent.canEscapeAfterBomb);
                            DrawNode(rightSub3, "W", "Esperar y no realizar ninguna acción", iacomponent.estadoDebugger == 6, iacomponent.waitsUsados);

                            // Espaciado para evitar superposición con otros elementos
                            ImGui::Dummy(ImVec2(0, 300)); // Ajusta este valor según el tamaño del árbol

                            ImGui::Separator(); // Línea divisoria para mayor claridad
                            ImGui::Text("Donde me pongo a cubierto");
                            ImGui::BulletText("PosX: %.2f, PosZ: %.2f", iacomponent.dondeVoyX, iacomponent.dondeVoyZ);
                            ImGui::Text("Busco camino hasta");
                            ImGui::BulletText("PosX: %.2f, PosZ: %.2f", iacomponent.buscoCaminoX, iacomponent.buscoCaminoZ);
                            ImGui::Text("Siguiente nodo");
                            ImGui::BulletText("PosX: %.2f, PosZ: %.2f", iacomponent.siguienteCaminoX, iacomponent.siguienteCaminoZ);
                            ImGui::Text("Nivel de peligro actual");
                            ImGui::BulletText("Peligro: %.2f", iacomponent.dangerLevel);
                            ImGui::Text("Distancia hasta la bomba");
                            ImGui::BulletText("Distancia: %.2f", iacomponent.distanceToBomb);
                            ImGui::Text("Distancia al objetivo");
                            ImGui::BulletText("Distancia: %.2f", iacomponent.distanceToTarget);
                            ImGui::Text("Distancia caminada");
                            ImGui::BulletText("Distancia: %.2f", iacomponent.distanceTraveled);

                            //EmpezarModo3D();
                            DibujarCubo({iacomponent.dondeVoyZ * 2, 0.0f, iacomponent.dondeVoyX * 2}, size, size / 2, size, MY_VIOLET);

                            DibujarCubo({iacomponent.buscoCaminoZ * 2, 0.0f, iacomponent.buscoCaminoX * 2}, size, size * 2, size, MY_LIME);

                            // Recorrer los vectores camx y camz
                            for (size_t i = 1; i < iacomponent.camx.size(); ++i)
                            {
                                float posX = iacomponent.camx[i] * 2.0f;
                                float posZ = iacomponent.camz[i] * 2.0f;

                                // Dibujar cubo sólido en la posición calculada
                                DibujarCubo({posZ, 0.0f, posX}, size, size * 2, size, MY_SKYBLUE);
                            }
                        }
                    }
                    if (ImGui::CollapsingHeader("Nueva IA"))
                    {
                        auto &iacomponent = em.ia.get(entities[current_entity_index].ia_key);
                        ImGui::Text("CAMINO");
                        ImGui::BulletText("Calculo: %.2f",  iacomponent.caminolargo);
                        ImGui::Separator(); // Línea divisoria para mayor claridad
                        ImGui::Text("HUIR");
                        ImGui::BulletText("Calculo: %.2f", iacomponent.huirlargo);
                        ImGui::Separator(); // Línea divisoria para mayor claridad
                        ImGui::Text("POWER UP");
                        ImGui::BulletText("Calculo: %.2f", iacomponent.powerlargo);
                        ImGui::Separator(); // Línea divisoria para mayor claridad
                        ImGui::Text("ATACAR");
                        ImGui::BulletText("Calculo: %.2f", iacomponent.atacarlargo);
                        ImGui::Separator(); // Línea divisoria para mayor claridad
                        ImGui::Text("POR DEFECTO");
                        ImGui::BulletText("Calculo: %.2f", iacomponent.defaultlargo);
                        ImGui::Separator(); // Línea divisoria para mayor claridad
                        ImGui::Text("MAPA");
                        // Tamaño de cada celda en píxeles
                        const float cellSize = 20.0f; 

                        static int mapaActual = 0;  // Índice del mapa actual (0 a 4)

                        // Botones para cambiar de mapa
                        ImGui::Text("Seleccionar Mapa:");
                        if (ImGui::Button("CAMINO")) mapaActual = 0; ImGui::SameLine();
                        if (ImGui::Button("HUIR")) mapaActual = 1; ImGui::SameLine();
                        if (ImGui::Button("POWER UP")) mapaActual = 2; ImGui::SameLine();
                        if (ImGui::Button("ATACAR")) mapaActual = 3; ImGui::SameLine();
                        if (ImGui::Button("Default")) mapaActual = 4;

                        ImGui::Separator();  // Línea divisoria

                        // Obtener el mapa seleccionado
                        std::vector<std::vector<float>>* mapaSeleccionado = nullptr;
                        switch (mapaActual) {
                            case 0: mapaSeleccionado = &iacomponent.mapaProbabilidades; break;
                            case 1: mapaSeleccionado = &iacomponent.mapaProbabilidades1; break;
                            case 2: mapaSeleccionado = &iacomponent.mapaProbabilidades2; break;
                            case 3: mapaSeleccionado = &iacomponent.mapaProbabilidades3; break;
                            case 4: mapaSeleccionado = &iacomponent.mapaProbabilidades4; break;
                        }

                        // Dibujar el mapa si está seleccionado correctamente
                        if (mapaSeleccionado && !mapaSeleccionado->empty()) {
                            int rows = mapaSeleccionado->size();
                            int cols = rows > 0 ? (*mapaSeleccionado)[0].size() : 0;

                            ImGui::BeginChild("Mapa", ImVec2(cols * cellSize, rows * cellSize), true);
                            for (int i = 0; i < rows; i++) {
                                for (int j = 0; j < cols; j++) {
                                    float score = (*mapaSeleccionado)[i][j];

                                    // Definir el color de la celda
                                    ImVec4 color;
                                    bool useBlackText = false;
                                    if (score == -1.0f) { 
                                        color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);  // Gris oscuro (inaccesible)
                                    } else if (score < 1.0f) {
                                        color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // Rojo (baja puntuación)
                                        useBlackText = true;
                                    } else if (score < 3.0f) {
                                        color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // Amarillo (puntuación media)
                                        useBlackText = true;
                                    } else {
                                        color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // Verde (alta puntuación)
                                        useBlackText = true;
                                    }

                                    // Formatear el float con 2 decimales y agregar ID único
                                    char label[32];
                                    std::sprintf(label, "%.2f##%d_%d", score, i, j);

                                    // Aplicar colores al botón
                                    ImGui::PushStyleColor(ImGuiCol_Button, color);
                                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
                                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);

                                    // Si el color es rojo, amarillo o verde, hacer que el texto sea negro
                                    if (useBlackText) {
                                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
                                    }

                                    ImGui::Button(label, ImVec2(cellSize, cellSize));

                                    // Restaurar el color del texto si se cambió
                                    if (useBlackText) {
                                        ImGui::PopStyleColor();
                                    }

                                    ImGui::PopStyleColor(3);

                                    // Mostrar tooltip con el valor cuando el cursor está encima
                                    if (ImGui::IsItemHovered()) {
                                        ImGui::SetTooltip("Valor: %.2f", score);
                                    }

                                    // Espaciado horizontal
                                    if (j < cols - 1)
                                        ImGui::SameLine();
                                }
                            }
                            ImGui::EndChild();
                        }
                    }
                    // Desplegable Añadir PowerUps
                    if (ImGui::CollapsingHeader("Añadir PowerUps"))
                    {
                        ImGui::Text("Opciones de Añadir PowerUps:");
                        auto &bomb = em.bombs.get(entities[current_entity_index].bomb_key);
                        auto &fisica = em.fisicas.get(entities[current_entity_index].fisica_key);
                        if (ImGui::Button("BOMB_UP"))
                        {
                            // Lógica para BOMB_UP
                            bomb.maxBombs += 1;
                        }
                        ImGui::SameLine(); // Coloca el siguiente botón en la misma línea

                        if (ImGui::Button("BOMB_DOWN"))
                        {
                            // Lógica para BOMB_DOWN
                            if (bomb.maxBombs > 0)
                                bomb.maxBombs -= 1;
                        }

                        if (ImGui::Button("EXP_RANGE_UP"))
                        {
                            // Lógica para EXP_RANGE_UP
                            if (bomb.expRange < 9)
                                bomb.expRange += 1;
                        }
                        ImGui::SameLine();

                        if (ImGui::Button("EXP_RANGE_DOWN"))
                        {
                            // Lógica para EXP_RANGE_DOWN
                            if (bomb.expRange > 0)
                                bomb.expRange -= 1;
                        }

                        if (ImGui::Button("EXP_RANGE_MAX"))
                        {
                            // Lógica para EXP_RANGE_MAX
                            bomb.expRange = 9;
                        }

                        if (ImGui::Button("SPEED_UP"))
                        {
                            // Lógica para SPEED_UP
                            fisica.vx += 0.1f;
                            fisica.vz += 0.1f;
                        }
                        ImGui::SameLine();

                        if (ImGui::Button("SPEED_DOWN"))
                        {
                            // Lógica para SPEED_DOWN
                            if (fisica.vx > 0.15f && fisica.vz > 0.15f)
                            {
                                fisica.vx -= 0.1f;
                                fisica.vz -= 0.1f;
                            }
                        }

                        if (ImGui::Button("PUSH_BOMB"))
                        {
                            // Lógica para PUSH_BOMB
                            fisica.canPushBombs = true;
                        }
                    }
                }
                ImGui::End();
            }
            else
            {
                // Si no hay entidades o el índice es inválido, mostrar un mensaje.
                ImGui::SetNextWindowPos(ImVec2(GetAnchuraPantalla() - 350, 330), ImGuiCond_Once); // Posición de la ventana.
                ImGui::SetNextWindowSize(ImVec2(340, 100), ImGuiCond_Once);                   // Tamaño de la ventana.

                if (ImGui::Begin("Ventana de Entidades", &show_entities_window))
                {
                    ImGui::Text("No hay entidades disponibles.");
                }
                ImGui::End();
            }
        }

        if (show_fisicas)
        {
            for (auto e : em.getEntities())
            {
                if (e.type == EntityType::NPC)
                {
                    auto &fisica = em.fisicas.get(e.fisica_key);
                    auto &render = em.renders.get(e.render_key);

                    DibujarCubo({fisica.x, fisica.y, fisica.z}, size, size, size, MY_VIOLET);
                    render.color = MY_TRANSPARENTE;
                    if (fisica.vx > 0)
                    {
                        if (fisica.chocaZ)
                        {
                            DibujarCubo({fisica.x, fisica.y + 1.5f, fisica.z + 1.0f}, 0.5f, 0.0f, 2.0f, MY_RED);
                        }
                        else
                        {
                            DibujarCubo({fisica.x, fisica.y + 1.5f, fisica.z + 1.0f}, 0.5f, 0.0f, 2.0f, MY_WHITE);
                        }
                    }
                    if (fisica.vx < 0)
                    {
                        if (fisica.chocaZ)
                        {
                            DibujarCubo({fisica.x, fisica.y + 1.5f, fisica.z - 1.0f}, 0.5f, 0.0f, 2.0f, MY_RED);
                        }
                        else
                        {
                            DibujarCubo({fisica.x, fisica.y + 1.5f, fisica.z - 1.0f}, 0.5f, 0.0f, 2.0f, MY_WHITE);
                        }
                    }
                    if (fisica.vz > 0)
                    {
                        if (fisica.chocaX)
                        {
                            DibujarCubo({fisica.x + 1.0f, fisica.y + 1.5f, fisica.z}, 2.0f, 0.0f, 0.5f, MY_RED);
                        }
                        else
                        {
                            DibujarCubo({fisica.x + 1.0f, fisica.y + 1.5f, fisica.z}, 2.0f, 0.0f, 0.5f, MY_WHITE);
                        }
                    }
                    if (fisica.vz < 0)
                    {
                        if (fisica.chocaX)
                        {
                            DibujarCubo({fisica.x - 1.0f, fisica.y + 1.5f, fisica.z}, 2.0f, 0.0f, 0.5f, MY_RED);
                        }
                        else
                        {
                            DibujarCubo({fisica.x - 1.0f, fisica.y + 1.5f, fisica.z}, 2.0f, 0.0f, 0.5f, MY_WHITE);
                        }
                    }
                }
                else if (e.type == EntityType::PLAYER)
                {
                    auto &fisica = em.fisicas.get(e.fisica_key);
                    auto &render = em.renders.get(e.render_key);
                    DibujarCubo({fisica.x, fisica.y, fisica.z}, size, size, size, MY_VIOLET);
                    render.color = MY_TRANSPARENTE;

                    if (fisica.vz > 0)
                    {
                        if (fisica.chocaZ)
                        {
                            DibujarCubo({fisica.x, fisica.y + 1.5f, fisica.z + 1.0f}, 0.5f, 0.0f, 2.0f, MY_RED);
                        }
                        else
                        {
                            DibujarCubo({fisica.x, fisica.y + 1.5f, fisica.z + 1.0f}, 0.5f, 0.0f, 2.0f, MY_WHITE);
                        }
                    }
                    if (fisica.vz < 0)
                    {
                        if (fisica.chocaZ)
                        {
                            DibujarCubo({fisica.x, fisica.y + 1.5f, fisica.z - 1.0f}, 0.5f, 0.0f, 2.0f, MY_RED);
                        }
                        else
                        {
                            DibujarCubo({fisica.x, fisica.y + 1.5f, fisica.z - 1.0f}, 0.5f, 0.0f, 2.0f, MY_WHITE);
                        }
                    }
                    if (fisica.vx > 0)
                    {
                        if (fisica.chocaX)
                        {
                            DibujarCubo({fisica.x + 1.0f, fisica.y + 1.5f, fisica.z}, 2.0f, 0.0f, 0.5f, MY_RED);
                        }
                        else
                        {
                            DibujarCubo({fisica.x + 1.0f, fisica.y + 1.5f, fisica.z}, 2.0f, 0.0f, 0.5f, MY_WHITE);
                        }
                    }
                    if (fisica.vx < 0)
                    {
                        if (fisica.chocaX)
                        {
                            DibujarCubo({fisica.x - 1.0f, fisica.y + 1.5f, fisica.z}, 2.0f, 0.0f, 0.5f, MY_RED);
                        }
                        else
                        {
                            DibujarCubo({fisica.x - 1.0f, fisica.y + 1.5f, fisica.z}, 2.0f, 0.0f, 0.5f, MY_WHITE);
                        }
                    }
                }
            }
        }
        else
        {
            for (auto e : em.getEntities())
            {
                if (e.type == EntityType::PLAYER)
                {
                    auto &fisica = em.fisicas.get(e.fisica_key);
                    auto &render = em.renders.get(e.render_key);
                    render.size = 2.0f;
                    render.color = MY_BLUE;
                }
                if (e.type == EntityType::NPC)
                {
                    auto &fisica = em.fisicas.get(e.fisica_key);
                    auto &render = em.renders.get(e.render_key);
                    render.size = 2.0f;
                    render.color = MY_RED;
                }
            }
        }
    }

    //  **Renderizar ImGui al final**
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

BellotaEngine &ElMotorRaylib::getMotorDeJuego()
{
    return MotorDeJuegoPropio;
}

void ElMotorRaylib::pintarDecoraciones()
{
    const std::string &nombred = "assets/modelos/taburetes.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred, 0.0f, 3);
    const std::string &nombred2 = "assets/modelos/sillasPoker.obj";
    MotorDeJuegoPropio.CargarDecoraciones(1.0f, 0.0f, 0.0f, nombred2, 0.0f, 1);
    const std::string &nombred3 = "assets/modelos/sillasNormales.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 2.0f, nombred3, 0.0f, 2);
    const std::string &nombred4 = "assets/modelos/mesaTaberna.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 2.0f, nombred4, 0.0f, 2);
    const std::string &nombred5 = "assets/modelos/poker.obj";
    MotorDeJuegoPropio.CargarDecoraciones(1.0f, 0.0f, 0.0f, nombred5, 0.0f, 1);
    const std::string &nombred6 = "assets/modelos/estanterias.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred6, 0.0f, 2);
    const std::string &nombred7 = "assets/modelos/perchero.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, -5.0f, nombred7, 0.0f, 1);
    const std::string &nombred8 = "assets/modelos/pianola.obj";
    MotorDeJuegoPropio.CargarDecoraciones(1.0f, 0.0f, 0.0f, nombred8, 0.0f, 2);
    const std::string &nombred10 = "assets/modelos/barra.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred10, 0.0f, 3);
    const std::string &nombred11 = "assets/modelos/musica.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred11, 0.0f, 0);
    const std::string &nombred12 = "assets/modelos/paredes.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred12, 0.0f, 2);
    const std::string &nombred13 = "assets/modelos/suelo.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred13, 0.0f, 2);
}


void ElMotorRaylib::pintarDecoracionesClasico(){
    const std::string &nombred = "assets/modelos/edificioNaranja.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred, 0.0f, 3);
    const std::string &nombred2 = "assets/modelos/edificioEsquina.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred2, 0.0f, 3);
    const std::string &nombred3 = "assets/modelos/Pizzeria.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred3, 0.0f, 0);
    const std::string &nombred4 = "assets/modelos/edificioNegro.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred4, 0.0f, 0);
    const std::string &nombred5 = "assets/modelos/edificioVinos.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred5, 0.0f, 2);
    const std::string &nombred6 = "assets/modelos/farolas.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred6, 0.0f, 3);
    const std::string &nombred7 = "assets/modelos/mesas.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred7, 0.0f, 0);
    const std::string &nombred8 = "assets/modelos/acera.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred8, 0.0f, 0);
    const std::string &nombred9 = "assets/modelos/carretera.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred9, 0.0f, 0);
    const std::string &nombred10 = "assets/modelos/cochePolicia.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred10, 0.0f, 2);
    const std::string &nombred11 = "assets/modelos/cocheNaranja.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred11, 0.0f, 1);
    const std::string &nombred13 = "assets/modelos/taxi.obj";
    MotorDeJuegoPropio.CargarDecoraciones(0.0f, 0.0f, 0.0f, nombred13, 0.0f, 2);
}

bool ElMotorRaylib::IsMouseOver(float x, float y, float w, float h, const std::string &nombre)
{
    bool isHovered = false;

    auto wh = MotorDeJuegoPropio.getWH(nombre);
    auto posMouse = ObtenerPosicionMouse();

    if (posMouse.x >= x && posMouse.x <= x + wh.x && posMouse.y >= y && posMouse.y <= y + wh.y)
    {
        isHovered = true;
    }
    else
    {
        isHovered = false;
    }
    return isHovered;
}

void ElMotorRaylib::ResetDibujadoDeMapa()
{
    mapaPintado = false;
    iniciadoYa = false;
}

//---------------------------FUNCIONES SELECCION---------------------------//

void ElMotorRaylib::initCamara()
{
    glm::vec3 camPosition = glm::vec3(2.0f, 6.0f, 10.0f);
    glm::vec3 camtarget = glm::vec3(2.0f, 4.0f, 2.0f);
    glm::vec3 camup = glm::vec3(0.0f, 1.0f, 0.0f);
    MotorDeJuegoPropio.CrearCamara(camtarget, camup, 65.0f, true, camPosition);
}

void ElMotorRaylib::setSeleccionbool(bool n){
    seleccionBool = n;
}

void ElMotorRaylib::SeleccionRender(EntityManager &em, const FisicaSystem &physics)
{
    const std::string &nombre0 = "assets/sprites/fondoSeleccion.png"; // Nombre de la textura de fondo
    MiColor fondo0(255, 255, 255, 255);
    DibujarTextura(nombre0, 0, 0, 1.0f, 0.0f, fondo0); // Dibuja la textura de fondo
    em.forEach(EntityType::BUTTON, [this, &em, &physics](const Entity &e)
               { this->dibujaEntidad(em, e, physics); });

    float dt = getFixedDeltaTime(); // tu lógica de tiempo entre frames
    for (auto& parti : particulas) {
            parti.actualizarParticulas(dt);
    }

    for(auto &parti : particulas ){
        MotorDeJuegoPropio.pintarParticula(parti);
    }
}

void ElMotorRaylib::noDibujaEntidad(EntityManager &em, Entity const &e, const FisicaSystem &physics)
{
    auto &render = em.renders.get(e.render_key);

    if (e.type == EntityType::PLAYER)
    {
        if(render.SusDatosEnElArbol != nullptr){
        render.SusDatosEnElArbol->setSepinta(false);
        }
    }
    else if (e.type == EntityType::NPC)
    {
        if(render.SusDatosEnElArbol != nullptr){
        render.SusDatosEnElArbol->setSepinta(false);
        }
    }
}

void ElMotorRaylib::pintaALosPersonajes(EntityManager &em, const FisicaSystem &physics, int nu)
{
    em.forEach(EntityType::PLAYER,
               [this, &em, &physics, nu](const Entity &e)
               {
                   if (e.id == nu)
                   {
                       this->activaDibujado(em, e, physics);
                   }
               });
}

void ElMotorRaylib::despintaALosPersonajes(EntityManager &em, const FisicaSystem &physics, int nu)
{
    em.forEach(EntityType::PLAYER,
               [this, &em, &physics, nu](const Entity &e)
               {
                   if (e.id == nu)
                   {
                       this->desactivaDibujado(em, e, physics);
                   }
               });
}

void ElMotorRaylib::activaDibujado(EntityManager &em, Entity const &e, const FisicaSystem &physics)
{
    auto &render = em.renders.get(e.render_key);

    if (e.type == EntityType::PLAYER || e.type == EntityType::NPC)
    {
        if(render.SusDatosEnElArbol != nullptr){
        render.SusDatosEnElArbol->setSepinta(true);
        }
    }
}

void ElMotorRaylib::desactivaDibujado(EntityManager &em, Entity const &e, const FisicaSystem &physics)
{
    auto &render = em.renders.get(e.render_key);

    if (e.type == EntityType::PLAYER || e.type == EntityType::NPC)
    {
        if(render.SusDatosEnElArbol != nullptr){
        render.SusDatosEnElArbol->setSepinta(false);
        }
    }
}

void ElMotorRaylib::ConfigurarLuces()
{
    // Luz direccional (tipo 0)
    glm::vec3 posicion = glm::vec3(12.0f, 20.0f, 25.0f);
    glm::vec3 direccion = glm::vec3(0.0f, -1.0f, 0.0f); // No se usa en luz puntual
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);      // Luz blanca

    float ambientStrength = 0.8f;
    float diffuseStrength = 1.8f;
    float specularStrength = 0.5f;

    float atenuacionConstante = 1.0f;
    float atenuacionLineal = 0.09f;
    float atenuacionCuadratica = 0.032f;

    float apertura = 12.5f;        // Solo se usa en spotlight
    float penumbra = 17.5f;        // Solo se usa en spotlight
    MotorDeJuegoPropio.CrearLuz(0, // tipoLuz: 0 = direccional
                                posicion,
                                direccion,
                                color,
                                ambientStrength,
                                diffuseStrength,
                                specularStrength,
                                atenuacionConstante,
                                atenuacionLineal,
                                atenuacionCuadratica,
                                apertura,
                                penumbra);
}
//--------------------nuevo cronometro--------------------//

bool ElMotorRaylib::canUpdate()  { return cronoManager.canUpdate(); }
bool   ElMotorRaylib::canRender()  { return cronoManager.canRender(); }
double ElMotorRaylib::getDeltaTime(){ return cronoManager.getDeltaTime(); }
int    ElMotorRaylib::getCurrentFPS(){ return cronoManager.getCurrentFPS(); }
void  ElMotorRaylib::tick(){cronoManager.tick();}
void ElMotorRaylib::iniciarCrono() {cronoManager.init();}
double ElMotorRaylib::getFixedDeltaTime() const {return cronoManager.getFixedDeltaTime();}

void ElMotorRaylib::crearParticulasGame(){
    glm::vec3 quadVertices[4] = {
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3( 0.5f, -0.5f, 0.0f),
        glm::vec3( 0.5f,  0.5f, 0.0f),
        glm::vec3(-0.5f,  0.5f, 0.0f)
        };
        
        // Suponiendo que ya tienes cargados tus shaders:
        const std::string &nombre = "assets/shaders/particulaShader.AlmondVertex";
        AlmondShader* shaderVertexParticulas = MotorDeJuegoPropio.gestor.getRecurso<AlmondShader>(nombre);
        const std::string &nombre2 = "assets/shaders/particulaShader.AlmondFragment";
        AlmondShader* shaderFragmentParticulas = MotorDeJuegoPropio.gestor.getRecurso<AlmondShader>(nombre2);
        const std::string &nombre3 = "assets/sprites/smoke.png";
        AlmondTextura* textu = MotorDeJuegoPropio.gestor.getRecurso<AlmondTextura>(nombre3);
        
        // Crear algunas partículas:
        for (int i = 0; i < 100; ++i) { // 100 partículas de ejemplo
        glm::vec3 pos(
            static_cast<float>(rand() % 10 - 5), 
            static_cast<float>(rand() % 10 - 5), 
            static_cast<float>(rand() % 10 - 5)
        );
        
        glm::vec3 color(
            1.0f, // Rojo fijo al máximo
            static_cast<float>(rand() % 40 + 20) / 100.0f, // Verde entre 0.20 y 0.59
            static_cast<float>(rand() % 10) / 100.0f // Azul entre 0.00 y 0.09
        );
        
        float tam = 0.5f + static_cast<float>(rand() % 50) / 100.0f; // Tamaño entre 0.5 y 1
        float vida = 0.0f; // 5 segundos de vida
        glm::vec3 vel(
            static_cast<float>(rand() % 10 - 5) / 10.0f,
            static_cast<float>(rand() % 10) / 10.0f,
            static_cast<float>(rand() % 10 - 5) / 10.0f
        );
        
        particulas.emplace_back(pos, color, tam, vida,vel, quadVertices, shaderVertexParticulas, shaderFragmentParticulas, 45.0f,textu,0);
        }
}

void ElMotorRaylib::generarHumo(glm::vec3 origen, int cantidad) {
    for (int i = 0; i < cantidad; ++i) {
        // Buscar partícula muerta
        auto it = std::find_if(particulas.begin(), particulas.end(), [](const AlmondParticula& p) {
            return p.getVida() <= 0.0f;
        });

        if (it != particulas.end()) {
            // Reutilizar partícula
            it->setVida(2.0f);

            // Recolocar en el origen
            it->setPosicion(origen);
            float tam = 0.3f + static_cast<float>(rand() % 40) / 100.0f;
            it->setTamanyo(tam);

            // Color gris con leve variación aleatoria
            float gris = 0.4f + static_cast<float>(rand() % 40) / 100.0f; // valor entre 0.4 y 0.8
            it->setColor(glm::vec3(gris, gris, gris));

            // Velocidad: algo de dispersión + subida
            glm::vec3 vel(
                static_cast<float>(rand() % 10 - 5) / 10.0f,
                static_cast<float>(rand() % 5 + 5) / 10.0f, // subida positiva
                static_cast<float>(rand() % 10 - 5) / 10.0f
            );
            it->setVelocidad(vel);

            // Ángulo de rotación aleatorio entre 0.0 y 2π
            float angulo = static_cast<float>(rand()) / RAND_MAX * 2.0f * glm::pi<float>();
            it->setAngulo(angulo);

            //el humo el la particula de tipo 1
            it->setTipoDeParticula(1);
            //le asignamos su textura del humo
            const std::string &nombre3 = "assets/sprites/smoke.png";
            AlmondTextura* textu = MotorDeJuegoPropio.gestor.getRecurso<AlmondTextura>(nombre3);
            it->setTextura(textu);
        }
    }
}

void ElMotorRaylib::generarExplosion(glm::vec3 origen, int cantidad) {
    for (int i = 0; i < cantidad; ++i) {
        // Buscar partícula muerta
        auto it = std::find_if(particulas.begin(), particulas.end(), [](const AlmondParticula& p) {
            return p.getVida() <= 0.0f;
        });

        if (it != particulas.end()) {
            // Reutilizar partícula
            it->setVida(2.0f);

            // Recolocar en el origen
            it->setPosicion(origen);

            float tam = 2.0f + static_cast<float>(rand() % 200) / 100.0f; // Rango: 2.0 - 4.0
            it->setTamanyo(tam);

             // Color fuego: mezcla de rojo, naranja y amarillo
             float r = 1.0f;
             float g = 0.3f + static_cast<float>(rand() % 50) / 100.0f; // entre 0.3 y 0.8
             float b = 0.0f;
             it->setColor(glm::vec3(r, g, b));

           // Velocidad aleatoria radial
           glm::vec3 dir(
            static_cast<float>(rand() % 200 - 100) / 100.0f,
            static_cast<float>(rand() % 200 - 100) / 100.0f,
            static_cast<float>(rand() % 200 - 100) / 100.0f
            );
            it->setVelocidad(glm::normalize(dir) * (1.0f + static_cast<float>(rand() % 100) / 100.0f)); // magnitud aleatoria


            // Ángulo de rotación aleatorio entre 0.0 y 2π
            float angulo = static_cast<float>(rand()) / RAND_MAX * 2.0f * glm::pi<float>();
            it->setAngulo(angulo);

            //el humo el la particula de tipo 1
            it->setTipoDeParticula(2);
            //le asignamos su textura del humo
            const std::string &nombre3 = "assets/sprites/smoke.png";
            AlmondTextura* textu = MotorDeJuegoPropio.gestor.getRecurso<AlmondTextura>(nombre3);
            it->setTextura(textu);
        }
    }
}

void ElMotorRaylib::generarChispas(glm::vec3 origen, int cantidad) {
    for (int i = 0; i < cantidad; ++i) {
        auto it = std::find_if(particulas.begin(), particulas.end(), [](const AlmondParticula& p) {
            return p.getVida() <= 0.0f;
        });

        if (it != particulas.end()) {
            it->setVida(0.8f); // vida muy corta
            it->setPosicion(origen);

            // Tamaño pequeño entre 0.05 y 0.1
            float tam = 0.05f + static_cast<float>(rand() % 30) / 1000.0f;
            it->setTamanyo(tam);

            // Color brillante tipo chispa (amarillo o blanco)
            float choice = static_cast<float>(rand()) / RAND_MAX;
            glm::vec3 color = (choice > 0.5f)
                ? glm::vec3(1.0f, 1.0f, 0.3f) // amarillo claro
                : glm::vec3(1.0f); // blanco puro
            it->setColor(color);

            // Dirección rápida aleatoria
            glm::vec3 dir(
                static_cast<float>(rand() % 200 - 100) / 100.0f,
                static_cast<float>(rand() % 200 - 100) / 100.0f,
                static_cast<float>(rand() % 200 - 100) / 100.0f
            );
            it->setVelocidad(glm::normalize(dir) * (2.0f + static_cast<float>(rand() % 100) / 50.0f)); // muy rápido

            float angulo = static_cast<float>(rand()) / RAND_MAX * 2.0f * glm::pi<float>();
            it->setAngulo(angulo);

            it->setTipoDeParticula(3); // chispa

            it->setTextura(nullptr);
        }
    }
}

