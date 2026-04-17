/*
 * task.cpp
 *
 * Clase en la cual se crea el formato de la task para utilizar en el scheduler.
 * Sus parametros van a ser la funcion a ejecutar, la frecuencia con la que se ejecutara y la fase en la que se ejecutara.
 *
 */

#pragma once

#include <functional>

class Task
{
public:
    Task(std::function<void(double)> funcion, int frecuencia, int fase) : funcion(funcion), frecuencia(frecuencia), fase(fase) {}
    void setFuncion(std::function<void(double)> fun) { this->funcion = fun; }
    void setFrecuencia(int freq) { this->frecuencia = freq; }
    void setFase(int f) { this->fase = f; }
    int getFrecuencia() { return frecuencia; }
    int getFase() { return fase; }
    std::function<void(double)> getFuncion() const { return funcion; }

private:
    std::function<void(double)> funcion;
    int frecuencia;
    int fase;
};