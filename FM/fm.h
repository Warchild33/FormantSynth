#ifndef FM_H
#define FM_H
#include <QFuture>
#include <QFutureWatcher>
#include "syntezer.h"

struct FmParams
{
    FmParams()
    {
        for(int i=0; i<7;i++)
        {
            out[i] = 0;
            d[i] = 0;
            f[i] = 1;
            for(int j=0; j<4;j++)
            {
                rate[i][j] = 1;
                level[i][j] = 0;
            }
        }

    }
    double faze[7];
    double f_oc;
    double A;
    double f[7];
    double I[7];
    double d[7];
    //evenlope
    double rate[7][4];
    double level[7][5];
    double out[7];
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

class FMSynth : public Syntezer
{
    Q_OBJECT
public:

    FMSynth();
    FmParams  gui_params;
    int n_test;
    double t_last;
    double algo1(FmParams* p, double t);
    double algo17(FmParams* p, double t);
    double algo19(FmParams* p, double t, int n);
    double algo5(FmParams* p, double t, int n);
    Buffer* play_note(char note, double duration, double velocity);
    double release_note(Buffer* buffer, char note, double key_time);
    double* Test1(Buffer* buffer, double f_oc, double SampleRate, double time, int* N);
    double* Test2(Buffer* buffer,double f_oc, double SampleRate, double time, bool bReleaseNote);
    double* Test3(Buffer* buffer,double f_oc, double SampleRate, double time, bool bReleaseNote);
    void TestEvenlope();
    double Evenlope(int op_index, FmParams* params, double t);
    void selectTest(float f, double duration, int N, bool bReleaseNote);


public slots:
    void handleFinished();
};
#endif // FM_H
