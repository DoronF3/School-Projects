// Activation.h

#include "Activation.h"

/**
 * A method that returns this activation’s type(Relu/Softmax).
 * @return This activation’s type(Relu/Softmax).
 */
ActivationType Activation::getActivationType() const
{
    return _actType;
}

/**
 * A method that applies activation function on input.
 * @return A matrix.
 */
Matrix Activation::operator()(const Matrix &m) const
{
    Matrix mat = Matrix(m.getRows(), m.getCols());
    if (getActivationType() == Relu)
    {
        for (int i = 0; i < mat.getRows() * mat.getCols(); i++)
        {
            if (m[i] >= 0)
            {
                mat[i] = m[i];
            }
            else
            {
                mat[i] = 0;
            }
        }
    }
    else
    {
        float sum = 0;
        for (int i = 0; i < mat.getRows(); i++)
        {
            sum += std::exp(m[i]);
        }
        for (int i = 0; i < mat.getRows(); i++)
        {
            mat[i] = (1 / sum) * std::exp(m[i]);
        }
    }
    return mat;
}