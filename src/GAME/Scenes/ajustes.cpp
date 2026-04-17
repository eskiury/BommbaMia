#include "ajustes.h"
#include <print>
#include "../AlmondGE/EntityManager.h"
#include "../../ENGINES/AlmondSE/FmodSoundEngine.h"

Ajustes::Ajustes()
{
}

Ajustes::~Ajustes()
{
}

void Ajustes::iniciarAjustes()
{
    Entity &salir = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton3{};
    MiColor colorPersonalizado4(0, 0, 0, 0);
    renderComponent renderBoton3{0, colorPersonalizado4};
    soundComponent soundBoton3;
    SoundEvent se4;
    se4.eventPath = "event:/PowerUps/RecogerItem";
    se4.playOnce = true;
    se4.looping = false;
    se4.isPlaying = false;
    se4.volume = 1.0f;
    se4.soundId = 1;
    soundBoton3.addSoundEvent(se4);
    powerUpComponent powerBloqueBoton3{};
    bombaComponent bombaBoton3{};
    iaComponent botontieneBT3{false};
    buttonComponent buttonBoton3{
        1, 25, 25, 600, 100, "assets/sprites/atras.png", "assets/sprites/atrasHover.png", "assets/sprites/atras.png", false};
    em.createEntityComponents(salir, fisicaBoton3, renderBoton3, powerBloqueBoton3, soundBoton3, bombaBoton3, botontieneBT3, buttonBoton3);

    Entity &subirVol = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton4{};
    MiColor colorPersonalizado5(0, 0, 0, 0);
    renderComponent renderBoton4{0, colorPersonalizado5};
    soundComponent soundBoton4;
    SoundEvent se5;
    se5.eventPath = "event:/PowerUps/RecogerItem";
    se5.playOnce = true;
    se5.looping = false;
    se5.isPlaying = false;
    se5.volume = 1.0f;
    se5.soundId = 2;
    soundBoton4.addSoundEvent(se5);
    powerUpComponent powerBloqueBoton4{};
    bombaComponent bombaBoton4{};
    iaComponent botontieneBT4{false};
    buttonComponent buttonBoton4{
        2, 917, 294, 200, 100, "assets/sprites/subirVolumenMusica.png", "assets/sprites/subirVolumenMusicaHOVER.png", "assets/sprites/subirVolumenMusica.png", false};
    em.createEntityComponents(subirVol, fisicaBoton4, renderBoton4, powerBloqueBoton4, soundBoton4, bombaBoton4, botontieneBT4, buttonBoton4);

    Entity &bajarVol = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton5{};
    MiColor colorPersonalizado6(0, 0, 0, 0);
    renderComponent renderBoton5{0, colorPersonalizado6};
    soundComponent soundBoton5;
    SoundEvent se6;
    se6.eventPath = "event:/PowerUps/RecogerItem";
    se6.playOnce = true;
    se6.looping = false;
    se6.isPlaying = false;
    se6.volume = 1.0f;
    se6.soundId = 3;
    soundBoton5.addSoundEvent(se6);
    powerUpComponent powerBloqueBoton5{};
    bombaComponent bombaBoton5{};
    iaComponent botontieneBT5{false};
    buttonComponent buttonBoton5{
        3, 280, 294, 200, 100, "assets/sprites/bajarVolumenMusica.png", "assets/sprites/bajarVolumenMusicaHOVER.png", "assets/sprites/bajarVolumenMusica.png", false};
    em.createEntityComponents(bajarVol, fisicaBoton5, renderBoton5, powerBloqueBoton5, soundBoton5, bombaBoton5, botontieneBT5, buttonBoton5);

    Entity &subirVol2 = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton6{};
    MiColor colorPersonalizado7(0, 0, 0, 0);
    renderComponent renderBoton6{0, colorPersonalizado7};
    soundComponent soundBoton6;
    SoundEvent se7;
    se7.eventPath = "event:/PowerUps/RecogerItem";
    se7.playOnce = true;
    se7.looping = false;
    se7.isPlaying = false;
    se7.volume = 1.0f;
    se7.soundId = 4;
    soundBoton6.addSoundEvent(se7);
    powerUpComponent powerBloqueBoton6{};
    bombaComponent bombaBoton6{};
    iaComponent botontieneBT6{false};
    buttonComponent buttonBoton6{
        4, 917, 503, 200, 100, "assets/sprites/subirVolumenSFX.png", "assets/sprites/subirVolumenSFXHOVER.png", "assets/sprites/subirVolumenSFX.png", false};
    em.createEntityComponents(subirVol2, fisicaBoton6, renderBoton6, powerBloqueBoton6, soundBoton6, bombaBoton6, botontieneBT6, buttonBoton6);

    Entity &bajarVol2 = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton7{};
    MiColor colorPersonalizado8(0, 0, 0, 0);
    renderComponent renderBoton7{0, colorPersonalizado8};
    soundComponent soundBoton7;
    SoundEvent se8;
    se8.eventPath = "event:/PowerUps/RecogerItem";
    se8.playOnce = true;
    se8.looping = false;
    se8.isPlaying = false;
    se8.volume = 1.0f;
    se8.soundId = 5;
    soundBoton7.addSoundEvent(se8);
    powerUpComponent powerBloqueBoton7{};
    bombaComponent bombaBoton7{};
    iaComponent botontieneBT7{false};
    buttonComponent buttonBoton7{
        5, 280, 503, 200, 100, "assets/sprites/bajarVolumenSFX.png", "assets/sprites/bajarVolumenSFXHOVER.png", "assets/sprites/bajarVolumenSFX.png", false};
    em.createEntityComponents(bajarVol2, fisicaBoton7, renderBoton7, powerBloqueBoton7, soundBoton7, bombaBoton7, botontieneBT7, buttonBoton7);
}

