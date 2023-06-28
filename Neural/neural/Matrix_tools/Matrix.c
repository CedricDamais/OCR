#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "Matrix.h"
#include <err.h>

void printMatrix(Matrix* m)
{
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            printf("%f ", m->data[i][j]);
        }
        printf("\n");
    }
}

//Matrix initializer to 0
void initMatrix(Matrix* m)
{
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            m->data[i][j] = 0.0f;
        }
    }
}

//Matrix constructor
Matrix* newMatrix(int rows, int cols)
{
    printf("newMatrix(%d, %d)\n", rows, cols);
    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    m->rows = rows;
    m->cols = cols;
    m->data = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++)
    {
        m->data[i] = (double*)malloc(cols * sizeof(double));
    }

    initMatrix(m);
    printf("result of newMatrix\n");
    printMatrix(m);
    return m;
}

//Matrix destructor
void freeMatrix(Matrix* m)
{
    printf("freeMatrix(%d, %d)\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++)
    {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}

//Matrix copy constructor
Matrix* copyMatrix(Matrix* m, Matrix* dst)
{
    printf("copyMatrix(%d, %d)\n", m->rows, m->cols);
    printMatrix(m);
    if (dst == NULL)
    {
        dst = newMatrix(m->rows, m->cols);
    }
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            dst->data[i][j] = m->data[i][j];
        }
    }
    return dst;
    printf("after copyMatrix(%d, %d)\n", m->rows, m->cols);
    printMatrix(dst);
}

//Matrix copy constructor
Matrix* copyMatrix1(Matrix* m)
{
    return copyMatrix(m, NULL);
}

//Matrix multiplication
Matrix* multiplyMatrix(Matrix* m1, Matrix* m2)
{
    printf("\nmultiplyMatrix(%d, %d) * (%d, %d)\n", m1->rows, m1->cols, m2->rows, m2->cols);
    printMatrix(m1);
    printf("x\n");
    printMatrix(m2);
    printf("\n");
    if (m1->cols != m2->rows)
    {
        errx(1, "multiplyMatrix: dimensions are not compatible for matrix multiplication");
        return NULL;
    }

    Matrix* result = newMatrix(m1->rows, m2->cols);
    for (int i = 0; i < result->rows; i++)
    {
        for (int j = 0; j < result->cols; j++)
        {
            double sum = 0;
            for (int k = 0; k < m1->cols; k++)
            {
                sum += m1->data[i][k] * m2->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    printf("result of multiplyMatrix(%d, %d) * (%d, %d)\n", m1->rows, m1->cols, m2->rows, m2->cols);
    printMatrix(result);
    return result;
}

//Matrix addition
void addMatrix(Matrix* m1, Matrix* m2)
{
    printf("\naddMatrix\n");
    printMatrix(m1);
    printf("+\n");
    printMatrix(m2);
    printf("\n");
    if (m1->rows != m2->rows || m1->cols != m2->cols)
    {
        errx(1, "addMatrix: dimensions are not compatible for matrix addition");
        //return NULL;
    }
    //Matrix* result = newMatrix(m1->rows, m1->cols);
    for (int i = 0; i < m1->rows; i++)
    {
        for (int j = 0; j < m1->cols; j++)
        {
            //result->data[i][j] = m1->data[i][j] + m2->data[i][j];
            m1->data[i][j] += m2->data[i][j];
        }
    }
    printf("addMatrix result\n");
    printMatrix(m1);
}

//Matrix subtraction
void subMatrix(Matrix* m1, Matrix* m2)
{
    printf("\nsubMatrix\n");
    printMatrix(m1);
    printf("-\n");
    printMatrix(m2);
    printf("\n");
    if (m1->rows != m2->rows || m1->cols != m2->cols)
    {
        printf("Error: Matrix dimensions are not compatible for subtraction");
        //return NULL;
    }
    else
    {
        //Matrix* result = newMatrix(m1->rows, m1->cols);
        for (int i = 0; i < m1->rows; i++)
        {
            for (int j = 0; j < m1->cols; j++)
            {
                //result->data[i][j] = m1->data[i][j] - m2->data[i][j];
                m1->data[i][j] -= m2->data[i][j];
            }
        }
    }
    printf("result of subMatrix\n");
    printMatrix(m1);
    //return result;
}

//Matrix scalar multiplication
void scalarMultiplyMatrix(Matrix* m, double scalar)
{
    printf("\nscalarMultiplyMatrix %f\n", scalar);
    printMatrix(m);
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            m->data[i][j] *= scalar;
        }
    }
    printf("result of scalarMultiplyMatrix %f\n", scalar);
    printMatrix(m);
}
//Matrix transpose
Matrix* transposeMatrix(Matrix* m)
{
    Matrix* result = newMatrix(m->cols, m->rows);
    for (int i = 0; i < result->rows; i++)
    {
        for (int j = 0; j < result->cols; j++)
        {
            result->data[i][j] = m->data[j][i];
        }
    }
    return result;
}

//Matrix element-wise multiplication
Matrix* hadamardProductMatrix(Matrix* m1, Matrix* m2)
{
    printf("\nhadamardProductMatrix (%d, %d) * (%d, %d)\n", m1->rows, m1->cols, m2->rows, m2->cols);
    printMatrix(m1);
    printf("*\n");
    printMatrix(m2);
    printf("\n");
    if (m1->rows != m2->rows || m1->cols != m2->cols)
    {
        printf("Error: Matrix dimensions are not compatible for element-wise multiplication");
        return NULL;
    }
    Matrix* result = newMatrix(m1->rows, m1->cols);
    for (int i = 0; i < result->rows; i++)
    {
        for (int j = 0; j < result->cols; j++)
        {
            result->data[i][j] = m1->data[i][j] * m2->data[i][j];
        }
    }
    printf("result of hadamardProductMatrix (%d, %d) * (%d, %d)\n", m1->rows, m1->cols, m2->rows, m2->cols);
    printMatrix(result);
    return result;
}

//Matrix map function
Matrix* mapMatrix(Matrix* m, double (*func)(double))
{
    printf("\nmapMatrix %d, %d (%p)\n", m->rows, m->cols, func);
    Matrix* result = newMatrix(m->rows, m->cols);
    for (int i = 0; i < result->rows; i++)
    {
        for (int j = 0; j < result->cols; j++)
        {
            result->data[i][j] = func(m->data[i][j]);
        }
    }
    printf("mapMatrix result\n");
    printMatrix(result);
    return result;
}