#define _USE_MATH_DEFINES
#include <math.h>
#include <QtConcurrentRun>
#include <QtConcurrent>
#include <QFuture>
#include "fm.h"
#include "envelope.h"
#include "common.h"
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

double sine_table[1000] = {0};

inline double sinT(double angle_rad);

FMSynth::FMSynth()
:Syntezer()
{
    TestEvenlope();
    n_test = 1;

    //generate sin table
    for (int i = 1; i <= 1000; i++)
    {
        sine_table[i-1] = std::sin(2 * M_PI * i/ 1000.0);
    }
    sinT( -4*M_PI );
    sinT( 4*M_PI );
}

inline double sinT(double angle_rad)
{
    if(angle_rad!=angle_rad)
        return 0;

    if( angle_rad < 0 )
    {
        angle_rad+=int(abs(angle_rad) / (2 * M_PI)) * 2 * M_PI + (2 * M_PI);
    }

    if( angle_rad > (2 * M_PI) )
    {
        angle_rad-=int(abs(angle_rad) / (2 * M_PI)) * 2 * M_PI ;
    }
   // qDebug() << "angle_rad=" << angle_rad;
   //return 0;

   int i = (angle_rad/(2*M_PI)) * 1000.0;
   return sine_table[i];
}

//inline double sinT(double angle)
//{
//    double x5=angle,x3=angle,x7=angle;
//    for(int i=0; i < 3; i++ )
//        x3*=x3;
//    for(int i=0; i < 5; i++ )
//        x5*=x5;
//    for(int i=0; i < 7; i++ )
//        x7*=x7;
//    value = angle - x3/6 + x5/120 - x7/5040;
//    return value;
//}

void FMSynth::TestEvenlope()
{
    FmParams params;
    params.level[1][0]=0.1;
    params.level[1][1]=1;     //attack
    params.level[1][2]=0.8;   //decay
    params.level[1][3]=0.7;   //sustain
    params.level[1][4]=0.1;   //release
    params.rate[1][0]=0.5;    //attack time
    params.rate[1][1]=0.5;    //decay time
    params.rate[1][2]=1;      //sustain time
    params.rate[1][3]=0.5;    //release time
//    Evenlope(&params, 0.3);
//    Evenlope(&params, 0.6);
//    Evenlope(&params, 1.5);
    Evenlope(1, &params, 3);

}


double FMSynth::Evenlope(int op_index, FmParams* params, double t)
{
    double level[5];
    double rate[4];

    level[0] = params->level[op_index][0];
    level[1] = params->level[op_index][1]; // attack
    level[2] = params->level[op_index][2]; // decay
    level[3] = params->level[op_index][3]; // sustain
    level[4] = params->level[op_index][4]; // release
    rate[0] = params->rate[op_index][0];
    rate[1] = params->rate[op_index][1];
    rate[2] = params->rate[op_index][2];
    rate[3] = params->rate[op_index][3];

    double t0 = 0;
    int i = 0;

    if(t > 0 && t < rate[0])
    {
        i = 0;
        t0 = 0;
    }
    if((t > rate[0]) && (t < (rate[0] + rate[1])))
    {
        i = 1;
        t0 = rate[0];
    }
    if((t > (rate[0] + rate[1])) && (t < (rate[0] + rate[1] + rate[2])))
    {
        i = 2;
        t0 = (rate[0] + rate[1] );
    }
    if((t > (rate[0] + rate[1] + rate[2])) && (t < (rate[0] + rate[1] + rate[2] + rate[3])))
    {
        i = 3;
        t0 = (rate[0] + rate[1] + rate[2] );
    }
    double l;
    if(t > (rate[0] + rate[1] + rate[2] + rate[3]))
        return 0;
    l = level[i] +  (level[i+1]-level[i]) * (t - t0) / rate[i];
    return l;

    //qDebug() << "rate i=" << i;
    //qDebug() << "level=" << l << " for t=" << t;




}
//testing feedback loop
double FMSynth::algo17(FmParams* p, double t)
{
    p->out[2] = p->I[2] * sin( 2 * M_PI * p->f[2] * t + p->out[2] );
    return p->I[1] * sin( 2 * M_PI * p->f[1] * t + p->out[2]);
}

static const int THREAD_COUNT = QThread::idealThreadCount();

double* FMSynth::Test1(Buffer* buffer, double f_oc=800, double SampleRate=48000, double time=2, int* N=0)
{
    FmParams param;
    for(int i=0; i < 7; i++)
    {
        param.f[i] = f_oc;
        param.I[i] = gui_params.I[i];
    }
    double* x = zeroes(0, floor(time*SampleRate));
    double t = 0;
    double dt = 1. / SampleRate;
    p->clearvals(0);

    for(int n=0; n < floor(time*SampleRate); n++) //
    {
        x[n] = algo17(&param, t);
        short sample = (x[n]) * 32768;
        buffer->samples[n*2] = sample;
        buffer->samples[n*2+1] = sample;
        if(n == 100)
            out_buffer(buffer);
        if(buffer->bWrited)
            break;
        t+=dt;
        if(n < 1000)
          p->setXY(0, t, x[n]);
    }

    p->autoscale = true;
    p->update_data();
    return x;
}

