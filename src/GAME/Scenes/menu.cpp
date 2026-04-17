#include "menu.h"
#include <iostream>
#include "../AlmondGE/EntityManager.h"
#include "../../ENGINES/AlmondSE/FmodSoundEngine.h"

Menu::Menu()
    : physics()
{
}

Menu::~Menu() {}

// -----------------------------------------------------------------------------
//                               iniciarMenu()
// -----------------------------------------------------------------------------
void Menu::iniciarMenu()
{
    initSonidos();

    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");

    Entity &jugar = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton{};
    MiColor colorPersonalizado2(0, 0, 0, 0);
    renderComponent renderBoton{0, colorPersonalizado2};
    soundComponent soundBoton;
    SoundEvent se2;
    se2.eventPath = "event:/PowerUps/RecogerItem";
    se2.playOnce = false;
    se2.looping = false;
    se2.isPlaying = false;
    se2.volume = 1.0f;

    soundBoton.addSoundEvent(se2);

    powerUpComponent powerBloqueBoton{};
    bombaComponent bombaBoton{};
    iaComponent botontieneBT{false};
    buttonComponent buttonBoton{
        1, 0, 225, 200, 100, "assets/sprites/jugar.png", "assets/sprites/jugarHover.png", "assets/sprites/jugar.png", false};
    em.createEntityComponents(jugar, fisicaBoton, renderBoton, powerBloqueBoton, soundBoton, bombaBoton, botontieneBT, buttonBoton);

    Entity &salir = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton3{};
    MiColor colorPersonalizado4(0, 0, 0, 0);
    renderComponent renderBoton3{0, colorPersonalizado4};
    soundComponent soundBoton3;
    SoundEvent se4;
    se4.eventPath = "event:/PowerUps/RecogerItem";
    se4.playOnce = false;
    se4.looping = false;
    se4.isPlaying = false;
    se4.volume = 1.0f;
    soundBoton3.addSoundEvent(se4);
    powerUpComponent powerBloqueBoton3{};
    bombaComponent bombaBoton3{};
    iaComponent botontieneBT3{false};
    buttonComponent buttonBoton3{
        3, 0, 570, 200, 100, "assets/sprites/salir.png", "assets/sprites/salirHover.png", "assets/sprites/salir.png", false};
    em.createEntityComponents(salir, fisicaBoton3, renderBoton3, powerBloqueBoton3, soundBoton3, bombaBoton3, botontieneBT3, buttonBoton3);

    Entity &opciones = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton4{};
    MiColor colorPersonalizado5(0, 0, 0, 0);
    renderComponent renderBoton4{0, colorPersonalizado5};
    soundComponent soundBoton4;
    SoundEvent se5;
    se5.eventPath = "event:/PowerUps/RecogerItem";
    se5.playOnce = false;
    se5.looping = false;
    se5.isPlaying = false;
    se5.volume = 1.0f;
    soundBoton4.addSoundEvent(se5);
    powerUpComponent powerBloqueBoton4{};
    bombaComponent bombaBoton4{};
    iaComponent botontieneBT4{false};
    buttonComponent buttonBoton4{
        4, 0, 340, 200, 100, "assets/sprites/opciones.png", "assets/sprites/opcionesHover.png", "assets/sprites/opciones.png", false};
    em.createEntityComponents(opciones, fisicaBoton4, renderBoton4, powerBloqueBoton4, soundBoton4, bombaBoton4, botontieneBT4, buttonBoton4);

    Entity &controles = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton5{};
    MiColor colorPersonalizado6(0, 0, 0, 0);
    renderComponent renderBoton5{0, colorPersonalizado6};
    soundComponent soundBoton5;
    SoundEvent se6;
    se6.eventPath = "event:/PowerUps/RecogerItem";
    se6.playOnce = false;
    se6.looping = false;
    se6.isPlaying = false;
    se6.volume = 1.0f;
    soundBoton5.addSoundEvent(se6);
    powerUpComponent powerBloqueBoton5{};
    bombaComponent bombaBoton5{};
    iaComponent botontieneBT5{false};
    buttonComponent buttonBoton5{
        5, 0, 455, 200, 100, "assets/sprites/controles.png", "assets/sprites/controlesHover.png", "assets/sprites/controles.png", false};
    em.createEntityComponents(controles, fisicaBoton5, renderBoton5, powerBloqueBoton5, soundBoton5, bombaBoton5, botontieneBT5, buttonBoton5);
}

