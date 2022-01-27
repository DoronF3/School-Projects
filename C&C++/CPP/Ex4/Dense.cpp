// Dense.cpp

#include "Dense.h"

/**
 * A method that returns the weight matrix.
 * @return The weight matrix.
 */
Matrix Dense::getWeights() const
{
    return _w;
}

/**
 * A method that returns the bias matrix.
 * @return The bias matrix.
 */
Matrix Dense::getBias() const
{
    return _bias;
}

/**
 * A method that returns the activation.
 * @return The activation.
 */
Activation Dense::getActivation() const
{
    return _act;
}

/**
 * A method that applies the layer on input and returns an output matrix.
 * @param m A matrix.
 * @return The output matrix.
 */
Matrix Dense::operator()(const Matrix &m) const
{
    return _act(_w * m + _bias);
}