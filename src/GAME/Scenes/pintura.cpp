// ================================================================
//                           INCLUDES
// ================================================================
#include "pintura.h"
#include "../../utils/miColor.h"
#include <cmath>
#include "../../utils/Const.h"
#include "../../utils/levelLoader.h"
#include "selecPersonajes.h"

// ================================================================
//                CONSTRUCTOR Y DESTRUCTOR
// ================================================================
Pintura::Pintura(ElMotorRaylib &motorRaylib)
    : motorRaylib(motorRaylib),

      renderSys(motorRaylib, em),
      jugando(true),
      physics(),
      iaSystem(),
      bombSystem(),
      gameplay(em),
      inputSystem(&eventSystem),
      scheduler(),
      seleccionActual(0),
      tiempoRestante(0.0f),
      minutosDePintura(1),
      contadorCuadrosPintura(0),
      compruebaPintura(true),
      mapSystem(15, 13),
      perdido(false),
      animacion(true)
{
    configurarEventos();
}

Pintura::~Pintura()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
}

// ================================================================
//                     CONFIGURAR EVENTOS
// ================================================================
void Pintura::configurarEventos()
{
    // Suscribirse a los eventos de movimiento.
    eventSystem.subscribe(EventType::MOVE_RIGHT, [this](Entity &entity) {});

    eventSystem.subscribe(EventType::MOVE_LEFT, [this](Entity &entity) {});

    eventSystem.subscribe(EventType::MOVE_UP, [this](Entity &entity) {});

    eventSystem.subscribe(EventType::MOVE_DOWN, [this](Entity &entity) {});

    eventSystem.subscribe(EventType::PLACE_BOMB, [this](Entity &entity)
                          { bombSystem.crearBomba(entity, em, mapSystem); });

       eventSystem.subscribe(EventType::PAUSE, [this](Entity &entity)
                          { jugando = !jugando; });
}

// ================================================================
//                 MÉTODOS DE INICIALIZACIÓN
// ================================================================

void Pintura::init1player()
{
    Entity &player = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaJugador{
        2.0f,
        0,
        2.0f,
    };
    MiColor colorPersonalizado(0, 121, 241, 255);
    renderComponent renderJugador{size, colorPersonalizado, 0.0f, "assets/modelos/animaciones/personaje1.obj"};
    powerUpComponent powerBloque{};
    soundComponent soundJugador{};
    bombaComponent bombaJugador{};
    iaComponent playertieneBT{false};
    buttonComponent buttonJugador{};

    em.createEntityComponents(player, fisicaJugador, renderJugador, powerBloque, soundJugador, bombaJugador, playertieneBT, buttonJugador);

    Entity &npc = em.createEntity(EntityType::NPC);
    fisicaComponent fisicaNpc{26.0f, 0, 22.0f};
    MiColor colorPersonalizado2(230, 41, 55, 255);
    renderComponent renderNpc{size, colorPersonalizado2, 180.0f, "assets/modelos/animaciones/personaje2.obj"};
    soundComponent soundNpc{};
    powerUpComponent powerBloqueNpc{};
    bombaComponent bombaNpc{};
    iaComponent npctieneBT{true};

    em.createEntityComponents(npc, fisicaNpc, renderNpc, powerBloqueNpc, soundNpc, bombaNpc, npctieneBT, buttonJugador);

    Entity &npc2 = em.createEntity(EntityType::NPC);
    fisicaComponent fisicaNpc2{2.0f, 0, 22.0f};
    MiColor colorPersonalizadoNPC(230, 41, 55, 255);
    renderComponent renderNpc2{size, colorPersonalizado2, 180.0f, "assets/modelos/animaciones/personaje3.obj"};
    soundComponent soundNpc2{};
    powerUpComponent powerBloqueNpc2{};
    bombaComponent bombaNpc2{};
    iaComponent npctieneBT2{true};

    em.createEntityComponents(npc2, fisicaNpc2, renderNpc2, powerBloqueNpc2, soundNpc2, bombaNpc2, npctieneBT2, buttonJugador);

    Entity &npc3 = em.createEntity(EntityType::NPC);
    fisicaComponent fisicaNpc3{26.0f, 0, 2.0f};
    MiColor colorPersonalizadoNPC2(230, 41, 55, 255);
    renderComponent renderNpc3{size, colorPersonalizado2, 0.0f, "assets/modelos/animaciones/personaje4.obj"};
    soundComponent soundNpc3{};
    powerUpComponent powerBloqueNpc3{};
    bombaComponent bombaNpc3{};
    iaComponent npctieneBT3{true};

    em.createEntityComponents(npc3, fisicaNpc3, renderNpc3, powerBloqueNpc3, soundNpc3, bombaNpc3, npctieneBT3, buttonJugador);
    iaManager managerdeia;
    managerdeia.cargarNPC3("assets/ia.json", em, npc, npc2, npc3);
}

