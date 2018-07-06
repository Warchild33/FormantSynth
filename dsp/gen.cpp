#define _USE_MATH_DEFINES
#include <math.h>
#include <complex>
#include <algorithm>
#include "common.h"
#include "fft.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <complex>
#include <qwt/qwt_plot_marker.h>
#include <qwt/qwt_symbol.h>
#include "print.h"
#include "envelope.h"
#include "ploter.h"
#include "stft.h"
#include "wave_in.h"

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

void test_deBoor(int i_fc, int max, std::vector<double>& deBourFFT)
{
    double t[] = {0, 0, i_fc-10.0, i_fc-3.0, i_fc, i_fc+7.0, i_fc+7.0, i_fc+7.0, i_fc+7.0, i_fc+7.0};
    double c[] = {0, 0, max/2, 0, 0};
    p->clearvals(0);

    for(int k=0; k < 12; k++)
    for(double x=t[k]; x < t[k+1]; x+=2)
    {
        double s = deBoor(k, x, t, c, 2);
        std::complex<double> c;
        c = std::polar(s, x); //
        deBourFFT[x+1] = c.real();
        deBourFFT[x+2] = c.imag();
        //p->setXY(0,x+0.5,s);
    }

}

void mirror_spec(std::vector<std::complex<double> >& FFT)
{
    for(int i=0; i < FFT.size()/2; i++)
        FFT[FFT.size() - i - 1] = FFT[i];
}

void mirror_spec(std::vector<double>& FFT)
{
    for(int i=0; i < FFT.size()/2; i++)
        FFT[FFT.size() - i - 1] = FFT[i];
}


