#include "intro.h"

#include "../../ENGINES/AlmondSE/FmodSoundEngine.h"

Intro::Intro()
    : alpha(0.0f), fadingIn(true), logoVisible(true)
{
    SoundSystem::getInstance(&em).init("assets/audioConfig.json");

    rutasParaCargar = {
        "assets/modelos/bomba.obj",
        "assets/modelos/bombDOWN.obj",
        "assets/modelos/bombUP.obj",
        "assets/modelos/rangeUP.obj",
        "assets/modelos/rangeDOWN.obj",
        "assets/modelos/rangeMAX.obj",
        "assets/modelos/speedUP.obj",
        "assets/modelos/speedDOWN.obj",
        "assets/modelos/pushBOMBS.obj",
        "assets/modelos/explosionProvisional.obj",
        "assets/modelos/taburetes.obj",
        "assets/modelos/sillasPoker.obj",
        "assets/modelos/sillasNormales.obj",
        "assets/modelos/mesas.obj",
        "assets/modelos/mesaTaberna.obj",
        "assets/modelos/poker.obj",
        "assets/modelos/estanterias.obj",
        "assets/modelos/perchero.obj",
        "assets/modelos/pianola.obj",
        "assets/modelos/barra.obj",
        "assets/modelos/musica.obj",
        "assets/modelos/paredes.obj",
        "assets/modelos/suelo.obj",
        "assets/modelos/acera.obj",
        "assets/modelos/carretera.obj",
        "assets/modelos/pinturaProvisional.obj",
        "assets/modelos/pinturaProvisional2.obj",
        "assets/modelos/pinturaProvisional3.obj",
        "assets/modelos/pinturaProvisional4.obj",
        "assets/modelos/npc.obj",
        "assets/modelos/barril.obj",
        "assets/modelos/personajes.obj",
        "assets/modelos/edificioEsquina.obj",
        "assets/modelos/edificioVinos.obj",
        "assets/modelos/cochePolicia.obj",
        "assets/modelos/cocheNaranja.obj",
        "assets/modelos/taxi.obj",
        "assets/modelos/animaciones/idle/player1Idle1.obj",
        "assets/modelos/animaciones/idle/player1Idle2.obj",
        "assets/modelos/animaciones/idle/player1Idle3.obj",
        "assets/modelos/animaciones/idle/player1Idle4.obj",
        "assets/modelos/animaciones/idle/player1Idle5.obj",
        "assets/modelos/animaciones/idle/player2Idle1.obj",
        "assets/modelos/animaciones/idle/player2Idle2.obj",
        "assets/modelos/animaciones/idle/player2Idle3.obj",
        "assets/modelos/animaciones/idle/player2Idle4.obj",
        "assets/modelos/animaciones/idle/player2Idle5.obj",
        "assets/modelos/animaciones/idle/player3Idle1.obj",
        "assets/modelos/animaciones/idle/player3Idle2.obj",
        "assets/modelos/animaciones/idle/player3Idle3.obj",
        "assets/modelos/animaciones/idle/player3Idle4.obj",
        "assets/modelos/animaciones/idle/player3Idle5.obj",
        "assets/modelos/animaciones/idle/player4Idle1.obj",
        "assets/modelos/animaciones/idle/player4Idle2.obj",
        "assets/modelos/animaciones/idle/player4Idle3.obj",
        "assets/modelos/animaciones/idle/player4Idle4.obj",
        "assets/modelos/animaciones/idle/player4Idle5.obj",
        "assets/modelos/animaciones/correr/player1Correr1.obj",
        "assets/modelos/animaciones/correr/player1Correr2.obj",
        "assets/modelos/animaciones/correr/player1Correr3.obj",
        "assets/modelos/animaciones/correr/player1Correr4.obj",
        "assets/modelos/animaciones/correr/player1Correr5.obj",
        "assets/modelos/animaciones/correr/player2Correr1.obj",
        "assets/modelos/animaciones/correr/player2Correr2.obj",
        "assets/modelos/animaciones/correr/player2Correr3.obj",
        "assets/modelos/animaciones/correr/player2Correr4.obj",
        "assets/modelos/animaciones/correr/player2Correr5.obj",
        "assets/modelos/animaciones/correr/player3Correr1.obj",
        "assets/modelos/animaciones/correr/player3Correr2.obj",
        "assets/modelos/animaciones/correr/player3Correr3.obj",
        "assets/modelos/animaciones/correr/player3Correr4.obj",
        "assets/modelos/animaciones/correr/player3Correr5.obj",
        "assets/modelos/animaciones/correr/player4Correr1.obj",
        "assets/modelos/animaciones/correr/player4Correr2.obj",
        "assets/modelos/animaciones/correr/player4Correr3.obj",
        "assets/modelos/animaciones/correr/player4Correr4.obj",
        "assets/modelos/animaciones/correr/player4Correr5.obj",
        "assets/modelos/animaciones/colocarBomba/player1bomba1.obj",
        "assets/modelos/animaciones/colocarBomba/player1bomba2.obj",
        "assets/modelos/animaciones/colocarBomba/player1bomba3.obj",
        "assets/modelos/animaciones/colocarBomba/player1bomba4.obj",
        "assets/modelos/animaciones/colocarBomba/player2bomba1.obj",
        "assets/modelos/animaciones/colocarBomba/player2bomba2.obj",
        "assets/modelos/animaciones/colocarBomba/player2bomba3.obj",
        "assets/modelos/animaciones/colocarBomba/player2bomba4.obj",
        "assets/modelos/animaciones/colocarBomba/player3bomba1.obj",
        "assets/modelos/animaciones/colocarBomba/player3bomba2.obj",
        "assets/modelos/animaciones/colocarBomba/player3bomba3.obj",
        "assets/modelos/animaciones/colocarBomba/player3bomba4.obj",
        "assets/modelos/animaciones/colocarBomba/player4bomba1.obj",
        "assets/modelos/animaciones/colocarBomba/player4bomba2.obj",
        "assets/modelos/animaciones/colocarBomba/player4bomba3.obj",
        "assets/modelos/animaciones/colocarBomba/player4bomba4.obj",
        "assets/modelos/animaciones/personaje1.obj",
        "assets/modelos/animaciones/personaje2.obj",
        "assets/modelos/animaciones/personaje3.obj",
        "assets/modelos/animaciones/personaje4.obj",
        "assets/modelos/barra.obj"};
    indiceCarga = 0;
}

