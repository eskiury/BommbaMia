#include "levelLoader.h"
#include "json.hpp"
#include <fstream>


bool LevelLoader::cargarNivel(const std::string& filename, MapSystem& mapSystem) {
    // Abrir el archivo JSON
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return false;
    }

    // Parsear el JSON
    nlohmann::json jsonData;
    file >> jsonData;

    // Validar que contiene una matriz de mapa
    if (!jsonData.contains("map_matrix") || !jsonData["map_matrix"].is_array()) {
        std::cerr << "Error: El archivo JSON no contiene una matriz de mapa válida." << std::endl;
        return false;
    }

    // Validar dimensiones
    int jsonWidth = jsonData["map_properties"]["width"];
    int jsonHeight = jsonData["map_properties"]["height"];

    if (jsonWidth != mapSystem.getWidth() || jsonHeight != mapSystem.getHeight()) {
        std::cerr << "Error: Las dimensiones del archivo JSON no coinciden con las del MapSystem.\n";
        return false;
    }

    // Leer la matriz de mapa
    auto matrixJson = jsonData["map_matrix"];
    for (int y = 0; y < jsonHeight; ++y) {
        for (int x = 0; x < jsonWidth; ++x) {
            int tileValue = matrixJson[y][x].get<int>();
            mapSystem.setTile(x, y, tileValue);
        }
    }

    std::cout << "Nivel cargado exitosamente desde " << filename << std::endl;
    return true;
}
