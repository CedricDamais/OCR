#include <stdio.h>
#include <err.h>
#include <string.h>
#include "MathTools.h"
#include "XorNeural.h"
#include "Persist.h"

double training_inputs[NUM_TRAINING_SETS][NUM_INPUTS] = {
	{0.0f,0.0f},
	{1.0f,0.0f},
	{0.0f,1.0f},
	{1.0f,1.0f}
};

double training_outputs[NUM_TRAINING_SETS][NUM_OUTPUTS] = {
	{0.0f},
	{1.0f},
	{1.0f},
	{0.0f}
};

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

//sets every bias, weight and activation of the network to 0
void ClearNeural()
{
	printf("Clearing the network... \n");
	for(int i = 0; i < NUM_INPUTS; i++)
	{
		hiddenLayer[i] = 0.0f;
	}

	for(int i = 0; i < NUM_OUTPUTS; i++)
	{
		outputLayer[i] = 0.0f;
	}

	for(int i = 0; i < NUM_INPUTS; i++)
	{
		for(int j = 0; j < NUM_HIDDEN_NODES; j++)
		{
			hiddenLayerWeights[i][j] = 0.0f;
		}
	}
	for(int i = 0; i < NUM_HIDDEN_NODES; i++)
	{
		for(int j = 0; j < NUM_OUTPUTS; j++)
		{
			outputLayerWeights[i][j] = 0.0f;
		}
	}

	for(int i = 0; i < NUM_HIDDEN_NODES; i++)
	{
		hiddenLayerBias[i] = 0.0f;
	}

	for(int i = 0; i < NUM_OUTPUTS; i++)
	{
		outputLayerBias[i] = 0.0f;
	}
}

//FeedForward phase of the network
void FeedForward(double* inputs)
{
	for(int j = 0; j < NUM_HIDDEN_NODES; j++)
	{
		double neuronActivation = hiddenLayerBias[j];
		for(int k = 0; k < NUM_INPUTS; k++)
		{
			neuronActivation += inputs[k] *
				hiddenLayerWeights[k][j];
		}
		hiddenLayer[j] = sigmoid(neuronActivation);
	}

	//compute output layer activation
	for(int j = 0; j < NUM_OUTPUTS; j++)
	{
		double neuronActivation = outputLayerBias[j];
		for(int k = 0; k < NUM_HIDDEN_NODES; k++)
		{
			neuronActivation += hiddenLayer[k] *
				outputLayerWeights[k][j];
		}
		outputLayer[j] = sigmoid(neuronActivation);
	}
}

const double lr = 0.1f;
int tryNum = 10000;

void train(double lr, int number_of_attempts, int verbose)
{
	init_random();
	//initialize randomly every weights and bias of the neural network
	for(int i = 0; i < NUM_INPUTS; i++)
	{
		hiddenLayer[i] = init_weight();
	}

	for(int i = 0; i < NUM_INPUTS; i++)
	{
		for(int j = 0; j < NUM_HIDDEN_NODES; j++)
		{
			hiddenLayerWeights[i][j] = init_weight();
		}
	}
	for(int i = 0; i < NUM_HIDDEN_NODES; i++)
	{
		for(int j = 0; j < NUM_OUTPUTS; j++)
		{
			outputLayerWeights[i][j] = init_weight();
		}
	}

	for(int i = 0; i < NUM_HIDDEN_NODES; i++)
	{
		hiddenLayerBias[i] = init_weight();
	}

	for(int i = 0; i < NUM_OUTPUTS; i++)
	{
		outputLayerBias[i] = init_weight();
	}

	//we want to iterate through all the training set for tryNum times
	for(int n = 0; n < number_of_attempts; n++)
	{
		//we shuffle the order of the training set
		int trainingSetOrder[] = {0,1,2,3};
		shuffle(trainingSetOrder, NUM_TRAINING_SETS);

		//Cycle through every elements of the training set
		for(int x = 0;x < NUM_TRAINING_SETS; x++)
		{
			int i = trainingSetOrder[x];

			//Feed Forward
			//
			//compute hidden layer activation

			FeedForward(training_inputs[i]);

			if(verbose)
			{
				printf("%f",training_inputs[i][0]);
				printf(" XOR ");
				printf("%f",training_inputs[i][1]);
				printf(" = ");
				printf("%f",outputLayer[0]);
				printf("    should be ");
				printf("%f \n",training_outputs[i][0]);
			}
			//BackPropagation

			//compute change in output weights
			double deltaOutput[NUM_OUTPUTS];
			for(int j = 0; j < NUM_OUTPUTS; j++)
			{
				double deltaError = (training_outputs[i][j] -
						outputLayer[j]);
				deltaOutput[j] = deltaError * 
					dSigmoid(outputLayer[j]);
			}

			//compute change in hidden weights
			double deltaHidden[NUM_HIDDEN_NODES];
			for(int j = 0; j < NUM_HIDDEN_NODES; j++)
			{
				double deltaError = 0.0f;
				for(int k = 0; k < NUM_OUTPUTS; k++)
				{
					deltaError += deltaOutput[k] *
						outputLayerWeights[j][k];
				}
				deltaHidden[j] = deltaError * 
					dSigmoid(hiddenLayer[j]);
			}

			//apply change in output weights
			for(int j = 0; j < NUM_OUTPUTS; j++)
			{
				outputLayerBias[j] += deltaOutput[j] * lr;
				for(int k = 0; k < NUM_HIDDEN_NODES; k++)
				{
					outputLayerWeights[k][j] += hiddenLayer[k]*
						deltaOutput[j] * lr;
				}
			}

			//apply change in hidden weights
			for(int j = 0; j < NUM_HIDDEN_NODES; j++)
			{
				hiddenLayerBias[j] += deltaHidden[j] * lr;
				for(int k = 0; k < NUM_INPUTS; k++)
				{
					hiddenLayerWeights[k][j] += 
						training_inputs[i][k] *
						deltaHidden[j] * lr;
				}
			}
		}
	}
	SaveNeural();
}

double NeuralXor(double x, double y)
{
	if((x != 1.0 && x!= 0.0) || (y != 0.0 && y != 1.0))
		errx(EXIT_FAILURE, "Invalid inputs for x or y or both");
		
	printf("Trained Neural Network version of XOR gives:\n");
	double inputs[] = {x,y};
	FeedForward(inputs);
	printf("%f NXOR %f = %f\n",x,y,outputLayer[0]);
	return outputLayer[0];
}



int main(int argc, const char* argv[])
{
	if (argc == 1)
	{
		ClearNeural();
		LoadNeural();
		NeuralXor(0.0,0.0);
		NeuralXor(1.0,0.0);
		NeuralXor(0.0,1.0);
		NeuralXor(1.0,1.0);
		return EXIT_SUCCESS;
	}
	if (argc < 3) errx(EXIT_FAILURE, "Wrong number of arguments");

	double learning_rate = atof(argv[1]);
	int attempts = atoi(argv[2]);

	if(learning_rate == 0.0f || attempts < 5000) errx(EXIT_FAILURE, "wrong arguments");

	char arr[7] = "verbose";
	if(argc == 4 && strcoll(argv[3], arr) == 0) 
	{
		train(learning_rate, attempts, 1);
	}
	else 
	{ 
		train(learning_rate, attempts, 0);
	}

	NeuralXor(0.0,0.0);
	NeuralXor(1.0,0.0);
	NeuralXor(0.0,1.0);
	NeuralXor(1.0,1.0);
	
	return EXIT_SUCCESS;
}

