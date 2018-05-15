#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include "stft.h"
#include <QLabel>
#include <QDoubleSpinBox>

class Spectrogram : public QLabel
{
    Q_OBJECT
public:
    QDoubleSpinBox*  porog;
    int sg_width;
    int sg_height;
    complex_double** mX2;
    double mstime;
    double mfmax;
    int mrows;
    int mcols;
    Spectrogram(complex_double** X2, double stime, double fmax, int rows, int cols);
    void Build(complex_double** X2, double stime, double fmax, int rows, int cols, double porog);
    void resizeEvent(QResizeEvent* e);

public slots:
    void porogChanged(double p);
};

#endif // SPECTROGRAM_H
