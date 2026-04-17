#include <iostream>
#include "selecPersonajes.h"
#include "../AlmondGE/EntityManager.h"
#include "../../ENGINES/AlmondSE/FmodSoundEngine.h"
#include "../../utils/Const.h"

int numPlayers = 0;

SelecPersonajes::SelecPersonajes()
{
}

SelecPersonajes::~SelecPersonajes() {}

void SelecPersonajes::iniciarSelecPersonajes()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");

    initPersonajes();
    iniciarBotones();
}

void SelecPersonajes::initBancos()
{
}

void SelecPersonajes::initPersonajes()
{
    Entity &player = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaJugador{
        -12.0f,
        8,
        -10.0f,
    };
    MiColor colorPersonalizado(0, 121, 241, 255);
    renderComponent renderJugador{size, colorPersonalizado};
    powerUpComponent powerBloque{};
    soundComponent soundJugador{};
    bombaComponent bombaJugador{};
    iaComponent playertieneBT{false};
    buttonComponent buttonJugador{};

    em.createEntityComponents(player, fisicaJugador, renderJugador, powerBloque, soundJugador, bombaJugador, playertieneBT, buttonJugador);

    Entity &player2 = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaNpc{-2.0f, 8, -10.0f};
    MiColor colorPersonalizado2(230, 41, 55, 255);
    renderComponent renderNpc{size, colorPersonalizado2};
    soundComponent soundNpc{};
    powerUpComponent powerBloqueNpc{};
    bombaComponent bombaNpc{};
    iaComponent npctieneBT{false};

    em.createEntityComponents(player2, fisicaNpc, renderNpc, powerBloqueNpc, soundNpc, bombaNpc, npctieneBT, buttonJugador);

    Entity &player3 = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaNpc2{8.0f, 8, -10.0f};
    MiColor colorPersonalizadoNPC(230, 41, 55, 255);
    renderComponent renderNpc2{size, colorPersonalizado2};
    soundComponent soundNpc2{};
    powerUpComponent powerBloqueNpc2{};
    bombaComponent bombaNpc2{};
    iaComponent npctieneBT2{false};

    em.createEntityComponents(player3, fisicaNpc2, renderNpc2, powerBloqueNpc2, soundNpc2, bombaNpc2, npctieneBT2, buttonJugador);

    Entity &player4 = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaNpc3{18.0f, 8, -10.0f};
    MiColor colorPersonalizadoNPC2(230, 41, 55, 255);
    renderComponent renderNpc3{size, colorPersonalizado2};
    soundComponent soundNpc3{};
    powerUpComponent powerBloqueNpc3{};
    bombaComponent bombaNpc3{};
    iaComponent npctieneBT3{false};

    em.createEntityComponents(player4, fisicaNpc3, renderNpc3, powerBloqueNpc3, soundNpc3, bombaNpc3, npctieneBT3, buttonJugador);
}

