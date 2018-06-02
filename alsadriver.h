#ifndef ALSADRIVER_H
#define ALSADRIVER_H

#include "buffer.h"
#include <alsa/asoundlib.h>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <map>


class AlsaDriver : public QThread
{
    Q_OBJECT
public:

    AlsaDriver();
    AlsaDriver(AlsaDriver* parent);
    int  open(char* device_name);
    int  out_pcm(short* buf, unsigned long len);
    void out_buffer(Buffer* buf);
    int  close();
    int  drop_pcm_frames();
    int  set_nonblock(bool flag);    

    void run();
    void nonBlockingLoop();
    void blockingLoop();

    std::vector<Buffer*>      quenue;
    QWaitCondition            waitCondition;
    QMutex                    mutex;
private:



    std::vector<AlsaDriver*> alsa_threads;
    AlsaDriver* parent;
    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *hw_params;
    bool bExitThread;
    std::string device_name;
    int Nthreads;

};

#endif // ALSADRIVER_H
