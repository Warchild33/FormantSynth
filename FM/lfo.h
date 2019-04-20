#ifndef LFO_H
#define LFO_H

class LfoParams
{
public:
    LfoParams()
    {
        controllerModVal = 0;
        lfoAmpModDepth = 0;
        lfoDelay = 33;
        lfoPitchModDepth = 20;
        lfoPitchModSens = 3;
        lfoWaveform = 4;
    }
    int lfoWaveform;
    int lfoPitchModSens;
    int lfoPitchModDepth;
    int lfoAmpModDepth;
    int lfoDelay;
    int controllerModVal;
};

void setGlobalLfoParams(LfoParams& p);

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
