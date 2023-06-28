#include <err.h>
#include <string.h>
#include "image_transformation/image/image_processing.h"
#include "detection/processing.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "Neural/neural2/Neural.h"
#include "Neural/neural2/MathTools.h"
#include "Neural/neural2/Persist.h"
#include "Neural/neural2/DataLoad.h"
#include "sudoku_solver/solver.h"
#include "grid_creation/grid_creation.h"
#include "grid_extraction/grid_numbers_extraction.h"
#include "grid_extraction/row_extract.h"
#include "grid_extraction/grid_extract.h"
#include "grid_extraction/utils.h"

int blank(double* number)
{
  int white = 0;
  for (int i = 0; i < NUM_INPUTS; i++)
    {
      
      if(number[i] == 1.0f)
          white++;
    }
  
  if (((white * 100) / NUM_INPUTS) < 2)
    return 1;
  
  return 0;
}

void p(char grid[])
{
  for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 9; j++)
    {
      printf("%hhi ", grid[i * 9 + j]);
    }
      printf("\n");
    }
}



int main(int argc, char** argv)
{
  if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

  SDL_Surface* surface = binarize_and_rotate(argv);
  
  IMG_SavePNG(surface, "binarized.png");

  int res = mkdir("numbers", 0777);
  if (res == 1)
    errx(1, "could not create : 'numbers/'");
  
  int x = grid_x_coordinate(surface);
  int y_top = grid_y_top_coordinate(surface, x);
  int y_bottom = grid_y_bottom_coordinate(surface, x);
  int extracted = extract_grid(surface, x, y_top, y_bottom);
  if (extracted)
    {
        printf("Grid Detection: starting row extraction.\n");
        int row_extracted = grid_row_extract("c_image.jpeg");
        if (row_extracted)
        {
            printf("Grid Detection: extraction done.\n");
            int numbers_extracted = grid_numbers_full_extract();
            
            if (numbers_extracted)
            {
                printf("Grid Numbers extraction: done!\n");
            }
        }
       
    }
  invert_black_white_all();
  magic_wand_all(); 
  SDL_Surface* im = surface;
  im = sobel(im);
  IMG_SavePNG(im, "sobel.png");
  //hough_transform2(surface);
  //cut(surface);
  IMG_SavePNG(im, "last_process.png");
  SDL_FreeSurface(surface);
  SDL_FreeSurface(im);
  magic_wand_all(); 
  char* numberFileName = calloc(15,sizeof(char));
  printf("HERE");
  char grid[81];
  printf("reached");
  //char* numberFileName = "numbers/00.png";

  numberFileName[0] = 'n';
  numberFileName[1] = 'u';
  numberFileName[2] = 'm';
  numberFileName[3] = 'b';
  numberFileName[4] = 'e';
  numberFileName[5] = 'r';
  numberFileName[6] = 's';
  numberFileName[7] = '/';
  numberFileName[8] = 'e';
  numberFileName[9] = 'd';
  numberFileName[10] = '.';
  numberFileName[11] = 'p';
  numberFileName[12] = 'n';
  numberFileName[13] = 'g';
  

  char six;
  char seven;
  for (int i = 1; i <82; i++)
    {
      six = *(itoa(i / 10,10));
      if(six == 0)
      {
         six = '0';
      }
      seven = *(itoa(i % 10,10));
      if(seven == 0)
      {
        seven = '0';
      }
      
      numberFileName[8] = six;
      
      numberFileName[9] = seven;
      
      SDL_Surface * img = load_image(numberFileName);
      SDL_Surface* im = scale(img,28,28);
      IMG_SavePNG(im,numberFileName);
      SDL_FreeSurface(img);
      SDL_FreeSurface(im);
      double* format_Image = calloc(NUM_INPUTS, sizeof(double));
      LoadImage(numberFileName, format_Image, 1);
      if(blank(format_Image))
      {
          grid[i-1] = '.';
      }
      else
      {
        
        LoadNeuralPath("Neural/neural2/trained_neural.txt");
        char detected = FeedForwardChar(format_Image);
        printf("Neural Detected for i = %i -> %c\n",i,detected);
        
        if(detected == '0')
        {
          grid[i-1] = '.';
        }
        else
        {
          grid[i - 1] = detected;
        }
        
        free(format_Image);
        
      }
    }
  
  FILE* file = fopen("sudoku_grid","a+");
  From_Grid_To_File(file, grid);
  fclose(file);
  FILE* file1 = fopen("sudoku_grid","r+");
  From_File_To_Grid(file1,grid);
  p(grid);
  fclose(file1);

  Solve(grid,0,0);
  printf("\n");
  p(grid);
  FILE* solved = fopen("sudoku_grid.result", "a+");
  Final_To_File(solved, grid);
  fclose(solved);

  FILE* final = fopen("sudoku_grid.result", "r");
  printf("File Was Open\n");
  FTG(final,grid);
  printf("FTG Done\n"); 
  p(grid);
  SDL_Surface* last = SDL_CreateRGBSurface(0, WINDOW_SIZE, WINDOW_SIZE, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  last = draw(last, grid);
  IMG_SavePNG(last, "z.png");
  SDL_FreeSurface(last);
  
  SDL_Quit();
  return 1;
}
