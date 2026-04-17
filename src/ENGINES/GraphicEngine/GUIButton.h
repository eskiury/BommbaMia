#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "GUIcomponent.h"
#include "../../ENGINES/AlmondSE/AlmondSE.h"

class GUIButton : public GUIComponent {
private:
    std::string text;        // Texto del botón
    MiColor normalColor;     // Color normal
    MiColor hoverColor;      // Color cuando el mouse está sobre el botón
    MiColor textColor;       // Color del texto
    bool isPressed;          // Estado del botón

    // Sonidos
    AlmondSE* soundEngine;       // Referencia al motor de sonido
    std::string clickSoundPath;  // Ruta del sonido de clic
    std::string hoverSoundPath;  // Ruta del sonido de hover
    bool hoverPlayed;            // Controla si ya se reprodujo el sonido de hover

public:
    GUIButton(const std::string &id, int x, int y, int width, int height, const std::string &text,
              MiColor normalColor, MiColor hoverColor, MiColor textColor, ElMotorRaylib &motor);

    void Update() override;
    void Draw() override;

    void setClickSound(AlmondSE &engine, const std::string &soundPath);
    void setHoverSound(AlmondSE &engine, const std::string &soundPath);

    bool IsPressed() const { return isPressed; }
};

#endif // GUIBUTTON_H
