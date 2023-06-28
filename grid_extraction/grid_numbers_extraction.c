#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "utils.h"
#include "../image_transformation/image/image_processing.h"

int extract_numbers(SDL_Surface* src_surface, int x, int src_height,
    int interval, int id)
{
    crop_image_name(src_surface, x, 0, interval, src_height, id, 1);
    return 1;
}

int grid_numbers_extract(const char* path, int start, int end)
{
	SDL_Surface* src_surface = load_image(path);
    int src_width = src_surface->w;
	int src_height = src_surface->h;
    int interval = src_width / 9;
    printf("Grid Numbers Extraction: interval set to: %i\n", interval);
    int x = 0;

    for (int i = start; i < end; ++i)
    {
    	extract_numbers(src_surface, x, src_height, interval, i);
        x += interval;
    }

    return 1;
}

int grid_numbers_full_extract()
{
    grid_numbers_extract("01.png", 1, 10);
    grid_numbers_extract("02.png", 10, 19);
    grid_numbers_extract("03.png", 19, 28);
    grid_numbers_extract("04.png", 28, 37);
    grid_numbers_extract("05.png", 37, 46);
    grid_numbers_extract("06.png", 46, 55);
    grid_numbers_extract("07.png", 55, 64);
    grid_numbers_extract("08.png", 64, 73);
    grid_numbers_extract("09.png", 73, 82);

    return 1;
}