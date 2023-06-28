#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include "image_rotation.h"
#include "image_processing.h"




//Elements needed clone_image(image);
// degrees_to_rad(angle);
/**
 * I just transform my degrees into radiants since I want to make a counterclock wise rotation
 * to do that we use the following formula :
                              |
                              |
                                ->>>>>>> α(radians) = α(degrees) × π / 180°

 * With this approximation of  ----> pi = 3.1415926535897932
**/

double degrees_to_rad(double degrees)
{
    double pi = 3.1415926535897932;
    return degrees*(pi/180);
}

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

void put_pixel(SDL_Surface *surface, unsigned x_position, unsigned y_position, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x_position, y_position);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
        //SDL_BIG_ENDIAN means byte order is 4321, where the most significant byte is stored first
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
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

Uint32 get_pixel(SDL_Surface *surface, unsigned x_position, unsigned y_position)
{
  /**
  *Param :
  * surface : The SDL surface we are using
  * x_postion : The position of the pixel on the x-axcis of our current pixel array .
  * y_position : The position of the pixel on the y-axis of our current pixen array .
  **/
    Uint8 *p = pixel_ref(surface, x_position, y_position);

    switch (surface->format->BytesPerPixel)
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
    }

    return 0;
}

void pixel_rotation(double x,double y,double angle,double* rotated_x,double* rotated_y,double center_image_x,double center_image_y)
{
    /**
     * ry rotated y coordinate
     * rx rotated x coordinate
    * This function will transform each pixel's position
    * Basically it will Perform the rotation of the pixel in position (x,y) by the input angle
    * In two dimensions, to carry out a rotation using a matrix, the point (x, y) to be rotated counterclockwise is written as a column vector,
    * then multiplied by a rotation matrix calculated from the angle θ
    *
    * We have the Following Formula
    *
    *  | x |     |  cos(θ)  -sin(θ) |    | rx |
    *  | y |  =  |  sin(θ)  cos(θ)  | =  | ry |
    *
    * Then we add Also the coordinates of the center of the Image
    **/

    *rotated_x = ceil((x-center_image_x)*cos(angle)-(y-center_image_y)*sin(angle)+ center_image_x);
    *rotated_y = ceil((x-center_image_x)*sin(angle) +(y-center_image_y)*cos(angle)+ center_image_y);

}

void image_rotation(SDL_Surface* src_surface,double angle)
{
	   SDL_Surface* dest_surface;
	   int src_height = src_surface->h;
	   int src_width = src_surface->w;
	   int m_x = (src_width / 2);
	   int m_y = (src_height / 2);
	   double angle_radian = degrees_to_rad(angle);
	   double cos_s = cos(angle_radian);
	   double sin_s = sin(angle_radian);
	   double dest_width  = ceil(src_width * fabs(cos_s)
		    + src_height * fabs(sin_s));
	   double dest_height = ceil(src_width * fabs(sin_s)
		   + src_height * fabs(cos_s));
	   double rotate_x, rotate_y;
	   Uint32 color;

	    printf("angle : %f\n", angle);
	    dest_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, dest_width, dest_height,
		src_surface->format->BitsPerPixel, src_surface->format->Rmask,
		src_surface->format->Gmask, src_surface->format->Bmask,
		src_surface->format->Amask);

	SDL_FillRect(dest_surface,
		NULL, SDL_MapRGB(dest_surface->format, 255, 255, 255));

	if (dest_surface == NULL)
	{
		errx(EXIT_FAILURE,"%s", SDL_GetError());
	}

	int m_x_dest = dest_surface->w/2.;
	int m_y_dest = dest_surface->h/2.;

	SDL_LockSurface(src_surface);
	SDL_LockSurface(dest_surface);

	for (int y = 0; y < dest_height; ++y)
	{
		for (int x = 0; x < dest_width; ++x)
		{
      /**
       * ry rotated y coordinate
       * rx rotated x coordinate
      * This function will transform each pixel's position
      * Basically it will Perform the rotation of the pixel in position (x,y) by the input angle
      * In two dimensions, to carry out a rotation using a matrix, the point (x, y) to be rotated counterclockwise is written as a column vector,
      * then multiplied by a rotation matrix calculated from the angle θ
      *
      * We have the Following Formula
      *
      *  | x |     |  cos(θ)  -sin(θ) |    | rx |
      *  | y |  =  |  sin(θ)  cos(θ)  | =  | ry |
      *
      * Then we add Also the coordinates of the center of the Image
      **/

			rotate_x = (ceil (cos_s * (x-m_x_dest)+ sin_s * (y-m_y_dest) + m_x));
			rotate_y = (ceil (-sin_s * (x-m_x_dest) + cos_s * (y-m_y_dest) + m_y));

			if (0 <= rotate_x && rotate_x < src_width && 0 <= rotate_y && rotate_y < src_height)
			{
				color = get_pixel(src_surface, rotate_x, rotate_y);
				put_pixel(dest_surface, x, y, color);
			}
		}
	}

	SDL_UnlockSurface(src_surface);
	SDL_UnlockSurface(dest_surface);
    *src_surface = *dest_surface;
    free(dest_surface);
}
