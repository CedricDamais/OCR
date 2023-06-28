#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <err.h>
#include "Neural.h"
#include "DataLoad.h"

//function that loads an image into an array of pixels reprensented by 0 and 1 in row-major order
void imageToBinary(SDL_Surface* surface, double inputs[], int blackNwhite)
{
    Uint8 r, g, b;
    Uint32* pixels = surface->pixels;
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);

    for (unsigned int i = 0; i < 28; i++)
    {
        for (unsigned int j = 0; j < 28; j++)
        {
            SDL_GetRGB(pixels[i * 28 + j],format,&r,&g,&b);
            inputs[i * 28 + j] = (r * 0.299 + g * 0.587 + b * 0.114) / 255; // grayscale then normalize
            if (blackNwhite) // Black and white
                inputs[i * 28 + j] = inputs[i * 28 + j] > 0.5 ? 1.0f : 0.0f;
        }
    }
    SDL_UnlockSurface(surface);
}

//function that stores an image as an array of pixels in an array of double of sufficient size
void LoadImage(char *path, double* format_image, int blackNwhite)
{
    SDL_Surface *surface = IMG_Load(path);
    SDL_Surface *surface1 = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
    imageToBinary(surface1, format_image, blackNwhite);
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface);
}

//function that loads all images of a directory into an array of array of double and return the index of the next free space
int LoadDirectory(char *path, double **format_images, double** label_configurations, int label, int blackNwhite)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL)
    {
        //int i = current_offset;
        int i = 0;
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] != '.')
            {
                char *path_to_image = malloc((strlen(path) + strlen(ent->d_name) + 2) * sizeof(char));
                strcpy(path_to_image, path);
                strcat(path_to_image, "/");
                strcat(path_to_image, ent->d_name);

                LoadImage(path_to_image, format_images[i], blackNwhite);

                label_configurations[i][label] = 1.0f;

                free(path_to_image);
                i++;
            }
        }
        closedir(dir);
        return i;
    }
    else
    {
        errx(1, "Could not open directory: %s", path);
    }
}

//function that loads all train images of all directories into an array of array of double and labels corresponding to the directory
void LoadTrainData(double ***format_images, double*** label_configurations, int blackNwhite)
{
    *format_images = malloc(NUM_TRAINING_SETS * sizeof(double *));
    if(*format_images == NULL)
    {
        errx(1, "Error: malloc failed in LoadAll");
    }

    for (int k = 0; k < NUM_TRAINING_SETS; k++)
    {
        (*format_images)[k] = calloc(NUM_INPUTS, sizeof(double));
        if((*format_images)[k] == NULL)
        {
            errx(1, "Error: malloc failed in LoadAll");
        }
    }

    *label_configurations = malloc(NUM_TRAINING_SETS * sizeof(double *));
    if(*label_configurations == NULL)
    {
        errx(1, "Error: malloc failed in LoadAll");
    }

    for (int k = 0; k < NUM_TRAINING_SETS; k++)
    {
        (*label_configurations)[k] = calloc(NUM_OUTPUTS, sizeof(double));
        if((*label_configurations)[k] == NULL)
        {
            errx(1, "Error: malloc failed in LoadDirectory");
        }
    }

    int i = 0;
    i += LoadDirectory(PATH_TO_DIRECTORY_OF_0, (*format_images) + i, (*label_configurations) + i, 0, blackNwhite);
    i += LoadDirectory(PATH_TO_DIRECTORY_OF_1, (*format_images) + i, (*label_configurations) + i, 1, blackNwhite);
    i += LoadDirectory(PATH_TO_DIRECTORY_OF_2, (*format_images) + i, (*label_configurations) + i, 2, blackNwhite);
    i += LoadDirectory(PATH_TO_DIRECTORY_OF_3, (*format_images) + i, (*label_configurations) + i, 3, blackNwhite);
    i += LoadDirectory(PATH_TO_DIRECTORY_OF_4, (*format_images) + i, (*label_configurations) + i, 4, blackNwhite);
    i += LoadDirectory(PATH_TO_DIRECTORY_OF_5, (*format_images) + i, (*label_configurations) + i, 5, blackNwhite);
    i += LoadDirectory(PATH_TO_DIRECTORY_OF_6, (*format_images) + i, (*label_configurations) + i, 6, blackNwhite);
    i += LoadDirectory(PATH_TO_DIRECTORY_OF_7, (*format_images) + i, (*label_configurations) + i, 7, blackNwhite);
    i += LoadDirectory(PATH_TO_DIRECTORY_OF_8, (*format_images) + i, (*label_configurations) + i, 8, blackNwhite);
    i += LoadDirectory(PATH_TO_DIRECTORY_OF_9, (*format_images) + i, (*label_configurations) + i, 9, blackNwhite);
}

//function that loads all test_images of all directories into an array of array of double and labels corresponding to the directory
void LoadTestData(double ***format_images, double*** label_configurations, int blackNwhite)
{
    *format_images = malloc(NUM_TEST_SETS * sizeof(double *));
    if(*format_images == NULL)
    {
        errx(1, "Error: malloc failed in LoadAllTests");
    }

    for (int k = 0; k < NUM_TEST_SETS; k++)
    {
        (*format_images)[k] = calloc(NUM_INPUTS, sizeof(double));
        if((*format_images)[k] == NULL)
        {
            errx(1, "Error: malloc failed in LoadAllTests");
        }
    }

    *label_configurations = malloc(NUM_TEST_SETS * sizeof(double *));
    if(*label_configurations == NULL)
    {
        errx(1, "Error: malloc failed in LoadAllTests");
    }

    for (int k = 0; k < NUM_TEST_SETS; k++)
    {
        (*label_configurations)[k] = calloc(NUM_OUTPUTS, sizeof(double));
        if((*label_configurations)[k] == NULL)
        {
            errx(1, "Error: malloc failed in LoadDirectory");
        }
    }

    int i = 0;
    i += LoadDirectory(PATH_TO_TEST_DIRECTORY_OF_0, (*format_images) + i, (*label_configurations) + i, 0, blackNwhite);
    i += LoadDirectory(PATH_TO_TEST_DIRECTORY_OF_1, (*format_images) + i, (*label_configurations) + i, 1, blackNwhite);
    i += LoadDirectory(PATH_TO_TEST_DIRECTORY_OF_2, (*format_images) + i, (*label_configurations) + i, 2, blackNwhite);
    i += LoadDirectory(PATH_TO_TEST_DIRECTORY_OF_3, (*format_images) + i, (*label_configurations) + i, 3, blackNwhite);
    i += LoadDirectory(PATH_TO_TEST_DIRECTORY_OF_4, (*format_images) + i, (*label_configurations) + i, 4, blackNwhite);
    i += LoadDirectory(PATH_TO_TEST_DIRECTORY_OF_5, (*format_images) + i, (*label_configurations) + i, 5, blackNwhite);
    i += LoadDirectory(PATH_TO_TEST_DIRECTORY_OF_6, (*format_images) + i, (*label_configurations) + i, 6, blackNwhite);
    i += LoadDirectory(PATH_TO_TEST_DIRECTORY_OF_7, (*format_images) + i, (*label_configurations) + i, 7, blackNwhite);
    i += LoadDirectory(PATH_TO_TEST_DIRECTORY_OF_8, (*format_images) + i, (*label_configurations) + i, 8, blackNwhite);
    i += LoadDirectory(PATH_TO_TEST_DIRECTORY_OF_9, (*format_images) + i, (*label_configurations) + i, 9, blackNwhite);
}