// ================================================================
//                           INCLUDES
// ================================================================
#include "juego.h"
#include "../../utils/miColor.h"
#include <cmath>
#include "../../utils/Const.h"
#include "../../utils/levelLoader.h"
#include "selecPersonajes.h"
#include "../../ENGINES/AlmondSE/FmodSoundEngine.h"

// ================================================================
//                CONSTRUCTOR Y DESTRUCTOR
// ================================================================
Juego::Juego(ElMotorRaylib &motorRaylib)
    : motorRaylib(motorRaylib),
      renderSys(motorRaylib, em),
      jugando(true),
      physics(),
      scheduler(),
      iaSystem(),
      bombSystem(),
      gameplay(em),
      inputSystem(&eventSystem),
      seleccionActual(0),
      tiempoRestante(0.0f),
      minutosDeJuego(3),
      contadorCuadrosJuego(0),
      compruebaJuego(true),
      mapSystem(15, 13),
      perdido(false),
      animacion(true)
{
    configurarEventos();
}

Juego::~Juego()
{
    eventSystem.clear();
    em.clear();
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
}

// ================================================================
//                     CONFIGURAR EVENTOS
// ================================================================
void Juego::configurarEventos()
{

    eventSystem.subscribe(EventType::MOVE_RIGHT, [this](Entity &entity) {});

    eventSystem.subscribe(EventType::MOVE_LEFT, [this](Entity &entity) {});

    eventSystem.subscribe(EventType::MOVE_UP, [this](Entity &entity) {});

    eventSystem.subscribe(EventType::MOVE_DOWN, [this](Entity &entity) {});

    eventSystem.subscribe(EventType::ENTER, [this](Entity &entity) {});

    eventSystem.subscribe(EventType::PAUSE, [this](Entity &entity)
                          { jugando = !jugando; });
}

// ================================================================
//                 MÉTODOS DE INICIALIZACIÓN
// ================================================================

void Juego::init1player()
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

void Juego::init2players()
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

void Juego::init3players()
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
    renderComponent renderNpc3{size, colorPersonalizado2, 180.0f, "assets/modelos/animaciones/personaje4.obj"};
    soundComponent soundNpc3{};
    powerUpComponent powerBloqueNpc3{};
    bombaComponent bombaNpc3{};
    iaComponent npctieneBT3{true};

    em.createEntityComponents(npc3, fisicaNpc3, renderNpc3, powerBloqueNpc3, soundNpc3, bombaNpc3, npctieneBT3, buttonJugador);
    iaManager managerdeia;
    managerdeia.cargarNPC1("assets/ia.json", em, npc3);
}

void Juego::init4players()
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

