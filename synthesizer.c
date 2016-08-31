#include "synthesizer.h"
#include "wavFormat.h"
#include <math.h>

/*
    Writes 16-bit samples to the passed destination.
    params: destination, frequency in Hz, duration in seconds, format_chunk from wave file
*/
// TODO: decouple this from the wave format
void Generate16SineWave(frame* dest, const double frequency, const double duration, const format_chunk* format) {
    // TODO: variable amplitudes
    // TODO: amplitude based on bit depth
    short amplitude = 32760; // Max amplitude for 16 bit sound
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

void mGenerate16SineWave(frame* dest, const double frequency, const double duration, const int num_channels, const format_chunk* format) {
    // TODO: variable amplitudes
    // TODO: amplitude based on bit depth
    short amplitude = 32760; // Max amplitude for 16 bit sound
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

}