void Pintura::init2players()
{
    Entity &player = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaJugador{
        2.0f,
        0,
        2.0f,
    };
    MiColor colorPersonalizado(0, 121, 241, 255);
    renderComponent renderJugador{size, colorPersonalizado, 0.0f, "assets/modelos/animaciones/personaje1.obj"};
    powerUpComponent powerBloque{};
    soundComponent soundJugador{};
    bombaComponent bombaJugador{};
    iaComponent playertieneBT{false};
    buttonComponent buttonJugador{};

    em.createEntityComponents(player, fisicaJugador, renderJugador, powerBloque, soundJugador, bombaJugador, playertieneBT, buttonJugador);

    Entity &player2 = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaNpc{26.0f, 0, 22.0f};
    MiColor colorPersonalizado2(230, 41, 55, 255);
    renderComponent renderNpc{size, colorPersonalizado2, 180.0f, "assets/modelos/animaciones/personaje2.obj"};
    soundComponent soundNpc{};
    powerUpComponent powerBloqueNpc{};
    bombaComponent bombaNpc{};
    iaComponent npctieneBT{false};

    em.createEntityComponents(player2, fisicaNpc, renderNpc, powerBloqueNpc, soundNpc, bombaNpc, npctieneBT, buttonJugador);

    Entity &npc2 = em.createEntity(EntityType::NPC);
    fisicaComponent fisicaNpc2{2.0f, 0, 22.0f};
    MiColor colorPersonalizadoNPC(230, 41, 55, 255);
    renderComponent renderNpc2{size, colorPersonalizado2, 180.0f, "assets/modelos/animaciones/personaje3.obj"};
    soundComponent soundNpc2{};
    powerUpComponent powerBloqueNpc2{};
    bombaComponent bombaNpc2{};
    iaComponent npctieneBT2{true};

    em.createEntityComponents(npc2, fisicaNpc2, renderNpc2, powerBloqueNpc2, soundNpc2, bombaNpc2, npctieneBT2, buttonJugador);

    Entity &npc3 = em.createEntity(EntityType::NPC);
    fisicaComponent fisicaNpc3{26.0f, 0, 2.0f};
    MiColor colorPersonalizadoNPC2(230, 41, 55, 255);
    renderComponent renderNpc3{size, colorPersonalizado2, 0.0f, "assets/modelos/animaciones/personaje4.obj"};
    soundComponent soundNpc3{};
    powerUpComponent powerBloqueNpc3{};
    bombaComponent bombaNpc3{};
    iaComponent npctieneBT3{true};

    em.createEntityComponents(npc3, fisicaNpc3, renderNpc3, powerBloqueNpc3, soundNpc3, bombaNpc3, npctieneBT3, buttonJugador);
    iaManager managerdeia;
    managerdeia.cargarNPC2("assets/ia.json", em, npc2, npc3);
}

