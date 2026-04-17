#include "GUIManager.h"
#include "GUIButton.h"
#include "GUISprite.h"

GUIManager::~GUIManager() {
    limpiarComponentes();
}

void GUIManager::AddComponent(GUIComponent* component) {
    components.push_back(component);
}

void GUIManager::Update() {
    for (auto* component : components) {
        component->Update();
    }
}

void GUIManager::Draw() {
    for (auto* component : components) {
        component->Draw();
    }
}

void GUIManager::limpiarComponentes() {
    for (auto* component : components) {
        delete component;
    }
    components.clear();
}

GUIComponent* GUIManager::GetComponentById(const std::string& id) {
    for (auto* component : components) {
        if (component->GetId() == id) {
            return component;
        }
    }
    return nullptr;
}

void GUIManager::agregarBoton(const std::string& id, int x, int y, int width, int height, 
                              const std::string& text, MiColor color, MiColor hoverColor, 
                              MiColor textColor, ElMotorRaylib& motorRaylib, 
                              AlmondSE& soundEngine, const std::string& clickSound, 
                              const std::string& hoverSound) {
    // Crear un botón con soporte de sonido
    GUIButton* boton = new GUIButton(id, x, y, width, height, text, color, hoverColor, textColor, motorRaylib);

    // Configurar los sonidos del botón con AlmondSE
    boton->setClickSound(soundEngine, clickSound);
    boton->setHoverSound(soundEngine, hoverSound);

    // Agregar el botón al administrador
    AddComponent(boton);
}

std::vector<std::string> GUIManager::obtenerBotonesPresionados() const {
    std::vector<std::string> presionados;
    for (const auto* component : components) {
        if (auto* boton = dynamic_cast<const GUIButton*>(component)) {
            if (boton->IsPressed()) {
                presionados.push_back(boton->GetId());
            }
        }
    }
    return presionados;
}

bool GUIManager::estaVacio() const {
    return components.empty();
}


void GUIManager::AgregarSprite(const std::string &id, const std::string &spriteName, int x, int y, int width, int height,ElMotorRaylib& motorRaylib, float scale, float rotation, MiColor tint) {
    auto *sprite = new GUISprite(id, spriteName, x, y, width, height, motorRaylib, scale, rotation, tint);
    AddComponent(sprite);
}
