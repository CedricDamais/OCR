#include "processing.h"
#include "../image_transformation/image/image_processing.h"

/// @brief Puts the pixel at pos x,y in the surface
/// @param surface the surface to put the pixel in
/// @param x the x coordinate to put the pixel in
/// @param y the y coordinate to put the pixel in
/// @param pixel the pixel to put in the surface
void putPixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
	Uint32* pixels = surface->pixels;
	if (y*surface->pitch/4+x > 0 && y*surface->pitch/4+x < surface->w*surface->h)
		pixels[ ( y * surface->pitch/4 ) + x ] = pixel;
}

/// @brief Retrieve the pixel at position x,y in surface
/// @param surface the surface we want to retrieve the pixel from
/// @param x the x coordinate where we want to get the pixel
/// @param y the y coordinate where we want to get the pixel
/// @return the pixel at the x,y coordinate
Uint32 getPixel(SDL_Surface* surface, int x, int y) {
	Uint32* pixels = surface->pixels;
	if (y*surface->pitch/4+x > 0 && y*surface->pitch/4+x < surface->w*surface->h)
		return pixels[ ( y * surface->pitch/4 ) + x ];
	return 0;
}

/*

/// @brief Transforms a pixel into it's grayscaled value
/// @param pixelColor the pixel to modify
/// @param format the format of the pixel
/// @return the pixel grayscaled
Uint32 pixelToGrayscale(Uint32 pixelColor, SDL_PixelFormat* format) {
	Uint8 r, g, b;
	SDL_GetRGB(pixelColor, format, &r, &g, &b);
	Uint8 average = 0.3*r + 0.59*g + 0.11*b;
	return SDL_MapRGB(format, average, average, average);
}

/// @brief Converts an entire surface to grayscale
/// @param surface the surface to transform
void surfaceToGrayscale(SDL_Surface* surface) {
	Uint32* pixels = surface->pixels;
	
	int len = surface->w * surface->h;
	
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	
	for (int i = 0; i < len; i++)
		pixels[i] = pixelToGrayscale(pixels[i], format);
	
	SDL_UnlockSurface(surface);
}

*/

/// @brief Gaussian Blur on Image using a kernel of 5x5 to smooth out the image for
/// better binarization later on
/// @param surface the surface we want to edit
/// @return The modified surface blurred out 
SDL_Surface* gaussianBlur(SDL_Surface* surface) {
	SDL_PixelFormat* format = surface->format;
	SDL_Surface* tempSurface = SDL_ConvertSurface(surface, format, surface->flags);

	int w = surface->w;
	int h = surface->h;

	int kernel[] = {1,  4,  7,  4, 1, 
					4, 16, 26, 16, 4,
					7, 26, 41, 26, 7,
					4, 16, 26, 16, 4,
					1,  4,  7,  4, 1,};

	int gaussWidth = 5;
	int halfGaussWidth = gaussWidth / 2;
	int gaussSum = 0, c = 0;
	for (int i = 0; i < gaussWidth * gaussWidth; i++)
		gaussSum += kernel[i];

	Uint8 sumr, sumg, sumb;
	Uint8 r, g, b;
	int flag = 0;
	
	SDL_LockSurface(tempSurface);
	SDL_LockSurface(surface);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			sumr = 0, sumg = 0, sumb = 0, flag = 0, c = 0;
			for (int i = -halfGaussWidth; i < halfGaussWidth+1; i++)
			{
				for (int j = -halfGaussWidth; j < halfGaussWidth+1; j++)
				{
					if (getPixel(surface, x+j, y+i)) {
						flag = 1;
						SDL_GetRGB(getPixel(surface, x+j, y+i), format, &r, &g, &b);
						sumr += r * kernel[c] / gaussSum;
						sumg += g * kernel[c] / gaussSum;
						sumb += b * kernel[c] / gaussSum;
					}
					c++;
				}
			}

			if (flag) putPixel(tempSurface, x, y, SDL_MapRGB(format, sumr, sumg, sumb));
		}		
		
	}

	SDL_UnlockSurface(tempSurface);
	SDL_UnlockSurface(surface);

	return tempSurface;
	
}