void Pintura::init3players()
{
    Entity &player = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaJugador{
        2.0f,
        0,
        2.0f,
    };
    MiColor colorPersonalizado(0, 121, 241, 255);
    renderComponent renderJugador{size, colorPersonalizado, 0.0f, "assets/modelos/animaciones/personaje1.obj"};
    powerUpComponent powerBloque{};
    soundComponent soundJugador{};
    bombaComponent bombaJugador{};
    iaComponent playertieneBT{false};
    buttonComponent buttonJugador{};

    em.createEntityComponents(player, fisicaJugador, renderJugador, powerBloque, soundJugador, bombaJugador, playertieneBT, buttonJugador);

    Entity &player2 = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaNpc{26.0f, 0, 22.0f};
    MiColor colorPersonalizado2(230, 41, 55, 255);
    renderComponent renderNpc{size, colorPersonalizado2, 180.0f, "assets/modelos/animaciones/personaje2.obj"};
    soundComponent soundNpc{};
    powerUpComponent powerBloqueNpc{};
    bombaComponent bombaNpc{};
    iaComponent npctieneBT{false};

    em.createEntityComponents(player2, fisicaNpc, renderNpc, powerBloqueNpc, soundNpc, bombaNpc, npctieneBT, buttonJugador);

    Entity &player3 = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaNpc2{2.0f, 0, 22.0f};
    MiColor colorPersonalizadoNPC(230, 41, 55, 255);
    renderComponent renderNpc2{size, colorPersonalizado2, 180.0f, "assets/modelos/animaciones/personaje3.obj"};
    soundComponent soundNpc2{};
    powerUpComponent powerBloqueNpc2{};
    bombaComponent bombaNpc2{};
    iaComponent npctieneBT2{false};

    em.createEntityComponents(player3, fisicaNpc2, renderNpc2, powerBloqueNpc2, soundNpc2, bombaNpc2, npctieneBT2, buttonJugador);

    Entity &npc3 = em.createEntity(EntityType::NPC);
    fisicaComponent fisicaNpc3{26.0f, 0, 2.0f};
    MiColor colorPersonalizadoNPC2(230, 41, 55, 255);
    renderComponent renderNpc3{size, colorPersonalizado2, 0.0f, "assets/modelos/animaciones/personaje4.obj"};
    soundComponent soundNpc3{};
    powerUpComponent powerBloqueNpc3{};
    bombaComponent bombaNpc3{};
    iaComponent npctieneBT3{true};

    em.createEntityComponents(npc3, fisicaNpc3, renderNpc3, powerBloqueNpc3, soundNpc3, bombaNpc3, npctieneBT3, buttonJugador);
    iaManager managerdeia;
    managerdeia.cargarNPC1("assets/ia.json", em, npc3);
}

void Pintura::init4players()
{
    Entity &player = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaJugador{
        2.0f,
        0,
        2.0f,
    };
    MiColor colorPersonalizado(0, 121, 241, 255);
    renderComponent renderJugador{size, colorPersonalizado, 0.0f, "assets/modelos/animaciones/personaje1.obj"};
    powerUpComponent powerBloque{};
    soundComponent soundJugador{};
    bombaComponent bombaJugador{};
    iaComponent playertieneBT{false};
    buttonComponent buttonJugador{};

    em.createEntityComponents(player, fisicaJugador, renderJugador, powerBloque, soundJugador, bombaJugador, playertieneBT, buttonJugador);

    Entity &player2 = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaNpc{26.0f, 0, 22.0f};
    MiColor colorPersonalizado2(230, 41, 55, 255);
    renderComponent renderNpc{size, colorPersonalizado2, 180.0f, "assets/modelos/animaciones/personaje2.obj"};
    soundComponent soundNpc{};
    powerUpComponent powerBloqueNpc{};
    bombaComponent bombaNpc{};
    iaComponent npctieneBT{false};

    em.createEntityComponents(player2, fisicaNpc, renderNpc, powerBloqueNpc, soundNpc, bombaNpc, npctieneBT, buttonJugador);

    Entity &player3 = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaNpc2{2.0f, 0, 22.0f};
    MiColor colorPersonalizadoNPC(230, 41, 55, 255);
    renderComponent renderNpc2{size, colorPersonalizado2, 180.0f, "assets/modelos/animaciones/personaje3.obj"};
    soundComponent soundNpc2{};
    powerUpComponent powerBloqueNpc2{};
    bombaComponent bombaNpc2{};
    iaComponent npctieneBT2{false};

    em.createEntityComponents(player3, fisicaNpc2, renderNpc2, powerBloqueNpc2, soundNpc2, bombaNpc2, npctieneBT2, buttonJugador);

    Entity &player4 = em.createEntity(EntityType::PLAYER);
    fisicaComponent fisicaNpc3{26.0f, 0, 2.0f};
    MiColor colorPersonalizadoNPC2(230, 41, 55, 255);
    renderComponent renderNpc3{size, colorPersonalizado2, 0.0f, "assets/modelos/animaciones/personaje4.obj"};
    soundComponent soundNpc3{};
    powerUpComponent powerBloqueNpc3{};
    bombaComponent bombaNpc3{};
    iaComponent npctieneBT3{false};

    em.createEntityComponents(player4, fisicaNpc3, renderNpc3, powerBloqueNpc3, soundNpc3, bombaNpc3, npctieneBT3, buttonJugador);
}

