#ifndef SYNTHESIZER_H_INCLUDED
#define SYNTHESIZER_H_INCLUDED
#include "wavFormat.h"
void Generate16SineWave(frame* dest, const double frequency, const double duration, const format_chunk* format);
void GenerateSineWave_Mono_16a(frame* dest, const double frequency, const double duration, const unsigned short amplitude, const format_chunk* format);
void GenerateSawWave_Mono_16a(frame* dest, const double frequency, const double duration, const unsigned short amplitude, const format_chunk* format);
void GenerateSineWave_Mono_16ah(frame* dest, const double frequency, const double duration, const unsigned short amplitude, const unsigned long state, const format_chunk* format);
#endif // SYNTHESIZER_H_INCLUDED