double degrees_to_rad2(double degrees)
{
    return degrees*180/M_PI;
}

typedef struct hLine {
	int x, y, r, t;
	struct hLine* next;
} hLine;

typedef struct vLine {
	int x, y, r, t;
	struct vLine* next;
} vLine;

void hough_transform2(SDL_Surface* surface)
{
	int height = surface->h;
	int width = surface->w;
	SDL_PixelFormat* format = surface->format;
    // Biggest possible accumulator.
    int diag = sqrt(width*width + height*height);
    int (*accumulator)[diag] = calloc(diag, sizeof(*accumulator));

	Uint8 r, g, b;
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
            Uint32 color = getPixel(surface, x, y);
            SDL_GetRGB(color, format, &r, &g, &b);
            float average = 0.3*r+0.59*g+0.11*b; 
			if (average >= 200) {
				for (int theta = 0; theta <= 180; theta++)
                {
                    int rho = ceil(x * cos(degrees_to_rad2(theta))
                        			+ y * sin(degrees_to_rad2(theta)));
                    accumulator[theta][rho]++;
                }
			}
		}
	}
	// define threshold genre 0.5*max accu
	// parcours accu tt >= threshold = good
	// parcours image si dans save alors line = good
	
	// get max of accu
	int maxi = accumulator[0][0];
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (accumulator[y][x] >= maxi) 
				maxi = accumulator[y][x];

	// define threshold 
	float threshold = 0.485 * maxi;

	// every value >= threshold in accu is saved
	int (*savedAccu)[diag] = calloc(diag, sizeof(*savedAccu));

	for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
			if (accumulator[y][x] >= threshold)
				savedAccu[y][x] = accumulator[y][x];
		}
	}
	free(accumulator);
	// Processing every saved value there was in the accu to get the lines
	//int (*coords)[width][4] = malloc(sizeof(int[height][width][4]));

	vLine* vL = calloc(200, sizeof(vLine));
	hLine* hL = calloc(200, sizeof(hLine));
	int countVL = 0, countHL = 0;

	int i = 0;
	//j = 0;
	//int flag = 0;
	//int lastx = 0, lasty = 0;

	for (int r = 0; r < width; r++) {
		for (int t = 0; t < height; t++) {
		  i = 0;
			if (savedAccu[t][r] != 0)
				for (int y = 0; y < height; y++, i++) {
					for (int x = 0; x < width; x++) {
						if (r == ceil(x * cos(degrees_to_rad2(t)) 
									+ y * sin(degrees_to_rad2(t)))) {
							// to see the lines
							if (sin(degrees_to_rad2(t)) > 0.85) { // horizontal
								hLine* l = calloc(1, sizeof(hLine));
								l->x = x;
								l->y = y;
								l->r = r;
								l->t = t;
								l->next = hL->next;
								hL->next = l;
								countHL++;
								//putPixel(surface, x, y, SDL_MapRGB(format, 255, 0, 0));
							} else if (sin(degrees_to_rad2(t)) < 0.2) { // vertical
								vLine* l = calloc(1, sizeof(vLine));
								l->x = x;
								l->y = y;
								l->r = r;
								l->t = t;
								l->next = vL->next;
								vL->next = l;
								countVL++;
								//putPixel(surface, x, y, SDL_MapRGB(format, 0, 255, 0));
							}
							//putPixel(surface, x, y, SDL_MapRGB(format, 255, 0, 0));

							// get the first point of the line
							/*if (!flag) {
								coords[i][j][0] = x;
								coords[i][j][1] = y;
								coords[i][j][2] = x;
								coords[i][j][3] = y;
								flag = 1;
							}
							// save the x y to get the last point of the line
							lastx = x;
							lasty = y;
							j++;*/
						}
					}
			}
			// save the last point of the line
			//coords[i][j][2] = lastx;
			//coords[i][j][3] = lasty;
		}
	}
	free(savedAccu);

	vLine* filtered = calloc(countVL, sizeof(vLine));
	vLine* temp = vL;
	int lasty = 0, lastx = 0;
	int value = 1500;
	printf("vl: %i, hl: %i\n", countVL, countHL);
	int c = 0;
	
	while (vL) {
		while (temp)
		{
			if (temp->y != lasty)
				lastx = temp->x;

			if (temp->x >= vL->x-value && temp->x <= vL->x+value) {
				vLine* l = calloc(1, sizeof(vLine));
				l->x = lastx;
				l->y = temp->y;
				l->r = temp->r;
				l->t = temp->t;
				l->next = filtered->next;
				filtered->next = l;
				lasty = temp->y;
				lastx = temp->x;
				c++;
				//putPixel(surface, temp->x, temp->y, SDL_MapRGB(format, 0, 255, 0));
			}
			temp = temp->next;
		}
		vL = vL->next;
	}

	printf("c: %i\n", c);

	while (filtered)
	{
		putPixel(surface, filtered->x, filtered->y, SDL_MapRGB(format, 0, 255, 0));
		filtered = filtered->next;
	}
	
	

