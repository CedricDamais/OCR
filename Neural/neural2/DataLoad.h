#ifndef DATA_LOAD_H
#define DATA_LOAD_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <err.h>

#define PATH_TO_DIRECTORY_OF_0 "data/0"
#define PATH_TO_DIRECTORY_OF_1 "data/1"
#define PATH_TO_DIRECTORY_OF_2 "data/2"
#define PATH_TO_DIRECTORY_OF_3 "data/3"
#define PATH_TO_DIRECTORY_OF_4 "data/4"
#define PATH_TO_DIRECTORY_OF_5 "data/5"
#define PATH_TO_DIRECTORY_OF_6 "data/6"
#define PATH_TO_DIRECTORY_OF_7 "data/7"
#define PATH_TO_DIRECTORY_OF_8 "data/8"
#define PATH_TO_DIRECTORY_OF_9 "data/9"

#define PATH_TO_TEST_DIRECTORY_OF_0 "test/0"
#define PATH_TO_TEST_DIRECTORY_OF_1 "test/1"
#define PATH_TO_TEST_DIRECTORY_OF_2 "test/2"
#define PATH_TO_TEST_DIRECTORY_OF_3 "test/3"
#define PATH_TO_TEST_DIRECTORY_OF_4 "test/4"
#define PATH_TO_TEST_DIRECTORY_OF_5 "test/5"
#define PATH_TO_TEST_DIRECTORY_OF_6 "test/6"
#define PATH_TO_TEST_DIRECTORY_OF_7 "test/7"
#define PATH_TO_TEST_DIRECTORY_OF_8 "test/8"
#define PATH_TO_TEST_DIRECTORY_OF_9 "test/9"

// #define NUMBER_OF_TRAINING_0 980
// #define NUMBER_OF_TRAINING_1 1135
// #define NUMBER_OF_TRAINING_2 1032
// #define NUMBER_OF_TRAINING_3 1010
// #define NUMBER_OF_TRAINING_4 982
// #define NUMBER_OF_TRAINING_5 892
// #define NUMBER_OF_TRAINING_6 958
// #define NUMBER_OF_TRAINING_7 1028
// #define NUMBER_OF_TRAINING_8 974
// #define NUMBER_OF_TRAINING_9 1009

// #define NUMBER_OF_TESTING_0  5923
// #define NUMBER_OF_TESTING_1  6742
// #define NUMBER_OF_TESTING_2  5958
// #define NUMBER_OF_TESTING_3  6131
// #define NUMBER_OF_TESTING_4  5842
// #define NUMBER_OF_TESTING_5  5421
// #define NUMBER_OF_TESTING_6  5918
// #define NUMBER_OF_TESTING_7  6265
// #define NUMBER_OF_TESTING_8  5851
// #define NUMBER_OF_TESTING_9  5949


//function that loads an image into an array of pixels reprensented by 0 and 1 in row-major order
void imageToBinary(SDL_Surface* surface, double inputs[], int blackNwhite);

//function that stores an image as an array of pixels in an array of double of sufficient size
void LoadImage(char *path, double* format_image, int blackNwhite);

//function that loads all images of a directory into an array of array of double and return the index of the next free space
int LoadDirectory(char *path, double **format_images, double** label_configurations, int label, int blackNwhite);

//function that loads all images of all directories into an array of array of double
void LoadTrainData(double*** format_images, double*** label_configurations, int blackNwhite);

//function that loads all test_images of all directories into an array of array of double
void LoadTestData(double*** format_images, double*** label_configurations, int blackNwhite);

#endif