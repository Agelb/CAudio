#ifndef SYNTHESIZER_H_INCLUDED
#define SYNTHESIZER_H_INCLUDED
#include "wavFormat.h"
void Generate16SineWave(frame* dest, const double frequency, const double duration, const format_chunk* format);

#endif // SYNTHESIZER_H_INCLUDED
