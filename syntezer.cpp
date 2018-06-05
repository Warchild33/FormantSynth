#include <QTimer>
#include <QSettings>
#include <stdio.h>
#include <stdlib.h>
#include "syntezer.h"
#include "alsadriver.h"




Syntezer::Syntezer()
    : QObject()
{
    QSettings settings("./settings/settings.ini", QSettings::IniFormat);
    alsa = new AlsaDriver();
    alsa->open((char*)settings.value("alsa_device").toString().toStdString().c_str());
    //connect(this, SIGNAL(sigDisableNote(char)),&alsa->mixer_thread, SLOT(disable_note(char)));
    bKeyPressed = false;
    bEnabled = false;
    QTimer* timer = new QTimer();
    connect(timer,SIGNAL(timeout()), this, SLOT(on_Timer()));
    timer->start(50);
}

void Syntezer::on_Timer()
{
    if( bKeyPressed )
    {
        if( key_time.elapsed() > 500)
        {
            //alsa->drop_pcm_frames();
            //alsa->out_pcm(&output_samples[0], output_samples.size()/2);
        }
        key_time.start();
    }
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

    if(key2noteBuffer.find(key_code) == key2noteBuffer.end())
    {

        Buffer* buf = play_note(key2note[key_code], 1, 1);
        buf->bWrited = false;
        key_time.start();
        bKeyPressed = true;
        key2noteBuffer[key_code] = buf;
        //fprintf(stderr,"keycode=%d", key_code);

    }
}

void Syntezer::on_key_release(int key_code)
{
   bKeyPressed = false;
   if(!bEnabled) return;
   //emit sigDisableNote(key2note[key_code]);
   if(key2noteBuffer.find(key_code)!=key2noteBuffer.end())
   {
       Buffer* buf = key2noteBuffer[key_code];
       buf->timeEnd = QTime::currentTime();       
       key2noteBuffer.erase(key2noteBuffer.find(key_code));
       //fprintf(stderr,"disable note");
   }
}


