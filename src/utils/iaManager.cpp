#include "iaManager.h"

void iaManager::cargarNPC1(const std::string& ruta, EntityManager& em, Entity& e1){    
    
    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo JSON.\n";
    }

    nlohmann::json datosNPCs;
    archivo >> datosNPCs;
    archivo.close();

    if(datosNPCs["npcs"][0]["id"] == 1){
        //std::cout<<"AQUI INICIALIZO NPC CON ID "<<datosNPCs["npcs"][1]["id"]<<"\n";
        auto &iaComponent             = em.ia.get(e1.ia_key);
        iaComponent.tieneBT           = datosNPCs["npcs"][0]["tieneBT"];
        iaComponent.elapsedTime       = datosNPCs["npcs"][0]["elapsedTime"];
        iaComponent.tiempoEntreBombas = datosNPCs["npcs"][0]["tiempoEntreBombas"];
        iaComponent.waitedTime        = datosNPCs["npcs"][0]["waitedTime"];
        iaComponent.tiempoEsperando   = datosNPCs["npcs"][0]["tiempoEsperando"];
        iaComponent.dangerLevel       = datosNPCs["npcs"][0]["dangerLevel"];
        iaComponent.distanceToBomb    = datosNPCs["npcs"][0]["distanceToBomb"];
        iaComponent.rasgo1            = datosNPCs["npcs"][0]["rasgo1"];
        iaComponent.rasgo2            = datosNPCs["npcs"][0]["rasgo2"];
        iaComponent.rasgo3            = datosNPCs["npcs"][0]["rasgo3"];
        iaComponent.tozudez           = datosNPCs["npcs"][0]["tozudez"];
    }
}

void iaManager::cargarNPC2(const std::string& ruta, EntityManager& em, Entity& e1, Entity& e2){    
    
    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo JSON.\n";
    }

    nlohmann::json datosNPCs;
    archivo >> datosNPCs;
    archivo.close();

    if(datosNPCs["npcs"][0]["id"] == 1){
        //std::cout<<"AQUI INICIALIZO NPC CON ID "<<datosNPCs["npcs"][1]["id"]<<"\n";
        auto &iaComponent             = em.ia.get(e1.ia_key);
        iaComponent.tieneBT           = datosNPCs["npcs"][0]["tieneBT"];
        iaComponent.elapsedTime       = datosNPCs["npcs"][0]["elapsedTime"];
        iaComponent.tiempoEntreBombas = datosNPCs["npcs"][0]["tiempoEntreBombas"];
        iaComponent.waitedTime        = datosNPCs["npcs"][0]["waitedTime"];
        iaComponent.tiempoEsperando   = datosNPCs["npcs"][0]["tiempoEsperando"];
        iaComponent.dangerLevel       = datosNPCs["npcs"][0]["dangerLevel"];
        iaComponent.distanceToBomb    = datosNPCs["npcs"][0]["distanceToBomb"];
        iaComponent.rasgo1            = datosNPCs["npcs"][0]["rasgo1"];
        iaComponent.rasgo2            = datosNPCs["npcs"][0]["rasgo2"];
        iaComponent.rasgo3            = datosNPCs["npcs"][0]["rasgo3"];
        iaComponent.tozudez           = datosNPCs["npcs"][0]["tozudez"];
    }
    
    if(datosNPCs["npcs"][1]["id"] == 2){
        //std::cout<<"AQUI INICIALIZO NPC CON ID "<<datosNPCs["npcs"][0]["id"]<<"\n";
        auto &iaComponent             = em.ia.get(e2.ia_key);
        iaComponent.tieneBT           = datosNPCs["npcs"][1]["tieneBT"];
        iaComponent.elapsedTime       = datosNPCs["npcs"][1]["elapsedTime"];
        iaComponent.tiempoEntreBombas = datosNPCs["npcs"][1]["tiempoEntreBombas"];
        iaComponent.waitedTime        = datosNPCs["npcs"][1]["waitedTime"];
        iaComponent.tiempoEsperando   = datosNPCs["npcs"][1]["tiempoEsperando"];
        iaComponent.dangerLevel       = datosNPCs["npcs"][1]["dangerLevel"];
        iaComponent.distanceToBomb    = datosNPCs["npcs"][1]["distanceToBomb"];
        iaComponent.rasgo1            = datosNPCs["npcs"][1]["rasgo1"];
        iaComponent.rasgo2            = datosNPCs["npcs"][1]["rasgo2"];
        iaComponent.rasgo3            = datosNPCs["npcs"][1]["rasgo3"];
        iaComponent.tozudez           = datosNPCs["npcs"][1]["tozudez"];
    }
}

