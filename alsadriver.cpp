#include <stdio.h>
#include <stdlib.h>
#include <QtGlobal>
#include <QThread>
#include <QSettings>
#include "alsadriver.h"

static QSettings settings("./settings/settings.ini", QSettings::IniFormat);

AlsaDriver::AlsaDriver()
{
    playback_handle = 0;
    bExitThread = false;
    parent = 0;
    Nthreads = 8;
}

AlsaDriver::AlsaDriver(AlsaDriver* parent)
    : parent(parent)
{
    playback_handle = 0;
    bExitThread = false;
    Nthreads = 0;
}

int AlsaDriver::open(char* device_name)
{
    int i;
    int err;
    short buf[128];

    this->device_name = device_name;
    if ((err = snd_pcm_open(&playback_handle, device_name, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
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



      if ((err = snd_pcm_nonblock (playback_handle, 1)) < 0) {
        fprintf (stderr, "cannot set non block (%s)\n",
                 snd_strerror (err));
        return 0;
    }


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

    for (i = 0; i < 10; ++i) {
        if ((err = snd_pcm_writei (playback_handle, buf, 128)) != 128) {
            fprintf (stderr, "write to audio interface failed (%s)\n",
                 snd_strerror (err));
        }
    }
    fprintf (stderr, "thread id = %d\n", QThread::currentThreadId());

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

    return 1;
}


int AlsaDriver::close()
{
    int err;
    bExitThread = true;
    int i=0;
    snd_pcm_close(playback_handle);


}


/*
 *   Underrun and suspend recovery
 */

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

void AlsaDriver::out_buffer(Buffer* buf)
{
    if(device_name!="plug:dmix")
    {
      drop_pcm_frames();
      out_pcm(&buf->samples[0], buf->samples.size()/2);
      return;
    }

    for(int i=0; i < Nthreads; i++)
    {
        if (alsa_threads[i]->quenue.size() == 0)
        {
            alsa_threads[i]->quenue.push_back(buf);
            alsa_threads[i]->waitCondition.wakeAll();
            break;
        }
    }
}

void AlsaDriver::run()
{
    int cptr,err;
    short* ptr;

    //open device if not parent
    if( parent!=0 )
    {
        if(!open((char*)settings.value("alsa_device").toString().toStdString().c_str()))
            return;
        set_nonblock(1);
        fprintf (stderr, "AlsaDriver thread id = %d device=dmix", QThread::currentThreadId());
    }

    while(1)
    {
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
                if( bExitThread )
                {
                    fprintf (stderr,"AlsaDriver Thread exit ");
                    if ((err = snd_pcm_close(playback_handle)) < 0) {
                        fprintf (stderr, "cannot close audio device (%s)\n",
                                 snd_strerror (err));
                        return;
                    }
                    return;
                }
                if( !buf->timeEnd.isNull() )
                {
                    break;
                }

                ptr += err * 2; //2 channels
                cptr -= err;
            }


            quenue.erase(quenue.end()-1);
        }
        mutex.unlock();
    }




    return;

}

int AlsaDriver::out_pcm(short* buf, unsigned long len)
{
    int err;
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
    return 1;
}

int AlsaDriver::drop_pcm_frames()
{
    int err;
    if( playback_handle==0 ) return 0;
    if ((err = snd_pcm_drop(playback_handle)) < 0) {
        fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
             snd_strerror (err));
        return 0;
    }
    return 1;
}

int AlsaDriver::set_nonblock(bool flag)
{
    int err;
    if ((err = snd_pcm_nonblock (playback_handle, flag)) < 0) {
      fprintf (stderr, "cannot set non block (%s)\n",
               snd_strerror (err));
      return 0;
    }
}
