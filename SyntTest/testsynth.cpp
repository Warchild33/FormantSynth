#include "gen.h"
#include "buffer.h"
#include "testsynth.h"
#include "envelope.h"
#include "guitarsynth.h"
#include "wave_in.h"

GuitarSynth* guitar_synt;

TestSynth::TestSynth()
  :Syntezer()
{
    gen_type = FM;
    percent = 0.5;
    ratio = 1./4.;
    //guitar_synt = new GuitarSynth();
}

void TestSynth::write_note(Buffer* buffer, long offset, char note, double duration)
{
    //генерация импульсов
    int N = floor( duration * 48000);

    double* x;
    float f = freq_table.getFreq(note);
    if(gen_type == SQUARE)
        x = pulse_nes(f,percent,48000,duration,&N);

    if(gen_type == TRIANGLE)
        x = tri_nes(f,percent,48000,duration,&N);

    if(gen_type == NOISE)
        x = noise_nes(f,percent,48000,duration,&N);

    if(gen_type == NOISE2)
        x = noise_nes1(f,percent,48000,duration,&N);

    if(gen_type == FM)
        x = test_fm(f,ratio,48000,duration,&N);

    //if(gen_type == GUITAR)
    //    x = guitar_synt->gen(f,48000,duration);


    double Amax = (*std::max_element(&x[0],&x[N-1]));
    normalize(0.5, x, Amax, 48000,duration);

    for(int i=0; i<N; i++)
    {
        short sample = (x[i]) * 32768;
        buffer->samples[(i+offset)*2] = sample;
        buffer->samples[(i+offset)*2+1] = sample;
    }
   // wavwrite("./wave/test_fm.wav",&buffer->samples[0],buffer->samples.size()*2,48000,2);

    delete [] x;
    buffer->note = note;

}

Buffer* TestSynth::play_note(char note, double duration, double velocity)
{
    Buffer* buffer = new Buffer(48000,duration,2);
    //генерация импульсов
    int N = floor( duration * 48000);

    double* x;
    float f = freq_table.getFreq(note);

    if(gen_type == SQUARE)
        x = pulse_nes(f,percent,48000,duration,&N);

    if(gen_type == TRIANGLE)
        x = tri_nes(f,percent,48000,duration,&N);

    if(gen_type == NOISE)
        x = noise_nes(f,percent,48000,duration,&N);

    if(gen_type == NOISE2)
        x = noise_nes1(f,percent,48000,duration,&N);

    if(gen_type == FM)
        x = test_fm(f,ratio,48000,duration,&N);

    //if(gen_type == GUITAR)
    //    x = guitar_synt->gen(f,48000,duration);


    double Amax = (*std::max_element(&x[0],&x[N-1]));    
    normalize(0.5, x, Amax, 48000,duration);

    for(int i=0; i<N; i++)
    {
        short sample = (x[i]) * 32768;
        buffer->samples[i*2] = sample;
        buffer->samples[i*2+1] = sample;
    }
   // wavwrite("./wave/test_fm.wav",&buffer->samples[0],buffer->samples.size()*2,48000,2);

    delete [] x;
    buffer->note = note;
    out_buffer( buffer );
    return buffer;
}