void SelecPersonajes::iniciarBotones()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
    Entity &jugador1 = em.createEntity(EntityType::BUTTON);
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
    se2.soundId = 3;

    soundBoton.addSoundEvent(se2);

    powerUpComponent powerBloqueBoton{};
    bombaComponent bombaBoton{};
    iaComponent botontieneBT{false};
    buttonComponent buttonBoton{
        1, 147, 75, 200, 100, "assets/sprites/personaje1.png", "assets/sprites/personaje1Hover.png", "assets/sprites/personaje1.png", false};
    em.createEntityComponents(jugador1, fisicaBoton, renderBoton, powerBloqueBoton, soundBoton, bombaBoton, botontieneBT, buttonBoton);

    Entity &jugador2 = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton2{};
    MiColor colorPersonalizado3(0, 0, 0, 0);
    renderComponent renderBoton2{0, colorPersonalizado3};
    soundComponent soundBoton2;
    SoundEvent se3;
    se3.eventPath = "event:/PowerUps/RecogerItem";
    se3.playOnce = false;
    se3.looping = false;
    se3.isPlaying = false;
    se3.volume = 1.0f;
    se3.soundId = 4;

    soundBoton2.addSoundEvent(se3);
    powerUpComponent powerBloqueBoton2{};
    bombaComponent bombaBoton2{};
    iaComponent botontieneBT2{false};
    buttonComponent buttonBoton2{
        2, 417, 75, 200, 100, "assets/sprites/personaje2.png", "assets/sprites/personaje2Hover.png", "assets/sprites/personaje2.png", false};
    em.createEntityComponents(jugador2, fisicaBoton2, renderBoton2, powerBloqueBoton2, soundBoton2, bombaBoton2, botontieneBT2, buttonBoton2);

    Entity &jugador3 = em.createEntity(EntityType::BUTTON);
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
    se4.soundId = 5;

    soundBoton3.addSoundEvent(se4);
    powerUpComponent powerBloqueBoton3{};
    bombaComponent bombaBoton3{};
    iaComponent botontieneBT3{false};
    buttonComponent buttonBoton3{
        3, 687, 75, 200, 100, "assets/sprites/personaje3.png", "assets/sprites/personaje3Hover.png", "assets/sprites/personaje3.png", false};
    em.createEntityComponents(jugador3, fisicaBoton3, renderBoton3, powerBloqueBoton3, soundBoton3, bombaBoton3, botontieneBT3, buttonBoton3);

    Entity &jugador4 = em.createEntity(EntityType::BUTTON);
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
    se5.soundId = 6;

    soundBoton4.addSoundEvent(se5);
    powerUpComponent powerBloqueBoton4{};
    bombaComponent bombaBoton4{};
    iaComponent botontieneBT4{false};
    buttonComponent buttonBoton4{
        4, 957, 75, 200, 100, "assets/sprites/personaje4.png", "assets/sprites/personaje4Hover.png", "assets/sprites/personaje4.png", false};
    em.createEntityComponents(jugador4, fisicaBoton4, renderBoton4, powerBloqueBoton4, soundBoton4, bombaBoton4, botontieneBT4, buttonBoton4);

    Entity &jugar = em.createEntity(EntityType::BUTTON);
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
    se6.soundId = 7;

    soundBoton5.addSoundEvent(se6);
    powerUpComponent powerBloqueBoton5{};
    bombaComponent bombaBoton5{};
    iaComponent botontieneBT5{false};
    buttonComponent buttonBoton5{
        5, 140, 550, 200, 100, "assets/sprites/clasico.png", "assets/sprites/clasicoHover.png", "assets/sprites/clasico.png", false};
    em.createEntityComponents(jugar, fisicaBoton5, renderBoton5, powerBloqueBoton5, soundBoton5, bombaBoton5, botontieneBT5, buttonBoton5);

    Entity &pintura = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton6{};
    MiColor colorPersonalizado7(0, 0, 0, 0);
    renderComponent renderBoton6{0, colorPersonalizado7};
    soundComponent soundBoton6;
    SoundEvent se7;
    se7.eventPath = "event:/PowerUps/RecogerItem";
    se7.playOnce = false;
    se7.looping = false;
    se7.isPlaying = false;
    se7.volume = 1.0f;
    se7.soundId = 8;

    soundBoton6.addSoundEvent(se7);
    powerUpComponent powerBloqueBoton6{};
    bombaComponent bombaBoton6{};
    iaComponent botontieneBT6{false};
    buttonComponent buttonBoton6{
        6, 680, 550, 200, 100, "assets/sprites/pintura.png", "assets/sprites/pinturaHover.png", "assets/sprites/pintura.png", false};
    em.createEntityComponents(pintura, fisicaBoton6, renderBoton6, powerBloqueBoton6, soundBoton6, bombaBoton6, botontieneBT6, buttonBoton6);

    Entity &atras = em.createEntity(EntityType::BUTTON);
    fisicaComponent fisicaBoton7{};
    MiColor colorPersonalizado8(0, 0, 0, 0);
    renderComponent renderBoton7{0, colorPersonalizado8};
    soundComponent soundBoton7;
    SoundEvent se8;
    se8.eventPath = "event:/PowerUps/RecogerItem";
    se8.playOnce = false;
    se8.looping = false;
    se8.isPlaying = false;
    se8.volume = 1.0f;
    se8.soundId = 9;

    soundBoton7.addSoundEvent(se8);
    powerUpComponent powerBloqueBoton7{};
    bombaComponent bombaBoton7{};
    iaComponent botontieneBT7{false};
    buttonComponent buttonBoton7{
        7, 25, 25, 200, 100, "assets/sprites/atras2.png", "assets/sprites/atras2Hover.png", "assets/sprites/atras2.png", false};
    em.createEntityComponents(atras, fisicaBoton7, renderBoton7, powerBloqueBoton7, soundBoton7, bombaBoton7, botontieneBT7, buttonBoton7);
}

