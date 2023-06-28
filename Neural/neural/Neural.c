#include <stdio.h>
#include "Neural.h"

//#include "TrainingData.h"

//initialize all weights and biases of the neural network to random values
void initNeural(Neural* n)
{
    printf("initNeural\n");
    for (int i = 0; i < NUM_HIDDEN_LAYERS; i++)
    {
        initLayer(n->hiddenLayers[i]);
    }
    initLayer(n->outputLayer);
}

//Neural network constructor
Neural* newNeural(int inputSize, int hiddenSize, int outputSize, int hiddenLayers)
{
    printf("newNeural\n");
    Neural* n = (Neural*)malloc(sizeof(Neural));
    n->hiddenLayers = (Layer**)malloc(hiddenLayers * sizeof(Layer*));
    for (int i = 0; i < hiddenLayers; i++)
    {
        if (i == 0)
        {
            n->hiddenLayers[i] = newLayer(inputSize, hiddenSize);
        }
        else
        {
            n->hiddenLayers[i] = newLayer(hiddenSize, hiddenSize);
        }
    }
    n->outputLayer = newLayer(hiddenSize, outputSize);
    initNeural(n);
    return n;
}

//Neural network destructor
void deleteNeural(Neural* n)
{
    for (int i = 0; i < NUM_HIDDEN_LAYERS; i++)
    {
        deleteLayer(n->hiddenLayers[i]);
    }
    free(n->hiddenLayers);
    deleteLayer(n->outputLayer);
    free(n);
}

//Neural network feedforward
void feedforwardNeural(Neural* n, Matrix* input)
{
    printf("feedforwardNeural\n");
    for (int i = 0; i < NUM_HIDDEN_LAYERS; i++)
    {
        feedforwardLayer(n->hiddenLayers[i], input);
        input = n->hiddenLayers[i]->output;
    }
    feedforwardLayer(n->outputLayer, input);
}

//Neural network backpropagation
void backpropagationNeural(Neural* n, Matrix* input, Matrix* target)
{
    for (int i = NUM_HIDDEN_LAYERS - 1; i >= 0; i--)
    {
        if (i == NUM_HIDDEN_LAYERS - 1)
        {
            backpropagationLayer(n->outputLayer, input, target);
        }
        else
        {
            backpropagationLayer(n->hiddenLayers[i + 1], input, target);
        }
        input = n->hiddenLayers[i]->output;
        target = n->hiddenLayers[i]->error;
    }
    backpropagationLayer(n->hiddenLayers[0], input, target);
}

//Neural network update weights
void updateWeightsNeural(Neural* n, double learningRate)
{
    for (int i = 0; i < NUM_HIDDEN_LAYERS; i++)
    {
        updateWeightsLayer(n->hiddenLayers[i], learningRate);
    }
    updateWeightsLayer(n->outputLayer, learningRate);
}

//Neural network update biases
void updateBiasesNeural(Neural* n, double learningRate)
{
    for (int i = 0; i < NUM_HIDDEN_LAYERS; i++)
    {
        updateBiasesLayer(n->hiddenLayers[i], learningRate);
    }
    updateBiasesLayer(n->outputLayer, learningRate);
}