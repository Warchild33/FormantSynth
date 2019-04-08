#ifndef FM_ENVELOPE_H
#define FM_ENVELOPE_H

class EnvelopeFM
{
public:
    EnvelopeFM(int levels[4], int rate[4]);
    int levels[4];
    int rate[4];

    double level, targetlevel;
    int i, state;
    bool down, rising;
    double decayIncrement;
    void advance(int newstate);
    double render();


};

#endif // FM_ENVELOPE_H
