# Neural Network Library

This directory contains a collection of C++ files that together form a neural network library for building and training multilayer perceptron (MLP) networks.

## Files

- `Activation.cpp`: Source code for implementing various activation functions used in the neural network.
- `Activation.h`: Header file containing declarations of activation function classes and their methods.
- `Dense.cpp`: Source code for implementing the dense (fully connected) layer of the neural network.
- `Dense.h`: Header file containing declarations of the dense layer class and its methods.
- `Makefile`: Makefile for compiling the neural network library.
- `Matrix.cpp`: Source code for implementing matrix operations used in the neural network.
- `Matrix.h`: Header file containing declarations of matrix-related classes and methods.
- `MlpNetwork.cpp`: Source code for implementing the multilayer perceptron network itself.
- `MlpNetwork.h`: Header file containing declarations of the MLP network class and its methods.

## Usage

1. Compile the library using the provided `Makefile`:
   ```sh
   make
   ```
2. Link the compiled library with your application code when building your neural network projects.
3. Include relevant header files in your source code files as needed:
   ```cpp
   #include "Activation.h"
   #include "Dense.h"
   #include "Matrix.h"
   #include "MlpNetwork.h"
   ```
4. Create and configure instances of activation functions, dense layers, matrices, and MLP networks according to your needs.
5.Train and evaluate your neural networks using the implemented library components.
