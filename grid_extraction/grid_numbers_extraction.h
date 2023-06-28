#ifndef GRID_NUMBERS_EXTRACTION_H
#define GRID_NUMBERS_EXTRACTION_H

int extract_numbers(SDL_Surface* src_surface, int x, int src_height,
    int interval, int id);

// Numbers extraction loop.
int grid_numbers_extract(const char* path);

int grid_numbers_full_extract();

#endif