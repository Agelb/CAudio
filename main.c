#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "wavFormat.h"

void populate_samples(short* dest, const format_chunk* format, unsigned long seconds){
    //printf("PI: %f", M_PI);
    short amplitude = 32760 / 2; // half of the max amplitude for an 8-bit depth sample
    double frequency = 440.0;
    double angle = (M_PI * 2 * frequency) / (format->dwSamplesPerSec * format->wChannels);
    long tick;
    double value;
    for(tick = 0; tick < seconds * format->dwSamplesPerSec - 1; tick += 1){
        value = amplitude * sin(angle * tick);
        //value = amplitude * sin(((M_PI*2) * tick * frequency)/format->dwSamplesPerSec);
        //value = amplitude * sin((2 * M_PI * tick * frequency) / 44100);
        //value = amplitude * sine(tick))
        dest[tick] = (short)value;
    }
}

int main(int argc, char *argv[])
{
    char* FILENAME = 0;
    const unsigned short MAX_FILENAME_SIZE;
    unsigned int seconds = 1;
    int i;
    for(i=0; i < argc; ++i){
        printf("Argument %d : %s\n",i, argv[i]);
        if(strcmp("-f", argv[i]) == 0) {
            unsigned short filename_argument_size;
            filename_argument_size = strlen(argv[i+1]);
            if(filename_argument_size >= MAX_FILENAME_SIZE - 1) {
                printf("Filenames must be at most %d characters.", MAX_FILENAME_SIZE - 1);
                return 1;
            }
            FILENAME = (char*)malloc(strlen(argv[i+1]+1));
            strncpy(FILENAME, argv[i+1], strlen(argv[i+1]));
            i++;
        } else if (strcmp("-s",argv[i]) == 0) {
            printf("Seconds are specified: %d\n", argv[i+1]);
            //seconds = 5;//atoi(argv[i+1]);
            i++;
        }
    }

    if(FILENAME == 0)
        FILENAME = "Defaultb.WAV";

    wavFile wFile;
    InitializeWaveFile(&wFile);
    SetFilename("refactored.wav", &wFile);

    // TODO: refactor samples/frames, and writing data to wave
    wFile.dChunk.shortArray = (short*)malloc((seconds * wFile.fChunk.dwSamplesPerSec) * sizeof(short));
    memset(wFile.dChunk.shortArray, '0', seconds * wFile.fChunk.dwSamplesPerSec * sizeof(short));
    wFile.dChunk.header.dwChunkSize = seconds * wFile.fChunk.dwAvgBytesPerSec;
    populate_samples(wFile.dChunk.shortArray, &wFile.fChunk, seconds);

    wFile.rHeader.dwFileLength = GetFileSize(&wFile);
    WriteFile(&wFile);
    Dispose(&wFile);
    free(FILENAME);
    return 0;
}