void SelecPersonajes::bucleSelecPersonajesRender()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
    motorRaylib.empezarEscena();
    motorRaylib.SeleccionRender(em, physics);
    motorRaylib.acabarEscena();
}

void SelecPersonajes::bucleSelecPersonajesUpdate()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
    if (!boolReset)
    {
        initPersonajes();
        boolReset = true;
    }
    if (initBancosBool)
    {
        initBancos();
        initBancosBool = false;
    }
    if (setReset == false)
    {
        motorRaylib.getMotorDeJuego().Reset();
        motorRaylib.initCamara();
        motorRaylib.ConfigurarLuces();
        setReset = true;
    }
    if (boolEntidades)
    {
        auto players = em.getEntitiesByType(EntityType::PLAYER);
        for (auto &player : players)
        {
            auto &jugadorRender = em.renders.get(player.render_key);
            jugadorRender.SusDatosEnElArbol = nullptr;
            jugadorRender.cargadoEnElArbol = false;
        }
        boolEntidades = false;
    }
    auto players = em.getEntitiesByType(EntityType::PLAYER);
    for (auto &player : players)
    {
        auto &jugadorRender = em.renders.get(player.render_key);
    }

    manejarClicBoton();
    SoundSystem::getInstance().update();
}

void SelecPersonajes::manejarClicBoton()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
    auto botones = em.getEntitiesByType(EntityType::BUTTON);

    int maxPlayers = inputSystem.contarMandosConectados();
    if (maxPlayers == 0)
    {
        maxPlayers = 1;
    }
    for (auto &boton : botones)
    {
        auto &botonCMP = em.buttons.get(boton.button_key);

        if (botonCMP.id == 1 && boolClickPinta == false)
        {
            botonCMP.pathSprite = "assets/sprites/personaje1.png";
        }
        else if (botonCMP.id == 2 && boolClickPinta2 == false)
        {
            botonCMP.pathSprite = "assets/sprites/personaje2.png";
        }
        else if (botonCMP.id == 3 && boolClickPinta3 == false)
        {
            botonCMP.pathSprite = "assets/sprites/personaje3.png";
        }
        else if (botonCMP.id == 4 && boolClickPinta4 == false)
        {
            botonCMP.pathSprite = "assets/sprites/personaje4.png";
        }

        botonCMP.isHovered = motorRaylib.IsMouseOver(botonCMP.x, botonCMP.y, botonCMP.ancho, botonCMP.largo, botonCMP.pathSprite);

        if (botonCMP.isHovered)
        {
            if (botonCMP.id != 1 && botonCMP.id != 2 && botonCMP.id != 3 && botonCMP.id != 4)
            {
                botonCMP.pathSprite = botonCMP.pathHovered;
            }

            if (motorRaylib.BotonMousePresionado(GLFW_MOUSE_BUTTON_LEFT) && mouseReleased && contFrames == 30)
            {
                mouseReleased = false;

                if (botonCMP.id == 1)
                {
                    Entity sonido = buscarSonido(3);
                    SoundSystem::getInstance().playSoundEffect(em, sonido);

                    if (boolClickPinta)
                    {
                        botonCMP.pathSprite = "assets/sprites/personaje1.png";
                        boolClickPinta = false;
                        --numPlayers;
                    }
                    else
                    {
                        if (numPlayers < maxPlayers)
                        {
                            botonCMP.pathSprite = "assets/sprites/personaje1Hover.png";
                            boolClickPinta = true;
                            ++numPlayers;
                        }
                    }
                }
                else if (botonCMP.id == 2)
                {
                    Entity sonido = buscarSonido(4);
                    SoundSystem::getInstance().playSoundEffect(em, sonido);

                    if (boolClickPinta2)
                    {
                        botonCMP.pathSprite = "assets/sprites/personaje2.png";
                        boolClickPinta2 = false;
                        --numPlayers;
                    }
                    else
                    {
                        if (numPlayers < maxPlayers)
                        {
                            botonCMP.pathSprite = "assets/sprites/personaje2Hover.png";
                            boolClickPinta2 = true;
                            ++numPlayers;
                        }
                    }
                }
                else if (botonCMP.id == 3)
                {
                    Entity sonido = buscarSonido(5);
                    SoundSystem::getInstance().playSoundEffect(em, sonido);

                    if (boolClickPinta3)
                    {
                        botonCMP.pathSprite = "assets/sprites/personaje3.png";
                        boolClickPinta3 = false;
                        --numPlayers;
                    }
                    else
                    {
                        if (numPlayers < maxPlayers)
                        {
                            botonCMP.pathSprite = "assets/sprites/personaje3Hover.png";
                            boolClickPinta3 = true;
                            ++numPlayers;
                        }
                    }
                }
                else if (botonCMP.id == 4)
                {
                    Entity sonido = buscarSonido(6);
                    SoundSystem::getInstance().playSoundEffect(em, sonido);

                    if (boolClickPinta4)
                    {
                        botonCMP.pathSprite = "assets/sprites/personaje4.png";
                        boolClickPinta4 = false;
                        --numPlayers;
                    }
                    else
                    {
                        if (numPlayers < maxPlayers)
                        {
                            botonCMP.pathSprite = "assets/sprites/personaje4Hover.png";
                            boolClickPinta4 = true;
                            ++numPlayers;
                        }
                    }
                }

                else if (botonCMP.id == 5 && numPlayers >= 1)
                {
                    Entity sonido = buscarSonido(7);
                    SoundSystem::getInstance().playSoundEffect(em, sonido);
                    initBancosBool = true;
                    boolEntidades = true;
                    boolClickPinta = false;
                    boolClickPinta2 = false;
                    boolClickPinta3 = false;
                    boolClickPinta4 = false;
                    setReset = false;
                    boolReset = false;
                    reset();
                    this->getESM()->SetAnterior(0);
                    this->getESM()->cambiarEstado("Juego");
                }
                else if (botonCMP.id == 6 && numPlayers >= 1)
                {
                    Entity sonido = buscarSonido(8);
                    SoundSystem::getInstance().playSoundEffect(em, sonido);
                    initBancosBool = true;
                    boolEntidades = true;
                    boolClickPinta = false;
                    boolClickPinta2 = false;
                    boolClickPinta3 = false;
                    boolClickPinta4 = false;
                    setReset = false;
                    boolReset = false;
                    reset();
                    this->getESM()->SetAnterior(0);
                    this->getESM()->cambiarEstado("Pintura");
                }
                else if (botonCMP.id == 7)
                {
                    Entity sonido = buscarSonido(9);
                    SoundSystem::getInstance().playSoundEffect(em, sonido);
                    initBancosBool = true;
                    this->getESM()->SetAnterior(0);
                    this->getESM()->cambiarEstado("Menu");
                }
            }

            if (!motorRaylib.BotonMousePresionado(GLFW_MOUSE_BUTTON_LEFT))
            {
                mouseReleased = true;
            }
        }
        else if (botonCMP.id != 1 && botonCMP.id != 2 && botonCMP.id != 3 && botonCMP.id != 4)
        {
            botonCMP.pathSprite = botonCMP.auxiliarArenderear;
        }
        if (boolRetraso && contFrames < 30)
        {
            contFrames++;
        }
        else if (contFrames >= 30)
        {
            boolRetraso = false;
        }
    }
}
Entity SelecPersonajes::buscarSonido(int idSonido)
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

    auto sonidos2 = em.getEntitiesByType(EntityType::BUTTON);
    for (auto &sonido : sonidos2)
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

void SelecPersonajes::reset()
{
    em.forEachErase(EntityType::PLAYER);
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
}