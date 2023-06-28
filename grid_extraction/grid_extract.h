#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#ifndef GRID_DETECTION_H
#define GRID_DETECTION_H

int vertical_check(SDL_Surface* src_surface, int src_height, int x,
					int line_offset);

int horizontal_check(SDL_Surface* src_surface, int src_width, int y,
					int line_offset);

int y_bottomm(SDL_Surface* src_surface);

int y_topp(SDL_Surface* src_surface);

int x_right(SDL_Surface* src_surface);

int x_left(SDL_Surface* src_surface);

// Find out first edge's x coordinate.
int grid_x_coordinate(SDL_Surface* src_surface);

// Find out if neighboring pixel on x axis are empty.
int are_neighbors_empty(SDL_Surface* src_surface, int src_width,
	int x_coordinate, int y_coordinate);

// Find out first edge's y top coordinate.
int grid_y_top_coordinate(SDL_Surface* src_surface, int x_coordinate);

// Find out first edge's y bottom coordinate.
int grid_y_bottom_coordinate(SDL_Surface* src_surface, int x_coordinate);

// Extract grid from surface.
int extract_grid(SDL_Surface* src_surface, int x_coordinate,
	int y_top, int y_bottom);

int grid_crop(const char* path);

#endif