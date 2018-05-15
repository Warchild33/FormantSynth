#ifndef WAVE_IN_H
#define WAVE_IN_H

void wavwrite(const char* outFile, short* buf, size_t bufSize, int sampleRate, short channels);
void wavwrite2(const char* outFile, double* buf, size_t bufSize, int sampleRate, short channels);
void write_raw_samples(const char* outFile, short* buf, size_t bufSize);

#endif // WAVE_IN_H
