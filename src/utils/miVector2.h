#ifndef MIVECTOR2_H
#define MIVECTOR2_H
#include <iostream>

/**
 * @brief MiVector2 representa un vector tridimensional con componentes en X e Y.
 * 
 * Esta clase simple permite definir y manipular vectores de dos componentes (x, y),
 */
class MiVector2
{
public:
    /**
     * @brief Constructor por defecto.
     * 
     * Inicializa el vector a (0.0f, 0.0f).
     */
    MiVector2() : x(0.0f), y(0.0f) {}
    /**
     * @brief Destructor por defecto.
     * 
     * No realiza ninguna operación especial ya que no se gestionan recursos dinámicos.
     */
    ~MiVector2() = default; // Destructor para limpiar los recursos
    /**
     * @brief Constructor con valores iniciales.
     * 
     * Permite inicializar el vector con valores personalizados.
     * 
     * @param xx Componente X del vector.
     * @param yy Componente Y del vector.
     */
    MiVector2(float xx, float yy)
        : x(xx), y(yy){}


    //variables
    float x, y; ///< COmponentes x,y,z

private:

 
};

#endif 