#ifndef ISTFT_H
#define ISTFT_H
#include "complex.h"
 double* istft(complex_double** d, int ftsize, int w=0, int h=0, int rows=0, int cols=0, int* ylen=0);
#endif // ISTFT_H
