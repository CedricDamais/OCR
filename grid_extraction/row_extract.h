#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef ROW_EXTRACT_H
#define ROW_EXTRACT_H

// Extract row from coordinates.
int extract_row(SDL_Surface* src_surface, int y, int src_width,
	int interval, int id);

// Extract all rows.
int grid_row_extract(const char* path);

#endif