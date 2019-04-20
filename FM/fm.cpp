#define _USE_MATH_DEFINES
#include <math.h>
#include <QtConcurrentRun>
#include <QFuture>
#include <QFile>
#include "fm.h"
#include "gen.h"
#include "lfo.h"
#include "envelope.h"
#include "common.h"
#include "fm_envelope.h"
#include "ploter.h"
/*
double algo0(double A, double f_oc, double fm1, double fm2, double I1, double I2)
{
    double x;
    x = A*sinT(2 * M_PI * f_oc * t + I1*sinT( 2 * M_PI * fm1 * t ) + I2*sinT(2 * M_PI * fm2 * t)  );
    return x;
}
*/
extern Ploter* p;


FMSynth::FMSynth()
:Syntezer()
{
    TestEvenlope();
    TestTriangle();
    for(int i=0; i < 32; i++)
      algoFuncs[i] = &algo1;
   algoFuncs[2] = &algo2;
   algoFuncs[3] = &algo3;
   algoFuncs[5] = &algo5;
   algoFuncs[16] = &algo16;
   algoFuncs[17] = &algo17;
   algoFuncs[19] = &algo19;
   algoFuncs[32] = &algo32;

    n_test = 1;
    n_op_osc = 0;
    bShowOSC = false;


}

void FMSynth::SavePatch(QString filename)
{
    QFile file(filename);
    if( file.open(QFile::WriteOnly | QFile::Truncate) )
    {
        file.write((const char*)&gui_params, sizeof(gui_params));
        file.close();
    }
}

void FMSynth::LoadPatch(QString filename, int n)
{
    if(n < 0 || n > 10) return;
    QFile file(filename);
    if( file.open(QFile::ReadOnly | QFile::Truncate) )
    {
        file.read((char*)&patches[n], sizeof(gui_params));
        file.close();
    }
}

void FMSynth::SetCurrentPatch(int n)
{
    if(n < 0 || n > 10) return;
    gui_params = patches[n];
}

void FMSynth::TestEvenlope()
{
    FmParams params;
    params.level[3][0]=0;
    params.level[3][1]=99;     //attack
    params.level[3][2]=97;   //decay
    params.level[3][3]=0.0;   //sustain
    params.level[3][4]=0;   //release
    params.rate[3][0]=95;    //attack rate
    params.rate[3][1]=20;    //decay rate
    params.rate[3][2]=10;      //sustain rate
    params.rate[3][3]=10;    //release rate

    int level[4]={99,97,0,0};
    int rate[4]={95,20,10,10};
    EnvelopeFM ev(level, rate);

//    double t=0;
//    double dt=0.01;
//    p->clearvals(0);
//    for(t = 0; t < 5; t+=dt)
//    {
//        double ev = Evenlope(3, &params, t, false, 0);
//        p->setXY(0, t, ev);
//    }
//    p->update_data();
//    Evenlope(&params, 0.6);
//    Evenlope(&params, 1.5);
//    Evenlope(1, &params, 3);

}

void  FMSynth::TestTriangle()
{
    double t=0;
    double dt=0.0001;
    p->clearvals(0);
    for(t = 0; t < 0.1; t+=dt)
    {
        double x = triangle(800,48000,t);
        p->setXY(0, t, x);
    }
    p->update_data();


}

//testing feedback loop
double FMSynth::algotest(FmParams* p, double t)
{
    p->out[2] = p->I[2] * sin( 2 * M_PI * p->f[2] * t + p->out[2] );
    return p->I[1] * sin( 2 * M_PI * p->f[1] * t + p->out[2]);
}

static const int THREAD_COUNT = QThread::idealThreadCount();

double amp_mod(double Amax, double fvib, double t, double faze)
{
    return Amax * sin( 2 * M_PI * fvib * t );
}

//testing algo 19
double FMSynth::algo19(FmParams* p, double t, int n, bool bReleaseNote, double key_time)
{
    static double d[7];

    d[6] = amp_mod(p->d[6],0.5,t,p->faze[6]);
    d[4] = amp_mod(p->d[4],0.5,t,p->faze[4]) ;
    d[5] = amp_mod(p->d[5],0.5,t,p->faze[5]);

   // qDebug() << d[6];

    double ev = p->envelope[6]->render();
    p->out[6] = p->I[6] * sin( 2 * M_PI * (p->f[6]) * t + d[6]*p->out[6]);
    p->out[4] = ev * p->I[4] * sin( 2 * M_PI * (p->f[4]) * t + d[4]*p->out[6]);
    p->out[5] = ev * p->I[5] * sin( 2 * M_PI * (p->f[5]) * t + d[5]*p->out[6]);
    p->out[3] = p->I[3] * sin( 2 * M_PI * p->f[3] * t );
    p->out[2] = p->I[2] * sin( 2 * M_PI * p->f[2] * t + p->out[3] ) ;
    p->out[1] = ev *p->I[1] * sin( 2 * M_PI * p->f[1] * t + p->out[2]);
    return p->out[1] + p->out[4] + p->out[5];
}


