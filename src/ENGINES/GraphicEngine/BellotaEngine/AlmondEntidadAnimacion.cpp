#include "AlmondEntidadAnimacion.h"
#include "../../../utils/Const.h"
#include <iostream>
#include <glm/glm.hpp>

AlmondEntidadAnimacion::AlmondEntidadAnimacion() {
    tiempoAcumulado = 0.0f;
    frameActual = 0;
    fps = 12.0f;
    loop = true;
    cargado = false;
    gestorGuardado = nullptr;
}

AlmondEntidadAnimacion::~AlmondEntidadAnimacion() {
    // No eliminamos las mallas, el gestor es el dueño
}

void AlmondEntidadAnimacion::guardarGestor(AlmondGestorRecursos* gestor) {
    gestorGuardado = gestor;
}

void AlmondEntidadAnimacion::setFPS(float nuevoFPS) {
    fps = nuevoFPS;
}

void AlmondEntidadAnimacion::setLoop(bool nuevoLoop) {
    loop = nuevoLoop;
}

bool AlmondEntidadAnimacion::getCargado() {
    return cargado;
}

void AlmondEntidadAnimacion::cargarAnimacion(const std::string& nombreAnim, const std::vector<std::string>& rutasFrames) {
    std::vector<AlmondMalla*> frames;
    for (const std::string& ruta : rutasFrames) {
        //obtenemos los recursos de las mallas
        AlmondMalla* malla = gestorGuardado->getRecurso<AlmondMalla>(ruta);
        if (malla) {
            //guardamos el puntero de la malla en el vector frames
            frames.push_back(malla);
        } else {
            std::cerr << "No se pudo cargar la malla para animacion: " << ruta << std::endl;
        }
    }
    if (!frames.empty()) {
        //guardo los frames en el unordered map
        animaciones[nombreAnim] = frames;
        if (animacionActual.empty()) {
            animacionActual = nombreAnim;
        }
        cargado = true;
    }
}

void AlmondEntidadAnimacion::reproducirAnimacion(const std::string& nombreAnim) {
    if (animaciones.find(nombreAnim) != animaciones.end()) {
        animacionActual = nombreAnim;
        frameActual = 0;
        tiempoAcumulado = 0.0f;
    } else {
        std::cerr << "Animación no encontrada: " << nombreAnim << std::endl;
    }
}

void AlmondEntidadAnimacion::actualizarAnimacion(float deltaTime) {
    tiempoAcumulado += deltaTime;
    float duracionFrame = 1.0f / fps;
    const auto& frames = animaciones[animacionActual];

    if (tiempoAcumulado >= duracionFrame && !frames.empty()) {
        tiempoAcumulado -= duracionFrame;
        frameActual++;
        if (frameActual >= static_cast<int>(frames.size())) {
            if (loop) {
                frameActual = 0;
            } else {
                frameActual = static_cast<int>(frames.size()) - 1;
            }
        }
    }
}

void AlmondEntidadAnimacion::dibujar(const glm::mat4& transformacion, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& campos, std::vector<AlmondNode*> luces) {
    const auto& frames = animaciones[animacionActual];
    if (!frames.empty() && frameActual < static_cast<int>(frames.size())) {
        frames[frameActual]->dibujar(transformacion, view, projection, campos, luces);
    }
}
