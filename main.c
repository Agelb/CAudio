#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct
{
    // 3 4-byte fields. Must be written to file in this order
    char sGroupID[4]; //This will be the string with "RIFF"
    unsigned long dwFileLength; // This is the file length, with an 8 byte offset
    char sRiffType[4]; //This will be the string "WAVE"
} RIFF_HEADER;

typedef struct
{
    char sChunkID[4]; //Will identify the chunk type
    unsigned long dwChunkSize;
} CHUNK_HEADER;

typedef struct
{
    CHUNK_HEADER header;
    unsigned short wFormatTag;
    unsigned short wChannels;
    unsigned long dwSamplesPerSec; //Sample Rate
    unsigned long dwAvgBytesPerSec;
    unsigned short wBlockAlign; //Sample frame size in bites
    unsigned short dwBitsPerSample;
} FORMAT_CHUNK;

typedef struct
{
    CHUNK_HEADER header;
    //uint8_t* byteArray; // for 8 bit sound
    short* shortArray; // for 16 bit sound
} DATA_CHUNK;

void populate_samples(char* dest, const FORMAT_CHUNK* format, unsigned long seconds){
    short amplitude = 127 / 2; // half of the max amplitude for an 8-bit depth sample
    double frequency = 60.0;
    double angle = (M_PI * 2 * frequency) / format->dwSamplesPerSec;
    long tick;
    double value;
    for(tick = 0; tick < seconds * format->dwSamplesPerSec - 1; tick += 1){
        value = amplitude * sin(angle * tick);
        if(value != 0) {
            dest[tick] = (short)value;
        }
    }
}

int main()
{
    const char* FILENAME = "testfile.WAV";
    unsigned int seconds = 2;
    //Initialize the header memory
    RIFF_HEADER main_header;
    FORMAT_CHUNK format;
    DATA_CHUNK data;

    strncpy(main_header.sGroupID, "RIFF", 4);
    strncpy(main_header.sRiffType, "WAVE", 4);
    strncpy(format.header.sChunkID, "fmt ", 4);
    strncpy(data.header.sChunkID, "data", 4);

    format.header.dwChunkSize = sizeof(format) - sizeof(format.header);
    format.wFormatTag = 1;
    format.wChannels = 1;
    format.dwSamplesPerSec = 44100;
    format.dwBitsPerSample = 16;
    format.wBlockAlign = format.wChannels * (format.dwBitsPerSample / 8);
    format.dwAvgBytesPerSec = format.wBlockAlign * format.dwSamplesPerSec;

    data.shortArray = (short*)malloc(seconds * format.dwSamplesPerSec);
    memset(data.shortArray, '0', seconds * format.dwSamplesPerSec);
    data.header.dwChunkSize = seconds * format.dwAvgBytesPerSec;
    populate_samples(data.shortArray, &format, seconds);
    main_header.dwFileLength = 4 + 16 + (format.dwAvgBytesPerSec * seconds);

    FILE *wav_file = fopen(FILENAME, "w+");
    fwrite(&main_header, 1, sizeof(main_header), wav_file);
    fwrite(&format, 1, sizeof(format), wav_file);
    fwrite(&data, 1, sizeof(data), wav_file);
    fwrite(data.shortArray, 1, (format.dwSamplesPerSec * seconds), wav_file);
    fclose(wav_file);
    free(data.shortArray);
    return 0;
}
