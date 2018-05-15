#ifndef COMMON_H
#define COMMON_H

#include "complex.h"

double rem(double x, double y);
double* ones(int i1, int i2);
double* zeroes(int i1, int i2);
complex_double** Make2DArray(int arraySizeX, int arraySizeY);

#endif // COMMON_H
