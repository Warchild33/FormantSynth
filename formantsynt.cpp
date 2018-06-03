#include "envelope.h"
#include "filter.h"
#include "common.h"
#include "gen.h"
#include "formantsynt.h"


FormantSynt::FormantSynt()
    :Syntezer()
{
    F1 = 660;
    F2 = 1700;
    F3 = 2400;
    BW = 0.0066;
    Ncascade = 5;
    cur_vowel = 'A';

}


void FormantSynt::get_params_for(char vowel)
{
    switch(vowel)
    {
        case 'I':
        {
            F1 = 270;
            F2 = 2300;
            F3 = 3000;
            BW = 0.0033;
        }break;
        case 'E':
        {
            F1 = 400;
            F2 = 2000;
            F3 = 2550;
            BW = 0.0066;
        }break;
        case 'A':
        {
            F1 = 660;
            F2 = 1700;
            F3 = 2400;
            BW = 0.0066;
        }break;
        case 'U':
        {
            F1 = 640;
            F2 = 1200;
            F3 = 2400;
            BW = 0.0066;
        }break;
    }
}

void FormantSynt::apply_filters(double* y, float k, double* x, double* y3, int N, double* y2)
{
    for(int s=0; s < Ncascade; s++)
    {
        if(s>0) for(int i=0; i<N; i++) x[i] = y[i];
        narrow_band_filter(x,y,N,F1*k,BW,48000);
        if(s>0) for(int i=0; i<N; i++) x[i] = y2[i];
        narrow_band_filter(x,y2,N,F2*k,BW,48000);
        if(s>0) for(int i=0; i<N; i++) x[i] = y3[i];
        narrow_band_filter(x,y3,N,F3*k,BW,48000);
    }
}

Buffer* FormantSynt::play_note(char note, double duration, double velocity)
{
    get_params_for(cur_vowel);
    Buffer* buffer = new Buffer(48000,duration,2);
    //генерация импульсов
    int N;
    float f = freq_table.getFreq(note);
    float k = 1;//f / 80;
    double* x = pulse_gen1(f,48000,duration,&N);
    //wavwrite("pulse.wav",data,N*2,48000,1);
    double* y = zeroes(0, N);
    double* y2 = zeroes(0, N);
    double* y3 = zeroes(0, N);

    apply_filters(y, k, x, y3, N, y2);

    for(int i=0; i<N; i++)
    {
        y[i] = (y[i]+y2[i]+y3[i])*3;
    }

    double Amax = (*std::max_element(&y[0],&y[N-1]));
    //fprintf(stderr,"Amax = %f\n", Amax);
    //envelope_Bspline(y,Amax,48000,duration);
    normalize(0.1, y, Amax, 48000,duration);

    for(int i=0; i<N; i++)
    {
        short sample = (y[i]*6) * 32768;
        buffer->samples[i*2] = sample;
        buffer->samples[i*2+1] = sample;
        buffer->samplesD[i*2] = y[i]*6;
        buffer->samplesD[i*2+1] = y[i]*6;
    }

    delete [] x;
    delete [] y;
    delete [] y2;
    delete [] y3;
    buffer->note = note;
    //out_pcm(&buffer->samples[0], buffer->samples.size()/2);
    //delete buffer;
    out_buffer( buffer );
    return buffer;
}
