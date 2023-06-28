#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef BINARIZATION_H
#define BINARIZATION_H


Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_grayscale(SDL_Surface* surface);
Uint32 simple(Uint32 pixel_color, SDL_PixelFormat* format, Uint32 threshold);
void simple_binarize(SDL_Surface* surface, Uint32 threshold);
void histogram(SDL_Surface* surface, int* histo);
void equalized(SDL_Surface* surface, int* histo);
Uint8 Otsusmethod(SDL_Surface* surface, int* histo);

#endif
