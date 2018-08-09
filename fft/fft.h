#ifndef FFT_H
#define FFT_H
#include <QString>
#include <vector>
#include <complex>
    void four1(double data[], int nn, int isign);
    void read_fft(QString filename, int& Nfft, std::vector<std::complex<double> >& signalFFT);
    void write_fft(QString wave_file, QString fft_file);
#endif // FFT_H
