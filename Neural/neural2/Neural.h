#ifndef NEURAL_H
#define NEURAL_H

#define NUM_INPUTS 784
#define NUM_HIDDEN_NODES 300
#define NUM_OUTPUTS 10
#define NUM_TRAINING_SETS 413217
#define NUM_TEST_SETS 10000

#define LEARN_ARG "learn"
#define LEARN_TEST_ARG "learnTest"
#define TEST_ARG "test"
#define APPLY_ARG "apply"

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

//Here we declare the matrix of every couple of inputs we are going to feed
//our neural network
//double training_inputs[NUM_TRAINING_SETS][NUM_INPUTS];
double** training_inputs; //training_inputs[][]


//Here we declare a matrix of the outputs we want the NN to give us
//double training_outputs[NUM_TRAINING_SETS][NUM_OUTPUTS];
double** training_outputs;

//FeedForward functions
int FeedForward(double* inputs, int train);
char FeedForwardChar(double* inputs);

#endif //NEURAL