#include <string>
#include <vector>

#ifndef FRACTALDRAWER_FRACTAL_H
#define FRACTALDRAWER_FRACTAL_H

#endif //FRACTALDRAWER_FRACTAL_H

/**
 * An abstract class representing a general fractal.
 */
class Fractal
{
public:

    /**
     * A method that prints the given fractal.
     */
    virtual void printFractal();

    /**
     * A destructor for the fractals.
     */
    virtual ~Fractal() = default;

protected:

    /**
     * A method that creates the vector of the Sierpinski Carpet fractal.
     */
    virtual void createVec() = 0;

    std::vector<std::string> _vec;
    size_t _size;

};

/**
 * A class that represents the Sierpinski Carpet fractal.
 */
class SierpinskiCarpet : public Fractal
{
public:

    /**
     * C'tor for the Sierpinski Carpet fractal.
     * @param dim The dimensions of the wanted fractal.
     */
    explicit SierpinskiCarpet(int dim);

private:

    /**
     * A method that decides if a point at a specific location is filled or not.
     * This works by iteration first checking if the pixel is unfilled in successively larger squares
     * or cannot be in the center of any larger square.
     * @param x The x coordinate of the point being checked with zero being the first pixel.
     * @param y The y coordinate of the point being checked with zero being the first pixel.
     * @return 1 if it is to be filled, or 0 if it is open.
     */
    int _isSierpinskiCarpetPixelFilled(int x, int y);

    /**
     * A method that creates the vector of the Sierpinski Carpet fractal.
     */
    void createVec() override;
};

/**
 * A class that represents the Sierpinski Triangle fractal.
 */
class SierpinskiTriangle : public Fractal
{
public:

    /**
     * C'tor for the Sierpinski Triangle fractal.
     * @param dim The dimensions of the wanted fractal.
     */
    explicit SierpinskiTriangle(int dim);

private:

    /**
    * A method that decides if a point at a specific location is filled or not.
    * This works by iteration first checking if the pixel is unfilled in successively larger squares
    * or cannot be in the center of any larger square.
    * @param x The x coordinate of the point being checked with zero being the first pixel.
    * @param y The y coordinate of the point being checked with zero being the first pixel.
    * @return 1 if it is to be filled, or 0 if it is open.
    */
    int _isSierpinskiTrianglePixelFilled(int x, int y);

    /**
     * A method that creates the vector of the Sierpinski Carpet fractal.
     */
    void createVec() override;
};

/**
 * A class that represents the Vicsek fractal.
 */
class VicsekFractal : public Fractal
{
public:

    /**
     * * C'tor for the Vicsek fractal.
     * @param dim The dimensions of the wanted fractal.
     */
    explicit VicsekFractal(int dim);

private:

    /**
     * A method that decides if a point at a specific location is filled or not.
     * This works by iteration first checking if the pixel is unfilled in successively larger squares
     * or cannot be in the center of any larger square.
     * @param x The x coordinate of the point being checked with zero being the first pixel.
     * @param y The y coordinate of the point being checked with zero being the first pixel.
     * @return 1 if it is to be filled, or 0 if it is open.
    */
    int _isVicsekPixelFilled(int x, int y);

    /**
     * A method that creates the vector of the Sierpinski Carpet fractal.
     */
    void createVec() override;
};