/*  // Quick Method to get intersection points between horizontal and vertical lines
	if (countVL > countHL) {
		while (vL) {
			hLine* temp = hL;
			while (temp) {
				if (temp->x == vL->x && temp->y == vL->y)
					putPixel(surface, temp->x, temp->y, SDL_MapRGB(format, 0, 255, 0));
				temp = temp->next;
			}
			vL = vL->next;
		}
	} else {
		while (hL) {
			vLine* temp = vL;
			while (temp) {
				if (temp->x == hL->x && temp->y == hL->y)
					putPixel(surface, temp->x, temp->y, SDL_MapRGB(format, 255, 0, 0));
				temp = temp->next;
			}
			hL = hL->next;
		}
	}
*/

	// ========= temp ==========
	//for (int k = 0; k < width; k++)
	//	printf("x1(%i,%i) x2(%i,%i) ", coords[0][k][0], coords[0][k][1], coords[0][k][2], coords[0][k][3]);
	// ========= temp ==========

	// for each coordinates saved in coords get the vectors of the lines
	/*for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int x1 = coords[y][x][0];
			int y1 = coords[y][x][1];
			int x2 = coords[y][x][2];
			int y2 = coords[y][x][3];
			//putPixel(surface, x, y, SDL_MapRGB(format, 255, 0, 0));
			coords[y][x][0] = abs(x1-x2);
			coords[y][x][1] = abs(y1-y2);
			coords[y][x][2] = 0;
			coords[y][x][3] = 0;
		}
//		printf("\n");
	}*/

	// goal is to get the intersection of two vectors and it should give every intersection of every line
	// then we can save only the 4 corners and crop the image to the grid
	/*int v1x = 0, v1y = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (!v1x) {
				v1x = coords[y][x][0];
				v1y = coords[y][x][1];
			}
			else {
				printf("Here\n");
				int v2x = coords[y][x][0], v2y = coords[y][x][1];
				float alpha = (v1x*v2x+v1y*v2y);
				printf("angle1: %f ", alpha);
				alpha /= (sqrt(v1x*v1x+v1y*v1y)*sqrt(v2x*v2x+v2y*v2y));
				printf("angle: %f\n", alpha);
				//v1x = 0; v1y = 0;
				if (alpha) {
					putPixel(surface, v1x, v1y, SDL_MapRGB(format, 0, 0, 255));
				}
				v1x = 0; v1y = 0;
			}
			//if (coords[y][x][0] || coords[y][x][1]) {
			//	putPixel(surface, coords[y][x][0], coords[y][x][1], SDL_MapRGB(format, 0, 0, 255));
			//}
		}
	}*/
	
	//free(coords);
}

