#pragma once

#include "miVector3.h"
#include "miVector2.h"

/**
 * @brief Archivo ConstantesJuego.h.Definición de constantes globales utilizadas en el motor gráfico, lógica del juego, sistema de bombas, tipos de objetos e inteligencia artificial.
 */
 
//================================================================ MOTOR GRAFICO ================================================================
/**
 * @brief Coordenada Y para tiles del motor gráfico.
 */
constexpr int TileY = 0;    

//===================TAMAÑO CUBO===================
/**
 * @brief Tamaño del las entidades de la escena
 */
constexpr float size = 2.0f;

//===================MAPA==========================
/**
 * @brief Tamaño de cada tile del mapa.
 */
constexpr float tileSize = 2.0f; // Tamaño de cada tile
/**
 * @brief Número de tiles a lo ancho del mapa.
 */
constexpr int gridWidth = 13;    // Número de tiles en la anchura
/**
 * @brief Número de tiles a lo alto del mapa.
 */
constexpr int gridHeight = 11;   // Número de tiles en la altura

/**
 * @brief Posición central del suelo en la escena.
 */
const MiVector3 SUELO_CENTER{14.0f, -0.05f, 10.0f};
/**
 * @brief Tamaño del suelo en dos dimensiones.
 */
const MiVector2 SUELO_SIZE2{35.0f, 35.0f};

//===================HUD=======================
/**
 * @brief Variables para pintar el hud
 */
const MiVector2 arriba_izquierda{20, 320};
const MiVector2 arriba_derecha{110, 320};
const MiVector2 abajo_izquierda{70, 420};
const MiVector2 abajo_derecha{160, 420};

//===================CUADRICULA=======================
/**
 * @brief Coordenada X de inicio de la cuadrícula.
 */
constexpr float startX = -1.0f; // Origen en X (sin desplazamiento en este caso)
/**
 * @brief Coordenada Z de inicio de la cuadrícula.
 */
constexpr float startZ = -1.0f; // Origen en Z (sin desplazamiento en este caso)



//================================================================ JUEGO ================================================================
/**
 * @brief Ancho de la ventana del juego por defecto.
 */
constexpr int screenW = 1280; // Ancho de la pantalla
/**
 * @brief Alto de la ventana del juego por defecto.
 */
constexpr int screenH = 720;  // Alto de la pantalla

//================================================================ BombSys ================================================================
/**
 * @brief Tamaño de la bomba.
 */
constexpr float sizeBomba = 1.0f;
/**
 * @brief Duración de la explosión en segundos.
 */
constexpr float explosionDuration = 1.0f;
/**
 * @brief Número inicial de bombas colocadas.
 */
constexpr int bombsPlaced = 0;
/**
 * @brief Tiempo de vida de una bomba antes de explotar.
 */
constexpr float lifetime = 3.0f;

//================================================================ ID TIPOS ================================================================
/**
 * @brief IDs de tipos para gestion de entidades del mapa.
 */
constexpr int SOLIDO = 1;
constexpr int DESTRUIBLE = 2;  
constexpr int ESQUINAS = 9;
constexpr int POWERUP = 100;
constexpr int BOMBA = 7;
constexpr int FUEGO = 8;

//================================================================ IA ================================================================

constexpr int D_MAX = 10;
constexpr int V_MAX = 200;
constexpr int B_MAX = 70;
constexpr int P_MAX = 20;