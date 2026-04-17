#include "utility.h"
#include <iostream>
#include "../AlmondGE/EntityManager.h"   // y EntityManager
#include "../Sys/IAsystem.h"
#include "../Sys/BombSystem.h"
#include "../Sys/MapSystem.h"


TaskStatus NodoUB::invoke(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& BombSystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    
    auto start = std::chrono::high_resolution_clock::now();
    if(start<end){                                                          
        return run(mapa, em, e, iaSystem, BombSystem, mapsystem, end);      // Devuelve success si esta hecho y failure si no
    }else{
        return TaskStatus::WAITING;                                         // Devuelve waiting si se queda sin tiempo
    }
}

TaskStatus NodoPrimerNivel::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    auto &iaComponent = em.ia.get(e.ia_key);

    int i = 0;
    TaskStatus r = TaskStatus::WAITING;

    
    while (i<childs.size()) {         //Mientras no haya recorrido todos sus hijos

        const auto& action = childs[i]; //La accion actual es el hijo[i]

        if(action->getStatus() == TaskStatus::RUNNING){                    // Si aun esta en ejecucion
            r = action->invoke(mapa, em, e, iaSystem, bombsystem, mapsystem, end);  // Miras si te queda tiempo y ejecutas

            if(r == TaskStatus::SUCCESS){           // Si el nodo ha terminado lo seteamos a success y seguimos con la ejecucion           
                action->setStatusSuccess();
            } else if(r == TaskStatus::FAILURE){    // Si el nodo ha fallado lo seteamos a failure y seguimos con la ejecucion
                action->setStatusFailure();
            } else if(r == TaskStatus::WAITING){    // Si el nodo no puede seguir ejecutandose por tiempo se setea a waiting y se sale del bucle
                action->setStatusWaiting();
                break;
            }
        } else if(action->getStatus() == TaskStatus::SUCCESS){                    // Si ya es success sumo uno a i y paso al siguiente hijo (las condiciones ya seran del siguiente hijo) 
            i++;
        } else if(action->getStatus() == TaskStatus::FAILURE){         // Si es failure return false y se para la ejecucion de la accion padre pq no puede continuar
            action->setStatusWaiting();
            return TaskStatus::FAILURE;
        } else if(action->getStatus() == TaskStatus::WAITING){         // Si el nodo estaba esperando se setea a running
            action->setStatusRunning();
        }
    }

    bool checkAllStatus = true;     

    for(auto& child : childs) {                             // Recorre los hijos y comprueba que todos estén a succes (hemos terminado jefe)
        if(child->getStatus() != TaskStatus::SUCCESS) {     
            checkAllStatus = false;
            break;
        }
    }
 
    if(checkAllStatus){                         // SI tiene todos los hijos a success ha terminado
        for(auto& action : childs){
            action->setStatusWaiting();
        }
        return TaskStatus::SUCCESS;
    }
    else{
        return TaskStatus::RUNNING;
    }
}

TaskStatus MoverNPC::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    


    auto &fisica = em.fisicas.get(e.fisica_key);

    if(fisica.estaMuerto){
        return TaskStatus::FAILURE;
    }

    auto &iaComponent = em.ia.get(e.ia_key);

    iaSystem.actualizarCamino(em, e);

    iaComponent.estadoDebugger = 3;

    if (iaComponent.inPositionToPlaceBomb){
        iaComponent.inPositionToPlaceBomb = false;

        return TaskStatus::SUCCESS;
    } else {
        return TaskStatus::WAITING;
    }
}

TaskStatus PararNPC::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    
    auto& fisica = em.fisicas.get(e.fisica_key);

    if(fisica.estaMuerto){
        return TaskStatus::FAILURE;
    }

    fisica.vx = 0;
    fisica.vz = 0;

    return TaskStatus::SUCCESS;
}

