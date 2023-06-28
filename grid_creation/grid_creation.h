#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_SIZE 1152
#define BOX_SIZE (WINDOW_SIZE / 9)

#ifndef GRID_CREATION_H
#define GRID_CREATION_H

SDL_Surface* load(const char* path);
void FTG(FILE* file, char* grid);
SDL_Surface* draw(SDL_Surface* surface, char* grid);

#endif