void Pintura::initNPCs()
{
    Entity &npc0 = em.createEntity(EntityType::NPC);
    fisicaComponent fisicaJugador{
        2.0f,
        0,
        2.0f,
    };
    MiColor colorPersonalizado(0, 121, 241, 255);
    renderComponent renderJugador{size, colorPersonalizado, 0.0f, "assets/modelos/animaciones/personaje1.obj"};
    powerUpComponent powerBloque{};
    soundComponent soundJugador{};
    bombaComponent bombaJugador{};
    iaComponent playertieneBT{true};
    buttonComponent buttonJugador{};

    em.createEntityComponents(npc0, fisicaJugador, renderJugador, powerBloque, soundJugador, bombaJugador, playertieneBT, buttonJugador);

    Entity &npc = em.createEntity(EntityType::NPC);
    fisicaComponent fisicaNpc{26.0f, 0, 22.0f};
    MiColor colorPersonalizado2(230, 41, 55, 255);
    renderComponent renderNpc{size, colorPersonalizado2, 180.0f, "assets/modelos/animaciones/personaje2.obj"};
    soundComponent soundNpc{};
    powerUpComponent powerBloqueNpc{};
    bombaComponent bombaNpc{};
    iaComponent npctieneBT{true};

    em.createEntityComponents(npc, fisicaNpc, renderNpc, powerBloqueNpc, soundNpc, bombaNpc, npctieneBT, buttonJugador);

    Entity &npc2 = em.createEntity(EntityType::NPC);
    fisicaComponent fisicaNpc2{2.0f, 0, 22.0f};
    MiColor colorPersonalizadoNPC(230, 41, 55, 255);
    renderComponent renderNpc2{size, colorPersonalizado2, 180.0f, "assets/modelos/animaciones/personaje3.obj"};
    soundComponent soundNpc2{};
    powerUpComponent powerBloqueNpc2{};
    bombaComponent bombaNpc2{};
    iaComponent npctieneBT2{true};

    em.createEntityComponents(npc2, fisicaNpc2, renderNpc2, powerBloqueNpc2, soundNpc2, bombaNpc2, npctieneBT2, buttonJugador);

    Entity &npc3 = em.createEntity(EntityType::NPC);
    fisicaComponent fisicaNpc3{26.0f, 0, 2.0f};
    MiColor colorPersonalizadoNPC2(230, 41, 55, 255);
    renderComponent renderNpc3{size, colorPersonalizado2, 0.0f, "assets/modelos/animaciones/personaje4.obj"};
    soundComponent soundNpc3{};
    powerUpComponent powerBloqueNpc3{};
    bombaComponent bombaNpc3{};
    iaComponent npctieneBT3{true};

    em.createEntityComponents(npc3, fisicaNpc3, renderNpc3, powerBloqueNpc3, soundNpc3, bombaNpc3, npctieneBT3, buttonJugador);
}

