#ifndef MICOLOR_H
#define MICOLOR_H
#include <iostream>

/**
 * @brief MiColor clase para representar colores utilizando el modelo RGBA (Rojo, Verde, Azul, Alfa).
 * 
 * Almacena componentes de color como valores enteros sin signo de 8 bits (0-255).
 */
class MiColor
{
public:
    /**
     * @brief Constructor por defecto.
     * 
     * Inicializa el color como negro opaco (r=0, g=0, b=0, a=255).
     */
    MiColor() : r(0), g(0), b(0), a(255) {}
    /**
     * @brief Destructor por defecto.
     * 
     * No realiza ninguna operación especial.
     */
    ~MiColor() = default; // Destructor para limpiar los recursos
    /**
     * @brief Constructor con valores RGBA personalizados.
     * 
     * @param red   Valor del componente rojo (0-255).
     * @param green Valor del componente verde (0-255).
     * @param blue  Valor del componente azul (0-255).
     * @param alpha Valor del componente alfa (transparencia) (0-255).
     */
    MiColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
        : r(red), g(green), b(blue), a(alpha) {}

    
     /**
     * @brief Establece los componentes RGBA del color.
     * 
     * @param red   Nuevo valor del componente rojo.
     * @param green Nuevo valor del componente verde.
     * @param blue  Nuevo valor del componente azul.
     * @param alpha Nuevo valor del componente alfa.
     */
    void set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
    {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

    /**
     * @brief Establece solo el componente alfa del color.
     * 
     * @param alpha Nuevo valor del componente alfa.
     */
    void setAlpha( unsigned char alpha)
    {
        a = alpha;
    }

    //variables
    unsigned char r, g, b, a; ///< Componentes RGBA como enteros (0-255)

private:

 
};

#endif 