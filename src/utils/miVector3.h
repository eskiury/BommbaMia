#ifndef MIVECTOR3_H
#define MIVECTOR3_H
#include <iostream>

/**
 * @brief MiVector3 representa un vector tridimensional con componentes en X, Y y Z.
 * 
 * Esta clase simple permite definir y manipular vectores de tres componentes (x, y, z),
 */
class MiVector3
{
public:
    /**
     * @brief Constructor por defecto.
     * 
     * Inicializa el vector a (0.0f, 0.0f, 0.0f).
     */
    MiVector3() : x(0.0f), y(0.0f), z(0.0f) {}
    /**
     * @brief Destructor por defecto.
     * 
     * No realiza ninguna operación especial ya que no se gestionan recursos dinámicos.
     */
    ~MiVector3() = default; // Destructor para limpiar los recursos
    /**
     * @brief Constructor con valores iniciales.
     * 
     * Permite inicializar el vector con valores personalizados.
     * 
     * @param xx Componente X del vector.
     * @param yy Componente Y del vector.
     * @param zz Componente Z del vector.
     */
    MiVector3(float xx, float yy, float zz)
        : x(xx), y(yy), z(zz){}


    //variables
    float x, y, z; ///< COmponentes x,y,z

private:

 
};

#endif 