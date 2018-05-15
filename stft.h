#ifndef STFT_H
#define STFT_H

#include "complex.h"

complex_double** stft(double x[], int Nt, double f, double w, double h, double sr, int* rows, int* cols);

#endif // STFT_H