double* noise_nes(double f_oc=800, double percent=0.75, double SampleRate=48000, double time=2, int* N=0)
{
    double* x = zeroes(0, floor(time*SampleRate));

    std::vector<std::complex<double> > guitarFFT;
    std::vector<std::complex<double> > guitarTrsFFT;
    int Nfft = 2048*4;
    read_fft("./fft/E_guitar.fft", Nfft, guitarFFT);
    guitarTrsFFT.resize(Nfft+1);

    double Amax=-100000000000000;
    int i_max=0,j=0;
    for(auto i=guitarFFT.begin(); i != guitarFFT.begin()+2000; i++,j++  )
    {
         if( std::abs(*i) > Amax )
         {
             Amax = std::abs(*i);
             i_max = j;
         }
    }

    double df = (SampleRate / ( 2 * Nfft));
    double fE = 82.4;
    double kf = f_oc / fE;
    prn("imax=%d fE=%f kf=%f",i_max, fE, kf);

    for(int i=0; i < 200; i++)
    {
        for(int k= kf*i; k < kf*(i+1); k++)
            guitarTrsFFT[k] = guitarFFT[i];
    }

    for(int i=0; i < Nfft; i++)
    {
        std::complex<double> c(guitarTrsFFT[i]);
        guitarTrsFFT[i] = std::abs(c);
    }




    p->clearvals(1);
    mirror_spec(guitarTrsFFT);
    // SPECTRUM OUTPUT
    for(int j=0; j < Nfft; j++)
    {
        //p->setXY(1, (float)(SampleRate / Nfft) * j, std::abs(a));
        p->setXY(0, j, std::abs(guitarFFT[j]));
        p->setXY(1, j, std::abs(guitarTrsFFT[j]));

    }

    QwtSymbol *sym=new QwtSymbol(QwtSymbol::Diamond,QBrush(Qt::red),QPen(Qt::red),QSize(5,5));
    QwtPlotMarker *mark=new QwtPlotMarker;
    mark->setSymbol(sym);
    mark->setValue(i_max,Amax);//here you have to set the coordinate axis i.e. where the axis are meeting.
    mark->attach(p->plot);

    for (int j=1; j<Nfft; j++)
    {
        std::complex<double> a(guitarTrsFFT[j]);
        guitarTrsFFT[j] = std::abs(std::polar(100*std::abs(a), (double)j*M_PI ));
        //p->setXY(0, j, x[j]);
    }


    four1((double*)&guitarTrsFFT[0], Nfft, -1);
    four1((double*)&guitarFFT[0], Nfft, -1);
    //four1(&deBourFFT[0], Nfft, -1);

    for (int j=1; j<Nfft; j++)
    {
        std::complex<double> a(guitarTrsFFT[j]);
        x[j] = std::abs(a);
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


double* noise_nes1(double f_oc=800, double percent=0.75, double SampleRate=48000, double time=2, int* N=0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0.2,0.25);

    prn("f_oc=%f Hz", f_oc);
    p->clearvals(1);

    //отчетов сигнала в периоде импульса
    int Nt = SampleRate / f_oc;
    double* x = zeroes(0, floor(time*SampleRate));
    double* y = tri_nes(f_oc,percent,SampleRate,time,N);

    double t = 0;
    double dt = 1. / SampleRate;
    double phase = d(gen);
    for(int n=0; n < floor(time*SampleRate); n++)
    {
        //x[n] = 0;
        double A = sin((2*M_PI*f_oc/100.)*t);
        x[n]+= A * sin((2*M_PI*(f_oc))*t); //+ d(gen) ;
        t+=dt;
        //x[n]+= 0.3*sin((2*M_PI/(f_oc-50))*n);
        //x[n] *= d(gen);
        //
        //p->setXY(0, n, x[n]);
    }



    int Nfft = 2048*4;
    std::vector<double> signalFFT = zeroesV(0, Nfft*2+1);

    std::vector<double> triFFT = zeroesV(0, Nfft*2+1);

    for (int j=0; j<Nfft; j++) signalFFT[2*j+1] = x[j];
    for (int j=0; j<Nfft; j++) triFFT[2*j+1] = y[j];
    four1(&signalFFT[0], Nfft,1);
    four1(&triFFT[0], Nfft,1);

    //GENERATE SPECTURM BY DEBOUR SPLINE
    std::vector<double> deBourFFT = zeroesV(0, Nfft*2+1);
    double df = (2*SampleRate / (Nfft));
    int i_f = f_oc / df;
    prn("i_f=%d", i_f);
    //test_deBoor(i_f, 12000, deBourFFT);
    deBourFFT[i_f*4] = 10e9;
    mirror_spec(deBourFFT);

    QwtSymbol *sym=new QwtSymbol(QwtSymbol::Diamond,QBrush(Qt::red),QPen(Qt::red),QSize(5,5));
    QwtPlotMarker *mark=new QwtPlotMarker;
    mark->setSymbol(sym);
    mark->setValue(i_f,100);//here you have to set the coordinate axis i.e. where the axis are meeting.
    mark->attach(p->plot);

    //abs_signal(signalFFT);
    //sort(signalFFT.begin(),signalFFT.end());
    //std::shuffle(signalFFT.begin(), signalFFT.end(), gen);
    //working with spectrum


    // SPECTRUM OUTPUT
    for(int j=0; j < 100; j+=2)
    {        
        std::complex<double> a(deBourFFT[2*j+1],deBourFFT[2*j+2]);
        std::complex<double> b(signalFFT[2*j+1],signalFFT[2*j+2]);

        //p->setXY(1, (float)(SampleRate / Nfft) * j, std::abs(a));
        p->setXY(0, j, std::abs(a));
        p->setXY(1, j, std::abs(b));
    }

    for(int j=6; j < Nfft-20; j++)
    {
        //for(int k=2; k < 200; k+=50)
            std::complex<double> a(signalFFT[2*j+1],signalFFT[2*j+2]);
            std::complex<double> b(deBourFFT[2*j+1],deBourFFT[2*j+2]);
            std::complex<double> c;
            std::complex<double> e(triFFT[2*j+1],triFFT[2*j+2]);
            c =  a + b;
            signalFFT[2*j+1] = c.real();
            signalFFT[2*j+2] = c.imag();

    }



    four1(&deBourFFT[0], Nfft, -1);
    four1(&signalFFT[0], Nfft, -1);

    for (int j=1; j<Nfft; j++)
    {
        std::complex<double> a(signalFFT[2*j+1],signalFFT[2*j+2]);
        std::complex<double> b(deBourFFT[2*j+1],deBourFFT[2*j+2]);
        std::complex<double> c;
        c = std::polar((double)rand()/RAND_MAX, (double)rand()/RAND_MAX); //(double)rand()/RAND_MAX

        x[j] = std::abs(b);
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

