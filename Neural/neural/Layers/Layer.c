#include "Layer.h"
//#include "TrainingData.h"
#include "../Matrix_tools/Matrix.h"
#include "../MathTools/MathTools.h"
#include <stdio.h>
#include <stdlib.h>

//layer constructor
Layer* newLayer(int inputSize, int outputSize)
{
    Layer* l = malloc(sizeof(Layer));
    l->weights = newMatrix(outputSize, inputSize);
    l->biases = newMatrix(outputSize, 1);
    l->output = newMatrix(outputSize, 1);
    l->error = newMatrix(outputSize, 1);
    l->delta = newMatrix(outputSize, 1);
    return l;
}

//initalize weights and biases of the layer to random values without mapMatrix
void initLayer(Layer* l)
{
    printf(" initLayer\n");
    for (int i = 0; i < l->weights->rows; i++)
    {
        for (int j = 0; j < l->weights->cols; j++)
        {
            l->weights->data[i][j] = init_weight();
        }
    }
    for (int i = 0; i < l->biases->rows; i++)
    {
        for (int j = 0; j < l->biases->cols; j++)
        {
            l->biases->data[i][j] = init_weight();
        }
    }
}

//layer destructor
void deleteLayer(Layer* l)
{
    freeMatrix(l->weights);
    freeMatrix(l->biases);
    freeMatrix(l->output);
    freeMatrix(l->error);
    freeMatrix(l->delta);
    free(l);
}

//layer feedforward
void feedforwardLayer(Layer* l, Matrix* input)
{
    //printMatrix(l->weights);
    //printMatrix(input);
    printf("feedForwardLayer\n\n");
    Matrix* z = multiplyMatrix(l->weights, input);
    
    addMatrix(z, l->biases);
    mapMatrix(z, sigmoid);
    copyMatrix(z, l->output);
    freeMatrix(z);
}

//layer backpropagation
void backpropagationLayer(Layer* l, Matrix* input, Matrix* target)
{
    Matrix* z = multiplyMatrix(l->weights, input);
    addMatrix(z, l->biases);
    mapMatrix(z, sigmoid);
    mapMatrix(z, dSigmoid);
    copyMatrix(z, l->error);
    hadamardProductMatrix(l->error, target);
    freeMatrix(z);
}

//layer update weights
void updateWeightsLayer(Layer* l, double learningRate)
{
    Matrix* deltaWeights = multiplyMatrix(l->error, l->output);
    scalarMultiplyMatrix(deltaWeights, learningRate);
    subMatrix(l->weights, deltaWeights);
    freeMatrix(deltaWeights);
}

//layer update biases
void updateBiasesLayer(Layer* l, double learningRate)
{
    Matrix* deltaBiases = copyMatrix1(l->error);
    scalarMultiplyMatrix(deltaBiases, learningRate);
    subMatrix(l->biases, deltaBiases);
    freeMatrix(deltaBiases);
}