double FMSynth::algo17(FmParams* p, double t, int n, bool bReleaseNote, double key_time)
{
    for(int i=1; i <=6; i++)
    {
        p->ev[i] = p->envelope[i]->render();
    }

    p->out[6] = p->lfo * p->ev[6] * p->I[6] * sin( 2 * M_PI * (p->f[6]+ p->d[6])* t);
    p->out[5] = p->lfo * p->ev[5] * p->I[5] * sin( 2 * M_PI * (p->f[5]+ p->d[5])* t+ p->out[6]);
    p->out[4] = p->lfo * p->ev[4] * p->I[4] * sin( 2 * M_PI * (p->f[4]+ p->d[4])* t);
    p->out[3] = p->lfo * p->ev[3] * p->I[3] * sin( 2 * M_PI * (p->f[3]+ p->d[3])* t+ p->out[4]);
    p->out[2] = p->lfo * p->ev[2] * p->I[2] * sin( 2 * M_PI * (p->f[2]+ p->d[2])* t+ p->out[2]);
    p->out[1] = p->lfo * p->ev[1] * p->I[1] * sin( 2 * M_PI * (p->f[1]+ p->d[1])* t+ p->out[2]+p->out[3]+p->out[5]);
    return p->out[1];

}

double FMSynth::algo16(FmParams* p, double t, int n, bool bReleaseNote, double key_time)
{
    //if( (n%100) == 0)
    for(int i=1; i <=6; i++)
    {
        p->ev[i] = p->envelope[i]->render();
    }

    p->out[6] = p->ev[6]* p->I[6] * sin( 2 * M_PI * (p->f[6])* t + p->out[6]);
    p->out[5] = p->ev[5] * p->I[5] * sin( 2 * M_PI * (p->f[5])* t+ p->out[6]);
    p->out[4] = p->ev[4] * p->I[4] * sin( 2 * M_PI * (p->f[4])* t);
    p->out[3] = p->ev[3] * p->I[3] * sin( 2 * M_PI * (p->f[3])* t+ p->out[4]);
    p->out[2] = p->ev[2] * p->I[2] * sin( 2 * M_PI * (p->f[2])* t);
    p->out[1] = p->ev[1] * p->I[1] * sin( 2 * M_PI * (p->f[1])* t+ p->out[2]+p->out[3]+p->out[5]);
    return p->out[1];
}

double FMSynth::algo2(FmParams* p, double t, int n, bool bReleaseNote, double key_time)
{
    //if( (n%100) == 0)
    for(int i=1; i <=6; i++)
    {
        p->ev[i] = p->envelope[i]->render();
    }

    p->out[6] = p->ev[6]* p->I[6] * sin( 2 * M_PI * (p->f[6])* t);
    p->out[5] = p->ev[5] * p->I[5] * sin( 2 * M_PI * (p->f[5])* t);
    p->out[4] = p->ev[4] * p->I[4] * sin( 2 * M_PI * (p->f[4])* t);
    p->out[3] = p->ev[3] * p->I[3] * sin( 2 * M_PI * (p->f[3])* t+ p->out[4]+p->out[5]+p->out[6]);
    p->out[2] = p->ev[2] * p->I[2] * sin( 2 * M_PI * (p->f[2])* t + p->out[2]);
    p->out[1] = p->ev[1] * p->I[1] * sin( 2 * M_PI * (p->f[1])* t + p->out[2]);
    return (p->out[1] + p->out[3])/2;
}

double FMSynth::algo3(FmParams* p, double t, int n, bool bReleaseNote, double key_time)
{
    //if( (n%100) == 0)
    for(int i=1; i <=6; i++)
    {
        p->ev[i] = p->envelope[i]->render();
    }

    p->out[6] = p->ev[6]* p->I[6] * sin( 2 * M_PI * (p->f[6])* t + p->out[6]);
    p->out[5] = p->ev[5] * p->I[5] * sin( 2 * M_PI * (p->f[5])* t);
    p->out[4] = p->ev[4] * p->I[4] * sin( 2 * M_PI * (p->f[4])* t);
    p->out[3] = p->ev[3] * p->I[3] * sin( 2 * M_PI * (p->f[3])*t);
    p->out[2] = p->ev[2] * p->I[2] * sin( 2 * M_PI * (p->f[2])* t);
    p->out[1] = p->ev[1] * p->I[1] * sin( 2 * M_PI * (p->f[1])* t + p->out[2] + p->out[3]);
    return (p->out[6] + p->out[5] + p->out[4] + p->out[1])/4;
}

