/*
 * Scheduler.cpp
 *
 * Cada tarea a ejecutar en el Scheduler se representa con un objeto de la clase Task.
 *   - Cada tarea tiene una funcion a ejecutar, una frecuencia que indica cada cuantos frames se ejecutara y una fase que indica en que frame empezara a ejecutarse.
 *   ----------------------------------------- EJEMPLO: Task([this]() { código de la tarea }, 5, 2); -----------------------------------------
 *   - La tarea se ejecutara cada 5 frames y empezara a ejecutarse en el frame cuyo numero sea (frameCount % 5 == 2).
 *
 * --------------------------------------------- ESTRUCTURA DEL SCHEDULER --------------------------------------------------------
 * - El Scheduler tiene un vector de tareas a ejecutar.
 * - Tambien se tiene un contador de frames que se incrementa cada vez que se llama a update().
 *
 * --------------------------------------------- FUNCIONALIDAD DEL SCHEDULER -----------------------------------------------------
 * - Cada vez que se llama al metodo update() se van a realizar una serie de pasos:
 *        1. Se incrementa el contador de frames.
 *        2. Se recorren todas las tareas
 *        3. En este paso se va a verificar si la tarea se ejecuta en este frame o no: con la condicion (frameCount % task.frequency == task.phase),
 *           si se cumple la condicion se ejecuta la tarea. Por ejemplo, si frameCount es 7, y una tarea tiene frequency = 5 y phase = 2, se verifica si 7 % 5 == 2.
 *           Como 7 % 5 es 2, se ejecuta la tarea.
 */
#include "Scheduler.h"

/*
 *=====================================================================
 *                            AGREGAR TASKS
 * ====================================================================
 */
void Scheduler::addTaskIA(Task tarea)
{
    tareasIA.push_back(tarea);
}

void Scheduler::addTaskFisica(Task tarea)
{
    tareasFisica.push_back(tarea);
}

/*
 *=====================================================================
 *                            UPDATES
 * ====================================================================
 */

 void Scheduler::updateIA(double deltaTime)
 {
     frameContIA++;
     for (auto &task : tareasIA)
     {
         if (frameContIA % task.getFrecuencia() == task.getFase())
         {
             task.getFuncion()(deltaTime);
         }
     }
 }



/*
 *=====================================================================
 *                            GETTERS
 * ====================================================================
 */

int Scheduler::getFrameContIA()
{
    return frameContIA;
}

int Scheduler::getFrameContFisica()
{
    return frameContFisica;
}

/*
 *=====================================================================
 *                               CLEARS
 * ====================================================================
 */

void Scheduler::clearTasksIA()
{
    tareasIA.clear();
}

void Scheduler::clearTasksFisica()
{
    tareasFisica.clear();
}