#ifndef SIGNAL_ITERATOR_H_INCLUDED
#define SIGNAL_ITERATOR_H_INCLUDED
#include "frame.h"
#include "wavFormat.h"

static const unsigned int BLOCK_SIZE = 10000;

typedef void (*UnaryFunction)(frame*, const frame*, const unsigned long, const frame*);
typedef void (*GeneratorMethod)(frame*, const double, const double, const unsigned short, const unsigned long, const format_chunk*);
typedef void (*GeneratorMethodB)(frame*, void*, const unsigned long);
typedef void (*BinaryFunction)(frame*, frame*, frame*, const unsigned long);
void GenerateSignalB(frame* dest, const unsigned long num_frames, void* generator_param, GeneratorMethodB generator);
void PerformBinaryFunction(frame* dest, const frame* signal_a, const frame* signal_b, const unsigned long num_frames, BinaryFunction func);
#endif // SIGNAL_ITERATOR_H_INCLUDED
