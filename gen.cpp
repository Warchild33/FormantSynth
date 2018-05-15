#define _USE_MATH_DEFINES
#include <math.h>
#include <complex>
#include "ploter.h"
#include "stft.h"
#include <algorithm>
#include "common.h"
#include "fft.h"
#include <cmath>

extern Ploter* p;

// impulse generator
double* pulse_gen1(double f_oc=800, double SampleRate=48000, double time=2, int* N=0)
{
    double g,tau;
    int N1, N2, n;

    double T = 1./f_oc;

    //отчетов сигнала в периоде импульса
    double Nt = SampleRate / f_oc;

    double* x = zeroes(0, floor(time*SampleRate));
    N1 = Nt/4;
    N2 = Nt/2;
    for(int i=0; i < floor(time*SampleRate); i+=Nt )
    {
        for(n=0; n < N1; n++)
        {
            g = 0.5 * (1 - cos((M_PI*n)/N1));
            x[i+n]=g;
        }
        for(n=N1; n < N2*2; n++)
        {
            g = cos(M_PI*(n - N1)/(2*N2));
            x[i+n]=g;
            if(g<0)break;
        }
        tau = n;
    }
    if(N)
      *N = floor(time*SampleRate);
    return x;
}

// скважность 0.125 0.25 0.5 0.75
double* pulse_nes(double f_oc=800, double percent=0.75, double SampleRate=48000, double time=2, int* N=0)
{
    //отчетов сигнала в периоде импульса
    double Nt = SampleRate / f_oc;
    double* x = zeroes(0, floor(time*SampleRate));
    int N1 = percent * Nt;
    for(int i=0; i < floor(time*SampleRate); i+=Nt )
    {
        for(int n=0; n < N1; n++)
        {
            x[i+n] = 1;
        }
        for(int n=N1; n < Nt; n++)
        {
            x[i+n] = 0;
        }
    }
    if(N)
      *N = floor(time*SampleRate);
    return x;

}


double* silence(double SampleRate=48000, double time=2, int* N=0)
{
    //отчетов сигнала в периоде импульса
    double* x = zeroes(0, floor(time*SampleRate));
    return x;
}

