// Original matlab code from http://labrosa.ee.columbia.edu/matlab/pvoc/

#include "stft.h"
#include <QDebug>
#include "ploter.h"
#include "spectrogram.h"

//complex_double** stft(double x[], int Nt, double f=256, double w=256, double h=0, double sr=8000, int* rows=0, int* cols=0);
complex_double** pvsample(complex_double** b, double* t, int size_t, double hop, int rows, int cols, int* ocols);
double* istft(complex_double** d, int ftsize, int w=0, int h=0, int rows=0, int cols=0, int* ylen=0);
complex_double** some_filter(complex_double** b, int rows, int cols);

double* pvoc(double x[], double r, int n, int Nt, double stime, double fmax, int* ylen, Ploter* p1, Ploter* p2)
{
    //With hann windowing on both input and output,
    //we need 25% window overlap for smooth reconstruction
    int hop = n/4;

    // Effect of hanns at both ends is a cumulated cos^2 window (for
    // r = 1 anyway); need to scale magnitudes by 2/3 for
    // identity input/output
    double scf = 1.0;

    //Plot waveform
    p1->clearvals(0);
    for(int i=0; i < Nt; i+=100) p1->setXY(0,i,x[i]);
    p1->update_data();

    complex_double** d;
    int rows, cols;
    d = stft(x, Nt, n, n, hop, 8000, &rows, &cols);
    qDebug("rows=%d cols=%d",rows, cols);

    // Calculate the new timebase samples
    //[rows, cols] = size(X);
    double* t = (double*) malloc(sizeof(double)*int((double)cols/r));
    qDebug("%d",int((double)cols/r));
    int i=0;
    for(double tt=0; tt < cols; tt+=r)
    {
        t[i++] = tt;
        //qDebug("%f",tt);
    }

    //Generate the new spectrogram    
    //complex_double** X2 = pvsample(d, t, cols/r, hop, rows, cols, &cols);
    //some_filter(d, rows, cols);
    //Spectrogram* sg = new Spectrogram(d,stime,4000,rows,cols);

    //Invert to a waveform
    double* y = istft(d, n, n, hop, rows, cols, ylen);

    //Plot waveform
    int skip = *ylen / 1000;
//    p2->clearvals(0);
//    for(int i=0; i < *ylen; i+=skip)
//    {
//        if(abs(y[i]) < 1)
//            p2->setXY(0,i,y[i]);
//    }
//    p2->update_data();

    //free(X2);
    return y;

}