void Ajustes::bucleAjustesUpdate()
{
    manejarClicBoton();
    SoundSystem::getInstance().update();
}

void Ajustes::bucleAjustesRender()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia"); // coge la instancia unica del motor

    motorRaylib.empezarEscena();
    motorRaylib.opcionesRender(em, physics);
    motorRaylib.acabarEscena();
}

void Ajustes::manejarClicBoton()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
    auto botones = em.getEntitiesByType(EntityType::BUTTON);

    for (auto &boton : botones)
    {
        auto &botonCMP = em.buttons.get(boton.button_key);

        botonCMP.isHovered = motorRaylib.IsMouseOver(botonCMP.x, botonCMP.y, botonCMP.ancho, botonCMP.largo, botonCMP.pathSprite);

        if (botonCMP.isHovered)
        {
            botonCMP.pathSprite = botonCMP.pathHovered;

            if (motorRaylib.BotonMousePresionado(GLFW_MOUSE_BUTTON_LEFT) && mouseReleased)
            {
                mouseReleased = false;

                if (botonCMP.id == 1)
                {
                    if (this->getESM()->GetAnterior() == 1)
                    {
                        this->getESM()->SetAnterior(1);
                        this->getESM()->cambiarEstado("Juego");
                    }
                    else if (this->getESM()->GetAnterior() == 5)
                    {
                        this->getESM()->SetAnterior(1);
                        this->getESM()->cambiarEstado("Pintura");
                    }
                    else
                    {
                        this->getESM()->SetAnterior(1);
                        this->getESM()->cambiarEstado("Menu");
                    }
                }
                else if (botonCMP.id == 2)
                {
                    valor_musica += 0.1f;

                    if (valor_musica > 1.0f)
                        valor_musica = 1.0f;

                    SoundSystem::getInstance().setBusVolume("bus:/Music", valor_musica);
                    SoundSystem::getInstance().playSoundEffect(em, boton);
                }
                else if (botonCMP.id == 3)
                {
                    valor_musica -= 0.1f;

                    if (valor_musica < 0.0f)
                        valor_musica = 0.0f;

                    SoundSystem::getInstance().setBusVolume("bus:/Music", valor_musica);
                    SoundSystem::getInstance().playSoundEffect(em, boton);
                }
                else if (botonCMP.id == 4)
                {

                    valor_sfx += 0.1f;

                    if (valor_sfx > 1.0f)
                        valor_sfx = 1.0f;

                    SoundSystem::getInstance().setBusVolume("bus:/Sfx", valor_sfx);
                    SoundSystem::getInstance().playSoundEffect(em, boton);
                }
                else if (botonCMP.id == 5)
                {

                    valor_sfx -= 0.1f;

                    if (valor_sfx < 0.0f)
                        valor_sfx = 0.0f;

                    SoundSystem::getInstance().setBusVolume("bus:/Sfx", valor_sfx);
                    SoundSystem::getInstance().playSoundEffect(em, boton);
                }
            }
        }
        else
        {
            botonCMP.pathSprite = botonCMP.auxiliarArenderear;
        }
    }

    if (!motorRaylib.BotonMousePresionado(GLFW_MOUSE_BUTTON_LEFT))
    {
        mouseReleased = true;
    }
}