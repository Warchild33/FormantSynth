#include "gen.h"
#include "happybirsday.h"
#include "filter.h"
#include "wave_in.h"
#include "envelope.h"
#include "common.h"
#include <QString>
#include <QFile>
#include <QStringList>
#include <QTextStream>

Happybirsday::Happybirsday()
{
    std::vector<Notestruct> notes = parse_hb_notes("./midi_data/happy_birsday.txt");
    generate_song(notes);
}

std::vector<Notestruct> Happybirsday::parse_hb_notes(QString file)
{
    std::vector<Notestruct> output;
   // Notes of classic song happy birsday
   // G G A G C B | G G A G D C | G G G^ E C B A | F F E C D C
   QFile   f(file);

   if( f.open( QFile::ReadOnly | QFile::Text) )
   {
        QString line;
        QTextStream stream(&f);
        while( !stream.atEnd() )
        {
            QStringList parts;
            line = stream.readLine();
            parts = line.split("\t");
            if( line.contains(";") )
                continue;
            Notestruct st;
            st.note = parts[3].toInt();
            st.t_start = parts[1].toFloat();
            st.t_end = st.t_start + parts[2].toFloat();
            output.push_back(st);
        }
   }
   return output;
}

void Happybirsday::generate_song(std::vector<Notestruct>& song)
{
    std::vector<short>* output_samples = new std::vector<short>();
    int n_note = 0;

    float total_time = (*(song.end()-1)).t_start;
    for(int i=0; i<int((total_time+5)*48000); i++)
        output_samples->push_back(0);

    for(auto ns=song.begin(); ns!=song.end(); ns++,n_note++)
    {
        float f= freq_table.getFreq((*ns).note);
        float t_start = (*ns).t_start;
        float t_end = (*ns).t_end;
        // sound of A
        generate_voice(f, int(t_start*48000), t_end-t_start, 400.00, 2000.00, 2550.00, 0.0066, 3, output_samples);

    }
    wavwrite("./wave/hb_song.wav",&(*output_samples)[0],(*output_samples).size(),44000,1);

}

void generate_voice(double f,int sample_offset, double duration, double F1, double F2, double F3, double BW, int Ncascade,
                    std::vector<short>* output_samples)
{
    //генерация импульсов
    int N;
    float k = f / 80;
    double* x = pulse_gen1(f,48000,duration,&N);
    //wavwrite("pulse.wav",data,N*2,48000,1);
    double* y = zeroes(0, N);
    double* y2 = zeroes(0, N);
    double* y3 = zeroes(0, N);
    for(int s=0; s < Ncascade; s++)
    {
        if(s>0) for(int i=0; i<N; i++) x[i] = y[i];
        narrow_band_filter(x,y,N,F1*k,BW,48000);
        if(s>0) for(int i=0; i<N; i++) x[i] = y2[i];
        narrow_band_filter(x,y2,N,F2*k,BW,48000);
        if(s>0) for(int i=0; i<N; i++) x[i] = y3[i];
        narrow_band_filter(x,y3,N,F3*k,BW,48000);
    }
    for(int i=0; i<N; i++)
    {
       y[i] = (y[i]+y2[i]+y3[i])*6;
    }

    double Amax = (*std::max_element(&y[0],&y[N-1]));
    envelope_Bspline(y,Amax,48000,duration);
    normalize(0.1, y,Amax,48000,duration);

    for(int i=0; i<N; i++)
    {
        short sample = (y[i]*6) * 32768;
        (*output_samples)[sample_offset+i] = (*output_samples)[sample_offset+i] + sample;
    }

    delete [] x;
    delete [] y;
    delete [] y2;
    delete [] y3;

}



