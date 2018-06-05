#include <math.h>
#include <complex>
#include "ploter.h"
#include "common.h"
#include "fft.h"

extern Ploter* p;

double* narrow_band_filter(double* x, double* y, int Nt, double f0, double BW, double SampleRate=48000)
{

    double w0 = 2 * M_PI * f0 / SampleRate;
    double R = 1 - 3 * BW;
    double K = (1 - 2*R*cos(w0) + R*R) /(2 - 2*cos(w0));
    double a0 = 1 - K;
    double a1 = 2*(K-R)*cos(w0);
    double a2 = R*R - K;
    double b1 = 2*R*cos(w0);
    double b2 = -R*R;

    //отчетов сигнала в периоде импульса
    for(int n=2; n < Nt; n++)
    {
      y[n] = a0*x[n] + a1*x[n-1] + a2*x[n-2] + b1*y[n-1] + b2*y[n-2];
      //qDebug("%f",y[n]);
    }

    return y;
}

double* biquad_irr_filter(double* x, int Nt, double dBgain, double f0, double BW, double SampleRate=48000)
{
      double A  = sqrt( pow(10,(dBgain/20)) );
      double w0 = 2 * M_PI * f0 / SampleRate;
      double alpha = sin(w0)*sinh( log((double)2)/2 * BW * w0/sin(w0) );
      double b0 =   alpha;
      double b1 =   0;
      double b2 =  -alpha;
      double a0 =   1 + alpha;
      double a1 =  -2*cos(w0);
      double a2 =   1 - alpha;

      double* y = zeroes(0, Nt);
      //clean x
      for(int n=0; n < Nt; n++)
      {
          //if(x[n]>)
          y[n] = x[n];
      }
      for(int n=2; n < Nt; n++)
      {
        y[n] = (b0/a0)*x[n] + (b1/a0)*x[n-1] + (b2/a0)*x[n-2] - (a1/a0)*y[n-1] - (a2/a0)*y[n-2];
        //qDebug("%f",y[n]);
      }

      return y;
}


double* Filter (const double in[], int Nt, double F1, double F2)
{
    const int N = 256; //Длина фильтра
    long double Fd = 48000; //Частота дискретизации входных данных
    //long double F1 = 0; //Частота полосы пропускания
    //long double F2 = 400; //Частота полосы затухания

    long double H [N] = {0}; //Импульсная характеристика фильтра
    long double H_id [N] = {0}; //Идеальная импульсная характеристика
    long double H_id2 [N] = {0}; //Идеальная импульсная характеристика
    long double W [N] = {0}; //Весовая функция


    //Расчет импульсной характеристики фильтра
    double Fc1 = F1 / (2 * Fd);
    double Fc2 = F2 / (2 * Fd);

    //LOWPASS
    for (int i=0;i<N;i++)
    {
        if (i==0) H_id[i] = 2*M_PI*Fc1;
        else H_id[i] = sinl(2*M_PI*Fc1*i )/(M_PI*i);
    }

    //HIPASS
    for (int i=0;i<N;i++)
    {
        if (i==0) H_id2[i] = 2*M_PI*Fc2;
        else H_id2[i] = sinl(2*M_PI*Fc2*i )/(M_PI*i);
        if (i%2==0) H_id2[i] = -1.0 * H_id2[i];
    }

    for (int i=0;i<N;i++)
    {
        // весовая функция Блекмена
        W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( N-1)) + 0.08 * cosl((4*M_PI*i) /( N-1));
        H [i] = H_id2[i] * W[i];
    }


    //Нормировка импульсной характеристики
    double SUM=0;
    for (int i=0; i<N; i++) SUM +=H[i];
    for (int i=0; i<N; i++) H[i]/=SUM; //сумма коэффициентов равна 1

    //Расчет Nfft
    int Nfft = pow(2,(ceil(log((double)N+Nt-1)))); //% or 2^nextpow2(L+M-1)

    //FFT фильтра
    double* filterFFT = zeroes(0, Nfft*2);
    for(int i=0; i<N; i++) filterFFT[2*i+1] = H[i];
    four1(filterFFT, Nfft,1);


    double* y = zeroes(0, Nt);
    double* W2 = zeroes(0, Nfft);
    for (int i=0;i<Nfft;i++)
    {
        W2[i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( Nfft-1)) + 0.08 * cosl((4*M_PI*i) /( Nfft-1));
        //qDebug("%f",W2[i]);
    }

    //FFT сигнала
/*
    double* signalFFT = zeroes(0, Nfft*2);
    for (int i=0; i<Nt-Nfft; i+=Nfft)
    {
        for (int j=i; j<i+Nfft; j++) signalFFT[2*(j-i)+1] = in[j];
        four1(signalFFT, Nfft,1);
        //свертка
        for(int j=0; j < Nfft; j++)
        {
            std::complex<double> a(filterFFT[2*j+1],filterFFT[2*j+2]);
            std::complex<double> b(signalFFT[2*j+1],signalFFT[2*j+2]);
            std::complex<double> c;
            c = a * b;
            //signalFFT[2*j+1] = c.real();
            //signalFFT[2*j+2] = c.imag();
        }
        //обратное fft
        four1(signalFFT, Nfft, -1);
        for(int j=0; j < Nfft; j++)
            y[i+j] = signalFFT[2*j+1];

    }

*/
    //Фильтрация входных данных time domain
   p->clearvals(0);
    for (int i=N; i<Nt; i++)
    {
        y[i]=0.;
        for (int j=0; j<N-1; j++)// та самая формула фильтра
        {
            y[i]+= H[j]*in[i-j];
            p->setXY(0, i, y[i]);
        }
    }

    p->update_data();
    return y;
}

