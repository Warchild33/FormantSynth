#ifndef FM_H
#define FM_H
#include <QFuture>
#include <QFutureWatcher>
#include "syntezer.h"
#include "fm_envelope.h"

struct FmParams
{
    FmParams()
    {
        for(int i=0; i<7;i++)
        {
            out[i] = 0;
            d[i] = 0;
            f[i] = 1;
            ev[i] = 0;
            lfo = 1;
            for(int j=0; j<4;j++)
            {
                rate[i][j] = 1;
                level[i][j] = 0;
            }
        }
        algo_n = 17;
    }
    double faze[7];
    double f[7];
    double I[7];
    double d[7];
    double ev[7];
    //evenlope
    double rate[7][4];
    double level[7][5];
    double out[7];
    EnvelopeFM* envelope[7];
    double lfo;
    int    algo_n;    
};


class MyWatcher : public QFutureWatcher<double*>
{
    Q_OBJECT
public:
    MyWatcher()
    :QFutureWatcher<double*>(){};
    double duration;
    int N;
    Buffer* buffer;
    char note;
};

struct AlgoParams
{
    AlgoParams()
    {
        offset = 0;
    }
    Buffer* buffer;
    double f_oc;
    double time;
    bool bReleaseNote;
    double key_time;
    FmParams fm_params;
    long offset;
};

class FMSynth : public Syntezer
{
    Q_OBJECT
public:

    FMSynth();
    FmParams  gui_params;
    FmParams  patches[10];
    int n_test;
    bool   bShowOSC;
    int    n_op_osc;
    double algotest(FmParams* p, double t);
    double algo1(FmParams* p, double t, int n, bool bReleaseNote, double key_time);
    double algo19(FmParams* p, double t, int n);
    double algo5(FmParams* p, double t, int n, bool bReleaseNote, double key_time);
    double algo17(FmParams* p, double t, int n, bool bReleaseNote, double key_time);
    double algo16(FmParams* p, double t, int n, bool bReleaseNote, double key_time);
    double algo32(FmParams* p, double t, int n, bool bReleaseNote, double key_time);
    double algo2(FmParams* p, double t, int n, bool bReleaseNote, double key_time);
    double algo3(FmParams* p, double t, int n, bool bReleaseNote, double key_time);
    Buffer* play_note(char note, double duration, double velocity);
    double release_note(Buffer* buffer, char note, double key_time);
    double* Test1(Buffer* buffer, double f_oc, double SampleRate, double time, int* N);
    double* Test2(Buffer* buffer,double f_oc, double SampleRate, double time, bool bReleaseNote);
    double* Test3(Buffer* buffer,double f_oc, double time, bool bReleaseNote, double key_time);
    void  Algorithm(AlgoParams& param);
    void  TestEvenlope();
    double Evenlope(int op_index, FmParams* params, double t, bool bReleaseNote, double key_time);
    double Evenlope2(int op_index, FmParams* params, double t, bool bReleaseNote, double key_time);
    double find_max_release_rate(FmParams& param);
    void selectTest(float f, double duration, int N, bool bReleaseNote);
    void write_note(Buffer* buffer, long offset, char note, double duration);
    void SavePatch(QString filename);
    void LoadPatch(QString filename, int n);
    void SetCurrentPatch(int n);

    FmParams getParams(double f_oc);

public slots:

};
#endif // FM_H
