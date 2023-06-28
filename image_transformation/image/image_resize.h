#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#ifndef IMAGE_RESIZE_H
#define IMAGE_RESIZE_H

SDL_Surface *scale(SDL_Surface *img,int width,int height);
void scale_on(SDL_Surface*src_surface,int width,int height);

#endif