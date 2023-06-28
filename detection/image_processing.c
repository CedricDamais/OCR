#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "processing.h"

/*

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// texture: Texture to display.
void event_loop(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Texture* other)
{
    SDL_Event event;
    SDL_Texture* text = texture;

    while (1)
    {
        // Waits for an event.
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
		return;

            // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    draw(renderer, texture);
                }
                break;
	    case SDL_KEYDOWN:
		text = text == texture ? other : texture;
		draw(renderer, text);
		break;
        }
    }
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    if (path == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* tmp = IMG_Load(path);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmp);
    return surface;
}

*/

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        argv[1] = "sudoku2.png";
        //errx(EXIT_FAILURE, "Usage: image-file");

    /*

    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Create a window.
    SDL_Window* window = SDL_CreateWindow("image", 0, 0, 640, 400,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Create a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
*/
    
    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);
    // - Resize the window according to the size of the image.
    //SDL_SetWindowSize(window, surface->w, surface->h);

    // - Create a texture from the image.
    //SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    // - Create an array to implement the histogram of the grayscale image
    // - Fill the histogram with values
    int histo[256];
    memset(histo, 0, sizeof(histo));

    //surfaceToGrayscale(surface);
    surface = gaussianBlur(surface);

    surface = sobel(surface);
    surface = dilate(surface);
    surface = dilate(surface);
    surface = erode(surface);

    SDL_Surface* new_one = SDL_ConvertSurface(surface, surface->format, surface->flags);

    // - Convert the surface into simple binarization
    histogram(new_one, histo);
    // - Equalize the histogram to enhance contrasts
    equalized(new_one, histo);
    // - Calculate the threshold of the image used to binarize
    Uint8 threshold = Otsusmethod(new_one, histo);
	simple_binarize(surface, threshold); //128 == threshold
    
    hough_transform2(surface);
    //cut(surface);

    //SDL_Texture* otherTexture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    SDL_FreeSurface(new_one);
    
    // - Dispatch the events.
    //event_loop(renderer, texture, otherTexture);

    // - Destroy the objects.
    //SDL_DestroyTexture(texture);
    //SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
