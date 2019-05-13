#include <QTimer>
#include <QSettings>
#include <QFuture>
#include <stdio.h>
#include <stdlib.h>
#include "syntezer.h"
#include "alsadriver.h"
#include "puseaudiodriver.h"
#include "qtaudiodriver.h"


Syntezer::Syntezer()
    : QObject()
{
    QSettings settings("./settings/settings.ini", QSettings::IniFormat);
    pulse = 0;
    //pulse->open();
    alsa = 0;
    qt = new QtAudioDriver();
    //connect(this, SIGNAL(sigDisableNote(char)),&alsa->mixer_thread, SLOT(disable_note(char)));
    bKeyPressed = false;
    bEnabled = false;
    enabledChannels = 0xFFFFFF;
    QTimer* timer = new QTimer();
    connect(timer,SIGNAL(timeout()), this, SLOT(on_Timer()));
    timer->start(1000);
}


void Syntezer::on_Timer()
{


}

void Syntezer::out_pcm(short* buffer, int len)
{
    if(!bEnabled) return;
    if(alsa)
    {
        alsa->drop_pcm_frames();
        alsa->out_pcm(buffer, (unsigned long)len);
    }else if(qt)
    {
        qt->drop_pcm_frames();
        qt->out_pcm(buffer, (unsigned long)len);
    }
}

void Syntezer::out_buffer(Buffer* buf)
{
    if(!bEnabled) return;
    if(alsa) alsa->out_buffer(buf);
    if(pulse) pulse->out_buffer(buf);
    if(qt) qt->out_buffer(buf);
}

void Syntezer::drop_pcm_frames()
{
    if(!bEnabled) return;
    if(alsa) alsa->drop_pcm_frames();
}

void Syntezer::on_key_press(int key_code)
{
    if(key_code > 128) return;
    if(!bEnabled) return;
    fprintf(stderr,"keycode=%d", key_code);

    if(key2noteBuffer.find(key_code) == key2noteBuffer.end())
    {

        Buffer* buffer = play_note(key2note[key_code], 10, 1);
        buffer->bWrited = false;
        key_time[key_code].start();
        bKeyPressed = true;
        key2noteBuffer[key_code] = buffer;


    }
}

void msleep(int ms){
   static QMutex mutex;
   static QMutexLocker locker(&mutex);
   mutex.tryLock(ms);
}

void Syntezer::on_key_release(int key_code)
{
   bKeyPressed = false;
   if(!bEnabled) return;
   //emit sigDisableNote(key2note[key_code]);
   Buffer* buf=0;
   bDoOutBuffer = true;
   bAsynch = false;
   if(key2noteBuffer.find(key_code)!=key2noteBuffer.end())
   {
       buf = key2noteBuffer[key_code];
       buf->bWrited = true;
       //QThread::currentThread()->msleep(50);
       //fprintf(stderr,"disable note");
       msleep(50);
       double time_release = release_note(buf, key2note[key_code], (double)key_time[key_code].elapsed()/1000.) ;
       buf->timeEnd = QTime::currentTime().addSecs(time_release);
       key2noteBuffer.erase(key2noteBuffer.find(key_code));

   }
}

bool Syntezer::isChannelEnabled(int n_channel)
{
    return (enabledChannels >> n_channel)&1;
}
