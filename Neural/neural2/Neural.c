#include <stdio.h>
#include <err.h>
#include <string.h>
#include "MathTools.h"
#include "Neural.h"
#include "Persist.h"
#include "DataLoad.h"


//will contain 10000 images in array format of 784 pixels
double** test_inputs;
double** test_outputs;

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
	for(int i = 0; i < NUM_HIDDEN_NODES; i++)
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

char FeedForwardChar(double* inputs)
{
	return  FeedForward(inputs, 0) + '0';
}

//FeedForward phase of the network
int FeedForward(double* inputs, int train)
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

	//find the index of the highest activation in the output layer
	if(train == 0)
	{
		int maxIndex = 0;
		for(int i = 0; i < NUM_OUTPUTS; i++)
		{
			if(outputLayer[i] > outputLayer[maxIndex])
			{
				maxIndex = i;
			}
		}

		return maxIndex;
	}
	return 0;
}

double TestNetwork()
{
	int correct = 0;
	for(int i = 0; i < NUM_TEST_SETS; i++)
	{
		int result = FeedForward(test_inputs[i], 0);
		if(test_outputs[i][result] > 0.7f)
		{
			correct++;
		}
	}

	return (double)(((double)correct / (double)NUM_TEST_SETS) * 100.0f);
}

const double lr = 0.1f;
int tryNum = 10000;

void train(double lr, int number_of_attempts)
{
	init_random();
	//initialize randomly every weights and bias of the neural network
	for(int i = 0; i < NUM_HIDDEN_NODES; i++)
	{
		hiddenLayer[i] = 0;
	}

	for(int i = 0; i < NUM_INPUTS; i++)
	{
		for(int j = 0; j < NUM_HIDDEN_NODES; j++)
		{
			hiddenLayerWeights[i][j] = init_weight(NUM_HIDDEN_NODES);
		}
	}
	for(int i = 0; i < NUM_HIDDEN_NODES; i++)
	{
		for(int j = 0; j < NUM_OUTPUTS; j++)
		{
			outputLayerWeights[i][j] = init_weight(NUM_OUTPUTS);
		}
	}

	for(int i = 0; i < NUM_HIDDEN_NODES; i++)
	{
		hiddenLayerBias[i] = init_bias();
	}

	for(int i = 0; i < NUM_OUTPUTS; i++)
	{
		outputLayerBias[i] = init_bias();
	}

	//we want to iterate through all the training set for tryNum times
	for(int n = 0; n < number_of_attempts; n++)
	{
		//we shuffle the order of the training set
		int trainingSetOrder[NUM_TRAINING_SETS];
		for(int i = 0; i < NUM_TRAINING_SETS; i++)
		{
			trainingSetOrder[i] = i;
		}
		shuffle(trainingSetOrder, NUM_TRAINING_SETS);

		//Cycle through every elements of the training set
		for(int x = 0; x < NUM_TRAINING_SETS; x++)
		{
			int i = trainingSetOrder[x];

			//Feed Forward
			FeedForward(training_inputs[i], 1); 

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

void FreeTrainData()
{
	for (int i = 0; i < NUM_TRAINING_SETS; i++)
    {
        free(training_inputs[i]);
    }

	free(training_inputs);
	
	for (int i = 0; i < NUM_TRAINING_SETS; i++)
    {
        free(training_outputs[i]);
    }

	free(training_outputs);
}

void FreeTestData()
{
	for (int i = 0; i < NUM_TEST_SETS; i++)
    {
        free(test_inputs[i]);
    }

	free(test_inputs);
	
	for (int i = 0; i < NUM_TEST_SETS; i++)
    {
        free(test_outputs[i]);
    }

	free(test_outputs);
}
// main {learn|test|learnTest|apply} [learning_rate] [attempts] [blackNwhite] 
// main learn {learning_rate} {attempts} {blackNwhite}
// main test {blackNwhite}
// main learnTest {learning_rate} {attempts} {blackNwhite}
// main apply {path} {blackNwhite}
/*
int main(int argc, const char* argv[])
{
	if (argc < 2) 
		errx(EXIT_FAILURE, "Wrong minmum number of arguments... use train, test, trainNtest or apply");

	int learn = 0;
	int test = 0;
	int apply = 0;

	double learning_rate = 0.3;
	int attempts = 2;
	int blackNwhite = 1;

	char numberFileName[1000];

	if (strcoll(argv[1], LEARN_ARG) == 0 || strcoll(argv[1], LEARN_TEST_ARG) == 0) 
	{
		if (argc != 5) 
			errx(EXIT_FAILURE, "Wrong number of arguments for train or trainTest... add learning rate, attempts and [back and white] parameters");

		learn = 1;
		learning_rate = atof(argv[2]);
		attempts = atoi(argv[3]);
		blackNwhite = atoi(argv[4]);
		test = strcoll(argv[1], LEARN_TEST_ARG) == 0;
	}
	else if (strcoll(argv[1], TEST_ARG) == 0) 
	{
		if (argc != 3) 
			errx(EXIT_FAILURE, "Wrong number of arguments for test... add [back and white] parameter");

		test = 1;
		blackNwhite = atoi(argv[2]);
	}
	else if (strcoll(argv[1], APPLY_ARG) == 0)
	{
		if (argc != 3) 
			errx(EXIT_FAILURE, "Wrong number of arguments for test... add file path parameter");

		apply = 1;
		strcpy(numberFileName, argv[2]);
	}

	if (learn) 
	{
		//Loads the training set into the training_inputs and training_outputs arrays
		LoadTrainData(&training_inputs, &training_outputs, blackNwhite);

		// Train
		train(learning_rate, attempts);

		// Free data structures
		FreeTrainData();
	}
	else 
	{
		// Load the last saved neural
		LoadNeural();		
	}

	if (test) 
	{
		// Loads the test set into test_inputs and test_outputs
		LoadTestData(&test_inputs, &test_outputs, blackNwhite);

		// Test
		double result = TestNetwork();
		
		// Free test structures
		FreeTestData();

		// Print success rate
		printf("The neural network has a success rate of %f%%\n", result);
	}

	if (apply)
	{
		// Load the image
		double* format_Image = calloc(NUM_INPUTS, sizeof(double));
		LoadImage(numberFileName, format_Image, blackNwhite);
		
		// Detect the value
		char detected = FeedForwardChar(format_Image);
		
		// Free the image
		free(format_Image);
		
		// Print the detected value
		printf("The neural network has detected a %c\n", detected);
	}
	
	return EXIT_SUCCESS;
}
*/

