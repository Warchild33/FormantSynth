#include "gen.h"
#include "happybirsday.h"
#include "filter.h"
#include "wave_in.h"
#include "envelope.h"
#include "common.h"
#include "buffer.h"
#include <QString>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QApplication>

Happybirsday::Happybirsday()
{
    buffer = 0;
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
            st.channel = parts[5].toInt();
            if(parts[1].toFloat()!=-1)
            output.push_back(st);
        }
   }
   return output;
}

void Happybirsday::generate_play_wave(std::vector<Notestruct>& song)
{
    int n_note = 0;
    float total_time = (*(song.end()-1)).t_end+2;
    buffer = new Buffer(48000,total_time,2);
    isPlaying =  true;
    bool bBufferSended = false;
    for(auto ns=song.begin(); ns!=song.end(); ns++,n_note++)
    {
        float t_start = (*ns).t_start;
        float t_end = (*ns).t_end;
        int channel = (*ns).channel;
        synt->SetCurrentPatch(channel);
        if( isPlaying &&  channel!=9) //only piano channel
          synt->write_note(buffer,long(t_start*48000),(*ns).note,t_end-t_start);
        //if(progress_bar)
        //   progress_bar->setValue(((float)n_note/song.size()) * 100);

        if((t_start > 2) && !bBufferSended)
        {
            synt->out_buffer(buffer);
            bBufferSended = true;
        }
    }


}

void Happybirsday::generate_wave_file(std::vector<Notestruct>& song, QProgressBar* progress_bar)
{
    std::vector<short>* output_samples = new std::vector<short>();
    int n_note = 0;

    float total_time = (*(song.end()-1)).t_start;
    for(int i=0; i<int((total_time+5)*48000); i++)
    {
        output_samples->push_back(0);
        output_samples->push_back(0);
    }

    for(auto ns=song.begin(); ns!=song.end(); ns++,n_note++)
    {
        float f= freq_table.getFreq((*ns).note);
        float t_start = (*ns).t_start;
        float t_end = (*ns).t_end;
        // sound of A
        //generate_voice(f, int(t_start*48000), t_end-t_start, synt.F1, synt.F2, synt.F3, synt.BW, synt.Ncascade, output_samples);
        generate_voice(f, int(t_start*48000), t_end-t_start, 400.00, 2000.00, 2550.00, 0.0066, 3, output_samples);
        if(progress_bar)
           progress_bar->setValue(((float)n_note/song.size()) * 100);

    }
    if(progress_bar)
      progress_bar->setValue(100);
    QApplication::processEvents();

    wavwrite("./wave/hb_song.wav",&(*output_samples)[0],(*output_samples).size(),48000,1);
    synt->out_pcm(&(*output_samples)[0], (*output_samples).size());

}

void msleep(int ms);

void Happybirsday::timerEvent(QTimerEvent *)
{
    int n_note = 0;
    if( isPlaying )
    {
        double t = (double)timer.nsecsElapsed()/1000000000.;
        for(auto ns=song->begin(); ns!=song->end(); ns++,n_note++)
        {
            float f= freq_table.getFreq((*ns).note);
            float t_start = (*ns).t_start;
            float t_end = (*ns).t_end;
            if((*ns).isPlayed == false)
            {
                if(((t_start - t) >= 0) && ((t_start - t) < 0.2))
                {
                    (*ns).isPlayed = true;
                    //generate_voice(f, 0, t_end-t_start, 400.00, 2000.00, 2550.00, 0.0066, 3, &buf->samples);
                    Buffer* buf = synt->play_note((*ns).note,t_end-t_start,1);
                    buf->bWrited = false;
                    //fprintf(stderr,"===========>>> play_note %d\n",(*ns).note);
                    (*ns).buf = buf;
                    if(progress_bar)
                      progress_bar->setValue(((float)n_note/song->size()) * 100);

                }
            }
            else
            {
                if(((t_end - t) < 0.1) && ((t_end - t) > 0))
                {
                    if((*ns).buf)
                    {

                        //synt->release_note((*ns).buf,(*ns).note,t_end-t_start);

                        (*ns).buf->bWrited = true;
                        msleep(50);
                        (*ns).buf->timeEnd = QTime::currentTime().addMSecs(50);
                        (*ns).buf = 0;
                    }
                }

            }


        }
    }
}

void Happybirsday::Play(std::vector<Notestruct>* song, QProgressBar* progress_bar)
{
    this->song = song;
    this->progress_bar = progress_bar;
    isPlaying = true;
    //timer.restart();
    //startTimer(1);
}

void Happybirsday::Stop()
{
    isPlaying = false;
    if(buffer!=0)
    {
        buffer->timeEnd = QTime::currentTime();
    }
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
       y[i] = (y[i]+y2[i]+y3[i]);
    }

    double Amax = (*std::max_element(&y[0],&y[N-1]));
    //envelope_Bspline(y,Amax,48000,duration);
    normalize(0.1, y,Amax,48000,duration);

    for(int i=0; i<N; i++)
    {
        short sample = (y[i]) * 32768;
        int os = 2*(sample_offset+i);
        (*output_samples)[os] = (*output_samples)[os] + sample;
        (*output_samples)[os+1] = (*output_samples)[os];
    }

    delete [] x;
    delete [] y;
    delete [] y2;
    delete [] y3;

}




void Happybirsday::set_synth(Syntezer* s)
{
    synt = s;
}
