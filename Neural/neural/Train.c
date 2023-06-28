#include <stdio.h>
#include <err.h>
#include <string.h>
#include "./MathTools/MathTools.h"
#include "./Matrix_tools/Matrix.h"
#include "./Layers/Layer.h"
#include "Neural.h"
//#include "Persist.h"

//instanciate a neural network
Neural* network;

//the input matrix
double training_inputs[NUM_TRAINING_SETS][NUM_INPUTS] = {
	{0.0f,0.0f},
	{1.0f,0.0f},
	{0.0f,1.0f},
	{1.0f,1.0f}
};

//the target matrix
double training_outputs[NUM_TRAINING_SETS][NUM_OUTPUTS] = {
	{0.0f},
	{1.0f},
	{1.0f},
	{0.0f}
};

//training_inputs to Matrix
// Matrix* training_inputs_to_matrix()
// {
// 	Matrix* m = newMatrix(NUM_TRAINING_SETS, NUM_INPUTS);
// 	for (int i = 0; i < NUM_TRAINING_SETS; i++)
// 	{
// 		for (int j = 0; j < NUM_INPUTS; j++)
// 		{
// 			m->data[i][j] = training_inputs[i][j];
// 		}
// 	}
// 	return m;
// }

// Matrix* training_outputs_to_matrix()
// {
// 	Matrix* m = newMatrix(NUM_TRAINING_SETS, NUM_OUTPUTS);
// 	for (int i = 0; i < NUM_TRAINING_SETS; i++)
// 	{
// 		for (int j = 0; j < NUM_OUTPUTS; j++)
// 		{
// 			m->data[i][j] = training_outputs[i][j];
// 		}
// 	}
// 	return m;
// }

// training_inputs1 = training_inputs_to_matrix();
// training_outputs1 = training_outputs_to_matrix();


//Function that suffles randomly the order of the elements in arr of length n
void shuffle(int* arr, size_t n)
{
	if(n > 1)
	{
		size_t i;
		for(i = 0; i < n - 1; i++)
		{
			size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
			int t = arr[j];
			arr[j] = arr[i];
			arr[i] = t;
		}
	}
}

const double lr = 1/60000;
int tryNum = 10000;

//train the neural network taking in parameter the number of training sets and the learning rate
void train(int numTrainingSets, double learningRate, int epochs)
{
	printMatrix(network->hiddenLayers[0]->weights);
	printMatrix(network->hiddenLayers[0]->biases);
	printMatrix(network->outputLayer->weights);
	printMatrix(network->outputLayer->biases);
	printf("Training the neural network...\n");
	int* arr = malloc(sizeof(int) * numTrainingSets);
	for (int i = 0; i < numTrainingSets; i++)
	{
		arr[i] = i;
	}
	for (int i = 0; i < epochs; i++)
	{
		shuffle(arr, numTrainingSets);
		for (int j = 0; j < numTrainingSets; j++)
		{
			Matrix* input = newMatrix(NUM_INPUTS, 1);
			Matrix* target = newMatrix(NUM_OUTPUTS, 1);
			for (int k = 0; k < NUM_INPUTS; k++)
			{
				input->data[k][0] = training_inputs[arr[j]][k];
			}
			for (int k = 0; k < NUM_OUTPUTS; k++)
			{
				target->data[k][0] = training_outputs[arr[j]][k];
			}
			feedforwardNeural(network, input);
			backpropagationNeural(network, input, target);
			updateWeightsNeural(network, learningRate);
			updateBiasesNeural(network, learningRate);
			freeMatrix(input);
			freeMatrix(target);
		}
	}
	free(arr);
}

//test the neural network
double NeuralXor(double x, double y)
{
	if((x != 1.0 && x!= 0.0) || (y != 0.0 && y != 1.0))
		errx(EXIT_FAILURE, "Invalid inputs for x or y or both");
		
	printf("Trained Neural Network version of XOR gives:\n");
	Matrix* input = newMatrix(1, NUM_INPUTS);
	input->data[0][0] = x;
	input->data[0][1] = y;
	feedforwardNeural(network, input);
	printf("%f NXOR %f = %f\n",x,y,network->outputLayer->output[0].data[0][0]);
	return network->outputLayer->output[0].data[0][0];
}



int main(int argc, const char* argv[])
{
	// if (argc == 1)
	// {
	// 	//ClearNeural();
	// 	//LoadNeural();
	// 	NeuralXor(0.0,0.0);
	// 	NeuralXor(1.0,0.0);
	// 	NeuralXor(0.0,1.0);
	// 	NeuralXor(1.0,1.0);
	// 	return EXIT_SUCCESS;
	// }
	printf("Training Neural Network version of XOR\n");
	network = newNeural(NUM_INPUTS, NUM_HIDDEN_NODES, NUM_OUTPUTS, NUM_HIDDEN_LAYERS);

	if (argc < 3) errx(EXIT_FAILURE, "Wrong number of arguments");

	double learning_rate = atof(argv[1]);
	int attempts = atoi(argv[2]);

	if(learning_rate == 0.0f || attempts < 5000) errx(EXIT_FAILURE, "wrong arguments");

	train(NUM_TRAINING_SETS, learning_rate, attempts);

	NeuralXor(0.0,0.0);
	NeuralXor(1.0,0.0);
	NeuralXor(0.0,1.0);
	NeuralXor(1.0,1.0);
	
	deleteNeural(network);
	return EXIT_SUCCESS;
}
