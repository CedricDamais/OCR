#ifndef TRAINING_DATA_H
#define TRAINING_DATA_H
//typedef struct that contains an array double that refers to an image and a pointer to int that refers to a label
typedef struct TrainingData
{
    double* image;
    int* label;
} TrainingData;

//Training data constructor
TrainingData* newTrainingData(double* image, int* label);

//Training data destructor
void deleteTrainingData(TrainingData* td);

#endif