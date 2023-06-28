#ifndef XORNEURAL_H
#define XORNEURAL_H

#define NUM_INPUTS 2
#define NUM_HIDDEN_NODES 2
#define NUM_OUTPUTS 1
#define NUM_TRAINING_SETS 4

//We declare arrays of neurons for each layer of the neural network
double hiddenLayer[NUM_HIDDEN_NODES];
double outputLayer[NUM_OUTPUTS];

//We declare arrays containing every bias for every neuron of 
//each layers
double hiddenLayerBias[NUM_HIDDEN_NODES];
double outputLayerBias[NUM_OUTPUTS];

//We decalre matrixes of weights between the neurons of every layer
double hiddenLayerWeights[NUM_INPUTS][NUM_HIDDEN_NODES];
double outputLayerWeights[NUM_HIDDEN_NODES][NUM_OUTPUTS];

//Here we decalre the matrix of every couple of inputs we are going to feed
//our neural network
double training_inputs[NUM_TRAINING_SETS][NUM_INPUTS];

//Here we declare a matrix of the outputs we want the NN to give us
double training_outputs[NUM_TRAINING_SETS][NUM_OUTPUTS];

#endif
