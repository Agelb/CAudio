#include "wavFormat.h"
#include <stdio.h>

int SetBitRate(unsigned long bit_rate, wavFile* wFile) {
    if(wFile->has_data == false){
        wFile->fChunk.dwSamplesPerSec = bit_rate;
        CalculateFormatFields(wFile);
        return 0;
    }
    return 1; // File has data, do not change the format chunk
}

int SetBitDepth(unsigned int bit_depth, wavFile* wFile) {
    if(wFile->has_data == false){
        wFile->fChunk.dwBitsPerSample = bit_depth;
        //TODO: update data chunk to use the appropriate sample data type based on bit_depth
        CalculateFormatFields(wFile);
        return 0;
    }
    return 1; // File has data, do not change the format chunk
}

int SetChannelCount(unsigned int channels, wavFile* wFile){
    if(wFile->has_data == false){
        wFile->fChunk.wChannels = channels;
        //TODO: update data chunk to use the appropriate sample data type based on channels
        CalculateFormatFields(wFile);
        return 0;
    }
    return 1; // File has data, do not change the format chunk
}

int SetFilename(const char* name, wavFile* wFile) {
    wFile->filename = (char*)malloc(strlen(name) + 1);
    memset(wFile->filename, 0, strlen(name) + 1);
    strncpy(wFile->filename, name, strlen(name) + 1);
    return 0;
}

unsigned long GetFileSize(const wavFile* wFile){
    /*
        The riff header is always 4 bytes (12 total, - 8 offset),
        and the format chunk is 24 (8 for the header, 16 for the content).
    */
    // TODO: calculate multiple data chunks
    return (4 + 24 + wFile->dChunk.header.dwChunkSize);
}

void CalculateFormatFields(wavFile* wFile) {
    format_chunk* pformat = &wFile->fChunk; // For readability of this method
    pformat->wBlockAlign = pformat->wChannels * (pformat->dwBitsPerSample / 8);
    pformat->dwAvgBytesPerSec = pformat->wBlockAlign * pformat->dwSamplesPerSec;
}

void InitializeWaveFile(wavFile* wFile){
    strncpy(wFile->rHeader.sGroupID, "RIFF", 4);
    strncpy(wFile->rHeader.sRiffType, "WAVE", 4);
    strncpy(wFile->fChunk.header.sChunkID, "fmt ", 4);
    strncpy(wFile->dChunk.header.sChunkID, "data", 4);

    wFile->fChunk.header.dwChunkSize = 16;
    wFile->fChunk.wFormatTag = 1;
    wFile->fChunk.wChannels = 1;
    wFile->fChunk.dwSamplesPerSec = 44100;
    wFile->fChunk.dwBitsPerSample = 16;
    CalculateFormatFields(wFile);
}

void WriteFile(wavFile* wFile) {
    // TODO: error handling in file write
    int frame_index;
    unsigned int bytes_per_frame = (wFile->fChunk.dwBitsPerSample / 8) * wFile->fChunk.wChannels;
    unsigned long total_frames = wFile->dChunk.header.dwChunkSize / bytes_per_frame;
    FILE *file_stream = fopen(wFile->filename, "wb+"); // Make sure this is opened in binary mode
    // TODO: platform independent file writing
    fwrite(&wFile->rHeader, 1, sizeof(wFile->rHeader), file_stream);
    fwrite(&wFile->fChunk, 1, sizeof(wFile->fChunk), file_stream);
    fwrite(&wFile->dChunk.header, 1, 8, file_stream); // Data chunk headers are always 8 bytes
    // TODO: writing multiple data chunks
    for(frame_index = 0; frame_index < total_frames - 1; frame_index++) {
        if(wFile->fChunk.dwBitsPerSample == 16) {
            fwrite(&wFile->dChunk.frames[frame_index].samples[0], sizeof(sample), 1, file_stream);
        }
    }
    fwrite(wFile->dChunk.frames, 1, wFile->dChunk.header.dwChunkSize, file_stream);
    fclose(file_stream);
}

void Dispose(wavFile* wFile) {
    int frame_index;
    unsigned int bytes_per_frame = (wFile->fChunk.dwBitsPerSample / 8) * wFile->fChunk.wChannels;
    unsigned long total_frames = wFile->dChunk.header.dwChunkSize / bytes_per_frame;
    for(frame_index = 0; frame_index < total_frames; frame_index++) {
        printf("frame index %d", frame_index);
        //DisposeFrame(&wFile->dChunk.frames[frame_index]);
  //      free(wFile->dChunk.frames[frame_index].sSamples);
    //    free(wFile->dChunk.frames[frame_index].tSamples);
    }
    free(wFile->dChunk.frames);
    free(wFile->filename);
}
