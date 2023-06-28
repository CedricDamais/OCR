#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef UTILS_H
#define UTILS_H

void save_image(SDL_Surface *image_surface, char *path);

int crop_image_name(SDL_Surface* src_surface, int x, int y,
    int width, int height, int id, int is_number);

int crop_image(SDL_Surface* src_surface, int x, int y, int width, int height);


#endif