double amp_mod(double Amax, double fvib, double t, double faze)
{
    return Amax * sin( 2 * M_PI * fvib * t );
}

//testing algo 19
double FMSynth::algo19(FmParams* p, double t, int n)
{
    static double d[7];

    d[6] = amp_mod(p->d[6],0.5,t,p->faze[6]);
    d[4] = amp_mod(p->d[4],0.5,t,p->faze[4]) ;
    d[5] = amp_mod(p->d[5],0.5,t,p->faze[5]);

   // qDebug() << d[6];

    double ev = Evenlope(6, p, t);
    p->out[6] = p->I[6] * sin( 2 * M_PI * (p->f[6]) * t + d[6]*p->out[6]);
    p->out[4] = ev * p->I[4] * sin( 2 * M_PI * (p->f[4]) * t + d[4]*p->out[6]);
    p->out[5] = ev * p->I[5] * sin( 2 * M_PI * (p->f[5]) * t + d[5]*p->out[6]);
    p->out[3] = p->I[3] * sin( 2 * M_PI * p->f[3] * t );
    p->out[2] = p->I[2] * sin( 2 * M_PI * p->f[2] * t + p->out[3] ) ;
    p->out[1] = ev *p->I[1] * sin( 2 * M_PI * p->f[1] * t + p->out[2]);
    return p->out[1] + p->out[4] + p->out[5];
}

double FMSynth::algo5(FmParams* p, double t, int n)
{
    double ev = Evenlope(6, p, t);
    double ev2 = Evenlope(2, p, t);
    static double d[7];
    d[6] = p->d[6];//amp_mod(p->d[6],0.5,t,p->faze[6]);
    d[1] = p->d[1];//amp_mod(p->d[1],0.5,t,p->faze[6]);
    d[5] = p->d[5];//amp_mod(p->d[5],0.5,t,p->faze[5]);
    p->out[6] = ev * p->I[6] * sin( 2 * M_PI * (p->f[6]+ d[6]) * t + p->out[6] );
    p->out[5] = ev * p->I[5] * sin( 2 * M_PI * (p->f[5]+ d[5]) * t + p->out[6] );
    p->out[4] = ev * p->I[4] * sin( 2 * M_PI * (p->f[4])* t);
    p->out[3] = ev * p->I[3] * sin( 2 * M_PI *  p->f[3] * t + p->out[4]);
    p->out[2] = ev2 * p->I[2] * sin( 2 * M_PI * (p->f[2])* t);
    p->out[1] = ev2 * p->I[1] * sin( 2 * M_PI * (p->f[1]+ d[1])* t+ p->out[2]);
    return (p->out[1] + p->out[3] + p->out[5])/3;

}

double* FMSynth::Test3(Buffer* buffer, double f_oc, double SampleRate, double time, bool bReleaseNote)
{
    FmParams param;
    for(int i=0; i < 7; i++)
    {
        param.f[i] = f_oc * gui_params.f[i];
        param.I[i] = gui_params.I[i];
        param.d[i] = gui_params.d[i];
        param.faze[i] = 2 * M_PI * (double) rand()/ RAND_MAX;
        for(int i=1; i <=6; i++)
            for(int j=0; j < 4; j++)
            {
                param.level[i][j+1] = gui_params.level[i][j+1];
                param.rate[i][j] = gui_params.rate[i][j];
            }
    }
    double* x = zeroes(0, floor(time*SampleRate));
    double t = 0;
    double dt = 1. / SampleRate;
    p->clearvals(0);

    for(int n=0; n < floor((time)*SampleRate); n++) //
    {
        x[n] = algo5(&param, t, n);
        short sample = (x[n]) * 10000;
        buffer->samples[n*2] = sample;
        buffer->samples[n*2+1] = sample;
        if(n == 100)
            out_buffer(buffer);
        if(buffer->bWrited)
            break;

        t+=dt;
        //if(n % 100 == 0)
        //if(n < 10000)
        //  p->setXY(0, t, x[n]);
        double ev = Evenlope(6, &param, t);
        if((n % 100)==0)
          p->setXY(0, t, ev);

    }
    //p->autoscale = true;
    p->update_data();
    return x;
}


