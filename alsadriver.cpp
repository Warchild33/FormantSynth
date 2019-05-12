#include <stdio.h>
#include <stdlib.h>
#include <QtGlobal>
#include <QThread>
#include <QSettings>
#include "alsadriver.h"

static QSettings settings("./settings/settings.ini", QSettings::IniFormat);
//snd_pcm_t* AlsaDriver::playback_handle = 0;
//snd_pcm_hw_params_t* AlsaDriver::hw_params=0;

AlsaDriver::AlsaDriver()
{
    //playback_handle = 0;
    bExitThread = false;
    parent = 0;
    Nthreads = 8;
#ifdef __linux__
    if(playback_handle != 0)
    {
        createThreads((char*)settings.value("alsa_device").toString().toStdString().c_str());
    }
 #endif

}

AlsaDriver::AlsaDriver(AlsaDriver* parent)
    : parent(parent)
{
    //playback_handle = 0;
    bExitThread = false;
    Nthreads = 0;
}

void AlsaDriver::createThreads(char* device_name)
{
    if(QString(device_name) == "plug:dmix")
    {
        if(parent == 0)
        {
            for(int i=0; i < Nthreads; i++)
            {
                AlsaDriver* alsa_thread = new AlsaDriver(this);
                alsa_threads.push_back(alsa_thread);
                alsa_thread->start();

            }
        }
    }
}

int AlsaDriver::open(char* device_name, bool bTest)
{
    int i;
    int err;
    short buf[128];
    //return 0;
    this->device_name = device_name;

#ifdef __linux__

    if ((err = snd_pcm_open(&playback_handle, device_name, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK)) < 0) {
        fprintf (stderr, "cannot open audio device %s (%s)\n",
             device_name,
             snd_strerror (err));
        if(playback_handle)
        {
            snd_pcm_close(playback_handle);
            playback_handle = 0;
        }
        return 0;
    }

    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
        fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
             snd_strerror (err));
        snd_pcm_close(playback_handle);
        return 0;
    }

    if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
             snd_strerror (err));
        snd_pcm_close(playback_handle);
        return 0;
    }



//      if ((err = snd_pcm_nonblock (playback_handle, 1)) < 0) {
//        fprintf (stderr, "cannot set non block (%s)\n",
//                 snd_strerror (err));
//        return 0;
//    }


    if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf (stderr, "cannot set access type (%s)\n",
             snd_strerror (err));
        snd_pcm_close(playback_handle);
        return 0;
    }

    if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
        fprintf (stderr, "cannot set sample format (%s)\n",
             snd_strerror (err));
        snd_pcm_close(playback_handle);
        return 0;
    }

    if ((err = snd_pcm_hw_params_set_rate (playback_handle, hw_params, 48000, 0)) < 0) {
        fprintf (stderr, "cannot set sample rate (%s)\n",
             snd_strerror (err));
        snd_pcm_close(playback_handle);
        return 0;
    }

    if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, 2)) < 0) {
        fprintf (stderr, "cannot set channel count (%s)\n",
             snd_strerror (err));
        snd_pcm_close(playback_handle);
        return 0;
    }

    if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot set parameters (%s)\n",
             snd_strerror (err));
        snd_pcm_close(playback_handle);
        return 0;
    }

    snd_pcm_hw_params_free (hw_params);


        if ((err = snd_pcm_prepare (playback_handle)) < 0) {
            fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
                 snd_strerror (err));
            snd_pcm_close(playback_handle);
            return 0;
        }

    if( bTest )
    {

        for (i = 0; i < 10; ++i) {
            if ((err = snd_pcm_writei (playback_handle, buf, 128)) != 128) {
                fprintf (stderr, "write to audio interface failed (%s)\n",
                     snd_strerror (err));
            }
        }
    }
    //fprintf (stderr, "thread id = %d\n", QThread::currentThreadId());

    createThreads(device_name);
#endif
    return 1;
}


int AlsaDriver::close()
{
    int err;
    bExitThread = true;
    int i=0;
#ifdef __linux__
    snd_pcm_close(playback_handle);
#endif

}


/*
 *   Underrun and suspend recovery
 */
