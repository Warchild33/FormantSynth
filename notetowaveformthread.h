#ifndef MIXERTHREAD_H
#define MIXERTHREAD_H

#include <QWaitCondition>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QSemaphore>
#include <QElapsedTimer>
#include "buffer.h"


#define SIZE_OF_WBUFFER 48000*2*100
#define SIZE_OF_CHUNK 48000*4*2
#define SIZE_OF_CIRC_BUFFER 48000

class ProducerThread : public QThread
{
    Q_OBJECT
public:
    ProducerThread(short* common_buffer, double* common_bufferD);
    void run();

    short* common_buffer;
    double* common_bufferD;
};

class NoteToWaveformThread : public QThread
{
    Q_OBJECT
public:
    NoteToWaveformThread();

    int append_waveform_chunk(double* common_samplesD, short* common_samples);
    void delete_writed_buffers();
    void take_new_buffers();
    short* get_common_samples(short* common_samples);
    int  get_bytes_written();
    void check_buffer_overrun();
    void run();
    int  out_buffer(Buffer* buffer);
    QMutex                      mutex, mutex2, mutex3;


    void copyMemory(short* common_samples, short* from, int size);

private:
    double                     common_samplesD[SIZE_OF_WBUFFER];
    short                      common_samples[SIZE_OF_WBUFFER];
    QElapsedTimer              timerLastNote;
    int                        period_size;
    int                        writed;


    std::vector<Buffer*>        output_buffers;
    std::vector<Buffer*>        process_buffers;


};
#endif // MIXERTHREAD_H
