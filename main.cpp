#include <iostream>
#include "src/GAME/Scenes/juego.h"
#include "src/GAME/Scenes/pintura.h"
#include "src/GAME/Scenes/menu.h"
#include "src/GAME/Scenes/intro.h"
#include "src/GAME/Scenes/ajustes.h"
#include "src/GAME/Scenes/sceneManager.h"
#include "src/GAME/Scenes/escena.h"
#include "src/GAME/Scenes/seleccionJuego.h"
#include "src/GAME/Scenes/gameOver.h"
#include "src/GAME/Scenes/win.h"
#include "src/GAME/Scenes/controles.h"
#include "src/GAME/Scenes/selecPersonajes.h"
#include "src/utils/CronoManager.h"

#include "src/utils/Const.h"

/*
    Desde el main unicamente se llamara al Manejador de Escenas para controlar el juego.
    INPUTS  -> NO
    OUTPUTS -> un int pero ns pa q
    */
int main()
{
    ElMotorRaylib &motorRaylib = ElMotorRaylib::CrearMotor(screenW, screenH, "BommbaMia");
    SceneManager ESM;
    EntityManager entityManager;

    
    
    Juego juego(motorRaylib);
    Pintura pintura(motorRaylib);
    Menu menu;
    Intro intro;
    Ajustes ajustes;
    Seleccion seleccion;
    GameOver gameOver;
    Win win;
    Controles controles;
    SelecPersonajes seleccionPersonajes;
    // inicializa las escenas
    juego.init();
    menu.init();
    pintura.init();
    intro.init();
    ajustes.init();
    seleccion.init();
    gameOver.init();
    win.init();
    controles.init();
    seleccionPersonajes.init();
    

    // les pasa el esm para despues poder usarlo y cambiar de escena
    juego.inicializarEscena(&ESM);
    menu.inicializarEscena(&ESM);
    intro.inicializarEscena(&ESM);
    ajustes.inicializarEscena(&ESM);
    seleccion.inicializarEscena(&ESM);
    pintura.inicializarEscena(&ESM);
    gameOver.inicializarEscena(&ESM);
    win.inicializarEscena(&ESM);
    controles.inicializarEscena(&ESM);
    seleccionPersonajes.inicializarEscena(&ESM);
    motorRaylib.iniciarCrono();

    while(motorRaylib.run()){
        motorRaylib.getMotorDeJuego().GuardarInicioDeFrame(); // ← Calcula delta y suma al acumulador

        // Update en pasos fijos
    while (motorRaylib.getMotorDeJuego().acumulador >= motorRaylib.getMotorDeJuego().tiempoPorFrame) {
        ESM.actualizar();   // Actualiza la escena actual
        motorRaylib.getMotorDeJuego().acumulador -= motorRaylib.getMotorDeJuego().tiempoPorFrame;
    }
        if(motorRaylib.run()){
            ESM.renderizar();   // Renderiza la escena actual
        }


        //parte que recibe si hay que cambiar de escena
        if(ESM.estado == 0){
            ESM.ActualmenteUsada = &menu;
        }
        else if(ESM.estado == 1){
            ESM.ActualmenteUsada = &juego;
        }
        else if(ESM.estado == 2){
            ESM.ActualmenteUsada = &intro;
        }
        else if(ESM.estado == 3){
            ESM.ActualmenteUsada = &ajustes;
        }
        else if(ESM.estado == 4){
            ESM.ActualmenteUsada = &seleccion;
        }
        else if(ESM.estado == 5){
            ESM.ActualmenteUsada = &pintura;
        }
        else if(ESM.estado == 6){
            ESM.ActualmenteUsada = &gameOver;
        }
        else if(ESM.estado == 7){
            ESM.ActualmenteUsada = &win;
        }
        else if(ESM.estado == 8){
            ESM.ActualmenteUsada = &controles;
        }
        else if(ESM.estado == 9){
            ESM.ActualmenteUsada = &seleccionPersonajes;
        }


    }

    return 0;
}

