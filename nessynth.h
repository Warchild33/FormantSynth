#ifndef NESSYNTH_H
#define NESSYNTH_H

#include "syntezer.h"


class NesSynth : public Syntezer
{
    Q_OBJECT
public:
    NesSynth();

    Buffer* play_note(char note, double duration, double velocity);


};
#endif // NESSYNTH_H