/// @brief cut the image in 9x9
/// @param surface  the surface to cut
void cut(SDL_Surface* surface) {
	int height = surface->h;
	int width = surface->w;
	int currWidth = 0, currHeight = 0, count = 0;
	for (int i = 9; i > 0; i--)
	{
		currWidth = 0;
		for (int j = 9; j > 0; j--)
		{
			SDL_Surface* tempSurface = SDL_CreateRGBSurface(surface->flags, width/9, height/9, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
			SDL_Rect rect = {currWidth, currHeight, width/9, height/9};
			currWidth += width/9;
			SDL_BlitSurface(surface, &rect, tempSurface, 0);
			char* name;
			if (count < 9)
				name = "cases/0%i.png";
			else
				name = "cases/%i.png";
			char res[100];
			count++;
			sprintf(res, name, count);
			SDL_SaveBMP(tempSurface, res);
		}
		currHeight += height/9;
	}
}

/// @brief simple truncate function
/// @param x the value to truncate
/// @return value maxed at 255
float truncate(float x) {
	return x > 255 ? 255 : x;
}


/// @brief apply sobel algorithm on image
/// @param surface the surface to apply algorithm on
/// @return the new surface where sobel was applied
SDL_Surface* sobel(SDL_Surface* surface) {
	SDL_PixelFormat* format = surface->format;
	SDL_Surface* tempSurface = SDL_ConvertSurface(surface, format, surface->flags);
	int w = surface->w;
	int h = surface->h;

	Uint8 r, g, b;

	float sumr, sumg, sumb;
	float resr, resg, resb;
	float tempr, tempg, tempb;
	int c = 0;

	int xKernel[] = {-1, 0, 1,
					 -2, 0, 2,
					 -1, 0, 1,};

	int yKernel[] = {-1,  -2,  -1,
					  0,  0,   0,
					  1,  2,  1,};

	SDL_LockSurface(tempSurface);
	SDL_LockSurface(surface);
	
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			sumr = 0, sumg = 0, sumb = 0, 
			tempr = 0, tempg = 0, tempb = 0, c = 0;
			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					if (getPixel(surface, x+j, y+i)) {
						SDL_GetRGB(getPixel(surface, x+j, y+i), format, &r, &g, &b);
						tempr += r * xKernel[c];
						tempg += g * xKernel[c];
						tempb += b * xKernel[c]; // horizontal convolution
						sumr += r * yKernel[c];
						sumg += g * yKernel[c];
						sumb += b * yKernel[c]; // vertical convolution
					}
					c++;
				}
			}
			resr = truncate(sqrt(tempr * tempr + sumr * sumr));
			resg = truncate(sqrt(tempg * tempg + sumg * sumg));
			resb = truncate(sqrt(tempb * tempb + sumb * sumb)); // magnitude
			putPixel(tempSurface, x, y, SDL_MapRGB(format, resr, resg, resb));
		}
	}
	
	SDL_UnlockSurface(tempSurface);
	SDL_UnlockSurface(surface);

	return tempSurface;
}

SDL_Surface* dilate(SDL_Surface* surface) {
	SDL_PixelFormat* format = surface->format;
	SDL_Surface* tempSurface = SDL_ConvertSurface(surface, format, surface->flags);
	int w = surface->w;
	int h = surface->h;

	Uint8 r, g, b;

	SDL_LockSurface(tempSurface);
	SDL_LockSurface(surface);
	
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			SDL_GetRGB(getPixel(surface, x, y), format, &r, &g, &b);
			float average = 0.3*r+0.59*g+0.11*b; 
			if (average < 200) { // if it is a black pixel
				SDL_GetRGB(getPixel(surface, x-1, y), format, &r, &g, &b); //back pixel
				average = 0.3*r+0.59*g+0.11*b; 
				if (average >= 200)
					putPixel(tempSurface, x, y, SDL_MapRGB(format, 255, 255, 255));
				else {
					SDL_GetRGB(getPixel(surface, x, y-1), format, &r, &g, &b); //top pixel
					average = 0.3*r+0.59*g+0.11*b;
					if (average >= 200)
						putPixel(tempSurface, x, y, SDL_MapRGB(format, 255, 255, 255));
					else {
						SDL_GetRGB(getPixel(surface, x+1, y), format, &r, &g, &b); //right pixel
						average = 0.3*r+0.59*g+0.11*b;
						if (average >= 200)
							putPixel(tempSurface, x, y, SDL_MapRGB(format, 255, 255, 255));
						else {
							SDL_GetRGB(getPixel(surface, x, y+1), format, &r, &g, &b); //top pixel
							average = 0.3*r+0.59*g+0.11*b;
							if (average >= 200)
								putPixel(tempSurface, x, y, SDL_MapRGB(format, 255, 255, 255));
						}
					}
				}
			}
		}
	}

	SDL_UnlockSurface(tempSurface);
	SDL_UnlockSurface(surface);

	return tempSurface;
}

