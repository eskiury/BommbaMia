/*
 *
 * Scheduler.h
 * Definicion de los metodos de la clase Scheduler.
 */

#pragma once
#include <vector>
#include "task.h"

class Scheduler
{
private:
    std::vector<Task> tareasIA;
    std::vector<Task> tareasFisica;
    int frameContIA = 0;
    int frameContFisica = 0;

public:
    /*
     * Metodos: addTask(IA/Fisica)
     * Funcion: Metodo para agregar una tarea al Scheduler.
     * INPUTS: Task tarea -> se le pasa uan tarea de la clase Task.
     * OUTPUT: void
     */
    void addTaskIA(Task tarea);
    void addTaskFisica(Task tarea);

    /*
     * Metodos: updates(IA/Fisica)
     * Funcion: Metodo para actualizar el Scheduler.
     * INPUTS: void
     * OUTPUT: void
     */
    void updateIA(double deltaTime);

    /*
     * Metodos: getFrameCont(IA/Fisica)
     * Funcion: Metodo para obtener el frameCont.
     * INPUTS: void
     * OUTPUT: int
     */
    int getFrameContIA();
    int getFrameContFisica();

    /*
     * Metodos: clearTasks(IA/Fisica)
     * Funcion: Metodo para limpiar las tareas del Scheduler.
     * INPUTS: void
     * OUTPUT: void
     */
    void clearTasksIA();
    void clearTasksFisica();
};