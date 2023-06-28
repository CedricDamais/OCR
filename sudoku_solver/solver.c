#include <stdio.h>
#include <stdlib.h>
#include <err.h>

/*char grid[] =
  {
    0, 0, 0, 0, 0, 4, 5, 8, 0,
    0, 0, 0, 7, 2, 1, 0, 0, 3,
    4, 0, 3, 0, 0, 0, 0, 0, 0,
    2, 1, 0, 0, 6, 7, 0, 0, 4,
    0, 7, 0, 0, 0, 0, 2, 0, 0,
    6, 3, 0, 0, 4, 9, 0, 0, 1,
    3, 0, 6, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 5, 8, 0, 0, 6,
    0, 0, 0, 0, 0, 6, 9, 5, 0
  };*/

int Safe(char grid[], char number, size_t row, size_t col)
{
  //checking if the given number already exists in row and col
  for (size_t i = 0; i < 9; i++)
    {
      if (grid[row * 9 + i] == number ||
      grid[9 * i + col] == number)
    {
      return 0;}
    }
  //checking if the given number already exists in its 3x3 box
  row -= row % 3;
  col -= col % 3;
  for (size_t i = 0; i < 3; i++)
    {
      for (size_t j = 0; j < 3; j++)
    {
      if (grid[row * 9 + 9 * j + i + col] == number)
        {
          return 0;
        }
    }
    }
  
  return 1;
}

int Solve(char grid[], size_t row, size_t col)
{
  //checking if we are at the end of the grid
  if (row == 8 && col == 9)
    return 1;

  //if we arre at the end of a column, we pass to the start of the next row
  if (col == 9)
    {
      row++;
      col = 0;
    }

  //if there is not a 0, we pass to the next column
  if (grid[row * 9 + col])
    return Solve(grid, row, col + 1);

  //trying every number from 1 to 9
  for (char i = 1; i < 10; i++)
    {
      
      //if the number can be put inside the case, we put it, then try with the next case
      if (Safe(grid, i, row, col))
    {
      grid[row * 9 + col] = i;
      if (Solve(grid, row, col + 1))
        return 1;
    }
      
      //else, we put the current case to 0 and pass to the next number
      grid[row * 9 + col] = 0;
    }
  
  //if nothing works, it means that the grid cannot be solved, so we can't do more
  return 0;
}

void From_File_To_Grid(FILE* file, char grid[])
{
  size_t i = 0;
  while(i < 81)
    {
      char c = fgetc(file);
      if (c != ' ' && c != '\n')
      {
        printf("char = %c\n",c);
        if (c == '.')
        {
          grid[i] = 0;
          printf("Grid Value is %c",grid[i]);
        }
        else if (c > '0' && c <= '9')
          grid[i] = c-48;
        else
        {
          printf("i = %zu",i);
          errx(1, "There is an unknown character in the given file : %hhi",c);
        }
        i++;
      }
    }
}

void From_Grid_To_File(FILE* file, char grid[])
{
  for (size_t i = 0; i < 9; i++)
    {
      for (size_t j = 0; j < 9; j++)
	{
    fprintf(file, "%c", grid[i * 9 + j]);
    
	  if (j == 2 || j == 5)
	    fputc(' ', file);
	}
      fprintf(file, "\n");
      if (i == 2 || i == 5)
	      fprintf(file, "\n");
    }
}

void Final_To_File(FILE* file, char grid[])
{
  for (size_t i = 0; i < 9; i++)
    {
      for (size_t j = 0; j < 9; j++)
    {
      fprintf(file, "%hhi", grid[i * 9 + j]);
      if (j == 2 || j == 5)
        fputc(' ', file);
    }
      fprintf(file, "\n");
      if (i == 2 || i == 5)
    fprintf(file, "\n");
    }
}


/*

int main(int argc, char** argv)
{
  //If there are more than 2 arguments, it is not a valid call for this program
  if (argc > 2)
    errx(1, "Arguments invalides");

  //Accessing the wanted file
  FILE* unsolved = NULL;
  unsolved = fopen(argv[1], "r");

  //If the file is not defined, error
  if (unsolved == NULL)
    errx(1, "La grille donnée n'existe pas");

  //creating the grid, and filling it with the file's data
  char grid[81];
  From_File_To_Grid(unsolved, grid);

  //solving the given grid
  Solve(grid, 0, 0);

  //Creating a new file in which we can put the new grid
  FILE* solved = NULL;
  char file[64];
  sprintf(file, "%s.result", argv[1]);
  
  solved = fopen(file, "a+");

  //Writting in the file the new solved grid
  From_Grid_To_File(solved, grid);

  //Closing the two opened files
  fclose(unsolved);
  fclose(solved);

  
  return 0;
}

*/
