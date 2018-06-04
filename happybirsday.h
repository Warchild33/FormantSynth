#ifndef HAPPYBIRSDAY_H
#define HAPPYBIRSDAY_H

#include <QString>
#include <QProgressBar>
#include <map>
#include <vector>
#include "freqtable.h"
#include "formantsynt.h"
#include "alsadriver.h"

void generate_voice(double f, int sample_offset, double time, double F1, double F2, double F3, double BW, int Ncascade,
                    std::vector<short>* output_samples);


struct Notestruct
{
    Notestruct()
    {
        isPlayed = false;
    }
    char note;
    float t_start;
    float t_end;
    bool  isPlayed;
};

class Happybirsday : public QObject
{
    Q_OBJECT
public:
    bool           isPlaying;
    QElapsedTimer  timer;
    FreqTable freq_table;
    FormantSynt* synt;
    std::vector<Notestruct>* song;
    QProgressBar*            progress_bar;

    Happybirsday();
    void set_synth(FormantSynt* s);
    void generate_wave_file(std::vector<Notestruct>& song, QProgressBar* progress_bar);
    std::vector<Notestruct> parse_hb_notes(QString file);
    void parse_notes_table(QString file); // creates note2fre table

    void timerEvent(QTimerEvent *);
    void Play(std::vector<Notestruct>* song, QProgressBar* progress_bar);
    void Stop();


};

#endif // HAPPYBIRSDAY_H
