#include "GUIButton.h"

GUIButton::GUIButton(const std::string &id, int x, int y, int width, int height, const std::string &text,
                     MiColor normalColor, MiColor hoverColor, MiColor textColor, ElMotorRaylib &motor)
    : GUIComponent(id, x, y, width, height, motor), text(text),
      normalColor(normalColor), hoverColor(hoverColor), textColor(textColor), 
      isPressed(false), soundEngine(nullptr), hoverPlayed(false) {}

void GUIButton::setClickSound(AlmondSE &engine, const std::string &soundPath) {
    soundEngine = &engine;
    clickSoundPath = soundPath;
}

void GUIButton::setHoverSound(AlmondSE &engine, const std::string &soundPath) {
    soundEngine = &engine;
    hoverSoundPath = soundPath;
}

void GUIButton::Update() {
    isPressed = false;
    if (IsMouseOver()) {
        if (!hoverPlayed && soundEngine && !hoverSoundPath.empty()) {
            soundEngine->playUnaVez(hoverSoundPath); // Reproduce el sonido de hover
            hoverPlayed = true; // Evitar múltiples reproducciones
        }
        if (motor.BotonMousePresionado(MOUSE_LEFT_BUTTON)) {
            isPressed = true;
            if (soundEngine && !clickSoundPath.empty()) {
                soundEngine->playUnaVez(clickSoundPath); // Reproduce el sonido de clic
            }
        }
    } else {
        hoverPlayed = false; // Reiniciar el estado de hover si el mouse se aleja
    }
}

void GUIButton::Draw() {
    MiColor currentColor = IsMouseOver() ? hoverColor : normalColor;
    motor.DibujarRectangulo(x, y, width, height, currentColor);

    int textWidth = motor.CalcularAnchoTexto(text.c_str(), 20); // Tamaño de texto 20
    int textX = x + (width - textWidth) / 2;
    int textY = y + (height - 20) / 2;

    motor.DibujarTextos(text, textX, textY, 20, textColor);
}
