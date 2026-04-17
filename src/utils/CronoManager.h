#pragma once
#include <chrono>

class CronoManager {
public:
    using clock    = std::chrono::steady_clock;
    using duration = std::chrono::duration<double>;

    // ups: updates per second, fps: frames per second
    CronoManager(int ups = 60, int fps = 60);

    // Llamar justo antes de entrar al bucle
    void init();

    // Llamar una vez por cada iteración del bucle
    void tick();

    // --------------------------------------------------
    // Decisiones de timing
    // --------------------------------------------------

    // ¿Debo ejecutar la lógica (ESM.actualizar())?
    bool canUpdate();

    // ¿Debo ejecutar el render (ESM.renderizar())?
    bool canRender();

    // --------------------------------------------------
    // Getters para UI / sistemas de físicas
    // --------------------------------------------------

    // Siempre devuelve 1/UPS (tiempo de simulación para cada update)
    double getDeltaTime() const;

    // Frames rendered en el último segundo
    int    getCurrentFPS() const;

    // Cambiar target FPS en caliente
    void setRenderRate(int fps);

    double getFixedDeltaTime() const;

private:
    // Intervalos fijos
    const duration updateInterval;
    duration       renderInterval;

    // Acumuladores
    duration accumulatorUpdate   = duration::zero();
    duration accumulatorRender   = duration::zero();

    // Para medir tiempo real
    clock::time_point lastTime;
    clock::time_point fpsTimerStart;

    // Para contar renders y calcular FPS
    int framesRendered = 0;
    int currentFPS     = 0;
    double lastDeltaTime = 0.0;
};
