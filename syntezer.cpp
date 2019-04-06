#include <QTimer>
#include <QSettings>
#include <QtConcurrent>
#include <QFuture>
#include <stdio.h>
#include <stdlib.h>
#include "syntezer.h"
#include "alsadriver.h"




Syntezer::Syntezer()
    : QObject()
{
    QSettings settings("./settings/settings.ini", QSettings::IniFormat);
    alsa = new AlsaDriver();
    alsa->open((char*)settings.value("alsa_device").toString().toStdString().c_str(),false);
    //connect(this, SIGNAL(sigDisableNote(char)),&alsa->mixer_thread, SLOT(disable_note(char)));
    bKeyPressed = false;
    bEnabled = false;
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
    alsa->drop_pcm_frames();
    alsa->out_pcm(buffer, (unsigned long)len);
}

void Syntezer::out_buffer(Buffer* buf)
{
    if(!bEnabled) return;
    alsa->out_buffer(buf);
}

void Syntezer::drop_pcm_frames()
{
    if(!bEnabled) return;
    alsa->drop_pcm_frames();
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

void Syntezer::on_key_release(int key_code)
{
   bKeyPressed = false;
   if(!bEnabled) return;
   //emit sigDisableNote(key2note[key_code]);
   Buffer* buf=0;
   if(key2noteBuffer.find(key_code)!=key2noteBuffer.end())
   {
       buf = key2noteBuffer[key_code];
       buf->bWrited = true;
       QThread::currentThread()->msleep(50);
       //fprintf(stderr,"disable note");
       double time_release = release_note(buf, key2note[key_code], (double)key_time[key_code].elapsed()/1000.) ;
       buf->timeEnd = QTime::currentTime().addSecs(time_release);
       key2noteBuffer.erase(key2noteBuffer.find(key_code));

   }
}


