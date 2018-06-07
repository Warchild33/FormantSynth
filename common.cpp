#include "math.h"
#include "stdlib.h"
#include "common.h"

double* ones(int i1, int i2)
{
    double* x = new double[sizeof(double)*i2];//(double*)malloc(sizeof(double)*i2);
    for(int i=i1; i < i2; i++)
        x[i] = 1;
    return x;
}

double* zeroes(int i1, int i2)
{
    double* x = new double[sizeof(double)*i2];
    for(int i=i1; i < i2; i++)
        x[i] = 0;
    return x;
}

std::vector<double> zeroesV(int i1, int i2)
{
    std::vector<double> x;
    x.resize(i2);
    for(int i=i1; i < i2; i++)
        x[i] = 0;
    return x;
}


complex_double** Make2DArray(int arraySizeX, int arraySizeY)
{
    complex_double** theArray;
    theArray =  new complex_double*[(arraySizeX*sizeof(complex_double*))];
    int i,j;
    for (i = 0; i < arraySizeX; i++)
    {
        theArray[i] = new complex_double[(arraySizeY*sizeof(complex_double))];
        for(j=0; j < arraySizeY; j++)
        {
            theArray[i][j] = std::complex<double>(0,0);
        }
    }
    return theArray;
}

double rem(double x, double y)
{
    return x - floor(x/y)*y;
}