SDL_Surface* erode(SDL_Surface* surface) {
	SDL_PixelFormat* format = surface->format;
	SDL_Surface* tempSurface = SDL_ConvertSurface(surface, format, surface->flags);
	int w = surface->w;
	int h = surface->h;

	Uint8 r, g, b;
	int c = 0;
	float sumr, sumg, sumb;

	int kernel[] = { 0, 1, 0,
					 1, 1, 1,
					 0, 1, 0,};

	SDL_LockSurface(tempSurface);
	SDL_LockSurface(surface);
	
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			sumr = 0, sumg = 0, sumb = 0, c = 0;
			SDL_GetRGB(getPixel(surface, x, y), format, &r, &g, &b);
			float average = 0.3*r+0.59*g+0.11*b; 
			if (average > 200) { // if it is a white pixel
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if (getPixel(surface, x+j, y+i)) {
							SDL_GetRGB(getPixel(surface, x+j, y+i), format, &r, &g, &b);
							sumr += r * kernel[c];
							sumg += g * kernel[c];
							sumb += b * kernel[c];
						}
						c++;
					}
				}

				average = 0.3*sumr/9+0.59*sumg/9+0.11*sumb/9;
				if (average >= 120)//if (sumr >= 2295)
					putPixel(tempSurface, x, y, SDL_MapRGB(format, 255, 255, 255));
				else
					putPixel(tempSurface, x, y, SDL_MapRGB(format, 0, 0, 0));
			}
		}
	}

	SDL_UnlockSurface(tempSurface);
	SDL_UnlockSurface(surface);

	return tempSurface;
}

