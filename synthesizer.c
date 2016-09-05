#include "synthesizer.h"
#include "wavFormat.h"
#include <math.h>
#include "signal_iterator.h"
/*
    Writes 16-bit samples to the passed destination.
    params: destination, frequency in Hz, duration in seconds, format_chunk from wave file
*/

// TODO: decouple this from the wave format
void GenerateSineWave_Mono_16(frame* dest, const double frequency, const double duration, const format_chunk* format) {
    // TODO: variable amplitudes
    // TODO: amplitude based on bit depth
    //short amplitude = 327603/8; // Max amplitude for 16 bit sound
    short amplitude = 5800;
    float angle = (M_PI * 2 * frequency) / (format->dwSamplesPerSec * format->wChannels);
    double seconds_per_sample = format->dwSamplesPerSec / frequency;
    unsigned long samples_per_cycle = seconds_per_sample * angle;
    unsigned long total_frames = duration * format->dwSamplesPerSec;
    unsigned long index;
    sample iterated_sample;
    for(index = 0; index < total_frames; index++)
    {
        iterated_sample.value = (short)(amplitude * sin(angle*index));
        WriteToFrame(&iterated_sample,&dest[index],1,format->dwBitsPerSample);
    }
}

void GenerateSineWave_Mono_16a(frame* dest, const double frequency, const double duration, const unsigned short amplitude, const format_chunk* format) {
    // TODO: amplitude based on bit depth
    float angle = (M_PI * 2 * frequency) / (format->dwSamplesPerSec * format->wChannels);
    unsigned long total_frames = duration * format->dwSamplesPerSec;
    unsigned long index;
    sample iterated_sample;
    for(index = 0; index < total_frames; index++)
    {
        iterated_sample.value = (short)(amplitude * sin(angle*index));
        WriteToFrame(&iterated_sample,&dest[index],1,format->dwBitsPerSample);
    }
}

void GenerateSineWave_Mono_16ah(frame* dest, const double frequency, const double duration, const unsigned short amplitude, const unsigned long block_num, const format_chunk* format) {
    // TODO: amplitude based on bit depth
    float angle = (M_PI * 2 * frequency) / (format->dwSamplesPerSec * format->wChannels);
    unsigned long total_frames = duration * format->dwSamplesPerSec;
    unsigned long index;
    sample iterated_sample;
    for(index = 0; index < total_frames; index++)
    {
        //printf("sample %d", index);
        iterated_sample.value = (short)(amplitude * sin(angle*(index+(block_num*BLOCK_SIZE))));
        WriteToFrame(&iterated_sample,&dest[index+(block_num*BLOCK_SIZE)],1,format->dwBitsPerSample);
    }
}

/*void GenerateSawWave_Mono_16a(frame* dest, const double frequency, const double duration, const unsigned short amplitude, const unsigned long block_num, const format_chunk* format) {
    //frequency cycles per second
    short max_amplitude = 32760/8;
    double step_size = ((float)max_amplitude*2 / format->dwSamplesPerSec) * frequency;
    //float angle = (M_PI * 2 * frequency) / (format->dwSamplesPerSec * format->wChannels);
    //unsigned long total_frames = duration * format->dwSamplesPerSec;
    unsigned long total_frames = duration * format->dwSamplesPerSec;
    unsigned long index;
    sample iterated_sample;
    for(index = 0; index < total_frames; index++)
    {
        //iterated_sample.value = ((short)(index * step_size) % max_amplitude) - max_amplitude;
        iterated_sample.value = ((index * (short)step_size) % (max_amplitude*2)) - max_amplitude;
        WriteToFrame(&iterated_sample,&dest[index],1,format->dwBitsPerSample);
    }
}*/

void SawWave(frame* dest, const void* saw, const unsigned long *input) {
    SawParameters* params = (SawParameters*)saw;
    double result = ((*input * ((int)((params->range)/params->bitrate) * params->frequency)) % params->range) - params->max_amplitude;
    dest->samples[params->channel].value = result;
}


void mGenerate16SineWave(frame* dest, const double frequency, const double duration, const int num_channels, const format_chunk* format) {
    // TODO: variable amplitudes
    // TODO: amplitude based on bit depth
    short amplitude = 32760/4; // Max amplitude for 16 bit sound
    float angle = (M_PI * 2 * frequency) / (format->dwSamplesPerSec * format->wChannels);
    unsigned long total_frames = duration * format->dwSamplesPerSec;
    unsigned long index;
    unsigned int channel_index;
    sample iterated_sample;
    for(index = 0; index < total_frames; index++)
    {
        iterated_sample.value = (short)(amplitude * sin(angle*index));
        for(channel_index = 0; channel_index < num_channels; channel_index++) {
            WriteToFrame(&iterated_sample,&dest[index],channel_index+1,format->dwBitsPerSample);
        }
        //WriteToFrame(&iterated_sample,&dest[index],2,format->dwBitsPerSample);
    }
}

void Add_Waves_Mono_16(frame* dest, const frame* signal_a, const frame* signal_b, const unsigned long num_samples) {
    unsigned long frame_index = 0;
    long overflow_detection;
    sample iterated_sample;
    for(frame_index = 0; frame_index < num_samples; frame_index++) {
        overflow_detection = signal_a[frame_index].samples[0].value + signal_b[frame_index].samples[0].value;
        if(overflow_detection > 32600) {
            iterated_sample.value = 32600;
        } else if (overflow_detection < -32600) {
            iterated_sample.value = -32600;
        } else
            iterated_sample.value = (short)overflow_detection;
        WriteToFrame(&iterated_sample, &dest[frame_index],1,16);
    }
}

//TODO: multi-channel frame copy
void Copy_Frames(frame* dest, const frame* src, const unsigned long num_frames) {
    unsigned long frame_index;
    for(frame_index = 0; frame_index < num_frames; frame_index++) {
        dest[frame_index].samples[0].value = src[frame_index].samples[0].value;
    }
}
