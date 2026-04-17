#include "MapSystem.h"
#include <iostream>


MapSystem::MapSystem(int width, int height) : width(width), height(height)
{
    mapMatrix = std::vector<std::vector<int>>(height, std::vector<int>(width, 0)); 
    ColorMatrix = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
}


void MapSystem::setTile(int x, int y, int value)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        mapMatrix[y][x] = value;
    }
    else
    {
        std::cerr << "Error: Coordenadas fuera del rango.\n";
    }
}

int MapSystem::getTile(int x, int y) const
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return mapMatrix[y][x];
    }
    else
    {
        std::cerr << "Error: Coordenadas fuera del rango.\n";
        return -1; // Valor de error
    }
}


void MapSystem::printMap() const
{
    for (const auto &row : mapMatrix)
    {
        for (int cell : row)
        {
            //std::cout << cell << " ";
        }
        //std::cout << "\n";
    }
}


std::vector<std::vector<int>> &MapSystem::getMap()
{
    return mapMatrix;
}

int MapSystem::getWidth() const
{
    return width;
}

int MapSystem::getHeight() const
{
    return height;
}

void MapSystem::setPowerUp(int x, int y, int powerUp)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        mapMatrix[y][x] = powerUp;
    }
    else
    {
        std::cerr << "Error: Coordenadas fuera del rango.\n";
    }
}

int MapSystem::getPowerUp(int x, int y) const
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return mapMatrix[y][x];
    }
    else
    {
        std::cerr << "Error: Coordenadas fuera del rango.\n";
        return -1; 
    }
}

void MapSystem::setTileColor(int x, int y, int value)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        ColorMatrix[y][x] = value;
    }
    else
    {
        std::cerr << "Error: Coordenadas fuera del rango.\n";
    }
}

int MapSystem::getTileColor(int x, int y) const
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return ColorMatrix[y][x];
    }
    else
    {
        std::cerr << "Error: Coordenadas fuera del rango.\n";
        return -1; 
    }
}

void MapSystem::printColorMap() const
{
    for (const auto &row : ColorMatrix)
    {
        for (int cell : row)
        {
            //std::cout << cell << " ";
        }
        //std::cout << "\n";
    }
}

const std::vector<std::vector<int>> &MapSystem::getColorMap() const
{
    return ColorMatrix;
}

void MapSystem::resetColorMap()
{
    ColorMatrix = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
}