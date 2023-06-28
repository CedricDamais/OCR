#ifndef LAYER_H
#define LAYER_H
#include "../Matrix_tools/Matrix.h"

//Layer structure
typedef struct Layer
{
    Matrix* weights;
    Matrix* biases;
    Matrix* output;
    Matrix* error;
    Matrix* delta;
} Layer;

//Layer constructor
Layer* newLayer(int inputSize, int outputSize);

//initalize weights and biases of the layer to random values
void initLayer(Layer* l);

//Layer destructor
void deleteLayer(Layer* l);

//Layer feedforward
void feedforwardLayer(Layer* l, Matrix* input);

//Layer backpropagation
void backpropagationLayer(Layer* l, Matrix* input, Matrix* target);

//Layer update weights
void updateWeightsLayer(Layer* l, double learningRate);

//Layer update biases
void updateBiasesLayer(Layer* l, double learningRate);

#endif