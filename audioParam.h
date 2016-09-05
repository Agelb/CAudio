#ifndef AUDIOPARAM_H_INCLUDED
#define AUDIOPARAM_H_INCLUDED

static const short MAX_AMPLITUDE_16 = 32670;
static const short MIN_AMPLITUDE_16 = -32670;

typedef struct {
    unsigned long sample_rate;
    unsigned int bit_depth;
    unsigned int channels;
} audioParam;

void clip_16(short* result, const double src);
#endif // AUDIOPARAM_H_INCLUDED
