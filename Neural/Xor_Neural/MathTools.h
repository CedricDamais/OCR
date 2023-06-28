#ifndef MATH_TOOLS_H
#define MATH_TOOLS_H
#include <math.h>
#include <stdlib.h>
#include <time.h>
//return the image of x by the sigmoid function
double sigmoid(double x);

double dSigmoid(double x);

//returns a random double between 0.0 and 1.0
double init_weight();

//initialize seed with current time
void init_random();

#endif