/*

// =========== OTSU GREGOIRE =========

Uint32 simple(Uint32 pixel_color, SDL_PixelFormat* format, Uint32 threshold)
{
  Uint8 r, g, b;
  SDL_GetRGB(pixel_color, format, &r, &g, &b);
  Uint32 color;
  if (r > threshold)
    color = SDL_MapRGB(format, 255, 255, 255);
  else
    color = SDL_MapRGB(format, 0, 0, 0);
  return color;
}


void simple_binarize(SDL_Surface* surface, Uint32 threshold)
{
  if (surface == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());
  Uint32* pixels = surface->pixels;
  int len = surface->w * surface->h;
  SDL_LockSurface(surface);
  SDL_PixelFormat* format = surface->format;
  for (int i = 0; i < len; i++)
    {
      pixels[i] = simple(pixels[i], format, threshold);
    }
  SDL_UnlockSurface(surface);
}


void histogram(SDL_Surface* surface, int* histo)
{
  Uint8 r, g, b;
  int len = surface->w * surface->h;
  Uint32* pixels = surface->pixels;
  SDL_PixelFormat* format = surface->format;
  SDL_LockSurface(surface);
  
  //creates a histogram of the grayscale of the image
  for (int i = 0; i < len; i++)
    {
      //pixels[i] = pixelToGrayscale(pixels[i],format);
      SDL_GetRGB(pixels[i],format,&r,&g,&b);
      histo[b]++;
    }
  SDL_UnlockSurface(surface);
}


void equalized(SDL_Surface* surface, int* histo)
{
  int sum = 0;
  int len = surface->w * surface->h;
  for (int i = 0; i < 256; i++)
    {
      sum += histo[i];
      histo[i] = (sum * 255) / len;
    }
  Uint32* pixels = surface->pixels;
  SDL_PixelFormat* format = surface->format;
  SDL_LockSurface(surface);
  int out;
  Uint8 r, g, b;
  for (int i = 0; i < len; i++)
    {
      SDL_GetRGB(pixels[i],format,&r,&g,&b);
      out = histo[b];
      pixels[i] = SDL_MapRGB(format, out, out, out);
    }
  SDL_UnlockSurface(surface);
  for (int i = 0; i < 256; i++)
    histo[i] = 0;
  histogram(surface, histo);
}


Uint8 Otsusmethod(SDL_Surface* surface, int* histo)
{
  int len = surface->w * surface->h;
  
  //calculating the total sum of the histogram
  unsigned long sum0 = 0, sum1 = 0;
  for (int i = 0; i < 256; i++)
    {
      sum0 += histo[i] * i;
    }

  unsigned long p0 = 0, p1 = 0;
  unsigned long mean0 = 0, mean1 = 0;
  float between = 0, maxi = 0;
  Uint8 threshold0 = 0, threshold1 = 0;
  
  for (int i = 0; i < 256; i++)
    {
      p0 += histo[i];
      p1 = len - p0;
      if (p0 > 0 && p1 > 0)
	{
	  sum1 += histo[i] * i;
	  mean0 = sum1 / p0;
	  mean1 = (sum0 - sum1) / p1;
	  between = p0 * p1 * (mean0 - mean1) * (mean0 - mean1);
	  if (between >= maxi)
	    {
	      threshold0 = i;
	      if (between > maxi)
	      threshold1 = i;
	      maxi = between;
	    }
	}
    }
  return (threshold0 + threshold1) / 2;
}

*/

