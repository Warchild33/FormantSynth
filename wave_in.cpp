#include <QDebug>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

template <typename T>
void write(std::ofstream& stream, const T& t) {
    stream.write((const char*)&t, sizeof(T));
}

void wavwrite(const char* outFile, short* buf, size_t bufSize, int sampleRate, short channels)
{
    std::ofstream stream(outFile, std::ios::binary);                // Open file stream at "outFile" location

    /* Header */
    stream.write("RIFF", 4);                                        // sGroupID (RIFF = Resource Interchange File Format)
    write<int>(stream, 36 + bufSize);                               // dwFileLength
    stream.write("WAVE", 4);                                        // sRiffType

    /* Format Chunk */
    stream.write("fmt ", 4);                                        // sGroupID (fmt = format)
    write<int>(stream, 16);                                         // Chunk size (of Format Chunk)
    write<short>(stream, 1);                                        // Format (1 = PCM)
    write<short>(stream, channels);                                 // Channels
    write<int>(stream, sampleRate);                                 // Sample Rate
    write<int>(stream, sampleRate * channels * sizeof(short)); // Byterate
    write<short>(stream, channels * sizeof(short));            // Frame size aka Block align
    write<short>(stream, 8 * sizeof(short));                   // Bits per sample

    /* Data Chunk */
    stream.write("data", 4);                                        // sGroupID (data)
    stream.write((const char*)&bufSize, 4);                         // Chunk size (of Data, and thus of bufferSize)
    stream.write((const char*)buf, bufSize);                        // The samples DATA!!!
}


void write_raw_samples(const char* outFile, short* buf, size_t bufSize)
{
    std::ofstream stream(outFile, std::ios::binary);                // Open file stream at "outFile" location
    stream.write((const char*)buf, bufSize);                        // The samples DATA!!!
}

void wavwrite2(const char* outFile, double* buf, size_t bufSize, int sampleRate, short channels)
{
     std::vector<short> output_samples;
     output_samples.reserve(bufSize);
     for(int i=0; i < bufSize; i++)
         output_samples.push_back(buf[i]*32768);
     wavwrite(outFile,(short*)&output_samples[0],bufSize,sampleRate,channels);
}