TaskStatus ColocarBomba::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end){
    
    auto& fisica = em.fisicas.get(e.fisica_key);

    if(fisica.estaMuerto){
        return TaskStatus::FAILURE;
    }

    auto &iaComponent = em.ia.get(e.ia_key);
    if(iaSystem.noQuieroMorir(mapa,em,e)){
        bombsystem.crearBomba(e, em, mapsystem);
    }

    iaComponent.estadoDebugger = 4;

    return TaskStatus::SUCCESS;
}

TaskStatus CrearCamino::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) {
    
    auto &fisicaComponent = em.fisicas.get(e.fisica_key);

    if(fisicaComponent.estaMuerto){
        return TaskStatus::FAILURE;
    }

    auto &iaComponent = em.ia.get(e.ia_key);
    

    switch (iaComponent.nodoActual){
        case NodoClass::CAMINO:
            iaComponent.camino = iaSystem.valorarCasillas(fisicaComponent.tileZ, fisicaComponent.tileX, mapsystem.getMap(), mapsystem.getColorMap(), em, e, 0);
            break;

        case NodoClass::HUIR:
            iaComponent.camino = iaSystem.valorarCasillas(fisicaComponent.tileZ, fisicaComponent.tileX, mapsystem.getMap(), mapsystem.getColorMap(), em, e, 1);
            break;

        case NodoClass::POWERUP:
            iaComponent.camino = iaSystem.valorarCasillas(fisicaComponent.tileZ, fisicaComponent.tileX, mapsystem.getMap(), mapsystem.getColorMap(), em, e, 2);
            break;

        case NodoClass::ATACAR:
            iaComponent.camino = iaSystem.valorarCasillas(fisicaComponent.tileZ, fisicaComponent.tileX, mapsystem.getMap(), mapsystem.getColorMap(), em, e, 3);
            break;

        case NodoClass::ATACARPINTURA:
            iaComponent.camino = iaSystem.valorarCasillas(fisicaComponent.tileZ, fisicaComponent.tileX, mapsystem.getMap(), mapsystem.getColorMap(), em, e, 4);
            break;
        default:
            break;
    }
        
    return TaskStatus::SUCCESS;
}

TaskStatus EscogerFoco::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) {
    auto &iaComponent = em.ia.get(e.ia_key);
    auto &fisicaComponent = em.fisicas.get(e.fisica_key);
    iaComponent.idFocoTarget = iaSystem.takeFocoTarget(fisicaComponent.x, fisicaComponent.z, em, e);
    return TaskStatus::SUCCESS;
}

TaskStatus MoverFoco::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) {
    iaSystem.moverFocoNpc(em, e);

    auto &iaComponent = em.ia.get(e.ia_key);

    if (iaComponent.inPositionToFlash){
        return TaskStatus::SUCCESS;
    } else {
        return TaskStatus::WAITING;
    }
}

TaskStatus AtacarFoco::run(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) {
    auto& iaComponent = em.ia.get(e.ia_key);
    iaComponent.inPositionToFlash = false;
    return TaskStatus::SUCCESS;
}

float Huir::calculateUtility(EntityManager& em, Entity& e) {
    auto &iaComponent = em.ia.get(e.ia_key);
    return iaComponent.peligrosidad;
}

float Camino::calculateUtility(EntityManager& em, Entity& e) {
    //Previamente rasgo1 ,2 y 3 eran a b y c respectivamente. Ahora son un iacomponent para poder cambiarlo desde el json
    auto &iaComponent = em.ia.get(e.ia_key);
    float utilidad = pow(iaComponent.areaNidos,iaComponent.rasgo3) * pow(1-iaComponent.bloquesTotales, iaComponent.rasgo1) * pow(1-iaComponent.peligrosidad, iaComponent.rasgo2);
    return utilidad;
    //de aqui falta ver el areaNido ya que devuleve un tipo conexion y deberia devolver un float
}

