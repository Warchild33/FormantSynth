#include "gen.h"
#include "buffer.h"
#include "nessynth.h"
#include "envelope.h"

NesSynth::NesSynth()
  :Syntezer()
{
    gen_type = SQUARE;
    gen_type = NOISE;
    percent = 0.5;
}

Buffer* NesSynth::play_note(char note, double duration, double velocity)
{
    Buffer* buffer = new Buffer(48000,duration,2);
    //генерация импульсов
    int N;
    double* x;
    float f = freq_table.getFreq(note);

    if(gen_type == SQUARE)
        x = pulse_nes(f,percent,48000,duration,&N);

    if(gen_type == TRIANGLE)
        x = tri_nes(f,percent,48000,duration,&N);

    if(gen_type == NOISE)
        x = noise_nes(f,percent,48000,duration,&N);

    double Amax = (*std::max_element(&x[0],&x[N-1]));    
    normalize(0.1, x, Amax, 48000,duration);

    for(int i=0; i<N; i++)
    {
        short sample = (x[i]*6) * 32768;
        buffer->samples[i*2] = sample;
        buffer->samples[i*2+1] = sample;
    }

    delete [] x;
    buffer->note = note;
    out_buffer( buffer );
    return buffer;
}
