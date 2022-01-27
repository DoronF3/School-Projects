// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <iosfwd>

/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
    int rows, cols;
} MatrixDims;

/**
 * A class that represents A matrix object.
 */
class Matrix
{
public:

    /**
     * The default c'tor that Constructs 1×1 Matrix inits the single element to 0.
     */
    Matrix();

    /**
     * A c'tor that Constructs Matrix rows × cols and inits all elements to 0.
     * @param rows The number of desired rows.
     * @param cols The number of desired cols.
     */
    Matrix(int rows, int cols);

    /**
     * A c'tor that Constructs matrix from another Matrix m.
     * @param m A Matrix.
     */
    Matrix(const Matrix &m);

    /**
     * A method that deletes the values array and the object itself.
     */
    ~Matrix();

    /**
     * A method that returns the amount of rows as int.
     * @return The amount of rows as int.
     */
    int getRows() const;

    /**
     * A method that returns the amount of cols as int.
     * @return The amount of cols as int.
     */
    int getCols() const;

    /**
     * A method that changes the matrix into a vector.
     */
    Matrix &vectorize();


    /**
     * A method that prints the elements of the matrix.
     */
    void plainPrint() const;

    /**
     * A method that puts the values of matrix m into this matrix.
     * @param m A matrix.
     * @return A reference to this matrix.
     */
    Matrix &operator=(const Matrix &m);

    /**
     * A method that multiplies two matrices if their sizes are legal.
     * @param m A matrix.
     * @return A const Matrix that represents the multiplication.
     */
    Matrix operator*(const Matrix &m) const;

    /**
     * A method that multiplies a matrix with a scalar on the right.
     * @param c A scalar.
     * @return A matrix.
     */
    Matrix operator*(float c) const;

    /**
     * A method that multiplies a matrix with a scalar on the left.
     * @param c A scalar.
     * @param m A matrix.
     * @return A matrix.
     */
    friend Matrix operator*(float c, const Matrix &m);

    /**
     * A method that adds two matrices if their sizes are legal.
     * @param m A matrix.
     * @return A matrix.
     */
    Matrix operator+(const Matrix &m) const;

    /**
     * A method that adds the values of the matrix m to this matrix and returns this matrix.
     * @param m A matrix.
     * @return The current matrix with the added values.
     */
    Matrix &operator+=(const Matrix &m);

    /**
     * A method that returns the element of the matrix on the i'th row and j'th column (const).
     * @param i The row number.
     * @param j The column number.
     * @return The element on the i'th row and j'th column (const).
     */
    float operator()(int i, int j) const;

    /**
     * A method that returns the element of the matrix on the i'th row and j'th column.
     * @param i The row number.
     * @param j The column number.
     * @return The element on the i'th row and j'th column.
     */
    float &operator()(int i, int j);

    /**
     * A method that returns the i'th element of the matrix (const).
     * @param i The number of the element we wish to get.
     * @return The i'th element of the matrix (const).
     */
    float operator[](int i) const;

    /**
     * A method that returns the i'th element of the matrix.
     * @param i The number of the element we wish to get.
     * @return The i'th element of the matrix.
     */
    float &operator[](int i);

    /**
     * A method that gets an input from the user and adds the values to the matrix.
     * @param in An input stream object.
     * @return An input stream object.
     */
    friend std::istream &operator>>(std::istream &in, const Matrix &m);

    /**
     * A method that prints the mask of the number to the user.
     * @param out An output stream object.
     * @return An output stream object.
     */
    friend std::ostream &operator<<(std::ostream &out, const Matrix &m);

private:
    int _rows{};
    int _cols{};
    float *_arr{};
};

#endif //MATRIX_H
