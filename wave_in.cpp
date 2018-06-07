#include <QDebug>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

template <typename T>
void write(std::ofstream& stream, const T& t) {
    stream.write((const char*)&t, sizeof(T));
}

template <typename T>
void read(std::ifstream& stream, const T& t) {
    stream.read((char*)&t, sizeof(T));
}


void wavwrite(const char* outFile, short* buf, int bufSize, int sampleRate, short channels)
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

void wavread(const char* outFile, std::vector<short>& buf, int& sampleRate, short& channels)
{
    int  Chunk_size;
    short Format, bits_per_sample;
    int bufSize;
    char buf2[128];

    std::ifstream stream(outFile, std::ios::binary);                // Open file stream at "outFile" location

    /* Header */
    stream.read(buf2, 4);                                        // sGroupID (RIFF = Resource Interchange File Format)
    read<int>(stream, bufSize);                               // dwFileLength
    buf.resize(bufSize);
    bufSize-=36;
    stream.read(buf2, 4);                                        // sRiffType

    /* Format Chunk */
    stream.read(buf2, 4);                                        // sGroupID (fmt = format)
    read<int>(stream, Chunk_size);                                         // Chunk size (of Format Chunk)
    read<short>(stream, Format);                                        // Format (1 = PCM)
    read<short>(stream, channels);                                 // Channels
    read<int>(stream, sampleRate);                                 // Sample Rate
    read<int>(stream, Chunk_size);                              // Byterate
    read<short>(stream, Chunk_size);            // Frame size aka Block align
    read<short>(stream, bits_per_sample);                   // Bits per sample

    /* Data Chunk */
    stream.read(buf2, 4);                                        // sGroupID (data)
    stream.read((char*)&bufSize, 4);                         // Chunk size (of Data, and thus of bufferSize)
    stream.read((char*)&buf[0], bufSize*sizeof(short));                        // The samples DATA!!!
}


void write_raw_samples(const char* outFile, short* buf, int bufSize)
{
    std::ofstream stream(outFile, std::ios::binary);                // Open file stream at "outFile" location
    stream.write((const char*)buf, bufSize);                        // The samples DATA!!!
}

void wavwrite2(const char* outFile, double* buf, int bufSize, int sampleRate, short channels)
{
     std::vector<short> output_samples;
     output_samples.reserve(bufSize);
     for(int i=0; i < bufSize; i++)
         output_samples.push_back(buf[i]*32768);
     wavwrite(outFile,(short*)&output_samples[0],bufSize,sampleRate,channels);
}
