#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "synthesizer.h"
#include "wavFormat.h"
#include "frame.h"

int main(int argc, char *argv[])
{
    char* FILENAME = 0;
    const unsigned short MAX_FILENAME_SIZE;
    float seconds = 2;
    int i;
    for(i=0; i < argc; ++i){
        if(strcmp("-f", argv[i]) == 0) {
            unsigned short filename_argument_size;
            filename_argument_size = strlen(argv[i+1]);
            if(filename_argument_size >= MAX_FILENAME_SIZE - 1) {
                printf("Filenames must be at most %d characters.", MAX_FILENAME_SIZE - 1);
                return 1;
            }
            FILENAME = (char*)malloc(strlen(argv[i+1])+1);
            memset(FILENAME, 0, strlen(argv[i+1]+1));
            strncpy(FILENAME, argv[i+1], strlen(argv[i+1]) + 1);
            i++;
        } else if (strcmp("-s",argv[i]) == 0) {
            seconds = atoi(argv[i+1]);
            i++;
        }
    }

    if(FILENAME == 0)
        FILENAME = "frame_test_2channel.WAV";

    wavFile wFile;
    InitializeWaveFile(&wFile);
    SetFilename(FILENAME, &wFile);
    SetBitRate(44100, &wFile);
    SetChannelCount(2, &wFile);

    AllocateFrames(&wFile.dChunk.frames, (long)seconds * wFile.fChunk.dwSamplesPerSec, wFile.fChunk.dwBitsPerSample, wFile.fChunk.wChannels);
    long frame_index;
    for(frame_index = 0; frame_index < (long)seconds * wFile.fChunk.dwSamplesPerSec; frame_index++) {
        AllocateFrame(&wFile.dChunk.frames[frame_index],16, 1);
    }
    //AllocateFrame(&wFile.dChunk.frames[0],16, 1);
    //memset(wFile.dChunk.shortArray, '0', seconds * wFile.fChunk.dwSamplesPerSec * sizeof(short));
    //sample testSample;
    //testSample.value = 9090;
    //WriteToFrame(&testSample,&wFile.dChunk.frames[0],1,16);
    mGenerate16SineWave(wFile.dChunk.frames, 441.5, seconds, 2, &wFile.fChunk);
    wFile.dChunk.header.dwChunkSize = seconds * wFile.fChunk.dwAvgBytesPerSec;

    wFile.rHeader.dwFileLength = GetFileSize(&wFile);
    WriteFile(&wFile);
    Dispose(&wFile);
    free(FILENAME);

    return 0;
}