float Wait::calculateUtility(EntityManager& em, Entity& e) {
    float utilidad = 0.0f;

    auto &iaComponent = em.ia.get(e.ia_key);
    utilidad = iaComponent.fuegoCercano;
    
    return utilidad;
        
}

float Atacar::calculateUtility(EntityManager& em, Entity& e) {
    auto &iaComponent = em.ia.get(e.ia_key);
    float utilidad = iaComponent.distanciaNPC * (1-iaComponent.bombaCercana);
    return utilidad;
}

float RecogerPowerup::calculateUtility(EntityManager& em, Entity& e) {
    auto &iaComponent = em.ia.get(e.ia_key);
    float a=0.3f, b=0.5f, c=0.8f;
    float utilidad = iaComponent.puedePW * (pow(1 - iaComponent.peligrosidad, a) * pow(iaComponent.distanciaPW, b) * pow(1 - iaComponent.pwObtenidos, c));
    return utilidad;

}

float AtacarPintura::calculateUtility(EntityManager& em, Entity& e) {
    auto &iaComponent = em.ia.get(e.ia_key);
    float a=0.3f, b=0.5f, c=0.8f;
    float utilidad = iaComponent.puedePW * (pow(1 - iaComponent.peligrosidad, a) * pow(iaComponent.distanciaPW, b) * pow(1 - iaComponent.pwObtenidos, c));
    // Por ahora le meto return 1
    return utilidad;
}

float Foco::calculateUtility(EntityManager& em, Entity& e) {
    return 0;    
}

void Root::evaluateAndExecute(std::vector<std::vector<int>> mapa, EntityManager& em, Entity& e, IASystem& iaSystem, BombSystem& bombsystem, MapSystem& mapsystem, std::chrono::high_resolution_clock::time_point& end) {
    float maxUtility = -std::numeric_limits<float>::infinity();
    std::shared_ptr<NodoPrimerNivel> bestAction = nullptr;
    auto &iacomponentVariables = em.ia.get(e.ia_key);

    int contador = 0;

    for (auto& action : childs) {
        float utility = action->calculateUtility(em, e);
        if (utility > maxUtility) {
            maxUtility = utility;
            bestAction = action;
        }
        
        switch (contador){
        case 0:
            iacomponentVariables.huirlargo = utility;
            break;
        case 1:
            iacomponentVariables.caminolargo = utility;
            break;
        case 2:
            iacomponentVariables.powerlargo = utility;
            break;
        case 3:
            iacomponentVariables.atacarlargo = utility;
            break;
        case 4:
            iacomponentVariables.defaultlargo = utility;
            break;
        
        default:
            break;
        }

        contador++;

    }
    
    if(iacomponentVariables.nodoActual != bestAction->getNodoClass() && abs(maxUtility-iacomponentVariables.utilidadNodoActual) < iacomponentVariables.tozudez){
        switch(iacomponentVariables.nodoActual){
            case NodoClass::ATACAR:
                bestAction = iacomponentVariables.Atacar;
                break;
            case NodoClass::CAMINO:
                bestAction = iacomponentVariables.Camino;
                break;
            case NodoClass::HUIR:
                bestAction = iacomponentVariables.Huir;
                break;
            case NodoClass::POWERUP:
                bestAction = iacomponentVariables.RecogerPowerup;
                break;
            case NodoClass::WAIT:
                bestAction = iacomponentVariables.Wait;
                break;
        }
    }

    iacomponentVariables.nodoActual = bestAction->getNodoClass();
    iacomponentVariables.utilidadNodoActual = maxUtility;

    switch(bestAction->getNodoClass()){
        case NodoClass::ATACAR:
            break;
        case NodoClass::CAMINO:
            break;
        case NodoClass::HUIR:
            break;
        case NodoClass::POWERUP:
            break;
        case NodoClass::WAIT:
            break;
    }

    if (bestAction) {
        bestAction->invoke(mapa, em, e, iaSystem, bombsystem, mapsystem, end);
    }
}