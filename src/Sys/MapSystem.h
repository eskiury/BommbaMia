/**
 * @file MapSystem.h
 * @brief Sistema que gestiona la representación lógica del mapa del juego.
 *
 * Permite modificar y consultar celdas del mapa, así como asociar power-ups y colores de celdas
 * (útil para visualización o depuración en modo pintura).
 */

#ifndef MAPSYSTEM_H
#define MAPSYSTEM_H

#include <vector>
#include <iostream>

/**
 * @class MapSystem
 * @brief Clase encargada de manejar la lógica del mapa del juego y operaciones sobre sus celdas.
 *
 * Administra una matriz de enteros que representa el estado de cada celda del mapa, así como una
 * matriz de colores adicional para visualización o efectos.
 */
class MapSystem
{
public:
    /**
     * @brief Constructor del mapa.
     * @param width Ancho del mapa (número de columnas).
     * @param height Alto del mapa (número de filas).
     */
    MapSystem(int width, int height);

    /**
     * @brief Modifica el valor de una celda del mapa.
     * @param x Coordenada X (columna).
     * @param y Coordenada Y (fila).
     * @param value Valor a asignar a la celda.
     */
    void setTile(int x, int y, int value);

    /**
     * @brief Obtiene el valor de una celda del mapa.
     * @param x Coordenada X.
     * @param y Coordenada Y.
     * @return Valor almacenado en la celda especificada.
     */
    int getTile(int x, int y) const;

    /**
     * @brief Devuelve una referencia al mapa completo (matriz de celdas).
     * @return Referencia a la matriz bidimensional del mapa.
     */
    std::vector<std::vector<int>> &getMap();

    /**
     * @brief Asigna un power-up a una celda.
     * @param x Coordenada X.
     * @param y Coordenada Y.
     * @param powerUp Valor asociado al tipo de power-up.
     */
    void setPowerUp(int x, int y, int powerUp);

    /**
     * @brief Obtiene el power-up de una celda.
     * @param x Coordenada X.
     * @param y Coordenada Y.
     * @return Valor del power-up en esa celda.
     */
    int getPowerUp(int x, int y) const;

    /**
     * @brief Imprime la matriz del mapa en consola (para depuración).
     */
    void printMap() const;

    /**
     * @brief Obtiene el ancho del mapa.
     * @return Número de columnas del mapa.
     */
    int getWidth() const;

    /**
     * @brief Obtiene el alto del mapa.
     * @return Número de filas del mapa.
     */
    int getHeight() const;

    // === MODO PINTURA ===

    /**
     * @brief Asigna un color a una celda del mapa.
     * @param x Coordenada X.
     * @param y Coordenada Y.
     * @param value Valor del color (por ejemplo, para visualización).
     */
    void setTileColor(int x, int y, int value);

    /**
     * @brief Obtiene el color asignado a una celda.
     * @param x Coordenada X.
     * @param y Coordenada Y.
     * @return Valor de color de la celda.
     */
    int getTileColor(int x, int y) const;

    /**
     * @brief Imprime la matriz de colores del mapa (para depuración visual).
     */
    void printColorMap() const;

    /**
     * @brief Obtiene una referencia constante a la matriz de colores del mapa.
     * @return Referencia constante a la matriz de colores.
     */
    const std::vector<std::vector<int>> &getColorMap() const;

    /**
     * @brief Resetea la matriz de colores a valores por defecto (por ejemplo, ceros).
     */
    void resetColorMap();

private:
    /// Matriz que representa el contenido lógico del mapa (bloques, caminos, etc.).
    std::vector<std::vector<int>> mapMatrix;

    /// Matriz auxiliar para representar colores por celda (modo visual).
    std::vector<std::vector<int>> ColorMatrix;

    /// Ancho del mapa en celdas.
    int width;

    /// Alto del mapa en celdas.
    int height;
};

#endif // MAPSYSTEM_H
