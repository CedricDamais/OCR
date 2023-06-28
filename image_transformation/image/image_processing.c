#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "image_processing.h"
#include <string.h>
#include <stddef.h>




SDL_Surface* load_image(const char* path)
{
    if (path == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* tmp = IMG_Load(path);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmp);
    return surface;
}



SDL_Surface* binarize_and_rotate(char** argv)
{
  // - Create a surface from the colored image.
  SDL_Surface* surface = load_image(argv[1]);
  SDL_Surface* new_one = load_image(argv[1]);
  
  // - Create an array to implement the histogram of the grayscale image
  int histo[256];
  for (int i = 0; i < 256; i++)
    histo[i] = 0;
  
  // - Fill the histogram with values
  histogram(new_one, histo);
  
  // - Equalize the histogram to enhance contrasts
  equalized(new_one, histo);
  
  // - Calculate the threshold of the image used to binarize
  Uint8 threshold = Otsusmethod(new_one, histo);
  //printf("%u\n", threshold);
  
  // - Convert the surface into simple binarization
  surface_to_grayscale(surface);
  simple_binarize(surface, threshold); //128 == threshold
  auto_rotate(surface);
  SDL_FreeSurface(new_one);
  return surface;
}

char* itoa(int val, int base)
{
    static char buf[32] = {0};
    int i = 30;

    for(; val && i ; --i, val /= base)
    {
        buf[i] = "0123456789abcdef"[val % base];
    }

    return &buf[i+1];
}

// Invert black and white of an image using path.
int invert_black_white(char* path)
{

    SDL_Surface* src_surface = load_image(path);
    int src_width = src_surface->w;
    int src_height = src_surface->h;
    Uint32 color;
    Uint32 white = SDL_MapRGB(src_surface->format, 255, 255, 255);
    Uint32 black = SDL_MapRGB(src_surface->format, 0, 0, 0);

    for (int y = 0; y < src_height; ++y)
    {
        for (int x = 0; x < src_width; ++x)
        {
            Uint8 r, g, b;
            color = get_pixel(src_surface, x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if (r < 5 && g < 5 && b < 5)
            {
                put_pixel(src_surface, x, y, white);
            }
            else
            {
                put_pixel(src_surface, x, y, black);
            }
        }
    }

    IMG_SavePNG(src_surface, path);

    return 1;
}

// Invert black and white color using id of an image.
void invert_black_white_id(int id)
{
    char file_name[64] = {0};

    int is_number = 1;

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
        strcat(file_name, "0");
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

    invert_black_white(file_name);
}

// Invert black and white colors of image with 1 <= id < 89.
int invert_black_white_all()
{
    printf("Color Invertion: starting color invertion of all images...\n");

    for (int id = 1; id < 82; ++id)
    {
        
        invert_black_white_id(id);
    }

    printf("Color Invertion: done!\n");

    return 1;
}


void aux_magic_wand(SDL_Surface* src_surface, int x, int y)
{
    Uint32 black = SDL_MapRGB(src_surface->format, 0, 0, 0);
    Uint32 color = get_pixel(src_surface, x, y);
    Uint8 r, g,b;
    SDL_GetRGB(color, src_surface->format, &r, &g, &b);

    if((r == 255 && g == 255 && b == 255)) // True if Green / probable noise.
    {
        SDL_LockSurface(src_surface);
        put_pixel(src_surface, x, y, black);
        SDL_UnlockSurface(src_surface);
        if (x - 1 >= 0)
        {
            aux_magic_wand(src_surface, x - 1, y);
        }

        if (x + 1 < src_surface->w)
        {
            aux_magic_wand(src_surface, x + 1, y);
        }

        if (y - 1 >= 0)
        {
            aux_magic_wand(src_surface, x, y - 1);
        }

        if (y + 1 < src_surface->h)
        {
            aux_magic_wand(src_surface, x, y + 1);
        }
    }
}

int magic_wand(char* path)
{
    SDL_Surface* src_surface = load_image(path);
    int src_width = src_surface->w;
    int src_height = src_surface->h;
    Uint32 color;

    for (int x = 0; x < src_width; ++x)
    {
        for (int y = 0; y < 6; ++y)
        {
            Uint8 r, g, b;
            color = get_pixel(src_surface, x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if ((r == 255 && g == 255 && b == 255)) // True if black / probable edge.
            {
                aux_magic_wand(src_surface, x, 0);
            }
        }
    }

    for (int x = 0; x < src_width; ++x)
    {
        for (int y = 1; y < 7; ++y)
        {
            Uint8 r, g, b;
            color = get_pixel(src_surface, x, src_height-y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if ((r == 255 && g == 255 && b == 255)) // True if black / probable edge.
            {
                aux_magic_wand(src_surface, x, src_height-y);
            }
        }
    }

    for (int y = 0; y < src_height; ++y)
    {
        for (int x = 0; x < 6; ++x)
        {
            Uint8 r, g, b;
            color = get_pixel(src_surface, x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if ((r == 255 && g == 255 && b == 255)) // True if black / probable edge.
            {
                aux_magic_wand(src_surface, 0, y);
            }
        }
    }

    for (int y = 0; y < src_height; ++y)
    {
        for (int x = 1; x < 7; ++x)
        {
            Uint8 r, g, b;
            color = get_pixel(src_surface, src_width-x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if ((r == 255 && g == 255 && b == 255)) // True if black / probable edge.
            {
                aux_magic_wand(src_surface, src_width-x, y);
            }
        }
    }
    IMG_SavePNG(src_surface,path);
    return 1;
}

void magic_wand_id(int id)
{
    char file_name[64] = {0};

    int is_number = 1;

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

    if(id<10)
    {
       strcat(file_name, "0");
    }

    char* id_str = itoa(id, 10);
    
    strcat(file_name, id_str);
    if (is_number)
    {
        strcat(file_name, ".png");
        printf("file applied was %s\n",file_name);
    }
    else
    {
        strcat(file_name, ".png");
    }
    magic_wand(file_name);
}


int magic_wand_all()
{
    printf("Magic Wand: starting magic wand process of all images...\n");

    for (int id = 1; id < 82; ++id)
    {
        magic_wand_id(id);
    }

    printf("Magic Wand: done!\n");

    return 1;
}





/*int main(int argc, char** argv)
{
  if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

  SDL_Surface* surface = binarize_and_rotate(argv);
  IMG_SavePNG(surface, "binarized_and_rotated.png");
  SDL_FreeSurface(surface);
  SDL_Quit();
  return 1;
}
*/


