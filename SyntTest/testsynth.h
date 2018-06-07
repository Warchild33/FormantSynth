#ifndef NESSYNTH_H
#define NESSYNTH_H

#include "syntezer.h"

enum Gen_type {SQUARE, TRIANGLE, NOISE, NOISE2, GUITAR};

class TestSynth : public Syntezer
{
    Q_OBJECT
public:
    Gen_type gen_type;
    double   percent;
    TestSynth();

    Buffer* play_note(char note, double duration, double velocity);
    void set_percent(double p){ percent = p; }


};
#endif // NESSYNTH_H