void Pintura::iniciarEntidades()
{

    auto &musicaPartida = em.createEntity(EntityType::SOUND);
    fisicaComponent fisicaMusica{};
    MiColor colorPersonalizado7(0, 0, 0, 0);
    renderComponent renderMusica{};
    soundComponent soundMusica;
    SoundEvent se7;
    se7.eventPath = "event:/Ambientes/Bar Italiano";
    se7.soundId = 16;
    soundMusica.addSoundEvent(se7);
    powerUpComponent powerBloqueMusica{};
    bombaComponent bombaMusica{};
    iaComponent musicatieneBT{false};
    buttonComponent buttonMusica{};
    em.createEntityComponents(musicaPartida, fisicaMusica, renderMusica, powerBloqueMusica, soundMusica, bombaMusica, musicatieneBT, buttonMusica);

    switch (numPlayers)
    {
    case 1:
        init1player();
        break;
    case 2:
        init2players();
        break;
    case 3:
        init3players();
        break;
    case 4:
        init4players();
        break;
    default:
        initNPCs();
        break;
    }
}

void Pintura::iniciarMap()
{

    LevelLoader::cargarNivel("assets/niveles/nivel1.json", mapSystem);
    gameplay.rellenarBloquesDestruibles(mapSystem, 0.75f);
}

void Pintura::iniciarIA()
{

    iaSystem.init(mapSystem.getMap());
    physics.saveIaSystem(iaSystem);

    std::vector<Entity> npcs = em.getEntitiesByType(EntityType::NPC);
    int cantidadNPCs = static_cast<int>(npcs.size());

    int frecuencia = cantidadNPCs;
    int fase = 0;

    for (Entity &entidad : npcs)
    {
        auto &iacomponent = em.ia.get(entidad.ia_key);
        if (iacomponent.tieneBT)
        {
            iaSystem.crearUtilityPintura(em, entidad);
        }

        int id = entidad.getId();

        scheduler.addTaskIA(Task(
            [this, id](double dt)
            {
                Entity &ent = em.getEntity(id);
                iaSystem.ejecutarIASlicePintura(em, ent, mapSystem.getMap(), bombSystem, mapSystem, dt);
            },
            frecuencia,
            fase));

        fase++;
    }
}

void Pintura::iniciarTodo()
{
}

void Pintura::resetDespuesDePintura()
{
    eventSystem.clear();
    em.clear();
    mapSystem.resetColorMap();
    iniciarMap();
    minutosDePintura = 1;
    contadorCuadrosPintura = 0;
    segundosIniciales = true;
    contadorCuadrosSegundosIniciales = 0;
    perdido = false;
    animacion = true;

    configurarEventos();

    animacion = true;
    perdido = false;
    compruebaPintura = true;
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(screenW, screenH, "BommbaMia");

    motorRaylib.getMotorDeJuego().Reset();
    motorRaylib.ResetDibujadoDeMapa();
    motorRaylib.iniciarMotor();
    motorRaylib.resetAnimacion();

    const auto &mapMatrix = mapSystem.getMap();
    motorRaylib.dibujaBloquesDestruibles(em, mapMatrix);
    boolEntidades = false;
}

void Pintura::reset()
{

    auto musica = buscarSonido(16);
    SoundSystem::getInstance().stopSound(em, musica);
    eventSystem.clear();

    em.clear();
    mapSystem.resetColorMap();

    minutosDePintura = 1;
    contadorCuadrosPintura = 0;
    segundosIniciales = true;
    contadorCuadrosSegundosIniciales = 0;
    perdido = false;
    animacion = true;

    configurarEventos();

    compruebaPintura = true;
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(screenW, screenH, "BommbaMia");

    iniciarMap();
    motorRaylib.getMotorDeJuego().Reset();
    motorRaylib.ResetDibujadoDeMapa();
    motorRaylib.iniciarMotor();
    motorRaylib.resetAnimacion();

    const auto &mapMatrix = mapSystem.getMap();
    motorRaylib.dibujaBloquesDestruibles(em, mapMatrix);
    mapSystem.printMap();
    boolEntidades = false;
}

// ================================================================
//                      BUCLES DEL Pintura
// ================================================================

