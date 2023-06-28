#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <err.h>
#include <stdint.h>

#define getByte(value, n) (value >> (n*8) & 0xFF)

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

float max(float a, float b) { return (a < b) ? a : b; };

float lerp(float s, float e, float t){return s+(e-s)*t;}

float blerp(float c00, float c10, float c01, float c11, float tx, float ty)
{
    return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
}


void scale(SDL_Surface*src_surface)
{
    SDL_Surface* dest_surface;
    int newWidth =28;
    int newHeight=28;

	dest_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, newWidth, newHeight,
		src_surface->format->BitsPerPixel, src_surface->format->Rmask,
		src_surface->format->Gmask, src_surface->format->Bmask,
		src_surface->format->Amask);

	SDL_FillRect(dest_surface,
		NULL, SDL_MapRGB(dest_surface->format, 255, 255, 255));

	if (dest_surface == NULL)
	{
		errx(EXIT_FAILURE,"%s", SDL_GetError());
	}

	SDL_LockSurface(src_surface);
	SDL_LockSurface(dest_surface);

    int x, y;
    for(x= 0, y=0; y < newHeight; x++)
    {
        if(x > newWidth)
        {
            x = 0; 
            y++;
        }
        // Image should be clamped at the edges and not scaled.
        float gx = max(x / (float)(newWidth) * (src_surface->w) - 0.5f, src_surface->w - 1);
        float gy = max(y / (float)(newHeight) * (src_surface->h) - 0.5, src_surface->h - 1);
        int gxi = (int)gx;
        int gyi = (int)gy;
        uint32_t result=0;
        uint32_t c00 = get_pixel(src_surface, gxi, gyi);
        uint32_t c10 = get_pixel(src_surface, gxi+1, gyi);
        uint32_t c01 = get_pixel(src_surface, gxi, gyi+1);
        uint32_t c11 = get_pixel(src_surface, gxi+1, gyi+1);
        uint8_t i;
        for(i = 0; i < 3; i++)
        {
            
            result |= (uint8_t)blerp(getByte(c00, i), getByte(c10, i), getByte(c01, i), getByte(c11, i), gx - gxi, gy -gyi) << (8*i);
        }
        put_pixel(dest_surface,x, y, result);
    }
    SDL_UnlockSurface(src_surface);
	SDL_UnlockSurface(dest_surface);
    *src_surface = *dest_surface;
    free(dest_surface);
}

//apply function scale to an image at path
SDL_Surface* load_image(const char* path)
{
    if (path == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* tmp = IMG_Load(path);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    scale(surface);
    SDL_FreeSurface(tmp);
    IMG_SavePNG(surface, path);
    return surface;
}

//apply function scale to all the images in the folder
void apply_scale(char* path)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                char* file_path = malloc(strlen(path) + strlen(dir->d_name) + 2);
                strcpy(file_path, path);
                strcat(file_path, "/");
                strcat(file_path, dir->d_name);
                load_image(file_path);
                free(file_path);
            }
        }
        closedir(d);
    }
}

int main(int argc, char *argv[])
{
    // if (argc != 2)
    // {
    //     printf("Usage: %s <path to folder>\n", argv[0]);
    //     return 1;
    // }
    if(argc < 2)
    {
        printf("Usage: %s <path to folder> or %s test <path to image>\n", argv[0], argv[0]);
        return 1;
    }
    if(strcmp(argv[1], "test") == 0)
    {
        load_image(argv[2]);
        return 0;
    }
    else
    {
        apply_scale(argv[1]);
        return 0;
    }
}