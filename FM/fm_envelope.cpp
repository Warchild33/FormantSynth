#include <QDebug>
#include <math.h>
#include "fm_envelope.h"
#include "ploter.h"

int outputlevel[] = {0, 5, 9, 13, 17, 20, 23, 25, 27, 29, 31, 33, 35, 37, 39,
    41, 42, 43, 45, 46, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
    62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
    81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
    115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127};

double outputLUT[4096];


extern Ploter* p;

EnvelopeFM::EnvelopeFM(int levels[4], int rate[4])
{
    for(int i=0; i < 4; i++)
    {
        this->levels[i] = levels[i];
        this->rate[i] = rate[i];
    }
    for (int i = 0; i < 4096; i++) {
        double dB = (i - 3824) * 0.0235;
        outputLUT[i] = pow(20, (dB/20));
    }

    level = 0;
    state = 0;
    i = 0;
    down = true;
    rising = true;
    decayIncrement = 0;
    advance(0);

//    p->clearvals(0);
//    for(int n=0; n < (48000); n++)
//    {
//        p->setXY(0, n, render());
//    }
//    p->update_data();

}

double EnvelopeFM::render()
{
    if (state < 3 || (state < 4 && !down)) {
            double lev;
            lev = level;
            if (rising) {
                lev += decayIncrement * (2 + (targetlevel - lev) / 256);
                if (lev >= targetlevel) {
                    lev = targetlevel;
                    advance(state + 1);
                }
            } else {
                lev -= decayIncrement;
                if (lev <= targetlevel) {
                    lev = targetlevel;
                    advance(state + 1);
                }
            }
            level = lev;
        }
        i++;

        // Convert DX7 level -> dB -> amplitude
    return outputLUT[(int)floor(level)];
}

void EnvelopeFM::advance(int newstate)
{
    state = newstate;
        if (state < 4) {
            int newlevel = levels[state];
            targetlevel = qMax(0, (outputlevel[(int)newlevel] << 5) - 224); // 1 -> -192; 99 -> 127 -> 3840
            rising = (targetlevel - level) > 0;
            int rate_scaling = 0;
            int qr = qMin(63, rate_scaling + ((rate[state] * 41) >> 6)); // 5 -> 3; 49 -> 31; 99 -> 63
            decayIncrement = pow(2, qr/4) / 2048;
    }
}
