#ifndef SYNTEZER_H
#define SYNTEZER_H

#include <QVariant>
#include <map>
#include <QString>
#include <QElapsedTimer>
#include "alsadriver.h"
#include "puseaudiodriver.h"
#include "freqtable.h"
#include "key2notetable.h"
#include "qtaudiodriver.h"
#include "buffer.h"

class Syntezer : public QObject
{
    Q_OBJECT
public:
    AlsaDriver*          alsa;
    PuseAudioDriver*     pulse;
    QtAudioDriver*       qt;
    std::map<int,Buffer*>       key2noteBuffer;
    FreqTable                   freq_table;
    Key2NoteTable               key2note;
    bool                        bEnabled;
    bool                        bAsynch;
    int                         enabledChannels;
    bool                        bDoOutBuffer;

    Syntezer();
    void parse_freqs_table(QString file);
    void out_pcm(short* buffer, int len);
    void drop_pcm_frames();
    virtual Buffer* play_note(char note, double duration, double velocity){}
    virtual void play_note2(Buffer* buffer, char note, double duration, double velocity){}
    virtual double release_note(Buffer* buffer, char note, double key_time){return 0;}
    virtual void write_note(Buffer* buffer, long offset, char note, double duration){}
    virtual void SetCurrentPatch(int n){}
    virtual void SetEnabledChannels(int word){ enabledChannels = word; }
    bool isChannelEnabled(int n_channel);
    virtual void LoadPatch(QString filename, int n){}
    bool isEnabled() { return bEnabled; }
    void out_buffer(Buffer* buf);



    void prepareKeyTable();

signals:
    void sigNote(char note);
    void sigDisableNote(char note);

public slots:
    virtual void on_key_press(int key_code);
    virtual void on_key_release(int key_code);
    void on_Timer();

private:
    std::map<int, QElapsedTimer>        key_time;

    bool                 bKeyPressed;

};

#endif // SYNTEZER_H