void Pintura::buclePinturaUpdate()
{
    if(getESM()->GetEstado() == 5){
    if (!boolPinturaReset)
    {
        resetDespuesDePintura();
        boolPinturaReset = true;
    }

    if (!boolEntidades)
    {
        iniciarEntidades();
        iniciarIA();
        physics.init(em, mapSystem);
        bombSystem.saveIaSystem(iaSystem);
        auto players = em.getEntitiesByType(EntityType::PLAYER);
        auto npcs = em.getEntitiesByType(EntityType::NPC);
        std::vector<Entity> todos;
        todos.insert(todos.end(), players.begin(), players.end());
        todos.insert(todos.end(), npcs.begin(), npcs.end());

        for (int i = 0; i < todos.size(); i++)
        {
            auto &entidad = todos[i];
            auto &bombas = em.bombs.get(entidad.bomb_key);

            bombas.pinturaId = i + 1;
        }

        boolEntidades = true;
    }

    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(screenW, screenH, "BommbaMia");
    float deltaTime = motorRaylib.getFixedDeltaTime();

    auto players = em.getEntitiesByType(EntityType::PLAYER);
    auto npcs = em.getEntitiesByType(EntityType::NPC);

    struct Pos
    {
        float x, y, z;
    };

    auto actualizarRespawn = [&](fisicaComponent &fisica, float respawnInicial)
    {
        if (fisica.vida <= 0 && !fisica.estaMuerto)
        {
            fisica.estaMuerto = true;
            fisica.respawn = respawnInicial;
        }
        if (fisica.estaMuerto)
        {
            fisica.respawn -= deltaTime;
            if (fisica.respawn <= 0.0f)
            {
                fisica.estaMuerto = false;
                fisica.vida = 200;
                fisica.x = fisica.respawnX;
                fisica.y = fisica.respawnY;
                fisica.z = fisica.respawnZ;
            }
        }
    };
    if (!PausarJuego)
    {
        if (!segundosIniciales && !perdido && !animacion)
        {
            auto musica = buscarSonido(16);
            SoundSystem::getInstance().playMusicaFondo(em, musica);
            std::vector<Entity> todos;
            todos.insert(todos.end(), players.begin(), players.end());
            todos.insert(todos.end(), npcs.begin(), npcs.end());

            for (auto &p : players)
            {
                inputSystem.actualizarInputMap(p);
                const auto &inputMap = inputSystem.getInputMap(p);

                if (inputMap.at("pause"))
                {
                    getESM()->SetAnterior(5);
                    getESM()->cambiarEstado("Ajustes");
                }

                if (this->getESM()->estado == 3)
                {
                    SoundSystem::getInstance().pauseSound(musica, true);
                }
                else
                {
                    SoundSystem::getInstance().pauseSound(musica, false);
                }
            }

            for (auto &entidad : todos)
            {
                auto &fisica = em.fisicas.get(entidad.fisica_key);
                actualizarRespawn(fisica, 3.0f);
            }

            physics.update(em, inputSystem, gameplay, players);

            bombSystem.updateBombs(em, deltaTime, mapSystem);
            for (auto bomb : em.getBombs())
            {
                auto bombCMP = em.bombs.get(bomb.bomb_key);
                if (bombCMP.isExploding)
                {
                    SoundSystem::getInstance().playMusicaFondo(em, bomb);
                }
            }

            scheduler.updateIA(deltaTime);
        }
    }
    contadorHud();

    if (!players.empty())
    {
        comprobarEscena();
    }
    SoundSystem::getInstance().update();
}
}

void Pintura::buclePinturaRender()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(screenW, screenH, "BommbaMia");
    const auto &mapMatrix = mapSystem.getMap();
    motorRaylib.empezarEscena();
    motorRaylib.gameRender(em, seleccionActual, tiempoRestante, mapSystem, contadorCuadrosSegundosIniciales, physics, perdido, animacion, true, contP1, contP2, contP3, contP4);
    motorRaylib.acabarEscena();
}

