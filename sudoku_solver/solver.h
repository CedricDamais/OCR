#ifndef SOLVER_H
#define SOLVER_H

int Safe(char grid[], char number, size_t row, size_t col);
int Solve(char grid[], size_t row, size_t col);
void From_File_To_Grid(FILE* file, char grid[]);
void From_Grid_To_File(FILE* file, char grid[]);
void Final_To_File(FILE* file, char grid[]);

#endif
