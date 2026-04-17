#include "CronoManager.h"

CronoManager::CronoManager(int ups, int fps)
  : updateInterval(1.0 / ups),
    renderInterval(1.0 / fps)
{}

void CronoManager::init() {
    lastTime      = clock::now();
    fpsTimerStart = lastTime;
}

void CronoManager::tick() {
    auto now = clock::now();
    duration delta = now - lastTime;
    lastTime = now;

    // Limita saltos gigantes
    if (delta.count() > 0.25) delta = duration(0.25);

    // Acumula tiempo para update y render
    accumulatorUpdate += delta;
    accumulatorRender += delta;
}

bool CronoManager::canUpdate() {
    if (accumulatorUpdate >= updateInterval) {
        accumulatorUpdate -= updateInterval;
        lastDeltaTime = updateInterval.count();  // <— fuerza el delta fijo
        return true;
    }
    return false;
}

bool CronoManager::canRender() {
    if (accumulatorRender >= renderInterval) {
        accumulatorRender -= renderInterval;

        // Contamos un render más y recalculamos FPS cada segundo
        framesRendered++;
        auto now = clock::now();
        if ((now - fpsTimerStart) >= duration(1.0)) {
            currentFPS     = framesRendered;
            framesRendered = 0;
            fpsTimerStart  = now;
        }
        return true;
    }
    return false;
}

double CronoManager::getDeltaTime() const {
    // La simulación avanza siempre en pasos de tamaño fijo
    return updateInterval.count();
}

int CronoManager::getCurrentFPS() const {
    return currentFPS;
}

void CronoManager::setRenderRate(int fps) {
    renderInterval = duration(1.0 / fps);
}


double CronoManager::getFixedDeltaTime() const {
    return updateInterval.count();
}