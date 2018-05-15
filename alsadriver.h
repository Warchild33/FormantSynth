#ifndef ALSADRIVER_H
#define ALSADRIVER_H

#include "buffer.h"
#include "notetowaveformthread.h"
#include <alsa/asoundlib.h>
#include <QObject>
#include <QThread>
#include <QMutex>


class AlsaDriver : public QThread
{
    Q_OBJECT
public:
    AlsaDriver();
    int  open(char* device_name);
    int  out_pcm(short* buf, unsigned long len);
    int  close();
    int  drop_pcm_frames();
    void run();
    NoteToWaveformThread     notetowaveform_thread;

private:


    QMutex mutex;
    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *hw_params;
    bool bProcessBuffers;

};

#endif // ALSADRIVER_H
