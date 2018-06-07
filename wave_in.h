#ifndef WAVE_IN_H
#define WAVE_IN_H

#include <vector>

void wavread(const char* outFile, std::vector<short>& buf, int& sampleRate, short& channels);
void wavwrite(const char* outFile, short* buf, int bufSize, int sampleRate, short channels);
void wavwrite2(const char* outFile, double* buf, int bufSize, int sampleRate, short channels);
void write_raw_samples(const char* outFile, short* buf, int bufSize);

#endif // WAVE_IN_H