// ================================================================
//                    MÉTODOS AUXILIARES
// ================================================================

bool Pintura::getTiempo()
{
    int segundosTotalesPintura = minutosDePintura * 60;
    tiempoRestante = segundosTotalesPintura - (float)contadorCuadrosPintura / 60.0f;

    if (tiempoRestante < 0.0f)
        tiempoRestante = 0.0f;

    if (compruebaPintura)
    {
        if (segundosIniciales == false && perdido == false && animacion == false && PausarJuego == false)
        {
            contadorCuadrosPintura++;
            if (contadorCuadrosPintura >= cuadrosMaximosPintura)
            {
                compruebaPintura = false;
            }
        }
    }

    if (tiempoRestante == 0)
    {
        return false;
    }
    return true;
}

void Pintura::comprobarEscena()
{

    if (!getTiempo())
    {
        int ganador = setGanador();
        auto jugadores = em.getAllEntitiesInGame();
        bool escenaCambiada = false;
        for (auto &jugador : jugadores)
        {
            auto &cmp = em.bombs.get(jugador.bomb_key);
            if (!escenaCambiada)
            {
                if (jugador.type == EntityType::PLAYER && cmp.pinturaId == ganador)
                {
                    escenaCambiada = true;
                    reset();
                    boolPinturaReset = false;
                    numPlayers = 0;

                    contP1 = contP2 = contP3 = contP4 = 0;
                    getESM()->SetAnterior(1);
                    getESM()->cambiarEstado("Win");
                    break;
                }
                else if (jugador.type == EntityType::NPC && cmp.pinturaId == ganador)
                {
                    escenaCambiada = true;
                    reset();
                    boolPinturaReset = false;
                    numPlayers = 0;
                    contP1 = contP2 = contP3 = contP4 = 0;
                    getESM()->SetAnterior(1);
                    getESM()->cambiarEstado("GameOver");
                    break;
                }
            }
        }
    }

    if (segundosIniciales && !animacion && !PausarJuego)
    {
        contadorCuadrosSegundosIniciales++;
        if (contadorCuadrosSegundosIniciales >= cuadrosSegundosIniciales)
        {
            segundosIniciales = false;
        }
    }
}

void Pintura::init()
{
    iniciarTodo();
}

int Pintura::setGanador()
{
    auto matrizPintura = mapSystem.getColorMap();
    std::unordered_map<int, int> contadorPintura;
    int maxPintura = 0;
    int ganador = -1;

    for (const auto &fila : matrizPintura)
    {
        for (int celda : fila)
        {
            if (celda > 0)
            {
                contadorPintura[celda]++;
            }
        }
    }
    for (const auto &[idJugador, cantidad] : contadorPintura)
    {
        if (cantidad > maxPintura)
        {
            maxPintura = cantidad;
            ganador = idJugador;
        }
    }

    return ganador;
}

Entity Pintura::buscarSonido(int idSonido)
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

void Pintura::contadorHud()
{
    auto matrizPintura = mapSystem.getColorMap();
    std::unordered_map<int, int> contadorPintura;

    for (const auto &fila : matrizPintura)
    {
        for (int celda : fila)
        {
            if (celda > 0)
            {
                contadorPintura[celda]++;
            }
        }
    }

    auto players = em.getAllEntitiesInGame();

    if (players.size() > 0)
    {
        int pinturaIdP1 = em.bombs.get(players[0].bomb_key).pinturaId;
        contP1 = contadorPintura[pinturaIdP1];
    }
    if (players.size() > 1)
    {
        int pinturaIdP2 = em.bombs.get(players[1].bomb_key).pinturaId;
        contP2 = contadorPintura[pinturaIdP2];
    }
    if (players.size() > 2)
    {
        int pinturaIdP3 = em.bombs.get(players[2].bomb_key).pinturaId;
        contP3 = contadorPintura[pinturaIdP3];
    }
    if (players.size() > 3)
    {
        int pinturaIdP4 = em.bombs.get(players[3].bomb_key).pinturaId;
        contP4 = contadorPintura[pinturaIdP4];
    }
}
