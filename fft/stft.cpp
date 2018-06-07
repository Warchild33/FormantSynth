// Original matlab code from http://labrosa.ee.columbia.edu/matlab/pvoc/
/* D = stft(X, F, W, H, SR)
 * Short-time Fourier transform.
    Returns some frames of short-term Fourier transform of x.  Each
    column of the result is one F-point fft (default 256); each
    successive frame is offset by H points (W/2) until X is exhausted.
       Data is hann-windowed at W pts (F), or rectangular if W=0, or
       with W if it is a vector.
       Without output arguments, will plot like sgram (SR will get
       axes right, defaults to 8000).
*/

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <QtGlobal>
#include "fft.h"
#include "stft.h"
#include "common.h"


complex_double** stft(double x[], int Nt, double f=256, double w=256, double h=0, double sr=8000, int* rows=0, int* cols=0)
{
    double *win;
    complex_double** d=0;

    if(w == 0)
        win = ones(0,f);
    else
    {
        if ( rem(w, 2) == 0 )   // force window to be odd-len
           w = w + 1;
        int halflen = (w-1)/2;
        int halff = f/2;     // midpoint of win
        double* halfwin = new double[sizeof(double)*halflen];
        for(int i=0; i < halflen; i++)
           halfwin[i] = 0.5 * ( 1 + cos((double)M_PI * (i)/halflen));
        win = zeroes(0, f);
        double acthalflen = qMin(halff, halflen);
        for(int i=0; i < acthalflen; i++)
            win[halff+i] = halfwin[i];
        int j=0;
        for(int i=halff; i > halff-acthalflen+1; i--)
        {
            win[i] = halfwin[j++];
        }

    }

    if (h == 0)
      h = floor(f/2);



    int s = Nt;

    // pre-allocate output array
    d = Make2DArray((1+f/2),1+floor((s-f)/h));

   double* u = new double[(int)f*sizeof(double)];
   double* data = new double[sizeof(double)*(int)f*2];

   int c = 0;
   *rows = 0;
   *cols = 0;
   for(int b=0; b < s-f; b+=h)
   {

       for(int i=b; i<b+f; i++)
       {
           u[i-b] = win[i-b]*x[i];
       }

       data[0] = 0;
       for(int n=0; n < f; n++)
       {
         data[2*n+1] = u[n];
         data[2*n+2] = 0;
       }
       four1(data,f,1);
       for(int j=0; j < f/2; j++)
       {
           d[j][c] = std::complex<double>(data[2*j+1], data[2*j+2]);
       }
       c = c + 1;
   }
   *cols = c;
   *rows = f/2;

   delete [] win;
   delete [] data;
   delete [] u;
   return d;
}



