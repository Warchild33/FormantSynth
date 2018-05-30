#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QStringList>
#include "freqtable.h"

FreqTable::FreqTable()
{
    parse_freqs_table("./midi_data/midinote2freq.txt");

}

QString FreqTable::getOctaveFromNoteName(QString name, char& octave)
{
    if( name.contains("C") )
    {
         QRegExp rx("(\\d+)");
         if( rx.indexIn(name)!=-1 )
         {
             octave = rx.cap(1).toInt();
         }
         return "C";
    }
    return name;
}

void FreqTable::parse_fields(int field, int octave, QStringList parts, QString name)
{
    int n1 = parts[field].toInt();
    midi_freq[n1] = parts[field+1].toFloat();
    QString key = name + QString::number(octave);
    note_table[key] = MidiNote(key,midi_freq[n1],n1,octave);
}

void FreqTable::parse_freqs_table(QString file)
{
    QFile   f(file);
    if( f.open( QFile::ReadOnly | QFile::Text) )
    {
         QString line, name;
         QTextStream stream(&f);
         char octave;
         while( !stream.atEnd() )
         {
             QStringList parts;
             line = stream.readLine();
             if( line.contains(";") )
                 continue;
             parts = line.split(" ",QString::SkipEmptyParts);
             if( parts.size() !=0 )
             {
                name = parts[0];
                name = getOctaveFromNoteName(name, octave);
             }
             if( parts.size() == 7)
             {
                 parse_fields(1, octave, parts, name);
                 parse_fields(3, octave+1, parts, name);
                 parse_fields(5, octave+2, parts, name);
             }
             if( parts.size() == 5)
             {
                 parse_fields(1, octave, parts, name);
                 parse_fields(3, octave+1, parts, name);
             }
             if( parts.size() == 3)
             {
                 parse_fields(1, octave, parts, name);
             }

         }
    }
    else
        fprintf(stderr, "cannot open midi freqs file!");
}


float FreqTable::getFreq(char note)
{
    return midi_freq[note];
}
