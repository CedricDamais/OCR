#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../image_transformation/image/image_processing.h"



void save_image(SDL_Surface *image_surface, char *path)
{
    printf("Image Save: saving image to %s\n", path);
    int success = IMG_SavePNG(image_surface, path);

    if (success != 0)
    {
        errx(1, "Image Save: could not save the image to '%s': %s.\n", path,
            SDL_GetError());
    }
}


int crop_image_name(SDL_Surface* src_surface, int x, int y,
    int width, int height, int id, int is_number)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(src_surface->flags, width,
        height, src_surface->format->BitsPerPixel, src_surface->format->Rmask,
        src_surface->format->Gmask, src_surface->format->Bmask,
        src_surface->format->Amask);
    SDL_Rect rect = {x, y, width, height};
    
    printf("Image Crop: cropping image...\n");
    SDL_BlitSurface(src_surface, &rect, surface, 0);
    printf("Image Crop: done!\n");

    char file_name[64] = {0};

    if (is_number)
    {
        file_name[0] = 'n';
        file_name[1] = 'u';
        file_name[2] = 'm';
        file_name[3] = 'b';
        file_name[4] = 'e';
        file_name[5] = 'r';
        file_name[6] = 's';
        file_name[7] = '/';
    }

    char* id_str = itoa(id, 10);
    if(id<10)
    {
        strcat(file_name,"0");
    }
    strcat(file_name, id_str);
    if (is_number)
    {
        strcat(file_name, ".png");
    }
    else
    {
        strcat(file_name, ".png");
    }

    save_image(surface, file_name);

    return 1;
}


// Crops image and returns 0, saves the cropped image as "cropped_image.jpeg".
int crop_image(SDL_Surface* src_surface, int x, int y, int width, int height)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(src_surface->flags, width,
        height, src_surface->format->BitsPerPixel, src_surface->format->Rmask,
        src_surface->format->Gmask, src_surface->format->Bmask,
        src_surface->format->Amask);
    SDL_Rect rect = {x, y, width, height};
    
    printf("Image Crop: cropping image...\n");
    SDL_BlitSurface(src_surface, &rect, surface, 0);
    printf("Image Crop: done!\n");
    save_image(surface, "c_image.jpeg");

    return 1;
}