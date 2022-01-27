// Matrix.cpp

#include "Matrix.h"
#include <iostream>

#define ERRMSG "ERROR: The dimensions of the matrices don't enable this kind of operation."
#define INMSG "ERROR: Something went wrong, check if the amount of arguments and that they are all float."
#define SIZEMSG "ERROR: rows and cols must be positive numbers."

/**
 * A c'tor that Constructs Matrix rows × cols and inits all elements to 0.
 * @param rows The number of desired rows.
 * @param cols The number of desired cols.
 */
Matrix::Matrix(int rows, int cols)
        : _rows(rows), _cols(cols)
{
    if (rows <= 0 || cols <= 0)
    {
        std::cerr << SIZEMSG << std::endl;
        exit(EXIT_FAILURE);
    }
    _arr = new float[rows * cols]{};
}

/**
 * The default c'tor that Constructs 1×1 Matrix inits the single element to 0.
 */
Matrix::Matrix()
        : Matrix(1, 1)
{
}

/**
 * A c'tor that Constructs matrix from another Matrix m.
 * @param m A Matrix.
 */
Matrix::Matrix(const Matrix &m) : Matrix(m._rows, m._cols)
{
    for (int i = 0; i < _rows * _cols; i++)
    {
        _arr[i] = m._arr[i];
    }
}

/**
 * A method that returns the amount of rows as int
 * .
 * @return The amount of rows as int.
 */
int Matrix::getRows() const
{
    return _rows;
}

/**
 * A method that returns the amount of cols as int.
 * @return The amount of cols as int.
 */
int Matrix::getCols() const
{
    return _cols;
}

/**
 * A method that puts the values of matrix m into this matrix.
 * @param m A matrix.
 * @return A reference to this matrix.
 */
Matrix &Matrix::operator=(const Matrix &m)
{
    if (this == &m) // Same object.
    {
        return *this;
    }
    delete[] _arr;
    _rows = m._rows;
    _cols = m._cols;
    int size = _rows * _cols;
    _arr = new float[size];
    for (int i = 0; i < size; i++)
    {
        _arr[i] = m._arr[i];
    }
    return *this;
}

/**
 * A method that deletes the values array and the object itself.
 */
Matrix::~Matrix()
{
    delete[] _arr;
//    delete this;
}

/**
 * A method that changes the matrix into a vector.
 */
Matrix &Matrix::vectorize()
{
    _rows = _rows * _cols;
    _cols = 1;
    return *this;
}

/**
 * A method that prints the elements of the matrix.
 */
void Matrix::plainPrint() const
{
    for (int i = 0; i < _rows; i++)
    {
        for (int j = 0; j < _cols; j++)
        {
            std::cout << _arr[i * _cols + j] << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * A method that multiplies to matrices if their sizes are legal.
 * @param m A matrix.
 * @return A const Matrix that represents the multiplication.
 */
Matrix Matrix::operator*(const Matrix &m) const
{
    if (_cols != m._rows) // Can't be multiplied.
    {
        std::cerr << ERRMSG << std::endl;
        exit(EXIT_FAILURE);
    }
    Matrix mat(_rows, m._cols);
    for (int i = 0; i < _rows; i++)
    {
        for (int j = 0; j < m._cols; j++)
        {
            for (int k = 0; k < _cols; k++)
            {
                mat(i, j) += (*this)(i, k) * m(k, j);
            }
        }
    }
    return mat;
}

/**
 * A method that multiplies a matrix with a scalar on the right.
 * @param c A scalar.
 * @return A matrix.
 */
Matrix Matrix::operator*(float c) const
{
    Matrix mat(_rows, _cols); // Create new matrix.
    for (int i = 0; i < mat._rows * mat._cols; i++) // Add values.
    {
        mat._arr[i] = _arr[i] * c;
    }
    return mat;
}

/**
 * A method that multiplies a matrix with a scalar on the left.
 * @param c A scalar.
 * @param m A matrix.
 * @return A matrix.
 */
Matrix operator*(float c, const Matrix &m)
{
    return m * c;
}

/**
 * A method that adds two matrices if their sizes are legal.
 * @param m A matrix.
 * @return A matrix.
 */
Matrix Matrix::operator+(const Matrix &m) const
{
    if (_rows != m._rows || _cols != m._cols) // Can't add.
    {
        std::cerr << ERRMSG << std::endl;
        exit(EXIT_FAILURE);
    }
    Matrix mat(_rows, _cols); // Copy and create new matrix.
    for (int i = 0; i < mat._rows * mat._cols; i++)
    {
        mat._arr[i] = _arr[i] + m._arr[i];
    }
    return mat;
}

/**
 * A method that adds the values of the matrix m to this matrix and returns this matrix.
 * @param m A matrix.
 * @return The current matrix with the added values.
 */
Matrix &Matrix::operator+=(const Matrix &m)
{
    if (_rows != m._rows || _cols != m._cols) // Can't add.
    {
        std::cerr << ERRMSG << std::endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < _rows * _cols; i++)
    {
        _arr[i] += + m._arr[i];
    }
    return *this;
}

/**
 * A method that returns the element of the matrix on the i'th row and j'th column (const).
 * @param i The row number.
 * @param j The column number.
 * @return The element on the i'th row and j'th column (const).
 */
float Matrix::operator()(int i, int j) const
{
    if (i < 0 || j < 0 || i >= _rows || j >= _cols)
    {
        std::cerr << SIZEMSG << std::endl;
        exit(EXIT_FAILURE);
    }
    return _arr[i * _cols + j];
}

/**
 * A method that returns the element of the matrix on the i'th row and j'th column.
 * @param i The row number.
 * @param j The column number.
 * @return The element on the i'th row and j'th column.
 */
float &Matrix::operator()(int i, int j)
{
    if (i < 0 || j < 0 || i >= _rows || j >= _cols)
    {
        std::cerr << SIZEMSG << std::endl;
        exit(EXIT_FAILURE);
    }
    return _arr[i * _cols + j];
}

/**
 * A method that returns the i'th element of the matrix (const).
 * @param i The number of the element we wish to get.
 * @return The i'th element of the matrix (const).
 */
float Matrix::operator[](int i) const
{
    if (i < 0 || i >= _rows * _cols)
    {
        std::cerr << SIZEMSG << std::endl;
        exit(EXIT_FAILURE);
    }
    return _arr[i];
}

/**
 * A method that returns the i'th element of the matrix.
 * @param i The number of the element we wish to get.
 * @return The i'th element of the matrix.
 */
float &Matrix::operator[](int i)
{
    if (i < 0 || i >= _rows * _cols)
    {
        std::cerr << SIZEMSG << std::endl;
        exit(EXIT_FAILURE);
    }
    return _arr[i];
}

/**
 * A method that gets an input from the user and adds the values to the matrix.
 * @param in An input stream object.
 * @return An input stream object.
 */
std::istream &operator>>(std::istream &in, const Matrix &m)
{
    size_t size = m.getRows() * m.getCols() * sizeof(float);
    in.read((char *) m._arr, size);
    if (!in.good())
    {
        std::cerr << INMSG << std::endl;
        exit(EXIT_FAILURE);
    }
    return in;
}

/**
 * A method that prints the mask of the number to the user.
 * @param out An output stream object.
 * @return An output stream object.
 */
std::ostream &operator<<(std::ostream &out, const Matrix &m)
{
    for (int i = 0; i < m._rows; i++)
    {
        for (int j = 0; j < m._cols; j++)
        {
            if (m(i, j) <= 0.1f)
            {
                out << "  ";
            }
            else
            {
                out << "**";
            }
        }
        out << std::endl;
    }
    return out;
}
