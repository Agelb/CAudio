#ifndef SYNTHESIZER_H_INCLUDED
#define SYNTHESIZER_H_INCLUDED
#include "wavFormat.h"
void GenerateSineWave(short* dest, const double frequency, const double duration, const format_chunk* format);

#endif // SYNTHESIZER_H_INCLUDED
