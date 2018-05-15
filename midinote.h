#ifndef MIDINOTE_H
#define MIDINOTE_H

#include <QString>

class MidiNote
{
public:
    QString name;
    float   freq;
    char    note;
    char    octave;
    MidiNote();
    MidiNote(QString name, float freq, char note, char octave)
        :name(name),freq(freq),note(note),octave(octave)
    {
    }
};

#endif // MIDINOTE_H