void iaManager::cargarNPC3(const std::string& ruta, EntityManager& em, Entity& e1, Entity& e2, Entity& e3){    
    
    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo JSON.\n";
    }

    nlohmann::json datosNPCs;
    archivo >> datosNPCs;
    archivo.close();

    if(datosNPCs["npcs"][0]["id"] == 1){
        //std::cout<<"AQUI INICIALIZO NPC CON ID "<<datosNPCs["npcs"][1]["id"]<<"\n";
        auto &iaComponent             = em.ia.get(e1.ia_key);
        iaComponent.tieneBT           = datosNPCs["npcs"][0]["tieneBT"];
        iaComponent.elapsedTime       = datosNPCs["npcs"][0]["elapsedTime"];
        iaComponent.tiempoEntreBombas = datosNPCs["npcs"][0]["tiempoEntreBombas"];
        iaComponent.waitedTime        = datosNPCs["npcs"][0]["waitedTime"];
        iaComponent.tiempoEsperando   = datosNPCs["npcs"][0]["tiempoEsperando"];
        iaComponent.dangerLevel       = datosNPCs["npcs"][0]["dangerLevel"];
        iaComponent.distanceToBomb    = datosNPCs["npcs"][0]["distanceToBomb"];
        iaComponent.rasgo1            = datosNPCs["npcs"][0]["rasgo1"];
        iaComponent.rasgo2            = datosNPCs["npcs"][0]["rasgo2"];
        iaComponent.rasgo3            = datosNPCs["npcs"][0]["rasgo3"];
        iaComponent.tozudez           = datosNPCs["npcs"][0]["tozudez"];
    }
    
    if(datosNPCs["npcs"][1]["id"] == 2){
        //std::cout<<"AQUI INICIALIZO NPC CON ID "<<datosNPCs["npcs"][0]["id"]<<"\n";
        auto &iaComponent             = em.ia.get(e2.ia_key);
        iaComponent.tieneBT           = datosNPCs["npcs"][1]["tieneBT"];
        iaComponent.elapsedTime       = datosNPCs["npcs"][1]["elapsedTime"];
        iaComponent.tiempoEntreBombas = datosNPCs["npcs"][1]["tiempoEntreBombas"];
        iaComponent.waitedTime        = datosNPCs["npcs"][1]["waitedTime"];
        iaComponent.tiempoEsperando   = datosNPCs["npcs"][1]["tiempoEsperando"];
        iaComponent.dangerLevel       = datosNPCs["npcs"][1]["dangerLevel"];
        iaComponent.distanceToBomb    = datosNPCs["npcs"][1]["distanceToBomb"];
        iaComponent.rasgo1            = datosNPCs["npcs"][1]["rasgo1"];
        iaComponent.rasgo2            = datosNPCs["npcs"][1]["rasgo2"];
        iaComponent.rasgo3            = datosNPCs["npcs"][1]["rasgo3"];
        iaComponent.tozudez           = datosNPCs["npcs"][1]["tozudez"];
    }
    if(datosNPCs["npcs"][2]["id"] == 3){
        //std::cout<<"AQUI INICIALIZO NPC CON ID "<<datosNPCs["npcs"][2]["id"]<<"\n";
        auto &iaComponent             = em.ia.get(e3.ia_key);
        iaComponent.tieneBT           = datosNPCs["npcs"][2]["tieneBT"];
        iaComponent.elapsedTime       = datosNPCs["npcs"][2]["elapsedTime"];
        iaComponent.tiempoEntreBombas = datosNPCs["npcs"][2]["tiempoEntreBombas"];
        iaComponent.waitedTime        = datosNPCs["npcs"][2]["waitedTime"];
        iaComponent.tiempoEsperando   = datosNPCs["npcs"][2]["tiempoEsperando"];
        iaComponent.dangerLevel       = datosNPCs["npcs"][2]["dangerLevel"];
        iaComponent.distanceToBomb    = datosNPCs["npcs"][2]["distanceToBomb"];
        iaComponent.rasgo1            = datosNPCs["npcs"][2]["rasgo1"];
        iaComponent.rasgo2            = datosNPCs["npcs"][2]["rasgo2"];
        iaComponent.rasgo3            = datosNPCs["npcs"][2]["rasgo3"];
        iaComponent.tozudez           = datosNPCs["npcs"][2]["tozudez"];
    }
}

void iaManager::GuardarJSON(const nlohmann::json &datos, const std::string &filename)
{
    std::ofstream file(filename, std::ios::trunc); // Fuerza el borrado del contenido antes de escribir
    if (file.is_open())
    {
        //std::cout << "Guardando JSON con los siguientes datos:\n" << datos.dump(4) << std::endl;
        file << datos.dump(4);
        file.close();
        //std::cout << "Archivo JSON guardado correctamente.\n";
    }
    else
    {
        //std::cerr << "Error al abrir el archivo JSON.\n";
    }
}