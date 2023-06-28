#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef PROCESSING_H
#define PROCESSING_H

void putPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
Uint32 getPixel(SDL_Surface* surface, int x, int y);
SDL_Surface* gaussianBlur(SDL_Surface* surface);
float truncate(float x);
SDL_Surface* sobel(SDL_Surface* surface);
SDL_Surface* erode(SDL_Surface* surface); 
SDL_Surface* dilate(SDL_Surface* surface);
double degrees_to_rad2(double degrees);
void hough_transform2(SDL_Surface* surface);
void cut(SDL_Surface* surface);
void largestConnectedComponent(SDL_Surface* surface);

#endif
