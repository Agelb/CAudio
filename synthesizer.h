#ifndef SYNTHESIZER_H_INCLUDED
#define SYNTHESIZER_H_INCLUDED
#include "wavFormat.h"

typedef struct {
    double max_amplitude;
    unsigned int bitrate;
    unsigned int frequency;
    unsigned int range;
    unsigned int channel;
} SawParameters;

typedef struct {
    double max_amplitude;
    unsigned int bitrate;
    unsigned int frequency;
    double angle;
    unsigned int channel;
} SineParameters;

void SetSineParameters(SineParameters* paramters, double amplitude, unsigned int bitrate, unsigned int frequency, unsigned int channel);
void SawWave(frame* dest, const void* saw, const unsigned long *input);
void SineWave(frame* dest, const void* generic_param, const unsigned long *input);
void AddSignalsMono(frame* dest, const frame* signal_a, const frame* signal_b, const unsigned long num_frames);
void Generate16SineWave(frame* dest, const double frequency, const double duration, const format_chunk* format);
void GenerateSineWave_Mono_16a(frame* dest, const double frequency, const double duration, const unsigned short amplitude, const format_chunk* format);
void GenerateSawWave_Mono_16a(frame* dest, const double frequency, const double duration, const unsigned short amplitude, const format_chunk* format);
void GenerateSineWave_Mono_16ah(frame* dest, const double frequency, const double duration, const unsigned short amplitude, const unsigned long state, const format_chunk* format);
#endif // SYNTHESIZER_H_INCLUDED
