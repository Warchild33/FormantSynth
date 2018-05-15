#include <stdio.h>
#include <stdlib.h>
#include <QtGlobal>
#include <QThread>
#include "alsadriver.h"

AlsaDriver::AlsaDriver()
{
    playback_handle = 0;
    bProcessBuffers = false;

}

int AlsaDriver::open(char* device_name)
{
    int i;
    int err;
    short buf[128];

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
    fprintf (stderr, "thread id = %d", QThread::currentThreadId());
    start();
    moveToThread(this);
    notetowaveform_thread.start();
    return 1;
}

int AlsaDriver::close()
{
    int err;
    Qt::HANDLE h = QThread::currentThreadId();
/*    snd_pcm_drop(playback_handle);
    if ((err = snd_pcm_close(playback_handle)) < 0) {
        fprintf (stderr, "cannot close audio device (%s)\n",
             snd_strerror (err));
        return 0;
    }
    */
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

extern QSemaphore                  freeBytes;
extern QSemaphore                  usedBytes;


void AlsaDriver::run()
{
    fprintf (stderr, "thread id = %d", QThread::currentThreadId());
    int cptr,err;
    short* ptr;
    short* common_samples;

    int period_size = SIZE_OF_CHUNK;
    //usleep(100);
    while (1) {
        //generate_sine(areas, 0, period_size, &phase);
        //for(int i=0; i < 48000*2; i++) common_samples[i] = rand();
        cptr = period_size;
        common_samples = notetowaveform_thread.get_common_samples(common_samples);

        ptr = common_samples;
        while (cptr > 0) {
              usedBytes.acquire();
              err = snd_pcm_writei(playback_handle, ptr, 1);
              freeBytes.release();
            //usleep(1);
            //mixer_thread.mixedCondition.wakeAll();

            //fprintf (stderr,"Write : %d\n", err);
            if (err == -EAGAIN)
                continue;
            if (err < 0) {
                if (xrun_recovery(playback_handle, err) < 0) {
                    fprintf (stderr,"Write error: %s\n", snd_strerror(err));
                    break;
                }
                break;
            }
            ptr += err * 2; //2 channels
            cptr -= err;
        }
    }
}

/*
void AlsaDriver::run()
{
    double phase = 0;
    const snd_pcm_channel_area_t *areas;
    snd_pcm_sframes_t avail, size, commitres;
    snd_pcm_uframes_t offset, frames;
    int err;
    snd_pcm_state_t state;

    state = snd_pcm_state(playback_handle);
    if (state == SND_PCM_STATE_XRUN) {
            err = xrun_recovery(playback_handle, -EPIPE);
            if (err < 0) {
                    printf("XRUN recovery failed: %s\n", snd_strerror(err));
            }

    } else if (state == SND_PCM_STATE_SUSPENDED) {
            err = xrun_recovery(playback_handle, -ESTRPIPE);
            if (err < 0) {
                    printf("SUSPEND recovery failed: %s\n", snd_strerror(err));
            }
    }
    avail = snd_pcm_avail_update(playback_handle);
    double common_samplesD[48000*2];
    short  common_samples[48000*2];

    while (1) {
        //generate_sine(areas, 0, period_size, &phase);
        memset(common_samplesD, 0, 48000*sizeof(double)*2);
        memset(common_samples, 0, 48000*sizeof(short)*2);
        //for(int i=0; i < 1024; i++) common_samples[i] = rand();
        fill_common_samples(common_samplesD, common_samples, 48000);
        // at this point, we can transfer at least 'avail' frames
        int period_size = 48000;
        // we want to process frames in chunks (period_size)
        if (avail < period_size)
          return;
        size = period_size;
        // it is possible that contiguous areas are smaller, thus we use a loop
        while (size > 0) {
          frames = size;
          err = snd_pcm_mmap_begin(playback_handle, &areas, &offset, &frames);
          if (err < 0)
          {
            fprintf (stderr,"Write error: %s\n", snd_strerror(err));
          }
          // this function fills the areas from offset with count of frames
          //generate_sine(areas, offset, frames, &phase);
          commitres = snd_pcm_mmap_commit(playback_handle, offset, frames);
          if (commitres < 0 || commitres != frames)
          {
            fprintf (stderr,"Write error: %s\n", snd_strerror(commitres >= 0 ? -EPIPE : commitres));
          }

          size -= frames;
        }
    }

}
*/

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
