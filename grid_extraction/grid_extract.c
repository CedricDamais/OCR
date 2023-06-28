#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "utils.h"
#include "../image_transformation/image/image_processing.h"

int vertical_check(SDL_Surface* src_surface, int src_height, int x,
                    int line_offset)
{
    int start_y = (src_height / 2) - line_offset;
    int end_y = (src_height / 2) + line_offset;
    Uint32 color;

    for (int y = start_y; y < end_y; ++y)
    {
        Uint8 r, g, b;
        color = get_pixel(src_surface, x, y);
        SDL_GetRGB(color, src_surface->format, &r, &g, &b);
        if (r < 127 && g < 127 && b < 127) // True if black / probable edge.
        {
            return 0;
        }
    }

    return 1;
}

int horizontal_check(SDL_Surface* src_surface, int src_width, int y,
                    int line_offset)
{
    int start_x = (src_width / 2) - line_offset;
    int end_x = (src_width / 2) + line_offset;
    Uint32 color;

    for (int x = start_x; x < end_x; ++x)
    {
        Uint8 r, g, b;
        color = get_pixel(src_surface, x, y);
        SDL_GetRGB(color, src_surface->format, &r, &g, &b);
        if (r < 127 && g < 127 && b < 127)
        {
            return 0;
        }
    }

    return 1;
}

int y_bottomm(SDL_Surface* src_surface)
{
    int src_height = src_surface->h;
    int src_width = src_surface->w;
    int start_y = src_height / 2;
    int line_offset = src_width / 5;

    for (int y = start_y; y < src_height - 1; ++y)
    {
        if (horizontal_check(src_surface, src_width, y, line_offset) != 0)
        {
            return y;
        }
    }

    return src_height - 1;
}

int y_topp(SDL_Surface* src_surface)
{
    int src_height = src_surface->h;
    int src_width = src_surface->w;
    int start_y = src_height / 2;
    int line_offset = src_width / 5;

    for (int y = start_y; y > 1; --y)
    {
        if (horizontal_check(src_surface, src_width, y, line_offset) != 0)
        {
            return y;
        }
    }

    return 1;
}

int x_right(SDL_Surface* src_surface)
{
    int src_height = src_surface->h;
    int src_width = src_surface->w;
    int start_x = src_width / 2;
    int line_offset = src_height / 6;

    for (int x = start_x; x < src_width - 1; ++x)
    {
        if (vertical_check(src_surface, src_height, x, line_offset) != 0)
        {
            return x;
        }
    }

    return src_width - 1;
}

int x_left(SDL_Surface* src_surface)
{
    int src_height = src_surface->h;
    int src_width = src_surface->w;
    int start_x = src_width / 2;
    int line_offset = src_height / 6;

    for (int x = start_x; x > 1; --x)
    {
        if (vertical_check(src_surface, src_height, x, line_offset) != 0)
        {
            return x;
        }
    }

    return 1;
}

int grid_x_coordinate(SDL_Surface* src_surface)
{
	int src_height = src_surface->h;
	int src_width = src_surface->w;
    int *array_x = calloc(src_width, sizeof(int));
    Uint32 color;

    printf("Grid Detection: getting amount of black pixels per x...\n");

    for (int x = 0; x < src_width; ++x)
    {
        for (int y = 0; y < src_height; ++y)
        {
            Uint8 r, g, b;
            color = get_pixel(src_surface, x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if (r < 127 && g < 127 && b < 127) // True if black / probable edge.
            {
                array_x[x]++;
            }
        }
    }

    printf("Grid Detection: x array done!\n");
    printf("Grid Detection: Retrieving maximum...\n");

    int maxi = 0;
    int x_coordinate = 0;
    int *edges_x = calloc(4, sizeof(int));

    for (int edge = 0; edge < 4; edge++)
    {
        for (int i = 0; i < src_width; ++i)
            {
                if (array_x[i] > maxi)
                {
                    maxi = array_x[i];
                    x_coordinate = i;
                }
            }

        maxi = 0;
        edges_x[edge] = x_coordinate;
        
        for (int offset = x_coordinate - 35; offset < x_coordinate + 35;
            offset++)
        {
            array_x[offset] = 0;
        }
    }
    x_coordinate = INT_MAX;

    for (int j = 0; j < 4; ++j)
    {
        if (edges_x[j] < x_coordinate)
        {
            x_coordinate = edges_x[j];
        }
    }

    printf("Grid Detection: found probable edge at x =  %i\n", x_coordinate);

    return x_coordinate;
}

int are_neighbors_empty(SDL_Surface* src_surface, int src_width,
    int x_coordinate, int y_coordinate)
{
    Uint32 color;

    // Limit left coordinate.
    int left = x_coordinate - 100;

    if (left < 1)
    {
        left = 1;
    }

    // Limit right coordinate.
    int right = x_coordinate + 100;

    if (right > src_width - 1)
    {
        right = src_width - 1;
    }


    for (int i = left; i < right; ++i)
    {
        Uint8 r, g, b;
        color = get_pixel(src_surface, i, y_coordinate);
        SDL_GetRGB(color, src_surface->format, &r, &g, &b);
        if (r < 127 && g < 127 && b < 127)
        {
            return 0;
        }
    }

    return 1;
}

int grid_y_top_coordinate(SDL_Surface* src_surface, int x_coordinate)
{
    int src_height = src_surface->h;
    int src_width = src_surface->w;
    int start = src_height / 2;

    // Top y coordinate.
    int y_top = start;
    while (y_top > 1 
        && are_neighbors_empty(src_surface, src_width, x_coordinate, y_top)
        != 1)
    {
        y_top--;
    }

    printf("Grid Detection: found y top at: %i\n", y_top);

    return y_top;

}

int grid_y_bottom_coordinate(SDL_Surface* src_surface, int x_coordinate)
{
    int src_height = src_surface->h;
    int src_width = src_surface->w;
    int start = src_height / 2;

    // Bottom y coordinate.
    int y_bottom = start;
    while (y_bottom < src_height - 1
        && are_neighbors_empty(src_surface, src_width, x_coordinate, y_bottom)
        != 1)
    {
        y_bottom++;
    }

    printf("Grid Detection: found y bottom at: %i\n", y_bottom);

    return y_bottom;
}

int grid_crop(const char* path)
{
    SDL_Surface* src_surface = load_image(path);
    int left_x = x_left(src_surface);
    int right_x = x_right(src_surface);
    int top_y = y_topp(src_surface);
    int bottom_y = y_bottomm(src_surface);

    // int max_width = max(right_x - left_x, bottom_y - top_y);

    if(right_x - left_x == 0 || bottom_y - top_y == 0)
    {
        return 0;
    }

    crop_image(src_surface, left_x, top_y, right_x - left_x, bottom_y - top_y);

    return 1;
}

int extract_grid(SDL_Surface* src_surface, int x_coordinate,
    int y_top, int y_bottom)
{
    int side = y_bottom - y_top + 15;

    crop_image(src_surface, x_coordinate, y_top - 15, side, side);

    return 1;
}