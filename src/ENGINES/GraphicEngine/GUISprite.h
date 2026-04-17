#ifndef GUISPRITE_H
#define GUISPRITE_H

#include "GUIcomponent.h"

class GUISprite : public GUIComponent {
private:
    std::string spriteName;  // Nombre del recurso de la textura
    float scale;             // Escala del sprite
    float rotation;          // Rotación del sprite
    MiColor tint;            // Tinte del sprite

public:
    GUISprite(
        const std::string &id,
        const std::string &spriteName,
        int x,
        int y,
        int width,
        int height,
        ElMotorRaylib &motor,
        float scale = 1.0f,
        float rotation = 0.0f,
        MiColor tint = MiColor(255, 255, 255, 255))
        : GUIComponent(id, x, y, width, height, motor),
          spriteName(spriteName), scale(scale), rotation(rotation), tint(tint) {}

    void Draw() override {
        motor.DibujarTextura(
            spriteName,
            x, y,
            scale, rotation,
            tint);
    }

    void Update() override {
        // Implementar comportamiento adicional si es necesario
    }
};
#endif // GUISPRITE_H
