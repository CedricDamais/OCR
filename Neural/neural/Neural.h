#ifndef NEURAL_H
#define NEURAL_H
#include "Layers/Layer.h"
#include "MathTools/MathTools.h"
#define NUM_INPUTS 2
#define NUM_HIDDEN_NODES 2
#define NUM_OUTPUTS 1
#define NUM_TRAINING_SETS 4
#define NUM_HIDDEN_LAYERS 1

//Neural network structure
typedef struct Neural
{
    Layer** hiddenLayers;
    Layer* outputLayer;
} Neural;

//Neural network constructor
Neural* newNeural(int inputSize, int hiddenSize, int outputSize, int hiddenLayers);

//Neural network destructor
void deleteNeural(Neural* n);

//Neural network feedforward
void feedforwardNeural(Neural* n, Matrix* input);

//Neural network backpropagation
void backpropagationNeural(Neural* n, Matrix* input, Matrix* target);

//Neural network update weights
void updateWeightsNeural(Neural* n, double learningRate);

//Neural network update biases
void updateBiasesNeural(Neural* n, double learningRate);

//initialize the network with random weights and biases
void init_Network();

#endif
