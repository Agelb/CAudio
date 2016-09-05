#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED
#include "frame.h"

void lowpass(frame* dest, const frame* src, const unsigned long num_frames, const frame* state);
#endif // FILTER_H_INCLUDED
