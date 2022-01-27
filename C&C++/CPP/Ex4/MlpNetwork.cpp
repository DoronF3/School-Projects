//
// Created by magidoro on 23/12/2019.
//

#include "MlpNetwork.h"
#include "Dense.h"

/**
 * A c'tor for that constructs an Mlp object.
 * @param weights An array of 4 matrices representing the weights.
 * @param biases An array of 4 matrices representing the biases.
 */
MlpNetwork::MlpNetwork(const Matrix *weights, const Matrix *biases): _weights(weights), _biases(biases),
_d1(_weights[0], _biases[0], Relu),
_d2(_weights[1], _biases[1], Relu),
_d3(_weights[2], _biases[2], Relu),
_d4(_weights[3], _biases[3], Softmax)
{}

/**
 * A method that applies the entire network on input, and returns digit struct.
 * @param m A matrix to apply onto.
 * @return digit struct.
 */
Digit MlpNetwork::operator()(const Matrix &m) const
{
    Digit d;
    d.value = 0;
    d.probability = 0;
    Matrix mat(_d4(_d3(_d2(_d1(m)))));
    for (int i = 0; i < mat.getRows() * mat.getCols(); i++)
    {
        if (mat[i] > d.probability)
        {
            d.probability = mat[i];
            d.value = i;
        }
    }
    return d;
}



