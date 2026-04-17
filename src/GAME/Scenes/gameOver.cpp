#include "gameOver.h"

GameOver::GameOver()
    : physics()
{
}

GameOver::~GameOver()
{
}

void GameOver::iniciarGameOver()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");

    // ENTIDADES
    Entity &win = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton{};
    MiColor colorPersonalizado(0, 0, 0, 0);
    renderComponent renderBoton{0, colorPersonalizado};
    soundComponent soundBoton;
    SoundEvent se;
    se.eventPath = "event:/PowerUps/RecogerItem";
    se.playOnce = false;
    se.looping = false;
    se.isPlaying = false;
    se.volume = 1.0f;
    soundBoton.addSoundEvent(se);
    powerUpComponent powerBloqueBoton{};
    bombaComponent bombaBoton{};
    iaComponent botontieneBT{false};
    buttonComponent buttonBoton{
        1, 450, 580, 1280, 720, "assets/sprites/volverMenuGO.png", "assets/sprites/volverMenuGOHover.png", "assets/sprites/volverMenuGO.png", false};
    em.createEntityComponents(win, fisicaBoton, renderBoton, powerBloqueBoton, soundBoton, bombaBoton, botontieneBT, buttonBoton);
}

void GameOver::bucleGameOverUpdate()
{
    manejarClicBoton();
}

void GameOver::bucleGameOverRender()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
    motorRaylib.empezarEscena();
    MiColor colorFondo(242, 205, 136, 255);
    motorRaylib.getMotorDeJuego().limpiarPantalla(colorFondo.r, colorFondo.g, colorFondo.b, colorFondo.a);
    motorRaylib.gameOverRender(em, physics);
    motorRaylib.acabarEscena();
}

void GameOver::manejarClicBoton()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");

    auto botones = em.getEntitiesByType(EntityType::BUTTON);

    for (auto &boton : botones)
    {
        auto &botonCMP = em.buttons.get(boton.button_key);

        botonCMP.isHovered = motorRaylib.IsMouseOver(botonCMP.x, botonCMP.y, botonCMP.ancho, botonCMP.largo, botonCMP.pathSprite);

        static bool isButtonPressed = false;

        if (botonCMP.isHovered)
        {

            botonCMP.pathSprite = botonCMP.pathHovered;

            if (motorRaylib.BotonMousePresionado(GLFW_MOUSE_BUTTON_LEFT) && !isButtonPressed)
            {
                isButtonPressed = true;
                if (botonCMP.id == 1)
                {
                    this->getESM()->SetAnterior(1);
                    this->getESM()->cambiarEstado("Menu");
                }
            }

            if (!motorRaylib.BotonMousePresionado(GLFW_MOUSE_BUTTON_LEFT))
            {
                isButtonPressed = false;
            }
        }
        else
        {
            botonCMP.pathSprite = botonCMP.auxiliarArenderear;
        }
    }
}
