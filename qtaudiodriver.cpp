#include "qtaudiodriver.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <QSettings>
#include <QStringList>
#include <QDebug>
#include <QApplication>
#include <fcntl.h>


static QSettings settings("./settings/settings.ini", QSettings::IniFormat);

QtAudioDriver::QtAudioDriver()
{
    //playback_handle = 0;
    bExitThread = false;
    parent = 0;
    Nthreads = 8;

    createThreads((char*)settings.value("device").toString().toStdString().c_str());
}

QtAudioDriver::QtAudioDriver(QtAudioDriver* parent)
    : parent(parent)
{
    //playback_handle = 0;
    bExitThread = false;
    Nthreads = 0;

}

void QtAudioDriver::createThreads(char* device_name)
{

    if(parent == 0)
    {
        for(int i=0; i < Nthreads; i++)
        {
            QtAudioDriver* thread = new QtAudioDriver(this);
            pulse_threads.push_back(thread);
            thread->start();
            thread->moveToThread(thread);

        }
    }

}

void QtAudioDriver::out_buffer(Buffer* buf)
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

void QtAudioDriver::run()
{

    //open device if not parent

    if( parent!=0 )
    {
        if(!open())
            return;
        //fprintf (stderr, "AlsaDriver thread id = %d device=dmix", QThread::currentThreadId());
    }
    unsigned char* ptr;
    unsigned char* ptr_last;

    while(1)
    {
        mutex.lock();
        waitCondition.wait(&mutex);
        if( quenue.size() > 0 )
        {
            Buffer* buf = quenue.back();

            ptr = (unsigned char*)&buf->samples[0];
            ptr_last = (unsigned char*)&buf->samples[buf->samples.size()-2];


            QIODevice* io = _ao->start();
            //io->write((const char*)ptr, buf->samples.size()*2);

            qDebug() << "_ao->periodSize" << _ao->periodSize();
            QByteArray buffer(32768, 0);
            bool Abort = false;
            while(1)
            {
                int chunks = _ao->bytesFree() / _ao->periodSize();
                while (chunks) {
                   for(int i=0; i < _ao->periodSize(); i++)
                   {
                       buffer[i] = *ptr;
                       ptr++;
                       if(ptr_last==ptr)
                       {
                           Abort = true;
                           break;

                       }

                   }
                   if( (!buf->timeEnd.isNull()) && (buf->timeEnd < QTime::currentTime()) )
                   {
                       Abort = true;
                       break;
                   }
                   io->write(buffer.data(), _ao->periodSize());
                   if(Abort) break;

                   --chunks;
                }
                if(Abort) break;
            }

            quenue.erase(quenue.end()-1);
        }
        mutex.unlock();
        if( bExitThread ) return;

    }

}

int  QtAudioDriver::out_pcm(short* buf, unsigned long len)
{
    return 0;
}

int  QtAudioDriver::drop_pcm_frames()
{
    return 0;
}

int  QtAudioDriver::close()
{

}

void QtAudioDriver::qtStateChanged(QAudio::State state)
{
    // Play finished, play next clip
    if (state == QAudio::IdleState)
    {

    }
    //qDebug() << "error " << _ao->error();
    //qDebug() << "state " << _ao->state();
}


int  QtAudioDriver::open()
{
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultOutputDevice();
    if(deviceInfo.isFormatSupported(format))
    {
       // qDebug() << "format supported";
    }
    _ao = new QAudioOutput(deviceInfo, format);
    _ao->setVolume(1);

    connect(_ao, SIGNAL(stateChanged(QAudio::State)), this, SLOT(qtStateChanged(QAudio::State)));

}
