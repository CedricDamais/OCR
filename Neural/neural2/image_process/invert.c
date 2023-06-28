#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <err.h>

//resize the image to 28x28 and saves it in the same directory with the same name
void ResizeImage(char *path)
{
    SDL_Surface *image = IMG_Load(path);
    if (image == NULL)
    {
        err(1, "Could not load image");
    }
    SDL_Surface *image_resized = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);
    SDL_BlitScaled(image, NULL, image_resized, NULL);
    char *path_to_save = malloc((strlen(path) + 5) * sizeof(char));
    strcpy(path_to_save, path);
    strcat(path_to_save, ".png");
    SDL_SaveBMP(image_resized, path_to_save);
    free(path_to_save);
    SDL_FreeSurface(image);
    SDL_FreeSurface(image_resized);
}

//get the pixel at position (x, y) in the image
Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch (bpp)
    {
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0;
    }
}

//put the pixel at position (x, y) in the image
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch (bpp)
    {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16 *)p = pixel;
        break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

//function that applies an inverse filter to an image taking in argument a path to the image and saving the result in the same directory with the same name
void InvertImage(char *path)
{
    SDL_Surface *image = IMG_Load(path);
    if (image == NULL)
    {
        err(1, "Could not load image");
    }
    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            Uint32 pixel = get_pixel(image, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;
            Uint32 new_pixel = SDL_MapRGB(image->format, r, g, b);
            put_pixel(image, i, j, new_pixel);
        }
    }
    char *path_to_save = malloc((strlen(path) + 5) * sizeof(char));
    strcpy(path_to_save, path);
    //strcat(path_to_save, ".png");
    SDL_SaveBMP(image, path_to_save);
    free(path_to_save);
    SDL_FreeSurface(image);
}

//function that applies an inverse filter to all images of a directory
void InvertDirectory(char *path)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] != '.')
            {
                char *path_to_image = malloc((strlen(path) + strlen(ent->d_name) + 2) * sizeof(char));
                strcpy(path_to_image, path);
                strcat(path_to_image, "/");
                strcat(path_to_image, ent->d_name);
                //InvertImage(path_to_image);
                InvertImage(path_to_image);
                free(path_to_image);
            }
        }
        closedir(dir);
    }
    else
    {
        err(1, "Could not open directory");
    }
}

//main function that applies an inverse filter to all images of a directory
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        err(1, "Usage: %s <path_to_directory>", argv[0]);
    }
    InvertDirectory(argv[1]);
    return 0;
}