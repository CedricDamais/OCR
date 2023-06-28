#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
//#include "hough_transform.h"
#include "image_processing.h"

#define _GNU_SOURCE

/* The Hough transform based image will convert (x,y) coordinate
 * Into polar coordinate (ρ,θ):
 * ρ : distance between the origin of the plot and the line
 * θ : the angle created by ρ
 * On a ρ = x*cos(θ)+y*sin(θ)
 * I will also be Using the function ceil to round the result of the conversion of basis
 * What I will be Doing is :
 *        For every X :
 *                For every Y :
 *                    Get the Color of the pixel (Get color function)
 *                    for θ from 0 to 180 :
 *                         ρ = ceil(x*cos(θ)+y*sin(θ))
 *
 *
 * The dimension of the accumulator equals the number of unknown parameters, in our case two
 * considering quantized values of r and θ in the pair (ρ, θ).
 * Since we are going to have a Two dimension array for the accumulator for the dimensions.
 * We want the accumulator first dimension to be the same size has the diagonal of the matrix
 *
 * To get this size we can just apply the pythagorean theorem :
 *                       so if we have 'd' has the diagonal
 *           d = √(image widht * image widht + image height * image_height)
 * Note that rho and theta cannot be Null or equal 0
 * We are going to build a parameter space with (ρ,0)
 * 
 *  Initializing all the values that are in my accumulator
 *
 * We know that 0 is between PI and Zero and it will be necessary to find the value of 0
 * What I will do here is Initialize two new Arrays one that will contain the of sin(0) and the other Array will contain the value of  cos(0)
 * I am doing this to facilitate the computation of ρ in terms of readabilite of the code
 */

int hough_transform(SDL_Surface* src_surface)
{
	int src_height = src_surface->h;
	int src_width = src_surface->w;
    // Biggest possible accumulator.
    int diag = sqrt(src_width*src_width + src_height*src_height);
    int (*accumulator)[diag] = calloc(diag, sizeof *accumulator);
    Uint32 color; // Needed to check if point can be considered as edge.
    int rho;
   
    for (int x = 0; x < src_width; ++x)
    {
        for (int y = 0; y < src_height; ++y)
        {
            Uint8 r, g, b;
            color = get_pixel(src_surface, x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if (r == 0 && g == 0 && b == 0)
            {
                for (int theta = 0; theta < 180; ++theta)
                {
                    rho = ceil(x * cos(degrees_to_rad(theta))
										- y * sin(degrees_to_rad(theta)));
                    accumulator[theta][rho]++;
                }
            }
        }
    }
   
    int maxi = 0;
    int theta = 0;

    for (int i = 0; i < src_width; ++i)
    {
        for (int j = 0; j < src_height; ++j)
        {
            if (accumulator[i][j] >= maxi)
            {
                maxi = accumulator[i][j];
                theta = i;
            }
        }
    }

   
    free(accumulator);

	if(theta<0)
	{
		return theta+90;
	}
    return theta-90;
}

int auto_rotate(SDL_Surface* src_surface)
{
    int angle = hough_transform(src_surface);
    image_rotation(src_surface,(double)angle);
    return angle;
}
