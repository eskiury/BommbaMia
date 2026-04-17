#pragma once

#include "../Sys/MapSystem.h"
#include <string>

class LevelLoader
{
public:
    /*
    Metodo: cargarNivel
    Descripcion: Carga un nivel a partir de un archivo JSON
    Input:
        - filename: Nombre del archivo JSON
        - mapSystem: Referencia al sistema de mapas
    Output: true si se ha cargado correctamente, false en caso contrario
    */
    static bool cargarNivel(const std::string &filename, MapSystem &mapSystem);
};