#ifndef LFO_H
#define LFO_H

struct LfoParams
{
    int lfoWaveform;
    int lfoPitchModSens;
    int lfoPitchModDepth;
    int lfoAmpModDepth;
    int lfoDelay;
    int controllerModVal;
};

class Lfo
{
public:
    Lfo(int lfoAmpModSens);
    double renderAmp();
    void   update();
    double render();
    int lfoSpeed;
    double lfoAmpModSens;
    double phase;
    double pitchVal;
    int counter;
    double ampVal;
    double ampValTarget;
    double ampIncrement;
    double delayVal;
    int delayState;

};

#endif // LFO_H