// ============ TEMP =================
/*
void largestConnectedComponent(SDL_Surface* surface) {
	SDL_PixelFormat* format = surface->format;
	int w = surface->w;
	int h = surface->h;

	Uint8 r, g, b;

	int zone = 0;putPixel(tempSurface, x, y, SDL_MapRGB(format, 255, 255, 255));

	int equivalenceList[w];
	float (*map)[w] = malloc(sizeof(float[h][w]));
	memset(equivalenceList, 0, sizeof(equivalenceList));
	memset(map, 0, sizeof(*map));

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
				if (x-1 > 0 && y-1 > 0) { // if top and back pixel exist
				SDL_GetRGB(getPixel(surface, x-1, y), format, &r, &g, &b); // get values of back pixel
				if (r == 255) { // check if that pixel is white
					SDL_GetRGB(getPixel(surface, x, y-1), format, &r, &g, &b); // get values of top pixel
					if (r == 255) { // check if top pixel is white
						int zoneToTake = map[y-1][x] < map[y][x-1] ? map[y-1][x] : map[y][x-1];
						equivalenceList[zone] = zoneToTake;
						map[y][x] = zoneToTake;
					} else { // if only back pixel is white
						map[y][x] = map[y][x-1];
					}
				} else {
					SDL_GetRGB(getPixel(surface, x, y-1), format, &r, &g, &b); // get values of top pixel
					if (r == 255) { // check if top pixel is white
						map[y][x] = map[y-1][x];
					} else { // if only back pixel is white
						zone++;
						map[y][x] = zone;
						equivalenceList[zone] = zone;
			 		}
				}
			}
			else if (y-1 > 0) {
				SDL_GetRGB(getPixel(surface, x, y-1), format, &r, &g, &b); // get values of top pixel
				if (r == 255) { // check if top pixel is white
					map[y][x] = map[y-1][x];
			 	}
			}
			else if (x-1 > 0) {
				SDL_GetRGB(getPixel(surface, x-1, y), format, &r, &g, &b); // get values of top pixel
				if (r == 255) { // check if top pixel is white
					map[y][x] = map[y][x-1];
				}
			}
		}}
	}

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			map[y][x] = equivalenceList[(int)map[y][x]];
		}
	}
	

	for (int j = 0; j < w; j++)
 	{
 		printf("%i ", equivalenceList[j]);
 	}

	int freq[zone];
	memset(freq, 0, sizeof(freq));
	printf(" zones: %i ", zone);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int value = map[y][x];
			if (value != 0) freq[value]++;
		}
	}

	

    int max = freq[0];
	int savedIndex = 0;
	for (int i = 1; i < zone; i++) {
		if (freq[i] > max) {
            max = freq[i];
			savedIndex = i;
		}
	}

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			if(map[y][x] !=0 && map[y][x] != savedIndex) {// 
				putPixel(surface, x, y, SDL_MapRGB(format, 0+10*map[y][x], 0, 0));
			}
		}
	}


}


void largestConnectedComponent(SDL_Surface* surface) {
	SDL_PixelFormat* format = surface->format;
	int w = surface->w;
	int h = surface->h;

	Uint8 r, g, b;

	int zone = 0;
	int equivalenceList[w];
	float (*map)[w] = malloc(sizeof(float[h][w]));
	memset(map, 0, sizeof(*map));
	memset(equivalenceList, 0, sizeof(equivalenceList));

	// map every pack of pixel in the map to get the largest
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			SDL_GetRGB(getPixel(surface, x, y), format, &r, &g, &b);
			if (r == 255) {
				


				if (x-1 > 0 && y-1 > 0) { // if top and back pixel exist
					SDL_GetRGB(getPixel(surface, x-1, y), format, &r, &g, &b); // get values of back pixel
					if (r == 255) { // check if that pixel is white
						SDL_GetRGB(getPixel(surface, x, y-1), format, &r, &g, &b); // get values of top pixel
						if (r == 255) { // check if top pixel is white
							int zoneToTake = map[y-1][x] < map[y][x-1] ? map[y-1][x] : map[y][x-1];
							equivalenceList[zone] = zoneToTake;
							map[y][x] = zoneToTake;
						} else { // if only back pixel is white
							map[y][x] = map[y][x-1];
						}
					} else {
						SDL_GetRGB(getPixel(surface, x, y-1), format, &r, &g, &b); // get values of top pixel
						if (r == 255) { // check if top pixel is white
							map[y][x] = map[y-1][x];
						} else { // if only back pixel is white
							zone++;
							map[y][x] = zone;
							equivalenceList[zone] = zone;
						}
					}
				}
				else if (y-1 > 0) {
					SDL_GetRGB(getPixel(surface, x, y-1), format, &r, &g, &b); // get values of top pixel
					if (r == 255) { // check if top pixel is white
						map[y][x] = map[y-1][x];
					}
				}
				else if (x-1 > 0) {
					SDL_GetRGB(getPixel(surface, x-1, y), format, &r, &g, &b); // get values of top pixel
					if (r == 255) { // check if top pixel is white
						map[y][x] = map[y][x-1];
					}
				}
			}
		}
	}

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			map[y][x] = equivalenceList[(int)map[y][x]];
		}
	}
	

	for (int j = 0; j < w; j++)
 	{
 		printf("%i ", equivalenceList[j]);
 	}

	int freq[zone];
	memset(freq, 0, sizeof(freq));
	printf(" zones: %i ", zone);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int value = map[y][x];
			if (value != 0) freq[value]++;
		}
	}

	

    int max = freq[0];
	int savedIndex = 0;
	for (int i = 1; i < zone; i++) {
		if (freq[i] > max) {
            max = freq[i];
			savedIndex = i;
		}
	}

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			if(map[y][x] !=0 && map[y][x] != savedIndex) {// 
				putPixel(surface, x, y, SDL_MapRGB(format, 0+10*map[y][x], 0, 0));
			}
		}
	}


	// ====== TO REMOVE ========
	// for (int i = 0; i < h; i++)
	// {
	// 	for (int j = 0; j < w; j++)
	// 	{
	// 		printf("%i ", map[i][j]);
	// 	}
	//  	printf("\n");
	// }
}
*/
