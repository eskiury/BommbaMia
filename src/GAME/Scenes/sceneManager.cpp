#include "sceneManager.h"

SceneManager::SceneManager()
{
    est = "Intro";
    estado = 2;
    estadoAnterior = 2;
    ActualmenteUsada = nullptr;
}

SceneManager::~SceneManager()
{
}

void SceneManager::cambiarEstado(std::string estadoRecibido)
{
    if (estadoRecibido == "Menu")
    {

        est = "Menu";
        estado = 0;
    }

    if (estadoRecibido == "Juego")
    {

        est = "Juego";
        estado = 1;
    }
    if (estadoRecibido == "Intro")
    {

        est = "Intro";
        estado = 2;
    }
    if (estadoRecibido == "Ajustes")
    {

        est = "Ajustes";
        estado = 3;
    }
    if (estadoRecibido == "Seleccion")
    {
        est = "Seleccion";
        estado = 4;
    }
    if (estadoRecibido == "Pintura")
    {
        est = "Pintura";
        estado = 5;
    }
    if (estadoRecibido == "GameOver")
    {
        est = "GameOver";
        estado = 6;
    }
    if (estadoRecibido == "Win")
    {
        est = "Win";
        estado = 7;
    }
    if (estadoRecibido == "Controles")
    {
        est = "Controles";
        estado = 8;
    }
    if (estadoRecibido == "SelecPersonajes")
    {
        est = "SelecPersonajes";
        estado = 9;
    }
}

void SceneManager::actualizar()
{
    if (ActualmenteUsada)
    {
        ActualmenteUsada->update();
    }
}

void SceneManager::renderizar()
{
    if (ActualmenteUsada)
    {
        ActualmenteUsada->render();
    }
}

int SceneManager::GetAnterior()
{
    return estadoAnterior;
}

int SceneManager::GetEstado()
{
    return estado;
}

void SceneManager::SetAnterior(int estadoAR)
{
    estadoAnterior = estadoAR;
}
