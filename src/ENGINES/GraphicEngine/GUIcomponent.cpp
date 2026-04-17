#include "GUIcomponent.h"

bool GUIComponent::IsMouseOver() const {
    MiVector2 mousePos = motor.ObtenerPosicionMouse();
    return mousePos.x >= x && mousePos.x <= x + width &&
           mousePos.y >= y && mousePos.y <= y + height;
}

