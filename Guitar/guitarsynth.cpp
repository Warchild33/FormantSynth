#include <fstream>
#include <QFile>
#include "wave_in.h"
#include "ploter.h"
#include "guitarsynth.h"
#include "print.h"
#include "common.h"
#include "istft.h"

extern Ploter* p;

GuitarSynth::GuitarSynth()
{
    //return; // ! TEMPORARY !
    strings[0] = new String("E", 82.4,    "./wave/E_guitar.wav");
    strings[1] = new String("B", 110,     "./wave/B_guitar.wav");
    strings[2] = new String("G", 146.83,  "./wave/G_guitar.wav");
    strings[3] = new String("D", 195.99,  "./wave/D_guitar.wav");
    strings[4] = new String("A", 246.94,  "./wave/A_guitar.wav");
    strings[5] = new String("E", 329.627, "./wave/e_guitar.wav");

}


String::String(QString name, double f, QString wave_file)
    : name(name), Fs(f)
{
    int sampleRate;
    short channels;
    std::vector<short> samples;
    std::vector<double> x;

    wavread(wave_file.toStdString().c_str(), samples, sampleRate, channels);
    prn("file=%s sampleRate=%d channels=%d",wave_file.toStdString().c_str(),sampleRate, channels);

    for(int i=0; i < samples.size()/2; i++)
        x.push_back((double)samples[2*i]/32768);

    Nfft = 32768/4;
    QString stft_file = wave_file.mid(0,wave_file.size()-3) + "stft";
    QFile file(stft_file);
    if( file.exists() )
      LoadStft( stft_file );
    else
    {
        fourie_samples = stft(&x[0], x.size(),Nfft,Nfft,floor(Nfft/4),sampleRate,&rows,&cols);
        SaveStft( stft_file );
    }

    d = Make2DArray(rows, cols);
    prn("Make stft rows=%d cols=%d",rows,cols);

}

void String::SaveStft(QString filename)
{
     std::ofstream stream(filename.toStdString(), std::ios::binary);
     stream.write((const char*)&rows, sizeof(rows));
     stream.write((const char*)&cols, sizeof(cols));
     for(int col=0; col < cols; col++)
     for(int i=0; i < rows; i++)
     {
         stream.write((const char*)&fourie_samples[i][col], sizeof(complex_double));
     }
}

void String::LoadStft(QString filename)
{
     std::ifstream stream(filename.toStdString(), std::ios::binary);
     stream.read((char*)&rows, sizeof(rows));
     stream.read((char*)&cols, sizeof(cols));
     fourie_samples = Make2DArray(rows, cols);
     for(int col=0; col < cols; col++)
     for(int i=0; i < rows; i++)
     {
         stream.read((char*)&fourie_samples[i][col], sizeof(complex_double));
     }
}


void draw_fft_frame(int plot, int n_cols, int n_rows, complex_double** d)
{
    p->clearvals(0);
    p->clearvals(1);
    for(int col=0; col < n_cols; col++)
    for(int i=0; i < n_rows; i++)
        p->setXY(plot, i, std::abs(d[i][col]));
    p->update_data();
}

Buffer* GuitarSynth::play_note(char note, double duration, double velocity)
{

}

double* GuitarSynth::gen(double f, int SampleRate, double time)
{
    double minD=2000000000000;
    int idx_string=0;
    for(int i=0; i < 6; i++)
    {
        double d = std::abs(strings[i]->Fs - f);
        if( d < minD )
        {
            minD = d;
            idx_string = i;
        }
    }
    return strings[idx_string]->gen(f, time);
}

double* String::gen(double f, double time)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> Distr(4,1);

    for(int j=0; j < cols; j++)
    for(int i=0; i < rows; i++)
    {
            if( std::abs(fourie_samples[i][j]) > 10 )
                d[i][j] = fourie_samples[i][j];
            else
                d[i][j] = 0;//std::complex<double>(Distr(gen),0);
    }

    double kf = (f / Fs);

    prn("%s f=%f kf=%f", name.toStdString().c_str(), f, kf);

    complex_double* row = new complex_double[Nfft*sizeof(complex_double)];
    for(int col=0; col < cols; col++)
    {
        for(int i=0; i < Nfft/2; i++)
            row[qRound(i*kf)] = d[i][col];
        for(int i=0; i < Nfft/2; i++)
        {
            std::complex<double> c(std::abs(row[i]),std::arg(row[i]));
            d[i][col] = c;
        }
    }
    //draw_fft_frame(0,Nfft/2, cols, d);
    int ylen;
    double* y = istft(d,Nfft,Nfft,floor(Nfft/4),rows,cols,&ylen);

    delete [] row;
    //draw_fft_frame(0, 128);
    return y;
}
