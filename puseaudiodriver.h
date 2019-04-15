#ifndef PUSEAUDIODRIVER_H
#define PUSEAUDIODRIVER_H

#include "buffer.h"
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <map>
#include <pulse/simple.h>

class PuseAudioDriver : public QThread
{
       Q_OBJECT
public:
    PuseAudioDriver();
    PuseAudioDriver(PuseAudioDriver* parent);
    int  open();
    int  out_pcm(short* buf, unsigned long len);
    void out_buffer(Buffer* buf);
    int  close();
    int  drop_pcm_frames();
    int  set_nonblock(bool flag);

    void run();
    void nonBlockingLoop();

    std::vector<Buffer*>      quenue;
    QWaitCondition            waitCondition;
    QMutex                    mutex;
    void createThreads(char* device_name);

private:
    std::vector<PuseAudioDriver*> pulse_threads;
    PuseAudioDriver* parent;
    bool bExitThread;
    std::string device_name;
    int Nthreads;
    pa_simple *s;
};

#endif // PUSEAUDIODRIVER_H
