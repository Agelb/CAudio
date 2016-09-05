#include <math.h>
#include "signal_iterator.h"
#include "frame.h"
#include "audioParam.h"

void GenerateSignal(frame* dest, const double frequency, const double duration, const unsigned short amplitude, const format_chunk* format,
                    GeneratorMethod generator) {
    unsigned int size = duration * format->dwSamplesPerSec;
    unsigned int num_blocks = ceil(size / BLOCK_SIZE);
    unsigned long block_index;
    unsigned long remaining_frames;
    for(block_index = 0; block_index < num_blocks+1; block_index++) {
        remaining_frames = size - (block_index * BLOCK_SIZE);
        if(remaining_frames < BLOCK_SIZE) {
            generator(dest, frequency, (remaining_frames/format->dwSamplesPerSec), MAX_AMPLITUDE_16, block_index, format);
        } else {
            generator(dest, frequency, (double)BLOCK_SIZE/format->dwSamplesPerSec, MAX_AMPLITUDE_16, block_index, format);
        }
    }

}

void unary_operate(frame* dest, const unsigned long size, UnaryMethod oper) {
    unsigned int num_blocks = ceil(size / BLOCK_SIZE);
    unsigned long block_index;
    unsigned long remaining_frames;
    for(block_index = 0; block_index < num_blocks; block_index++) {
        remaining_frames = size - (block_index * BLOCK_SIZE);
        if(remaining_frames < BLOCK_SIZE) {
            oper(dest, remaining_frames, block_index);
        }
        oper(dest, BLOCK_SIZE, block_index);
    }
}

void operate(frame* dest, const frame* src_a, const frame* src_b, const unsigned long size) {



}
