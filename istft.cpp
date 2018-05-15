// X = istft(D, F, W, H)                   Inverse short-time Fourier transform.
//	Performs overlap-add resynthesis from the short-time Fourier transform
//	data in D.  Each column of D is taken as the result of an F-point
//	fft; each successive frame was offset by H points (default
//	W/2, or F/2 if W==0). Data is hann-windowed at W pts, or
//       W = 0 gives a rectangular window (default);
//       W as a vector uses that as window.
//       This version scales the output so the loop gain is 1.0 for
//       either hann-win an-syn with 25% overlap, or hann-win on
//       analysis and rect-win (W=0) on synthesis with 50% overlap.

#include "stft.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <QtGlobal>

double* ones(int i1, int i2);
double* zeroes(int i1, int i2);
double rem(double x, double y);
void four1(double data[], int nn, int isign);

complex_double conj(complex_double d)
{
    complex_double o;
    o.re = d.re;
    o.im = -d.im;
    return o;
}

double* istft(complex_double** d, int ftsize, int w=0, int h=0, int rows=0, int cols=0, int* ylen=0)
{
    // special case: rectangular window
    double* win;
    if(w == 0)
        win = ones(0,ftsize);
    else
    {
        if ( rem(w, 2) == 0 )   // force window to be odd-len
           w = w + 1;
        int halflen = (w-1)/2;
        int halff = ftsize/2;     // midpoint of win
        double* halfwin = (double*)malloc(sizeof(double)*halflen);
        for(int i=0; i < halflen; i++)
           halfwin[i] = 0.5 * ( 1 + cos((double)M_PI * (i)/halflen));
        win = zeroes(0, ftsize);
        double acthalflen = qMin(halff, halflen);
        for(int i=0; i < acthalflen; i++)
            win[halff+i] = halfwin[i];
        int j=0;
        for(int i=halff; i > halff-acthalflen+1; i--)
        {
            win[i] = halfwin[j++];
            //qDebug("win[%d]=%f",i,win[i]);
        }

    }

    if( h == 0)
      h = floor((double)ftsize/2);

    int xlen = ftsize + (cols-1)*h;
    double* x = zeroes(0,xlen);
    *ylen = xlen;

  complex_double* ft = (complex_double*)malloc(sizeof(complex_double)*rows*2);
  double* data = (double*)malloc(sizeof(double)*ftsize*2);

  for(double b=0; b < h*(cols-1); b+=h)
  {
      for(int i=0; i<2*rows; i++) { ft[i].im = 0; ft[i].re = 0; }

      for(int i=0; i<rows; i++)
            ft[i] = d[i][(int)(b/h)];

        int j = ftsize/2;

        for(int i=rows; i<2*rows; i++)
        {
            ft[i] = conj(ft[j--]);
        }
        data[0] = 0;
        for(int n=0; n < ftsize; n++)
        {
          data[2*n+1] = ft[n].re;
          data[2*n+2] = ft[n].im;
        }
        four1(data,ftsize,-1);
        for(int j=0; j < ftsize; j++)
        {
           x[(int)b+j] = x[(int)b+j] + (1./ftsize)*data[2*j+1] * win[j];
        }
  }
  //free(ft);
  //free(data);
  //free(win);
  return x;
}
