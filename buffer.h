#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <QTime>

class Buffer
{
public:
    Buffer();
    Buffer(int SampleRate, double duration, int channels);
    ~Buffer();
    bool checkExpired();
    std::vector<short> samples;
    std::vector<double> samplesD;
    int              note;
    bool            bWrited;
    QTime           timeStart;
    QTime           timeEnd;  // then note is canceled by unpressing key
    double          duration; // default duration of note

};

#endif // BUFFER_H
