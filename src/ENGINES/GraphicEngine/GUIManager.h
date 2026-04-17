#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <vector>
#include <string>
#include "GUIcomponent.h"
#include "../../ENGINES/AlmondSE/AlmondSE.h"

class GUIManager
{
private:
    std::vector<GUIComponent *> components; // Almacena todos los componentes

public:
    GUIManager() = default;
    ~GUIManager();

    void AddComponent(GUIComponent *component);
    void Update();
    void Draw();
    void limpiarComponentes();

    // Agregar botones con soporte de sonido
    void agregarBoton(const std::string &id, int x, int y, int width, int height,
                      const std::string &text, MiColor color, MiColor hoverColor,
                      MiColor textColor, ElMotorRaylib &motorRaylib,
                      AlmondSE &soundEngine, const std::string &clickSound,
                      const std::string &hoverSound);

    GUIComponent *GetComponentById(const std::string &id);
    std::vector<std::string> obtenerBotonesPresionados() const;
    bool estaVacio() const;

    void AgregarSprite(const std::string &id, const std::string &spriteName, 
                       int x, int y, int width, int height,ElMotorRaylib& motorRaylib, float scale = 1.0f,
                       float rotation = 0.0f, MiColor tint = {255, 255, 255, 255});
};

#endif // GUIMANAGER_H
