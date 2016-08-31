#ifndef WAVFORMAT_H_INCLUDED
#define WAVFORMAT_H_INCLUDED
#include <stdbool.h>
#include "audioParam.h"
#include "frame.h"

typedef struct
{
    // 3 4-byte fields. Must be written to file in this order
    char sGroupID[4]; //This will be the string with "RIFF"
    unsigned long dwFileLength; // This is the file length, with an 8 byte offset
    char sRiffType[4]; //This will be the string "WAVE"
} riff_header;

typedef struct
{
    char sChunkID[4]; //Will identify the chunk type
    unsigned long dwChunkSize; // Size in bytes
} chunk_header;

typedef struct
{
    chunk_header header;
    unsigned short wFormatTag;
    unsigned short wChannels;
    unsigned long dwSamplesPerSec; //Sample Rate
    unsigned long dwAvgBytesPerSec;
    unsigned short wBlockAlign; //Sample frame size in bites
    unsigned short dwBitsPerSample;
} format_chunk;

typedef struct
{
    chunk_header header;
    //uint8_t* byteArray; // for 8 bit sound
    frame* frames; // for 16 bit sound
} data_chunk;


typedef struct {
    char* filename;
    bool has_data; // prevents format changes that could corrupt data
    riff_header rHeader;
    format_chunk fChunk;
    data_chunk dChunk;
} wavFile;

unsigned long GetFileSize(const wavFile* wFile);
int SetBitRate(unsigned long bit_rate, wavFile* wFile);
int SetBitDepth(unsigned int bit_depth, wavFile* wFile);
int SetChannelCount(unsigned int channels, wavFile* wFile);
int SetFilename(const char* name, wavFile* wFile);
void CalculateFormatFields(wavFile* wFile);
void InitializeWaveFile(wavFile* wFile);
void WriteFile(wavFile* wFile);
void Dispose(wavFile* wFile);
#endif // WAVFORMAT_H_INCLUDED
