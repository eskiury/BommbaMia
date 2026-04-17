# Almond Studio: ABP Game Engine & Project

This repository showcases my "Masterpiece" project: a custom-built Game Engine (**AlmondGE**) and a complete game prototype developed in C++. The project follows a modular **Entity Component System (ECS)** architecture, focusing on high-performance logic and scalability.

## 🏆 My Roles: Lead AI Programmer & Game Designer
In this project, I was responsible for the core AI architecture and the overall technical design of the engine's systems.

## 🚀 Technical Core Highlights (Code Tour)
Instead of browsing the whole repo, here are the most significant parts of my work:

* **Custom A* Pathfinding & Graph Logic:** I implemented a robust A* algorithm from scratch to handle complex NPC navigation.
    * [View A* Implementation](src/utils/astar.cpp)
    * [View Graph System](src/utils/graph.cpp)
* **AI Management System:** The brain of the NPCs, managing decision-making and path requests.
    * [View AI Manager](src/utils/iaManager.cpp)
    * [View AI System Logic](src/Sys/IAsystem.cpp)
* **Engine Architecture (ECS):** I designed the data flow to separate logic (Systems) from data (Components).
    * [View Entity Manager](src/AlmondGE/EntityManager.cpp)
    * [View Component Definitions](src/CMP/Entity.h)

## 🛠️ Tech Stack
* **Language:** C++17
* **Graphics:** Custom OpenGL-based engine (BellotaEngine).
* **Audio:** FMOD Integration (AlmondSE).
* **Data:** JSON for level loading and configuration.

## 🎮 Key Design Features
* **Dynamic Level Loading:** Levels are parsed from JSON, allowing for rapid iteration.
* **Modular Physics:** Dedicated movement and collision controllers.
* **Event-Driven System:** Decoupled communication between engine modules.

## 🕹️ Play the Game
**The latest stable build is available on itch.io:** 👉 [**Play Bommba Mia! on itch.io**](https://almondstudio.itch.io/bommba-mia)
---
*Note: This repository contains the source code only. Assets and external binaries have been removed for showcase purposes.*