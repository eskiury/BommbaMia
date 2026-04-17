#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H

#include <string>
#include "../utils/miVector2.h"
#include "../utils/miColor.h"
#include "../ENGINES/GraphicEngine/motorGrafico.h"

class GUIComponent {
protected:
    int x, y;                // Posición del componente
    int width, height;       // Dimensiones del componente
    std::string id;          // Identificador único del componente
    ElMotorRaylib &motor;    // Referencia al motor gráfico
    

public:
    GUIComponent(const std::string &id, int x, int y, int width, int height, ElMotorRaylib &motor)
        : id(id), x(x), y(y), width(width), height(height), motor(motor) {}

    virtual ~GUIComponent() = default;

    // Comprueba si el mouse está sobre el componente
    virtual bool IsMouseOver() const;

    // Actualiza el componente (para implementar en clases derivadas)
    virtual void Update() = 0;

    // Dibuja el componente (para implementar en clases derivadas)
    virtual void Draw() = 0;

    // Devuelve el ID del componente
    std::string GetId() const { return id; }

    // Setters y Getters adicionales si se necesitan
    void SetPosition(int newX, int newY) { x = newX; y = newY; }
    void SetSize(int newWidth, int newHeight) { width = newWidth; height = newHeight; }

    
};

#endif // GUICOMPONENT_H
