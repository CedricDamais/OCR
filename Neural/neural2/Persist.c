#include "Persist.h"
#include "Neural.h"
#include <err.h>
#include <stdlib.h>

void SaveNeural()
{
    FILE *ptr;
    ptr = fopen("trained_neural.txt", "w");

    //saves the hiddenlayer's activations
    for(int i = 0; i < NUM_HIDDEN_NODES; i++)
    {
        //printf("%lf",hiddenLayer[i]);
        fprintf(ptr, "%lf", hiddenLayer[i]);
        fputc('|', ptr);
    }
    fputc('\n', ptr);

    //saves the outputLayer's activations
    for(int i = 0; i < NUM_OUTPUTS; i++)
    {
        fprintf(ptr,"%lf",outputLayer[i]);
        fputc('|', ptr);
    }
    fputc('\n', ptr);

    //saves the hiddenLayer's biases
    for(int i = 0; i < NUM_HIDDEN_NODES; i++)
    {
        fprintf(ptr, "%lf", hiddenLayerBias[i]);
        fputc('|', ptr);
    }
    fputc('\n', ptr);

    //saves the outputLayer's biases
    for(int i = 0; i < NUM_OUTPUTS; i++)
    {
        fprintf(ptr, "%lf", outputLayerBias[i]);
        fputc('|', ptr);
    }
    fputc('\n', ptr);

    //saves the hiddenLayer's weights
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        for(int j = 0; j < NUM_HIDDEN_NODES; j++)
        {
            fprintf(ptr, "%lf", hiddenLayerWeights[i][j]);
            fputc('|', ptr);
        }
    }
    fputc('\n', ptr);

    //saves the outputLayer's weights
    for(int i = 0; i < NUM_HIDDEN_NODES; i++)
    {
        for(int j = 0; j < NUM_OUTPUTS; j++)
        {
            fprintf(ptr, "%lf", outputLayerWeights[i][j]);
            fputc('|', ptr);
        }
    }
    fputc('\n', ptr);
    fclose(ptr);
}


//Loads the values of the Xor Neural Network from the file trained_neural.txt
void LoadNeural()
{
    LoadNeuralPath("trained_neural.txt");
}


//Loads the values of the Xor Neural Network from the file trained_neural.txt
void LoadNeuralPath(char* fileName)
{
    FILE *ptr;
    ptr = fopen(fileName, "r");
    if(ptr == NULL){ errx(EXIT_FAILURE, "file does not exist");}

    //loads the hiddenlayer's activations
    for(int i = 0; i < NUM_HIDDEN_NODES; i++)
    {
        if(fscanf(ptr, "%lf", &(hiddenLayer[i])) <= 0)
        {
            errx(EXIT_FAILURE, "no value assigned by fscanf");
            fclose(ptr);
        }
        //printf("%f",hiddenLayer[i]);
        fgetc(ptr);
    }
    fgetc(ptr);

    //loads the outputlayer's activations
    for(int i = 0; i < NUM_OUTPUTS; i++)
    {
        if(fscanf(ptr, "%lf", &(outputLayer[i])) <= 0)
        {
            errx(EXIT_FAILURE, "no value assigned by fscanf");
            fclose(ptr);
        }
        //printf("%f",outputLayer[i]);
        fgetc(ptr);
    }
    fgetc(ptr);

    //loads the hidden layer's biases
    for(int i = 0; i < NUM_HIDDEN_NODES; i++)
    {
        if(fscanf(ptr, "%lf", &(hiddenLayerBias[i])) <= 0)
        {
            errx(EXIT_FAILURE, "no value assigned by fscanf");
            fclose(ptr);
        }
        //printf("%f",hiddenLayerBias[i]);
        fgetc(ptr);
    }
    fgetc(ptr);

    //loads the output layer's biases
    for(int i = 0; i < NUM_OUTPUTS; i++)
    {
        if(fscanf(ptr, "%lf", &(outputLayerBias[i])) <= 0)
        {
            errx(EXIT_FAILURE, "no value assigned by fscanf");
            fclose(ptr);
        }
        //printf("%f",outputLayerBias[i]);
        fgetc(ptr);
    }
    fgetc(ptr);

    //loads the hidden layer's weights
    for(int i = 0; i < NUM_INPUTS; i++)
    {
        for(int j = 0; j < NUM_HIDDEN_NODES; j++)
        {
            if(fscanf(ptr, "%lf", &(hiddenLayerWeights[i][j])) <= 0)
            {
                errx(EXIT_FAILURE, "no value assigned by fscanf");
                fclose(ptr);
            }
            //printf("%f",outputLayerBias[i]);
            fgetc(ptr);
        }
    }
    fgetc(ptr);

    //loads the output layer's weights
    for(int i = 0; i < NUM_HIDDEN_NODES; i++)
    {
        for(int j = 0; j < NUM_OUTPUTS; j++)
        {
            if(fscanf(ptr, "%lf", &(outputLayerWeights[i][j])) <= 0)
            {
                errx(EXIT_FAILURE, "no value assigned by fscanf");
                fclose(ptr);
            }
            //printf("%f",outputLayerBias[i]);
            fgetc(ptr);
        }
    }
    fgetc(ptr);

    printf("\nValues loaded from %s\n", fileName);
    fclose(ptr);
}