double* FMSynth::Test2(Buffer* buffer, double f_oc, double SampleRate, double time, bool bReleaseNote)
{
    FmParams param;
    for(int i=0; i < 7; i++)
    {
        param.f[i] = f_oc * gui_params.f[i];
        param.I[i] = gui_params.I[i];
        param.d[i] = gui_params.d[i];
        param.faze[i] = 2 * M_PI * (double) rand()/ RAND_MAX;
    }
    //evenlope params
    param.level[6][0]=0;
    param.level[6][1]=0.8;     //attack
    param.level[6][2]=0.8;   //decay
    param.level[6][3]=0.7;   //sustain
    param.level[6][4]=0.1;   //release
    param.rate[6][0]=0.1;    //attack time
    param.rate[6][1]=0.5;    //decay time
    param.rate[6][2]=10;      //sustain time
    param.rate[6][3]=0.5;    //release time

    double* x = zeroes(0, floor(time*SampleRate));
    double t = 0;

    double dt = 1. / SampleRate;
    //p->clearvals(0);

    for(int n=0; n < floor((time)*SampleRate); n++) //
    {
        x[n] = algo19(&param, t, n);
        short sample = (x[n]) * 8000;
        buffer->samples[n*2] = sample;
        buffer->samples[n*2+1] = sample;
        if(n == 100)
            out_buffer(buffer);
        if(buffer->bWrited)
            break;

        t+=dt;
        //if(n % 100 == 0)
        //if(n < 100)
        //  p->setXY(0, t-t_last, x[n]);

    }
    //p->autoscale = true;
    //p->update_data();
    return x;
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

void FMSynth::selectTest(float f, double duration, int N, bool bReleaseNote)
{
        //x = Test2(f,48000,duration,&N, bReleaseNote);

}

void FMSynth::handleFinished()
{
    return;
    MyWatcher* watcher = (MyWatcher*)sender();
    double* x = watcher->result();
    double Amax = (*std::max_element(&x[0],&x[watcher->N-1]));
    normalize(0.5, x, Amax, 48000,watcher->duration);
    double t = 0;
    double dt = 1. / 48000;
    for(int i=0; i<watcher->N; i++)
    {
        short sample = (x[i]) * 32768;
        watcher->buffer->samples[i*2] = sample;
        watcher->buffer->samples[i*2+1] = sample;
    }
   // wavwrite("./wave/test_fm.wav",&buffer->samples[0],buffer->samples.size()*2,48000,2);
    qDebug() << "finished thread  " << QThread::currentThreadId();
    delete [] x;
    out_buffer(watcher->buffer );
}

Buffer* FMSynth::play_note(char note, double duration, double velocity)
{
    Buffer* buffer = new Buffer(48000,10,2);
    qDebug() << "starting thread  " << QThread::currentThreadId();

    int N = floor( duration * 48000);

    float f = freq_table.getFreq(note);

    QElapsedTimer timer;
    timer.start();
    MyWatcher* watcher = new MyWatcher();
    watcher->duration = duration;
    watcher->note = note;
    watcher->N = N;
    watcher->buffer = buffer;
    watcher->pause();

    connect(watcher, SIGNAL(finished()), this, SLOT(handleFinished()));
    QFuture<double*> future;
    //if(n_test == 3) future = QtConcurrent::run(this, &FMSynth::Test3, buffer, f,48000,duration,false);
    if(n_test == 3) Test3(buffer, f,48000,duration,false);
    if(n_test == 2) future = QtConcurrent::run(this, &FMSynth::Test2, buffer, f,48000,duration,false);
    //if(n_test == 1) future = QtConcurrent::run(this, &FMSynth::Test1, buffer, f, 48000,duration,&N);
    watcher->setFuture(future);
    if(n_test == 1) Test1(buffer, f, 48000, duration, &N);
    qDebug() << "concurrent run " << timer.elapsed();



    return buffer;
}

double FMSynth::release_note(Buffer* buffer, char note, double key_time)
{


//    t_last = key_time;

//    int N = floor( buffer->duration * 48000);

//    double* x;
//    float f = freq_table.getFreq(note);

//    x = selectTest(x, f, buffer->duration, N, true);

//    double Amax = (*std::max_element(&x[0],&x[N-1]));
//    normalize(0.5, x, Amax, 48000,buffer->duration);
//    double t = 0;
//    double dt = 1. / 48000;
//    int offset = key_time * 48000;
//    int release_time = 1;
//    for(int i=offset; i<N; i++)
//    {
//        buffer->samples[(i)*2] = 0;
//        buffer->samples[(i)*2+1] = 0;
//    }

//    for(int i=0; i<release_time*48000; i++)
//    {
//        short sample = (x[i]) * 32768;
//        buffer->samples[(i+offset)*2] = sample;
//        buffer->samples[(i+offset)*2+1] = sample;
//    }
//   // wavwrite("./wave/test_fm.wav",&buffer->samples[0],buffer->samples.size()*2,48000,2);

//    delete [] x;
//    return release_time;
//    //out_buffer( buffer );
    return 0;
}
