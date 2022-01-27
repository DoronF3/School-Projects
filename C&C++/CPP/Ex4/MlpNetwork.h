//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"

#define MLP_SIZE 4

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784},
                                  {64,  128},
                                  {20,  64},
                                  {10,  20}};
const MatrixDims biasDims[] = {{128, 1},
                               {64,  1},
                               {20,  1},
                               {10,  1}};

/**
 *  A class that represents Mlp network object.
 */
class MlpNetwork
{
public:

    /**
     * A c'tor for that constructs an Mlp object.
     * @param weights An array of 4 matrices representing the weights.
     * @param biases An array of 4 matrices representing the biases.
     */
    MlpNetwork(const Matrix *weights, const Matrix *biases);

    /**
     * A method that applies the entire network on input, and returns digit struct.
     * @param m A matrix to apply onto.
     * @return digit struct.
     */
    Digit operator()(const Matrix &m) const;

private:
    const Matrix *_weights{};
    const Matrix *_biases{};
    const Dense _d1;
    const Dense _d2;
    const Dense _d3;
    const Dense _d4;

};

#endif // MLPNETWORK_H