double FMSynth::algo1(FmParams* p, double t, int n, bool bReleaseNote, double key_time)
{
    double A = 0.1;
    double f_oc = p->f[1];
    double S = f_oc / 200;
    double fm1 = f_oc;
    double fm2 = 4 * f_oc;
    static double I1=0, I2=0;

    //if( n==0 )
    {
        I1 = 17 * ( 8 - log(p->f[1])) / (log(p->f[1])*log(p->f[1]));
        I2 = 20 * ( 8 - log(p->f[1])) / f_oc;
    }

    //if( (n%100) == 0)
    for(int i=1; i <=6; i++)
    {
        p->ev[i] = p->envelope[i]->render();
    }
    return p->ev[1]*sin(2 * M_PI * f_oc * t + I1*sin( 2 * M_PI * (fm1 + S) * t ) + I2*sin(2 * M_PI * (fm2 + S) * t)  );

}



double FMSynth::algo32(FmParams* p, double t, int n, bool bReleaseNote, double key_time)
{
    //if( (n%100) == 0)
    for(int i=1; i <=6; i++)
    {
        p->ev[i] = p->envelope[i]->render();
    }

    p->out[6] = p->ev[6]* p->I[6] * sin( 2 * M_PI * (p->f[6])* t + p->out[6]);
    p->out[5] = p->ev[5] * p->I[5] * sin( 2 * M_PI * (p->f[5])* t);
    p->out[4] = p->ev[4] * p->I[4] * sin( 2 * M_PI * (p->f[4])* t);
    p->out[3] = p->ev[3] * p->I[3] * sin( 2 * M_PI * (p->f[3])* t);
    p->out[2] = p->ev[2] * p->I[2] * sin( 2 * M_PI * (p->f[2])* t);
    p->out[1] = p->ev[1] * p->I[1] * sin( 2 * M_PI * (p->f[1])* t);
    return (p->out[1] + p->out[2] + p->out[3] + p->out[4] + p->out[5] + p->out[6])/6;


}


FmParams FMSynth::getParams(double f_oc)
{
    FmParams param;
    int levels[4];
    int rates[4];
    const double OCTAVE_1024 = 1.0006771307;
    for(int i=0; i < 7; i++)
    {
        param.f[i] = f_oc * gui_params.f[i];
        param.I[i] = gui_params.I[i];
        param.d[i] = gui_params.d[i];
        param.f[i] = param.f[i] * pow(OCTAVE_1024, param.d[i]);

        param.algo_n = gui_params.algo_n;
        param.faze[i] = 2 * M_PI * (double) rand()/ RAND_MAX;
        for(int i=1; i <=6; i++)
        {
            for(int j=0; j < 4; j++)
            {
                param.level[i][j+1] = gui_params.level[i][j+1];
                param.rate[i][j] = gui_params.rate[i][j];
                levels[j] = gui_params.level[i][j+1];
                rates[j] = gui_params.rate[i][j];
            }
            param.envelope[i] = new EnvelopeFM(levels, rates);
        }

    }

    return param;
}

double FMSynth::find_max_release_rate(FmParams& param)
{
    double max = 0;
    for(int i=1; i <=6; i++)
        for(int j=0; j < 4; j++)
        {
             if( max < gui_params.rate[i][3] )
                 max = gui_params.rate[i][3];
        }
    return max;
}

void setLfoParams(FmParams* fm_params)
{
    LfoParams lfo_params;
    lfo_params.lfoPitchModDepth = fm_params->pitchModDepth;
    setGlobalLfoParams(lfo_params);
}