Intro::~Intro()
{
}

void Intro::iniciarIntro()
{

    Entity &musicaPartida = em.createEntity(EntityType::SOUND);
    fisicaComponent fisicaMusica{};
    MiColor colorPersonalizado3(0, 0, 0, 0);
    renderComponent renderMusica{0, colorPersonalizado3};

    soundComponent soundMusica;
    SoundEvent se;
    se.eventPath = "event:/PowerUps/RecogerItem";
    se.playOnce = false;
    se.looping = false;
    se.isPlaying = false;
    se.volume = 1.0f;

    soundMusica.addSoundEvent(se);

    powerUpComponent powerBloqueMusica{};
    bombaComponent bombaMusica{};
    iaComponent musicatieneBT{false};
    buttonComponent buttonMusica{};

    em.createEntityComponents(musicaPartida, fisicaMusica, renderMusica, powerBloqueMusica, soundMusica, bombaMusica, musicatieneBT, buttonMusica);
}

void Intro::bucleIntroUpdate()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");

    if (logoVisible)
    {
        if (fadingIn)
        {
            alpha += 0.01f;
            if (alpha >= 1.0f)
            {
                fadingIn = false;
            }
        }
        else
        {
            alpha -= 0.01f;
            if (alpha <= 0.0f)
            {

                logoVisible = false;
            }
        }
    }
    else
    {
        pantallasDeCarga = true;

        if (indiceCarga < rutasParaCargar.size())
        {
            motorRaylib.getMotorDeJuego().PrecargarRecursosMalla(rutasParaCargar[indiceCarga]);
            indiceCarga++;
            if (indiceCarga == rutasParaCargar.size())
            {
                recursosCargados = true;
            }
        }
        else
        {
            if (recursosCargados == true)
            {
                this->getESM()->SetAnterior(2);
                this->getESM()->cambiarEstado("Menu");
            }
        }
    }
}

void Intro::bucleIntroRender()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(1280, 720, "BommbaMia");
    motorRaylib.empezarEscena();

    if (logoVisible || pantallasDeCarga)
    {

        motorRaylib.introRender(alpha, pantallasDeCarga);
    }
    else
    {
        MiColor colorFondo(217, 181, 137, 255);
        motorRaylib.getMotorDeJuego().limpiarPantalla(colorFondo.r, colorFondo.g, colorFondo.b, colorFondo.a);
    }

    motorRaylib.acabarEscena();
}
bool Intro::isFinished()
{
    return alpha <= 0.0f && !fadingIn;
}