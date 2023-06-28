#include "TrainingData.h"

TrainingData* newTrainingData(double* image, int* label)
{
    TrainingData* td = malloc(sizeof(TrainingData));
    td->image = image;
    td->label = label;
    return td;
}

void deleteTrainingData(TrainingData* td)
{
    free(td->image);
    free(td->label);
    free(td);
}

// Path: Neural\neural\TrainingData\TrainingData.c