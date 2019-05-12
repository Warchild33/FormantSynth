#ifndef QTAUDIODRIVER_H
#define QTAUDIODRIVER_H

#include "buffer.h"
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <map>
#include <QAudioOutput>
#include <QBuffer>

class QtAudioDriver : public QThread
{
       Q_OBJECT
public:
    QtAudioDriver();
    QtAudioDriver(QtAudioDriver* parent);
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

public slots:
    void qtStateChanged(QAudio::State state);

private:
    std::vector<QtAudioDriver*> pulse_threads;
    QtAudioDriver* parent;
    bool bExitThread;
    std::string device_name;
    int Nthreads;
    QAudioOutput *_ao;
    QAudioFormat _af;
    QBuffer _buf;
};

#endif // PUSEAUDIODRIVER_H
