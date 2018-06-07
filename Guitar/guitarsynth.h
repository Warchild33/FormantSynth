#ifndef GUITARSYNTH_H
#define GUITARSYNTH_H

#include "syntezer.h"
#include "stft.h"

class String
{
public:
    String();
    String(QString name, double f, QString wave_file);
    double* gen(double f, double time);

    double  Fs;
    int     Nfft;
    QString name;
    complex_double** fourie_samples;
    complex_double** d;
    int              rows, cols;
};


class GuitarSynth : public Syntezer
{
public:
    GuitarSynth();
    Buffer* play_note(char note, double duration, double velocity);
    double* gen(double f, int SampleRate, double time);
    void draw_fft_frame(int plot, int col, int n_rows);
private:
    String* strings[6];

};

#endif // GUITARSYNTH_H
