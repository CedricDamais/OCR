#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
//#include "binarization.h"
#include "image_processing.h"


// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint32 average = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

void surface_to_grayscale(SDL_Surface* surface)
{
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_LockSurface(surface);
    SDL_PixelFormat* format = surface->format;
    for (int i = 0; i < len; i++)
    {
        pixels[i] = pixel_to_grayscale(pixels[i],format);
    }
    SDL_UnlockSurface(surface);
}


Uint32 simple(Uint32 pixel_color, SDL_PixelFormat* format, Uint32 threshold)
{
  Uint8 r, g, b;
  SDL_GetRGB(pixel_color, format, &r, &g, &b);
  Uint32 color;
  if (r > threshold)
    color = SDL_MapRGB(format, 255, 255, 255);
  else
    color = SDL_MapRGB(format, 0, 0, 0);
  return color;
}


void simple_binarize(SDL_Surface* surface, Uint32 threshold)
{
  if (surface == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());
  Uint32* pixels = surface->pixels;
  int len = surface->w * surface->h;
  SDL_LockSurface(surface);
  SDL_PixelFormat* format = surface->format;
  for (int i = 0; i < len; i++)
    {
      pixels[i] = simple(pixels[i], format, threshold);
    }
  SDL_UnlockSurface(surface);
}


void histogram(SDL_Surface* surface, int* histo)
{
  Uint8 r, g, b;
  int len = surface->w * surface->h;
  Uint32* pixels = surface->pixels;
  SDL_PixelFormat* format = surface->format;
  SDL_LockSurface(surface);

  //creates a histogram of the grayscale of the image
  for (int i = 0; i < len; i++)
    {
      pixels[i] = pixel_to_grayscale(pixels[i],format);
      SDL_GetRGB(pixels[i],format,&r,&g,&b);
      histo[b]++;
    }
  SDL_UnlockSurface(surface);
}


void equalized(SDL_Surface* surface, int* histo)
{
  int sum = 0;
  int len = surface->w * surface->h;
  for (int i = 0; i < 256; i++)
    {
      sum += histo[i];
      histo[i] = (sum * 255) / len;
    }
  Uint32* pixels = surface->pixels;
  SDL_PixelFormat* format = surface->format;
  SDL_LockSurface(surface);
  int out;
  Uint8 r, g, b;
  for (int i = 0; i < len; i++)
    {
      SDL_GetRGB(pixels[i],format,&r,&g,&b);
      out = histo[b];
      pixels[i] = SDL_MapRGB(format, out, out, out);
    }
  SDL_UnlockSurface(surface);
  for (int i = 0; i < 256; i++)
    histo[i] = 0;
  histogram(surface, histo);
}




Uint8 Otsusmethod(SDL_Surface* surface, int* histo)
{
  int len = surface->w * surface->h;
  
  //calculating the total sum of the histogram
  unsigned long sum0 = 0, sum1 = 0;
  for (int i = 0; i < 256; i++)
    {
      sum0 += histo[i] * i;
    }

  unsigned long p0 = 0, p1 = 0;
  unsigned long mean0 = 0, mean1 = 0;
  float between = 0, maxi = 0;
  Uint8 threshold0 = 0, threshold1 = 0;
  
  for (int i = 0; i < 256; i++)
    {
      p0 += histo[i];
      p1 = len - p0;
      if (p0 > 0 && p1 > 0)
	{
	  sum1 += histo[i] * i;
	  mean0 = sum1 / p0;
	  mean1 = (sum0 - sum1) / p1;
	  between = p0 * p1 * (mean0 - mean1) * (mean0 - mean1);
	  if (between >= maxi)
	    {
	      threshold0 = i;
	      if (between > maxi)
	      threshold1 = i;
	      maxi = between;
	    }
	}
    }
  return (threshold0 + threshold1) / 2;
}
