#define _USE_MATH_DEFINES
#include <math.h>
#include <complex>
#include "ploter.h"
#include "stft.h"
#include <algorithm>
#include "common.h"
#include "fft.h"
#include <cmath>
#include <random>
#include <algorithm>

extern Ploter* p;

// impulse generator
double* pulse_gen1(double f_oc=800, double SampleRate=48000, double time=2, int* N=0)
{
    double g,tau;
    int N1, N2, n;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(1,0.25);


    double T = 1./f_oc;

    //отчетов сигнала в периоде импульса
    int Nt = SampleRate / f_oc;

    double* x = zeroes(0, floor(time*SampleRate));
    N1 = Nt/4;
    N2 = Nt/2;

    for(n=0; n < N1; n++)
    {
        g = 0.5 * (1 - cos((M_PI*n)/N1))*d(gen);
        x[n]=g;
    }
    for(n=N1; n < N2*2; n++)
    {
        g = cos(M_PI*(n - N1)/(2*N2))*d(gen);
        x[n]=g;
        if(g<0) break;
    }

    for(int i=Nt; i < floor(time*SampleRate); i++ )
    {
        x[i]=x[i%Nt];
    }

    if(N)
      *N = floor(time*SampleRate);
    return x;
}

// скважность 0.125 0.25 0.5 0.75
double* pulse_nes(double f_oc=800, double percent=0.75, double SampleRate=48000, double time=2, int* N=0)
{
    //отчетов сигнала в периоде импульса
    int Nt = SampleRate / f_oc;
    double* x = zeroes(0, floor(time*SampleRate));
    int N1 = percent * Nt;
    for(int n=0; n < N1; n++)
    {
        x[n] = 1;
    }
    for(int n=N1; n < Nt; n++)
    {
        x[n] = 0;
    }
    for(int i=Nt; i < floor(time*SampleRate); i++ )
    {
        x[i]=x[i%Nt];
    }
    if(N)
      *N = floor(time*SampleRate);
    return x;

}


double* tri_nes(double f_oc=800, double percent=0.75, double SampleRate=48000, double time=2, int* N=0)
{
    //отчетов сигнала в периоде импульса
    int Nt = SampleRate / f_oc;
    double* x = zeroes(0, floor(time*SampleRate));
    int N1 = 0.5 * Nt;
    for(int n=0; n < N1; n++)
    {
        x[n] = (1. / N1) * n;
    }
    for(int n=N1; n < Nt; n++)
    {
        x[n] = 1 - (1. / N1) * (n-N1);
    }
    for(int i=Nt; i < floor(time*SampleRate); i++ )
    {
        x[i]=x[i%Nt];
    }

    if(N)
      *N = floor(time*SampleRate);
    return x;

}


void abs_signal(std::vector<double>& signal)
{
    for (int j=0; j<signal.size()/2-1; j++)
    {
        std::complex<double> c(signal[2*j], signal[2*j+1]);
        signal[2*j] = std::abs(c);
        signal[2*j+1] = 0;
    }
}

double* noise_nes(double f_oc=800, double percent=0.75, double SampleRate=48000, double time=2, int* N=0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0.2,0.25);

    //отчетов сигнала в периоде импульса
    int Nt = SampleRate / f_oc;
    double* x = zeroes(0, floor(time*SampleRate));

    double t = 0;
    double dt = 1. / SampleRate;
    double phase = d(gen);
    for(int n=0; n < floor(time*SampleRate); n++)
    {
        //x[n] = 0;
        double A = sin((2*M_PI*f_oc/100.)*t);
        x[n]+= A * sin((2*M_PI*(2*f_oc))*t) ;
        x[n]+= cos((2*M_PI*(f_oc))*t+d(gen)) ;
        t+=dt;
        //x[n]+= 0.3*sin((2*M_PI/(f_oc-50))*n);
        //x[n] *= d(gen);
        //
        //p->setXY(0, n, x[n]);
    }


    int Nfft = 2048*4;
    std::vector<double> signalFFT = zeroesV(0, Nfft*2+1);
    for (int j=0; j<Nfft; j++) signalFFT[2*j+1] = x[j];
    four1(&signalFFT[0], Nfft,1);
    //abs_signal(signalFFT);
    //sort(signalFFT.begin(),signalFFT.end());
    //std::shuffle(signalFFT.begin(), signalFFT.end(), gen);
    //working with spectrum
    for(int j=0; j < Nfft-20; j++)
    {
        for(int k=2; k < 20; k+=20)
        {
            std::complex<double> a(0.2*signalFFT[2*j+1+k],0.2*signalFFT[2*j+2+k]);
            std::complex<double> b(signalFFT[2*j+1],signalFFT[2*j+2]);
            std::complex<double> c;
            c = a * b;
            signalFFT[2*j+1] = c.real();
            signalFFT[2*j+2] = c.imag();
        }
    }

    four1(&signalFFT[0], Nfft, -1);
    for (int j=1; j<Nfft; j++)
    {
        x[j] = signalFFT[2*j];
        //p->setXY(0, j, x[j]);
    }

    p->update_data();

    for(int i=Nfft; i < floor(time*SampleRate); i++ )
    {
        x[i]=x[i%Nfft];
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