void Juego::initNPCs()
{
    Entity &npc0 = em.createEntity(EntityType::PLAYER);
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

void Juego::iniciarEntidades()
{

    auto &musicaPartida = em.createEntity(EntityType::SOUND);
    fisicaComponent fisicaMusica{};
    MiColor colorPersonalizado7(0, 0, 0, 0);
    renderComponent renderMusica{};
    soundComponent soundMusica;
    SoundEvent se7;
    se7.eventPath = "event:/Musica/TemaPropio";
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

void Juego::iniciarMap()
{
    LevelLoader::cargarNivel("assets/niveles/nivel1.json", mapSystem);
    gameplay.rellenarBloquesDestruibles(mapSystem, 0.75f);
}

void Juego::iniciarIA()
{
    iaSystem.init(mapSystem.getMap());
    physics.saveIaSystem(iaSystem);
    bombSystem.saveIaSystem(iaSystem);

    std::vector<Entity> npcs = em.getEntitiesByType(EntityType::NPC);
    int cantidadNPCs = static_cast<int>(npcs.size());

    int frecuencia = cantidadNPCs;
    int fase = 0;

    for (Entity &entidad : npcs)
    {
        auto &iacomponent = em.ia.get(entidad.ia_key);
        if (iacomponent.tieneBT)
        {
            iaSystem.crearUtility(em, entidad);
        }

        int id = entidad.getId();

        scheduler.addTaskIA(Task(
            [this, id](double dt)
            {
                Entity &ent = em.getEntity(id);
                iaSystem.ejecutarIASlice(em, ent, mapSystem.getMap(), bombSystem, mapSystem, dt);
            },
            frecuencia,
            fase));

        fase++;
    }
}

void Juego::iniciarTodo()
{
}

void Juego::initMotorMallas()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(screenW, screenH, "BommbaMia");
    iniciarMap();
    minutosDeJuego = 3;
    contadorCuadrosJuego = 0;
    segundosIniciales = true;
    contadorCuadrosSegundosIniciales = 0;
    perdido = false;
    animacion = true;

    configurarEventos();
    animacion = true;
    perdido = false;
    compruebaJuego = true;

    motorRaylib.getMotorDeJuego().Reset();
    motorRaylib.ResetDibujadoDeMapa();
    motorRaylib.iniciarMotor();
    motorRaylib.resetAnimacion();

    const auto &mapMatrix = mapSystem.getMap();
    motorRaylib.dibujaBloquesDestruibles(em, mapMatrix);
    boolEntidades = false;
}

void Juego::reset()
{

    auto musica = buscarSonido(16);
    SoundSystem::getInstance().stopSound(em, musica);

    eventSystem.clear();

    em.clear();

    minutosDeJuego = 3;
    contadorCuadrosJuego = 0;
    segundosIniciales = true;
    contadorCuadrosSegundosIniciales = 0;
    perdido = false;
    animacion = true;

    configurarEventos();

    animacion = true;
    perdido = false;
    compruebaJuego = true;
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(screenW, screenH, "BommbaMia");

    iniciarMap();

    motorRaylib.getMotorDeJuego().Reset();
    motorRaylib.ResetDibujadoDeMapa();
    motorRaylib.iniciarMotor();

    const auto &mapMatrix = mapSystem.getMap();
    motorRaylib.dibujaBloquesDestruibles(em, mapMatrix);
    mapSystem.printMap();
    boolEntidades = false;
}

void Juego::resetModo()
{

    rondasPorEntidad.clear();
    eventSystem.clear();
    em.clear();
    boolInit = false;
    boolEntidades = false;
    motorRaylib.getMotorDeJuego().Reset();
}

// ================================================================
//                      BUCLES DEL JUEGO
// ================================================================

void Juego::bucleJuegoUpdate()
{
    if(getESM()->GetEstado() == 1){
         if (!boolInit)
    {
        initMotorMallas();
        boolInit = true;
    }

    if (!boolEntidades)
    {
        iniciarEntidades();
        iniciarIA();
        physics.init(em, mapSystem);
        boolEntidades = true;
    }

    auto deltaTime = motorRaylib.getFixedDeltaTime();

    if (!PausarJuego && !segundosIniciales && !perdido && !animacion)
    {
        auto musica = buscarSonido(16);
        SoundSystem::getInstance().playMusicaFondo(em, musica);

        auto players = em.getEntitiesByType(EntityType::PLAYER);
        for (auto &player : players)
        {
            auto &fisicaPlayer = em.fisicas.get(player.fisica_key);
            inputSystem.actualizarInputMap(player);
            const auto &inputMap = inputSystem.getInputMap(player);
            if (inputMap.at("pause"))
            {
                getESM()->SetAnterior(1);
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
            if (fisicaPlayer.vida <= 0)
                fisicaPlayer.esFantasma = true;

            if (inputMap.at("placeBomb"))
            {
                if (!fisicaPlayer.esFantasma)
                    bombSystem.crearBomba(player, em, mapSystem);
                else
                    fisicaPlayer.estuneo = true;
            }
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

    for (auto &npc : em.getEntitiesByType(EntityType::NPC))
    {
        auto &fisica = em.fisicas.get(npc.fisica_key);
        if (fisica.vida <= 0)
            fisica.esFantasma = true;
    }

    comprobarJugadores();
    comprobarEscena(em);

    SoundSystem::getInstance().update();
    }

}

void Juego::bucleJuegoRender()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(screenW, screenH, "BommbaMia");
    const auto &mapMatrix = mapSystem.getMap();
    motorRaylib.empezarEscena();
    motorRaylib.gameRender(em, seleccionActual, tiempoRestante, mapSystem, contadorCuadrosSegundosIniciales, physics, perdido, animacion, pintura, contP1, contP2, contP3, contP4);
    motorRaylib.acabarEscena();
}

// ================================================================
//                    MÉTODOS AUXILIARES
// ================================================================

void Juego::iniciarRonda()
{
    reset();
}

bool Juego::getTiempo()
{
    int segundosTotalesJuego = minutosDeJuego * 60;
    tiempoRestante = segundosTotalesJuego - (float)contadorCuadrosJuego / 60.0f;

    if (tiempoRestante < 0.0f)
        tiempoRestante = 0.0f;

    if (compruebaJuego)
    {
        if (segundosIniciales == false && perdido == false && animacion == false && PausarJuego == false)
        {
            contadorCuadrosJuego++;
            if (contadorCuadrosJuego >= cuadrosMaximosJuego)
            {
                compruebaJuego = false;
            }
        }
    }

    if (tiempoRestante == 0)
    {
        return false;
    }
    return true;
}

void Juego::comprobarJugadores()
{

    auto players = em.getEntitiesByType(EntityType::PLAYER);
    for (auto &player : players)
    {
        auto &fisicaPlayer = em.fisicas.get(player.fisica_key);
        if (fisicaPlayer.vida <= 0)
        {
            gameplay.aplicarModoFantasma(em, player);
        }
    }

    auto NPCs = em.getEntitiesByType(EntityType::NPC);
    for (auto &npc : NPCs)
    {
        auto &fisica = em.fisicas.get(npc.fisica_key);
        if (fisica.vida <= 0)
        {
            gameplay.aplicarModoFantasma(em, npc);
        }
    }
}

void Juego::comprobarEscena(EntityManager &em)
{
    int rondasTotales = gameplay.getRondasTotales();

    bool debeResetear = false;
    bool debeResetearModo = false;
    std::string proximoEstado;
    auto players = em.getAllEntitiesInGame();
    std::vector<Entity> vivos;

    if (!getTiempo())
    {
        reset();
    }

    for (auto &p : players)
    {
        auto &cmp = em.fisicas.get(p.fisica_key);
        if (cmp.vida > 0)
        {
            vivos.push_back(p);
        }

        if (rondasPorEntidad[p.id] == rondasTotales)
        {
            if (p.type == EntityType::PLAYER)
            {
                contP1 = 0;
                contP2 = 0;
                contP3 = 0;
                contP4 = 0;
                numPlayers = 0;
                boolInit = false;
                debeResetearModo = true;
                proximoEstado = "Win";
                break;
            }
            else if (p.type == EntityType::NPC)
            {
                contP1 = 0;
                contP2 = 0;
                contP3 = 0;
                contP4 = 0;
                numPlayers = 0;
                boolInit = false;
                debeResetearModo = true;
                proximoEstado = "GameOver";
                break;
            }
        }
    }

    if (vivos.size() == 1 && numPlayers > 0)
    {

        rondasPorEntidad[vivos[0].id]++;
        if (vivos[0].id == players[0].getId())
        {
            contP1++;
        }
        if (vivos[0].id == players[1].getId())
        {
            contP2++;
        }
        if (vivos[0].id == players[2].getId())
        {
            contP3++;
        }
        if (vivos[0].id == players[3].getId())
        {
            contP4++;
        }
        debeResetear = true;
    }

    if (segundosIniciales && !animacion && !PausarJuego)
    {
        contadorCuadrosSegundosIniciales++;
        if (contadorCuadrosSegundosIniciales >= cuadrosSegundosIniciales)
            segundosIniciales = false;
    }
    if (debeResetear)
    {
        reset();
    }

    if (debeResetearModo)
    {
        resetModo();
        getESM()->SetAnterior(1);
        getESM()->cambiarEstado(proximoEstado);
    }
}

void Juego::init()
{
    iniciarTodo();
}

Entity Juego::buscarSonido(int idSonido)
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