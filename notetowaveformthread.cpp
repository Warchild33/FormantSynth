#include "notetowaveformthread.h"

QSemaphore                  freeBytes(SIZE_OF_CIRC_BUFFER);
QSemaphore                  usedBytes;


NoteToWaveformThread::NoteToWaveformThread()
{
    period_size = 48000;
    //ProducerThread* thread = new ProducerThread(common_samples, common_samplesD);
    //thread->start();
}


void NoteToWaveformThread::delete_writed_buffers()
{
    std::vector<Buffer*> writedBuffers;


    for(auto it = process_buffers.begin(); it!=process_buffers.end(); it++)
        if( (*it)->bWrited)
            writedBuffers.push_back(*it);

    int cnt=0;
    for(auto it = writedBuffers.begin(); it!=writedBuffers.end(); it++)
    {
        process_buffers.erase(std::find(process_buffers.begin(),process_buffers.end(),(*it)));
        cnt++;
    }

}

void NoteToWaveformThread::take_new_buffers()
{
    mutex.lock();    
    if( output_buffers.size()!=0 )
    {
        for(auto it = output_buffers.begin(); it!=output_buffers.end(); it++)
            process_buffers.push_back((*it));
        output_buffers.clear();
    }
    mutex.unlock();
}

int NoteToWaveformThread::append_waveform_chunk(double* common_samplesD, short* common_samples)
{
    int count = 0;

    take_new_buffers();


    mutex.lock();
    int maxCount = 0;
    for(auto it = process_buffers.begin(); it!=process_buffers.end(); it++)
    {
        Buffer* buf = (*it);
        if(!buf->bWrited)
        {

            double* optrD = &common_samplesD[0];
            short* optr = &common_samples[0];
            double* iptr = &buf->samplesD[0];
//            count = buf->timeStart.msecsTo(buf->timeEnd) * 48 * 2;
//             if(count > buf->samplesD.size() || buf->timeEnd.isNull() )
//                 count = buf->samplesD.size();
//            if( buf->timeEnd.isNull() )
//                count = buf->timeStart.msecsTo(QTime::currentTime()) * 48 * 2;
            count = buf->samplesD.size();

            maxCount = qMax(maxCount, count);
            int i = 0;
            while(i < count)
            {

                *optrD = (*optrD/4 + (*iptr)/4)*2;
                i++; optrD++; iptr++; optr++;

            }

            fprintf(stderr,"buf %d note %d count %d time_last = %d \n",it - process_buffers.begin(), buf->note, count, timerLastNote.elapsed());
            if( !buf->timeEnd.isNull() )
              buf->bWrited = true;
        }
    }
    //fprintf(stderr,"lastnote time %d", timerLastNote.elapsed());
    if(process_buffers.size()>0)
    if(timerLastNote.elapsed() > 100)
    {
        int i = 0;
        while(i < SIZE_OF_CIRC_BUFFER)
        {
            freeBytes.acquire();
            common_samples[i % SIZE_OF_CIRC_BUFFER] = common_samplesD[i % SIZE_OF_CIRC_BUFFER]*32768;
            usedBytes.release();
            i++;
        }
        memset(common_samplesD, 0, SIZE_OF_WBUFFER*sizeof(double));
        memset(common_samples, 0, SIZE_OF_WBUFFER*sizeof(short));
        delete_writed_buffers();

    }


    mutex.unlock();


    return 0;
}


int  NoteToWaveformThread::out_buffer(Buffer* buffer)
{
    //mutex.lock();
    output_buffers.push_back(buffer);
    timerLastNote.start();
    //mutex.unlock();
    return 0;
}

void NoteToWaveformThread::copyMemory(short* common_samples, short* from, int size)
{
    short* ptr1 = common_samples;
    short* ptr2 = from;
    int count = size ;
    while(count>0)
    {
       *ptr1 = *ptr2;
       count--; ptr1++; ptr2++;
    }
}

short* NoteToWaveformThread::get_common_samples(short* common_samples)
{   
    //mutex2.lock();
    //mixedCondition.wait(&mutex2);
    //mixedCondition.wait(&mutex2);
    //copyMemory(common_samples, this->common_samples, SIZE_OF_CHUNK);
    //mutex2.unlock();
    //common_samples = this->common_samples;
    return this->common_samples;
}

int NoteToWaveformThread::get_bytes_written()
{
    return writed;
}

void NoteToWaveformThread::run()
{

    while (1)
    {
         mutex3.lock();
         //mixedCondition.wait(&mutex3);
         writed = append_waveform_chunk(common_samplesD, common_samples);

         //copyMemory(this->common_samples2, common_samples, period_size * 2);
         mutex3.unlock();
         //mixedCondition.wakeAll();
         //usleep(10);

         //fprintf(stderr, "%d writed", writed);
         //

    }
}

ProducerThread::ProducerThread(short* common_buffer, double* common_bufferD)
    :common_buffer(common_buffer),common_bufferD(common_bufferD)
{

}

void ProducerThread::run()
{

    while (1)
    {
        //if(process_buffers.size() > 0)
        {
            int i = 0;
            while(i < SIZE_OF_CIRC_BUFFER)
            {
                freeBytes.acquire();
                common_buffer[i % SIZE_OF_CIRC_BUFFER] = common_bufferD[i % SIZE_OF_CIRC_BUFFER]*32768;
                usedBytes.release();
                i++;
            }
        }
    }
}
