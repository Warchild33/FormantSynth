#include "test.h"
#include "envelope.h"
#include "gen.h"
#include "filter.h"
#include "common.h"
#include "ploter.h"
#include "wave_in.h"

extern Ploter* p;

Test::Test()
{
}

//Pnt spline_kochaken(Pnt P1, Pnt P2, Pnt P3, Pnt P4, float t,
//           double tension, double bias, double continuity)


void Test::startup_tests()
{
    //test_spline_kochaken();
    //test_deBoor();
    //test_nes_gen();
}

void Test::test_deBoor()
{
    double t[] = {0, 0, 0, 1, 2, 3, 3, 3};
    double c[] = {0, 0, 4, 0, 0};
    p->clearvals(0);

    for(int k=0; k < 11; k++)
    for(double x=t[k]; x < t[k+1]; x+=0.01)
    {
        double s = deBoor(k, x, t, c, 2);
        p->setXY(0,x+0.5,s);
    }
    p->update_data();
}

void Test::test_spline_kochaken()
{
    Pnt P1, P2, P3, P4;

    double tension=-0.5, bias=0, continuity=0;

    P1 = Pnt(0,0,0);
    P2 = Pnt(20,0,1);
    P3 = Pnt(70,0,1);
    P4 = Pnt(100,0,0);

    p->clearvals(0);
    for(double t=0; t < 1; t+=0.01)
    {
        Pnt sp = spline_kochaken(P1,P2,P3,P4,t,tension,bias,continuity);
        p->setXY(0,sp.x,sp.z);
    }
    p->update_data();



}

void Test::test_gen_voice(double f_ot, double F1, double F2, double F3, double BW, int Ncascade)
{
    int N;
    //генерация импульсов
    double* x = pulse_gen1(f_ot,48000,2,&N);
    //WAVEFORMATEX wfmt;
    //double* x = wavread("1.wav",&wfmt, &N);
    //полосовой фильтр
    //double* y = biquad_irr_filter(x,N,ui->Gain->value(),ui->F->value(),ui->BW->value(),48000);
    //double* y = Filter(x, N, ui->F->value(),ui->F->value());
    //double* y2 = Filter(x, N,2200,2400);
    //double* y3 = Filter(x, N,2900,3100);
    short* data = (short*)malloc(N*sizeof(short));
    for(int i=0; i<N; i++)
        data[i] = x[i] * 32768;
    wavwrite("pulse.wav",data,N*2,48000,1);
    double* y = zeroes(0, N);
    double* y2 = zeroes(0, N);
    double* y3 = zeroes(0, N);
    for(int s=0; s<Ncascade; s++)
    {
        if(s>0) for(int i=0; i<N; i++) x[i] = y[i];
        narrow_band_filter(x,y,N,F1,BW,48000);
        if(s>0) for(int i=0; i<N; i++) x[i] = y2[i];
        narrow_band_filter(x,y2,N,F2,BW,48000);
        if(s>0) for(int i=0; i<N; i++) x[i] = y3[i];
        narrow_band_filter(x,y3,N,F3,BW,48000);
    }

    //огибающая
    double Amax = (*std::max_element(&y[0],&y[N-1]));
    //envelope_cos(y,Amax,48000,2);
    //envelope_Bspline(y,Amax,48000,2);

    p->clearvals(0);
    for(int i=0; i<N; i++)
    {
       p->setXY(0,i,y[i]);
       data[i] = ((y[i]+y2[i]+y3[i])*3) * 32768;
    }
    p->update_data();
    wavwrite("./wave/pulse_filt.wav",data,N*2,44000,1);
    write_raw_samples("./wave/pulse_filt.raw",data,N*2);
}


void Test::test_nes_gen()
{
    //double* pulse_nes(double f_oc=800, double percent, double SampleRate=48000, double time=2, int* N=0)
    int N;
    double* x = pulse_nes(100, 0.75,48000,1,&N);
    p->clearvals(0);
    for(int i=0; i < 1000; i++ )
    {
        p->setXY(0,i,x[i]);
    }
    p->update_data();
    wavwrite2("./wave/nes_pulse75.wav",x,N,48000,1);
    x = pulse_nes(100, 0.50, 48000,1,&N);
    wavwrite2("./wave/nes_pulse50.wav",x,N,48000,1);
    x = pulse_nes(100, 0.25, 48000,1,&N);
    wavwrite2("./wave/nes_pulse25.wav",x,N,48000,1);
    x = pulse_nes(100, 0.125, 48000,1,&N);
    wavwrite2("./wave/nes_pulse125.wav",x,N,48000,1);
}
