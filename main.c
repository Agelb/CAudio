#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "synthesizer.h"
#include "wavFormat.h"

int main(int argc, char *argv[])
{
    char* FILENAME = 0;
    const unsigned short MAX_FILENAME_SIZE;
    float seconds = 1.2;
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
        FILENAME = "default.WAV";

    wavFile wFile;
    InitializeWaveFile(&wFile);
    SetFilename(FILENAME, &wFile);
    SetBitRate(41000, &wFile);

    // TODO: refactor samples/frames
    wFile.dChunk.shortArray = (short*)malloc((seconds * wFile.fChunk.dwSamplesPerSec) * sizeof(short));
    memset(wFile.dChunk.shortArray, '0', seconds * wFile.fChunk.dwSamplesPerSec * sizeof(short));
    GenerateSineWave(wFile.dChunk.shortArray, 441.5, seconds, &wFile.fChunk);
    wFile.dChunk.header.dwChunkSize = seconds * wFile.fChunk.dwAvgBytesPerSec;

    wFile.rHeader.dwFileLength = GetFileSize(&wFile);
    WriteFile(&wFile);
    Dispose(&wFile);
    free(FILENAME);
    return 0;
}
