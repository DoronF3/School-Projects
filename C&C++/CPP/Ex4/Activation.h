//Activation.h

#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"
#include <math.h>

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    Relu,
    Softmax
};

/**
 * A class that represents an activation type object.
 */
class Activation
{
public:

    /**
     * A c'tor that Accepts activation type (Relu/Softmax)
     * and defines this instance’s activation accordingly.
     * @param actType An activation type object.
     */
    explicit Activation(ActivationType actType) : _actType(actType)
    {
    };

    /**
     * A method that returns this activation’s type(Relu/Softmax).
     * @return This activation’s type(Relu/Softmax).
     */
    ActivationType getActivationType() const;

    /**
     * A method that applies activation function on input.
     * @param m A matrix.
     * @return A matrix.
     */
    Matrix operator()(const Matrix &m) const;

private:
    const ActivationType _actType{};
};

#endif //ACTIVATION_H
