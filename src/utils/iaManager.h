#ifndef IA_MANAGER_HPP
#define IA_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <json.hpp>
#include "../AlmondGE/EntityManager.h"
struct Entity;

struct iaManager {
    void cargarNPC1(const std::string&, EntityManager&, Entity&);
    void cargarNPC2(const std::string&, EntityManager&, Entity&, Entity&);
    void cargarNPC3(const std::string&, EntityManager&, Entity&, Entity&, Entity&);
    void GuardarJSON(const nlohmann::json &datos, const std::string &filename);
};

#endif // IA_MANAGER_HPP