void FMSynth::Algorithm(AlgoParams& param)
{
    param.fm_params = getParams(param.f_oc);

    double x;
    double t = 0;
    double dt = 1. / 48000;

    if( bShowOSC)
       p->clearvals(0);

    Lfo lfo(0);
    setLfoParams(&gui_params);

    for(int n=param.offset; n < param.offset + floor((param.time)*48000); n++) //
    {
        //lfo
        double T_lfo = 1./2;

        //param.fm_params.lfo = lfo.renderAmp();//0.7 + 0.3*triangle(2,48000,t);
        x = algoFuncs[param.fm_params.algo_n](&param.fm_params, t, n, param.bReleaseNote, param.key_time);

        if( param.buffer->bWrited) break;
        param.buffer->samplesD[n*2] =  (param.buffer->samplesD[n*2] + x);
        param.buffer->samplesD[n*2+1] =(param.buffer->samplesD[n*2] + x);
        short sample = param.buffer->samplesD[n*2] * 5000;
        param.buffer->samples[n*2] = sample;
        param.buffer->samples[n*2+1] = sample;

        if(n == 100 && bDoOutBuffer)
            out_buffer(param.buffer);

        t+=dt*lfo.render();

        //p->setXY(0, t, lfo.render());
        //if(n % 100 == 0)
        //if(n < 10000)
        //  p->setXY(0, t, x);
        if( bShowOSC)
        {
            if(n_op_osc==0)
                p->setXY(0, t, x);
             else
                p->setXY(0, t, param.fm_params.out[n_op_osc]);

        }

    }

    if( bShowOSC)
    {
      p->set_autoscale(true);
      p->zoomer->setAxis(QwtPlot::xBottom, QwtPlot::yRight);
      p->update_data();
    }
}

double FMSynth::algo5(FmParams* p, double t, int n, bool bReleaseNote, double key_time)
{
    for(int i=1; i <=6; i++)
    {
        p->ev[i] = p->envelope[i]->render();
    }

    p->out[6] = p->lfo * p->ev[6] * p->I[6] * sin( 2 * M_PI * (p->f[6]) * t + p->out[6] ) * p->lfo;
    p->out[5] = p->lfo * p->ev[5] * p->I[5] * sin( 2 * M_PI * (p->f[5]) * t + p->out[6] ) * p->lfo ;
    p->out[4] = p->lfo * p->ev[4] * p->I[4] * sin( 2 * M_PI * (p->f[4])* t) * p->lfo ;
    p->out[3] = p->lfo * p->ev[3] * p->I[3] *  sin( 2 * M_PI *  p->f[3] * t + p->out[4]) * p->lfo;
    p->out[2] = p->lfo * p->ev[2] * p->I[2] * sin( 2 * M_PI * (p->f[2])* t) * p->lfo;
    p->out[1] = p->lfo * p->ev[1] * p->I[1] * sin( 2 * M_PI * (p->f[1])* t+ p->out[2]) * p->lfo;
    return (p->out[1] + p->out[3] + p->out[5])/3;
}


//double FMSynth::algo1(FmParams* p, double t)
//{
//    double OP6 = p->I[6] * sin( 2 * M_PI * p->f[6] * t +  p->I[6] *sin( 2 * M_PI * p->f[6] * t ) );
//    double OP5 = p->I[5] * sin( 2 * M_PI * p->f[5] * t );
//    double OP4 = p->I[4] * sin( 2 * M_PI * p->f[4] * t );
//    double OP3 = p->I[3] * sin( 2 * M_PI * p->f[3] * t + OP4 + OP5 + OP6 );
//    double OP2 = p->I[2] * sin( 2 * M_PI * p->f[2] * t );
//    double OP1 = p->I[1] * sin( 2 * M_PI * p->f[1] * t + OP2);
//    return OP1 + OP3;

//}


void FMSynth::write_note(Buffer* buffer, long offset, char note, double duration)
{
    float f = freq_table.getFreq(note);
    AlgoParams param;
    param.bReleaseNote = false;
    param.buffer = buffer;
    param.f_oc = f;
    param.offset = offset;
    param.key_time = 0;
    param.time = duration;
    bShowOSC = false;
    bDoOutBuffer = false;
    Algorithm(param);
}

Buffer* FMSynth::play_note(char note, double duration, double velocity)
{
    Buffer* buffer = new Buffer(48000,duration*1.5,2);
    //qDebug() << "starting thread  " << QThread::currentThreadId();

    float f = freq_table.getFreq(note);

    AlgoParams param;
    param.bReleaseNote = false;
    param.buffer = buffer;
    param.f_oc = f;
    param.key_time = 0;
    param.time = duration;

    if(!bShowOSC)
    {
        if( bAsynch )
            QtConcurrent::run(this, &FMSynth::Algorithm, param);
        else
            Algorithm(param);
    }
    else
        Algorithm(param);


    return buffer;
}

double FMSynth::release_note(Buffer* buffer, char note, double key_time)
{
    return 0;

}
