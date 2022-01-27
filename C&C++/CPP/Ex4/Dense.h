// Dense.h

#include "Matrix.h"
#include "Activation.h"

#ifndef EX4_DENSE_H
#define EX4_DENSE_H

/**
 * A class that represents a dense type object.
 */
class Dense
{
public:

    /**
     * A c'tor that inits a new layer with given parameters.
     * @param w Weights matrix.
     * @param bias Bias matrix.
     * @param actType Activation type.
     */
    Dense(const Matrix &w, const Matrix &bias, ActivationType actType) : _w(w), _bias(bias), _act(actType)
    {
    };

    /**
     * A method that returns the weight matrix.
     * @return The weight matrix.
     */
    Matrix getWeights() const;

    /**
     * A method that returns the bias matrix.
     * @return The bias matrix.
     */
    Matrix getBias() const;

    /**
     * A method that returns the activation.
     * @return The activation.
     */
    Activation getActivation() const;

    /**
     * A method that applies the layer on input and returns an output matrix.
     * @param m A matrix.
     * @return The output matrix.
     */
    Matrix operator()(const Matrix &m) const;

private:
    const Matrix _w{};
    const Matrix _bias{};
    const Activation _act;
};

#endif //EX4_DENSE_H
