#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "synthesizer.h"
#include "wavFormat.h"
#include "frame.h"
#include "signal_iterator.h"
#include "filter.h"

int main(int argc, char *argv[])
{
    char* FILENAME = 0;
    const unsigned short MAX_FILENAME_SIZE;
    float seconds = 4.0;
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
        FILENAME = "lowpass.WAV";

    wavFile wFile;
    InitializeWaveFile(&wFile);
    SetFilename(FILENAME, &wFile);
    SetBitRate(44100, &wFile);
    SetChannelCount(1, &wFile);
    long frame_index;
    frame* signal_a;
    frame* signal_b;
    AllocateFrames(&signal_a, (long)seconds * 44100, 16, 1);
    AllocateFrames(&signal_b, (long)seconds * 44100, 16, 1);
    GeneratorMethod sine_method;
    UnaryFunction lp_function = &lowpass;
    GeneratorMethodB wave_function = &SawWave;
    GeneratorMethodB sine_function = &SineWave;


    for(frame_index = 0; frame_index < (long)seconds * wFile.fChunk.dwSamplesPerSec; frame_index++) {
        AllocateFrame(&signal_a[frame_index],16, 1);
        AllocateFrame(&signal_b[frame_index],16, 1);
    }

    short max_amplitude = 32600;

    AllocateFrames(&wFile.dChunk.frames, (long)seconds * wFile.fChunk.dwSamplesPerSec, wFile.fChunk.dwBitsPerSample, wFile.fChunk.wChannels);

    for(frame_index = 0; frame_index < (long)seconds * wFile.fChunk.dwSamplesPerSec; frame_index++) {
        AllocateFrame(&wFile.dChunk.frames[frame_index],16, 1);
    }
    SawParameters params;
    params.bitrate = wFile.fChunk.dwSamplesPerSec;
    params.channel = 1;
    params.frequency = 200;
    params.max_amplitude = MAX_AMPLITUDE_16 - 5000;
    params.range = (MAX_AMPLITUDE_16-5000) * 2;

    SineParameters sparams;
    SetSineParameters(&sparams, MAX_AMPLITUDE_16 - 5000, wFile.fChunk.dwSamplesPerSec, 200, 1);

    GenerateSignalB(signal_a, (long)(seconds * wFile.fChunk.dwSamplesPerSec), &params, wave_function);

    SetSineParameters(&sparams, MAX_AMPLITUDE_16 - 5000, wFile.fChunk.dwSamplesPerSec, 480, 1);
    GenerateSignalB(signal_b, (long)(seconds * wFile.fChunk.dwSamplesPerSec), &sparams, sine_function);

    PerformBinaryFunction(wFile.dChunk.frames, signal_a, signal_b, (long)(seconds * wFile.fChunk.dwSamplesPerSec), &AddSignalsMono);
    Copy_Frames(signal_b, wFile.dChunk.frames, (long)(seconds * wFile.fChunk.dwSamplesPerSec));
    PerformUnaryFunction(wFile.dChunk.frames, signal_b, (long)(seconds * wFile.fChunk.dwSamplesPerSec), lp_function);
    //GenerateSawWave_Mono_16a(wFile.dChunk.frames, 440.0, seconds, max_amplitude/7, &wFile.fChunk);
    /*
    Add_Waves_Mono_16(wFile.dChunk.frames,signal_a, signal_b, 44100 * (long)seconds);
    Copy_Frames(signal_b, wFile.dChunk.frames, 44100 * (long)seconds);

    GenerateSawWave_Mono_16a(signal_a, 659.25, seconds, max_amplitude/7, &wFile.fChunk);
    Add_Waves_Mono_16(wFile.dChunk.frames, signal_a, signal_b, 44100 * (long)seconds);

    Copy_Frames(signal_b, wFile.dChunk.frames, 44100 * (long)seconds);
    GenerateSawWave_Mono_16a(signal_a, 174.61, seconds, max_amplitude/7, &wFile.fChunk);
    Add_Waves_Mono_16(wFile.dChunk.frames, signal_a, signal_b, 44100 * (long)seconds);

    Copy_Frames(signal_b, wFile.dChunk.frames, 44100 * (long)seconds);
    GenerateSawWave_Mono_16a(signal_a, 392.00, seconds, max_amplitude/7, &wFile.fChunk);
    Add_Waves_Mono_16(wFile.dChunk.frames, signal_a, signal_b, 44100 * (long)seconds);

    Copy_Frames(signal_b, wFile.dChunk.frames, 44100 * (long)seconds);
    GenerateSawWave_Mono_16a(signal_a, 1318.51, seconds, max_amplitude/7, &wFile.fChunk);
    Add_Waves_Mono_16(wFile.dChunk.frames, signal_a, signal_b, 44100 * (long)seconds);

    Copy_Frames(signal_b, wFile.dChunk.frames, 44100 * (long)seconds);
    GenerateSawWave_Mono_16a(signal_a, 43.65, seconds, max_amplitude/7, &wFile.fChunk);
    Add_Waves_Mono_16(wFile.dChunk.frames, signal_a, signal_b, 44100 * (long)seconds);

    //lowpass(signal_b, wFile.dChunk.frames, 44100 * (long)seconds);
    //Copy_Frames(wFile.dChunk.frames, signal_b, 44100 * (long)seconds);
    */
    wFile.dChunk.header.dwChunkSize = seconds * wFile.fChunk.dwAvgBytesPerSec;

    wFile.rHeader.dwFileLength = GetFileSize(&wFile);
    WriteFile(&wFile);
    Dispose(&wFile);
    free(FILENAME);

    return 0;
}
