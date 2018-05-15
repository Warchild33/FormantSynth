#ifndef FORMANTSYNT_H
#define FORMANTSYNT_H

#include <vector>
#include "syntezer.h"


class FormantSynt : public Syntezer
{
    Q_OBJECT
public:
    FormantSynt();

    void get_params_for(char vowel);
    Buffer* play_note(char note, double duration, double velocity);

    double F1;
    double F2;
    double F3;
    double BW;
    int Ncascade;
    char cur_vowel;
    void apply_filters(double* y2, float k, int N, double* y3, double* x, double* y);
    void apply_filters(double* y, float k, double* x, double* y3, int N, double* y2);
    void alloc_buffer(double duration);


};

#endif // FORMANTSYNT_H