void Menu::initSonidos()
{
    Entity &clic = em.createEntity(EntityType::SOUND);
    fisicaComponent fisicaMusica{};
    MiColor colorPersonalizado7(0, 0, 0, 0);
    renderComponent renderMusica{};
    soundComponent soundMusica;
    SoundEvent se7;
    se7.eventPath = "event:/PowerUps/RecogerItem";
    se7.playOnce = false;
    se7.looping = false;
    se7.isPlaying = false;
    se7.volume = 1.0f;
    se7.soundId = 1;
    soundMusica.addSoundEvent(se7);
    powerUpComponent powerBloqueMusica{};
    bombaComponent bombaMusica{};
    iaComponent musicatieneBT{false};
    buttonComponent buttonMusica{};
    em.createEntityComponents(clic, fisicaMusica, renderMusica, powerBloqueMusica, soundMusica, bombaMusica, musicatieneBT, buttonMusica);

    Entity &musicaMenu = em.createEntity(EntityType::SOUND);
    fisicaComponent fisicaMusicaMenu{};
    MiColor colorPersonalizado8(0, 0, 0, 0);
    renderComponent renderMusicaMenu{};
    soundComponent soundMusicaMenu;
    SoundEvent se8;
    se8.eventPath = "event:/Menus/musicaMenu";
    se8.playOnce = false;
    se8.looping = false;
    se8.isPlaying = false;
    se8.volume = 1.0f;
    se8.soundId = 2;
    soundMusicaMenu.addSoundEvent(se8);
    powerUpComponent powerBloqueMusicaMenu{};
    bombaComponent bombaMusicaMenu{};
    iaComponent musicaMenuTieneBT{false};
    buttonComponent buttonMusicaMenu{};
    em.createEntityComponents(musicaMenu, fisicaMusicaMenu, renderMusicaMenu, powerBloqueMusicaMenu, soundMusicaMenu, bombaMusicaMenu, musicaMenuTieneBT, buttonMusicaMenu);
}
// -----------------------------------------------------------------------------
//                               bucleMenuUpdate()
// -----------------------------------------------------------------------------
void Menu::bucleMenuUpdate()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
    Entity sonido = buscarSonido(2);
    SoundSystem::getInstance().playMusicaFondo(em, sonido);

    manejarClicBoton();

    SoundSystem::getInstance().update();
}

// -----------------------------------------------------------------------------
//                               bucleMenuRender()
// -----------------------------------------------------------------------------
void Menu::bucleMenuRender()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");

    motorRaylib.empezarEscena();

    MiColor colorFondo(242, 205, 136, 255);
    motorRaylib.getMotorDeJuego().limpiarPantalla(colorFondo.r, colorFondo.g, colorFondo.b, colorFondo.a);

    motorRaylib.menuRender(em, physics);

    if (botonActivo == 1)
    {
        motorRaylib.DibujarTextura("assets/sprites/dibujo1.png", 790, 120, 1, 1, MiColor(255, 255, 255, 255));
    }
    else if (botonActivo == 2)
    {
        motorRaylib.DibujarTextura("assets/sprites/dibujo2.png", 800, 120, 1, 1, MiColor(255, 255, 255, 255));
    }
    else if (botonActivo == 3)
    {
        motorRaylib.DibujarTextura("assets/sprites/dibujo3.png", 800, 120, 1, 1, MiColor(255, 255, 255, 255));
    }
    else if (botonActivo == 4)
    {
        motorRaylib.DibujarTextura("assets/sprites/dibujo4.png", 750, 215, 1, 1, MiColor(255, 255, 255, 255));
    }

    motorRaylib.acabarEscena();
}

// -----------------------------------------------------------------------------
//                             manejarClicBoton()
// -----------------------------------------------------------------------------
void Menu::manejarClicBoton()
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

            if (botonCMP.id == 1)
            {
                botonActivo = 1;
            }
            else if (botonCMP.id == 4)
            {
                botonActivo = 2;
            }
            else if (botonCMP.id == 5)
            {
                botonActivo = 3;
            }
            else if (botonCMP.id == 3)
            {
                botonActivo = 4;
            }
            if (motorRaylib.BotonMousePresionado(GLFW_MOUSE_BUTTON_LEFT) && mouseReleased)
            {
                mouseReleased = false;

                Entity sonido = buscarSonido(1);
                SoundSystem::getInstance().playSound(sonido);

                if (botonCMP.id == 1)
                {
                    Entity pararMusica = buscarSonido(2);
                    SoundSystem::getInstance().stopSound(em, pararMusica);
                    this->getESM()->SetAnterior(8);
                    this->getESM()->cambiarEstado("SelecPersonajes");
                }
                else if (botonCMP.id == 3)
                {
                    motorRaylib.getMotorDeJuego().cerrarJuego();
                }
                else if (botonCMP.id == 4)
                {
                    this->getESM()->SetAnterior(8);
                    this->getESM()->cambiarEstado("Ajustes");
                }
                else if (botonCMP.id == 5)
                {
                    this->getESM()->SetAnterior(8);
                    this->getESM()->cambiarEstado("Controles");
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

Entity Menu::buscarSonido(int idSonido)
{
    auto sonidos = em.getEntitiesByType(EntityType::SOUND);
    for (auto &sonido : sonidos)
    {
        const auto &sonidoCMP = em.sounds.get(sonido.sound_key);
        for (auto &evento : sonidoCMP.events)
        {
            if (evento.soundId == idSonido)
            {
                return sonido;
            }
        }
    }

    return Entity();
}