#ifdef __linux__
static int xrun_recovery(snd_pcm_t *handle, int err)
{
        if (err == -EPIPE) {    /* under-run */
                err = snd_pcm_prepare(handle);
                if (err < 0)
                        fprintf (stderr,"Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));
                return 0;
        } else if (err == -ESTRPIPE) {
                while ((err = snd_pcm_resume(handle)) == -EAGAIN)
                        sleep(1);       /* wait until the suspend flag is released */
                if (err < 0) {
                        err = snd_pcm_prepare(handle);
                        if (err < 0)
                                fprintf (stderr,"Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));
                }
                return 0;
        }
        return err;
}
#endif

void AlsaDriver::out_buffer(Buffer* buf)
{
    if(device_name!="plug:dmix")
    {
      //drop_pcm_frames();
      out_pcm(&buf->samples[0], buf->samples.size()/2);
      return;
    }

    int cnt = 0;
    //for(int i=0; i < Nthreads; i++)
    for(auto t = alsa_threads.begin(); t!=alsa_threads.end(); t++,cnt++)
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

void AlsaDriver::run()
{

    //open device if not parent

    if( parent!=0 )
    {
        if(!open((char*)settings.value("alsa_device").toString().toStdString().c_str(), false))
            return;
        //fprintf (stderr, "AlsaDriver thread id = %d device=dmix", QThread::currentThreadId());
    }

    while(1)
    {
        bool multiple_voice = settings.value("multiple_voice").toBool();
        //set_nonblock(!multiple_voice);
        if( multiple_voice )
            nonBlockingLoop();//blockingLoop();
        else
            nonBlockingLoop();
        if( bExitThread )
        {
            int err;
            fprintf (stderr,"AlsaDriver Thread exit ");
  #ifdef __linux__
            if ((err = snd_pcm_close(playback_handle)) < 0) {
                fprintf (stderr, "cannot close audio device (%s)\n",
                         snd_strerror (err));
                return;
            }
  #endif
            return;
        }
    }


    return;
}

void AlsaDriver::nonBlockingLoop()
{
    int cptr,err;
    short* ptr;

    mutex.lock();
    waitCondition.wait(&mutex);
    if( quenue.size() > 0 )
    {
        Buffer* buf = quenue.back();
        //out_pcm(&buf->samples[0], buf->samples.size()/2);

        int period_size = buf->samples.size()/2;

        cptr = period_size;
        ptr = &buf->samples[0];
        while (cptr > 0) {
            #ifdef __linux__
            err = snd_pcm_writei(playback_handle, ptr, 1);

            if (err == -EAGAIN)
                continue;
            if (err < 0) {
                if (xrun_recovery(playback_handle, err) < 0) {
                    fprintf (stderr,"Write error: %s\n", snd_strerror(err));
                    continue;
                }
                continue;
            }
            #endif

            if( (!buf->timeEnd.isNull()) && (buf->timeEnd < QTime::currentTime()) )
            {
                break;
            }

            ptr += err * 2; //2 channels
            cptr -= err;
        }

        delete buf;
        quenue.erase(quenue.end()-1);
    }
    mutex.unlock();


}

void AlsaDriver::blockingLoop()
{
    mutex.lock();
    waitCondition.wait(&mutex);
    if( quenue.size() > 0 )
    {
        Buffer* buf = quenue.back();
        out_pcm(&buf->samples[0], buf->samples.size()/2);
        quenue.erase(quenue.end()-1);
        delete buf;
    }
    mutex.unlock();

}


int AlsaDriver::out_pcm(short* buf, unsigned long len)
{
    int err;
#ifdef __linux__
    if( playback_handle==0 ) return 0;
    if ((err = snd_pcm_prepare (playback_handle)) < 0) {
        fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
             snd_strerror (err));
        return 0;
    }

    if ((err = snd_pcm_writei (playback_handle, buf, len)) != len) {
        fprintf (stderr, "write to audio interface failed (%s)\n",
             snd_strerror (err));
        return 0;
    }
#endif
    return 1;
}

int AlsaDriver::drop_pcm_frames()
{
    int err;
 #ifdef __linux__
    if( playback_handle==0 ) return 0;
    if ((err = snd_pcm_drop(playback_handle)) < 0) {
        fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
             snd_strerror (err));
        return 0;
    }
#endif
    return 1;
}

int AlsaDriver::set_nonblock(bool flag)
{
    int err;
    #ifdef __linux__
    if ((err = snd_pcm_nonblock (playback_handle, flag)) < 0) {
      fprintf (stderr, "cannot set non block (%s)\n",
               snd_strerror (err));
      return 0;
    }
#endif
}
