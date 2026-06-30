# BommbaMia! | C++ Custom Engine & Multi-Mode Utility AI

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=for-the-badge&logo=c%2B%2B" alt="C++17">
  <img src="https://img.shields.io/badge/Architecture-ECS%20from%20scratch-orange.svg?style=for-the-badge" alt="ECS Architecture">
  <img src="https://img.shields.io/badge/Role-Lead%20AI%20Programmer-red.svg?style=for-the-badge" alt="Lead AI Programmer">
</p>

## 🕹️ Overview
BommbaMia! is a grid-based action game built completely **from scratch using a custom-made C++17 graphics engine**. As the **Lead AI Programmer**, I analyzed, designed, and implemented a highly flexible decision-making architecture and pathfinding system. The system manages multiple distinct agent behaviors across different game modes without relying on third-party middleware.

---

## 📸 Media & Screenshots

<p align="center">
   <img width="45%" alt="Gameplay Mode 1" src="https://github.com/user-attachments/assets/b09f1e6b-dd11-4f6c-a6cd-5e83f4a4fcae" />
   <img width="45%" alt="Gameplay Mode 2" src="https://github.com/user-attachments/assets/cab50014-193f-4576-a21c-20d0a90c70ad" />


</p>

---

## 🧠 Technical Highlights & Multi-Behavior AI

Instead of using rigid Finite State Machines, I architected a dynamic **Utility AI** system that evaluates grid tiles and movement ranges in real-time. This single architecture was scaled to handle **three completely different AI behavioral profiles** depending on the game state and mode:

### 1. Classic Deathmatch Behavior (Bomberman Style)
NPCs score multiple parameters to pick the highest-utility tactical action:
*   **Hiding / Fleeing:** Evaluating safe zones and pathing away when bombs are active.
*   **Attacking:** Scoring offensive positions based on enemy proximity and bomb explosion radiuses.
*   **Item Collection:** Pathing towards power-ups dynamically based on utility weights.

### 2. Post-Death "Spotlight" Ghost Behavior
Death isn't the end of the loop. Dead players and NPCs respawn as ghosts tracking remaining targets:
*   **Stun Mechanics:** The AI switches priorities to stalk living players, positioning itself directly over them to trigger a blinding flash utility that temporarily stuns the target.

### 3. Territory Control Behavior (Paint Mode)
A dedicated game mode where the win condition shifts from killing to territory dominance via explosions:
*   **Area Evaluation:** The AI completely overrides its combat utility vectors to focus on grid coverage, calculating paths that maximize unpainted terrain coverage with its blast radius before the timer runs out.

### 🗺️ Custom A* Pathfinding
*   Designed and implemented a custom **A* Pathfinding algorithm from scratch** tailored for our grid layout.
*   Integrated directly with the utility system to dynamically calculate node weights based on environmental hazards (e.g., ticking bombs, fire zones, or paint targets).

---

## ⚡ The Engineering Challenge: Memory & Cache Optimization
**The Problem:** Running complex grid evaluations and pathfinding queries for multiple dynamic NPCs simultaneously across different game modes inside a custom-built engine created a heavy CPU bottleneck.

**The Solution:** Leveraging my experience with **low-level hardware and SM83/Z80 Assembly** (from legacy hardware projects), I utilized a deep understanding of internal memory layouts, cache misses, and CPU cycles. 
*   Optimized data structures within the A* open/closed sets to dramatically reduce dynamic memory allocations during runtime.
*   Structured AI decision components to maximize data locality within our **custom Entity Component System (ECS)**.
*   **Result:** Achieved a highly optimized, lag-free, and fun gameplay experience with distinct, smart NPC archetypes operating smoothly inside our engine loop.

---

## 🛠️ Tech Stack
*   **Language:** C++17
*   **Architecture:** Custom Entity Component System (ECS)
*   **Systems:** Multi-profile Utility AI, Custom A* Pathfinding
