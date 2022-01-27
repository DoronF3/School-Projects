#include <math.h>
#include "Fractal.h"
#include <iostream>

/**
 * A method that prints a fractal.
 */
void Fractal::printFractal()
{
    for (size_t i = 0; i < _size; i++)
    {
        for (size_t j = 0; j < _size; j++)
        {
            std::cout << _vec[i * _size + j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
 * A method that stores the image fo the sierpinski Carpet fractal inside the vector.
 * @param dim The dimensions of the given fractal.
 */
SierpinskiCarpet::SierpinskiCarpet(int dim)
{
    _size = std::pow(3, dim);
    createVec();
}

/**
 * A method that decides if a point at a specific location is filled or not.
 * This works by iteration first checking if the pixel is unfilled in successively larger squares
 * or cannot be in the center of any larger square.
 * @param x The x coordinate of the point being checked with zero being the first pixel.
 * @param y The y coordinate of the point being checked with zero being the first pixel.
 * @return 1 if it is to be filled, or 0 if it is open.
*/
int SierpinskiCarpet::_isSierpinskiCarpetPixelFilled(int x, int y)
{
    while (x > 0 || y > 0)
    {
        if (x % 3 == 1 && y % 3 == 1)
        {
            return 0;
        }
        x /= 3;
        y /= 3;
    }
    return 1;
}

/**
 * A method that creates the vector of the Sierpinski Carpet fractal.
 */
void SierpinskiCarpet::createVec()
{
    for (size_t i = 0; i < _size; i++)
    {
        for (size_t j = 0; j < _size; j++)
        {
            if (_isSierpinskiCarpetPixelFilled(i, j))
            {
                _vec.push_back("#");
            }
            else
            {
                _vec.push_back(" ");
            }
        }
    }
}

/**
 * C'tor for the Sierpinski Triangle fractal.
 * @param dim The dimensions of the wanted fractal.
 */
SierpinskiTriangle::SierpinskiTriangle(int dim)
{
    _size = std::pow(2, dim);
    createVec();
}

/**
 * A method that decides if a point at a specific location is filled or not.
 * This works by iteration first checking if the pixel is unfilled in successively larger squares
 * or cannot be in the center of any larger square.
 * @param x The x coordinate of the point being checked with zero being the first pixel.
 * @param y The y coordinate of the point being checked with zero being the first pixel.
 * @return 1 if it is to be filled, or 0 if it is open.
*/
int SierpinskiTriangle::_isSierpinskiTrianglePixelFilled(int x, int y)
{
    while (x > 0 || y > 0)
    {
        if (x % 2 == 1 && y % 2 == 1)
        {
            return 0;
        }
        x /= 2;
        y /= 2;
    }
    return 1;
}

/**
 * A method that creates the vector of the Sierpinski Triangle fractal.
 */
void SierpinskiTriangle::createVec()
{
    for (size_t i = 0; i < _size; i++)
    {
        for (size_t j = 0; j < _size; j++)
        {
            if (_isSierpinskiTrianglePixelFilled(i, j))
            {
                _vec.push_back("#");
            }
            else
            {
                _vec.push_back(" ");
            }
        }
    }
}

/**
 * * C'tor for the Vicsek fractal.
 * @param dim The dimensions of the wanted fractal.
 */
VicsekFractal::VicsekFractal(int dim)
{
    _size = std::pow(3, dim);
    createVec();
}

/**
 * A method that creates the vector of the Vicsek fractal.
 */
void VicsekFractal::createVec()
{
    for (size_t i = 0; i < _size; i++)
    {
        for (size_t j = 0; j < _size; j++)
        {
            if (_isVicsekPixelFilled(i, j))
            {
                _vec.push_back("#");
            }
            else
            {
                _vec.push_back(" ");
            }
        }
    }
}

/**
 * A method that decides if a point at a specific location is filled or not.
 * This works by iteration first checking if the pixel is unfilled in successively larger squares
 * or cannot be in the center of any larger square.
 * @param x The x coordinate of the point being checked with zero being the first pixel.
 * @param y The y coordinate of the point being checked with zero being the first pixel.
 * @return 1 if it is to be filled, or 0 if it is open.
*/
int VicsekFractal::_isVicsekPixelFilled(int x, int y)
{
    while (x > 0 || y > 0)
    {
        if ((x % 3 == 1 && y % 3 != 1) || (x % 3 != 1 && y % 3 == 1))
        {
            return 0;
        }
        x /= 3;
        y /= 3;
    }
    return 1;
}