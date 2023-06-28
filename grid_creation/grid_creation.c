#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
#include "grid_creation.h"
//#include "../image_transformation/image/image_processing.h"
//#include "../sudoku_solver/solver.h"

SDL_Surface* load(const char* path)
{
  if (path == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());
  SDL_Surface* tmp = IMG_Load(path);
  SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
  SDL_FreeSurface(tmp);
  return surface;
}

void FTG(FILE* file, char* grid)
{
  size_t i = 0;
  while(i < 81)
    {
      char c = fgetc(file);
      if (c != ' ' && c != '\n')
	{
	  if (c == '.')
	    grid[i] = 0;
	  else if (c > '0' && c <= '9')
	    grid[i] = c - '0';
	  else
	    errx(1, "There is an unknown character in the given file");
	  i++;
	}
    }
}

SDL_Surface* draw(SDL_Surface* surface, char* grid)
{
  SDL_Rect num = {0, 0, BOX_SIZE, BOX_SIZE};
  SDL_Rect rect = {0, 0, WINDOW_SIZE, WINDOW_SIZE};
  SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 255, 255));
  SDL_Surface* one = load("digits/1.png");
  SDL_Surface* two = load("digits/2.png");
  SDL_Surface* three = load("digits/3.png");
  SDL_Surface* four = load("digits/4.png");
  SDL_Surface* five = load("digits/5.png");
  SDL_Surface* six = load("digits/6.png");
  SDL_Surface* seven = load("digits/7.png");
  SDL_Surface* eight = load("digits/8.png");
  SDL_Surface* nine = load("digits/9.png");
  SDL_Surface* number;
  //char* numberFile = "digits/1.png";
  //SDL_Surface* number;
  for (int i = 0; i < 9; i++)
    {
      rect.y = BOX_SIZE * i;
      for (int j = 0; j < 9; j++)
	{
	  rect.x = BOX_SIZE * j;
	  //printf("%hhi\n", grid[i * 9 + j]);
	  if (grid[i * 9 + j] == 1)
	    number = one;
	  else if (grid[i * 9 + j] == 2)
	    number = two;
	  else if (grid[i * 9 + j] == 3)
	    number = three;
	  else if (grid[i * 9 + j] == 4)
	    number = four;
	  else if (grid[i * 9 + j] == 5)
	    number = five;
	  else if (grid[i * 9 + j] == 6)
	    number = six;
	  else if (grid[i * 9 + j] == 7)
	    number = seven;
	  else if (grid[i * 9 + j] == 8)
	    number = eight;
	  else if (grid[i * 9 + j] == 9)
	    number = nine;
	  SDL_BlitSurface(number, &num, surface, &rect);
	}
    }
  rect.w = WINDOW_SIZE;
  rect.h = WINDOW_SIZE;
  rect.x = 0;
  rect.y = 0;
  for (int i = 1; i < 9; i++)
    {
      if (i % 3 == 0)
	{
	  rect.x = BOX_SIZE * i - 2;
	  rect.w = 4;
	}
      else
	{
	  rect.x = BOX_SIZE * i - 1;
	  rect.w = 2;
	}
      SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 0));
    }
  rect.w = WINDOW_SIZE;
  rect.x = 0;
  for (int i = 1; i < 9; i++)
    {
      if (i % 3 == 0)
	{
	  rect.y = BOX_SIZE * i - 2;
	  rect.h = 4;
	}
      else
	{
	  rect.y = BOX_SIZE * i - 1;
	  rect.h = 2;
	}
      SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 0));
    }
  return surface;
}

/*

int main(int argc, char** argv)
{
  if (argc > 2)
    errx(1, "Arguments invalides");
  
  SDL_Surface* surface = SDL_CreateRGBSurface(0, WINDOW_SIZE, WINDOW_SIZE, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  FILE* file = NULL;
  file = fopen(argv[1], "r");
  char* grid = malloc(81 * sizeof(char));
  FTG(file, grid);
  fclose(file);
  surface = draw(surface, grid);
  printf("leaving\n");
  IMG_SavePNG(surface, "resolved.png");
  SDL_FreeSurface(surface);
  SDL_Quit();
  
}

*/
