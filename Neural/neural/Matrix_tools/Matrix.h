#ifndef MARIX_H
#define MARIX_H
#include <stdlib.h>

//Matrix structure
typedef struct Matrix
{
    int rows;
    int cols;
    double** data;
} Matrix;

//Matrix constructor
Matrix* newMatrix(int rows, int cols);

//Matrix destructor
void freeMatrix(Matrix* m);

//Matrix copy constructor
Matrix* copyMatrix(Matrix* m, Matrix* dst);

//Matrix copy constructor
Matrix* copyMatrix1(Matrix* m);

//Matrix multiplication
Matrix* multiplyMatrix(Matrix* m1, Matrix* m2);

//Matrix addition
void addMatrix(Matrix* m1, Matrix* m2);

//Matrix subtraction
void subMatrix(Matrix* m1, Matrix* m2);

//Matrix scalar multiplication
void scalarMultiplyMatrix(Matrix* m, double scalar);

//Matrix transpose
Matrix* transposeMatrix(Matrix* m);

//Matrix element-wise multiplication
Matrix* hadamardProductMatrix(Matrix* m1, Matrix* m2);

//Matrix map function
Matrix* mapMatrix(Matrix* m, double (*func)(double));

//Matrix print function
void printMatrix(Matrix* m);
#endif