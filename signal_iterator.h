#ifndef SIGNAL_ITERATOR_H_INCLUDED
#define SIGNAL_ITERATOR_H_INCLUDED
#include "frame.h"
#include "wavFormat.h"

static const unsigned int BLOCK_SIZE = 10000;

typedef void (*UnaryMethod)(frame*, const unsigned long, const unsigned long);
typedef void (*GeneratorMethod)(frame*, const double, const double, const unsigned short, const unsigned long, const format_chunk*);
typedef void (*GeneratorMethodB)(frame*, void*, const unsigned long);

void GenerateSignalB(frame* dest, const unsigned long num_frames, void* generator_param, GeneratorMethodB generator);
#endif // SIGNAL_ITERATOR_H_INCLUDED
