#include "synthesizer.h"
#include "wavFormat.h"
#include <math.h>

/*
    Writes 16-bit samples to the passed destination.
    params: destination, frequency in Hz, duration in seconds, format_chunk from wave file
*/
// TODO: decouple this from the wave format
void GenerateSineWave(short* dest, const double frequency, const double duration, const format_chunk* format) {
    // TODO: variable amplitudes
    // TODO: amplitude based on bit depth
    short amplitude = 32760; // Max amplitude for 16 bit sound
    float angle = (M_PI * 2 * frequency) / (format->dwSamplesPerSec * format->wChannels);
    unsigned long total_samples = duration * format->dwSamplesPerSec;
    unsigned long index;
    for(index = 0; index < total_samples; index++)
    {
        dest[index] = amplitude * sin(angle*index);
    }
}
