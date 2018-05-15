#ifndef FREQTABLE_H
#define FREQTABLE_H

#include <QString>
#include <map>
#include "midinote.h"

class FreqTable
{
public:
    FreqTable();
    void parse_freqs_table(QString file);
    float getFreq(char note);
    MidiNote getNoteByName(QString note_name);
    QString getOctaveFromNoteName(QString name, char& octave);
    void parseName(QString name);
    void parse_fields(int field, int octave, QStringList parts, QString name);

private:
    float midi_freq[127];
    std::map<QString, MidiNote> note_table;
};

#endif // FREQTABLE_H
