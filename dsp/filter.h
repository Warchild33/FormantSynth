#ifndef FILTER_H
#define FILTER_H

double* narrow_band_filter(double* x, double* y, int Nt, double f0, double BW, double SampleRate=48000);
double* biquad_irr_filter(double* x, int Nt, double dBgain, double f0, double BW, double SampleRate=48000);
double* Filter (const double in[], int Nt, double F1, double F2);

#endif // FILTER_H
