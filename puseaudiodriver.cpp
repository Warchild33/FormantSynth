#include "puseaudiodriver.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <QSettings>
#include <QStringList>
#include <QDebug>
#include <QApplication>
#include <fcntl.h>
#ifdef __linux__
#include <pulse/simple.h>
#include <pulse/error.h>
#endif

static QSettings settings("./settings/settings.ini", QSettings::IniFormat);

PuseAudioDriver::PuseAudioDriver()
{
    //playback_handle = 0;
    bExitThread = false;
    parent = 0;
    Nthreads = 4;
#ifdef __linux__
    s = NULL;
#endif
    createThreads((char*)settings.value("pulse_device").toString().toStdString().c_str());
}

PuseAudioDriver::PuseAudioDriver(PuseAudioDriver* parent)
    : parent(parent)
{
    //playback_handle = 0;
    bExitThread = false;
    Nthreads = 0;
 #ifdef __linux__
    s = NULL;
 #endif
}

void PuseAudioDriver::createThreads(char* device_name)
{
    if(QString(device_name) == "pulseaudio")
    {
        if(parent == 0)
        {
            for(int i=0; i < Nthreads; i++)
            {
                PuseAudioDriver* thread = new PuseAudioDriver(this);
                pulse_threads.push_back(thread);
                thread->start();

            }
        }
    }
}

void PuseAudioDriver::out_buffer(Buffer* buf)
{
    int cnt = 0;
    //for(int i=0; i < Nthreads; i++)
    for(auto t = pulse_threads.begin(); t!=pulse_threads.end(); t++,cnt++)
    {
        if ((*t)->quenue.size() == 0)
        {
            (*t)->quenue.push_back(buf);
            (*t)->waitCondition.wakeAll();
            fprintf(stderr,"write to thread %d\n", cnt);
            break;
        }
    }
}

void PuseAudioDriver::run()
{
#ifdef __linux__
    //open device if not parent

    if( parent!=0 )
    {
        if(!open())
            return;
        //fprintf (stderr, "AlsaDriver thread id = %d device=dmix", QThread::currentThreadId());
    }
    int cptr,err;
    unsigned char* ptr;

    while(1)
    {
        mutex.lock();
        waitCondition.wait(&mutex);
        if( quenue.size() > 0 )
        {
            Buffer* buf = quenue.back();
            //out_pcm(&buf->samples[0], buf->samples.size()/2);

            int period_size = buf->samples.size();

            cptr = period_size;
            ptr = (unsigned char*)&buf->samples[0];
            pa_usec_t latency;
            if ((latency = pa_simple_get_latency(s, &err)) == (pa_usec_t) -1) {
                fprintf(stderr, __FILE__": pa_simple_get_latency() failed: %s\n", pa_strerror(err));
            }
            size_t buf_size = 8192*10;
            qDebug() << "latency " << latency;
            while (cptr > 0) {
                if (pa_simple_write(s, ptr, (size_t) buf_size, &err) < 0) {
                    fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(err));
                }
                /* Make sure that every single sample was played */
                if (pa_simple_drain(s, &err) < 0) {
                    fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(err));

                }


                if( (!buf->timeEnd.isNull()) && (buf->timeEnd < QTime::currentTime()) )
                {
                    break;
                }

                ptr += buf_size; //2 channels
                cptr -= buf_size;
            }

            delete buf;
            quenue.erase(quenue.end()-1);
        }
        mutex.unlock();
        if( bExitThread ) return;

    }
#endif
}

int  PuseAudioDriver::close()
{
 #ifdef __linux__
    pa_simple_free(s);
#endif
}

char gCounter = 'A';

int  PuseAudioDriver::open()
{
#ifdef __linux__
    /* The Sample format to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 48000,
        .channels = 2
    };

    int ret = 1;
    int error;
    /* Create a new playback stream */
    char app_path[1024];

    gCounter++;

    QString arg0 = QApplication::arguments()[0];
    for(int i=0; i < arg0.size(); i++)
        app_path[i] = arg0[i].toLatin1();
    app_path[arg0.size()] = 0;
    if (!(s = pa_simple_new(NULL, app_path, PA_STREAM_PLAYBACK, NULL, "playback" + gCounter, &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        pa_simple_free(s);
        return 0;
    }
    return 1;
#endif

}
