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

void GenerateSignalB(frame* dest, const unsigned long num_frames, void* generator_param, GeneratorMethodB generator) {
    unsigned int num_blocks = ceil(num_frames / BLOCK_SIZE);
    unsigned long block_index;
    unsigned long frame_index;
    unsigned long remaining_frames;
    unsigned long input;
    for(block_index = 0; block_index < num_blocks+1; block_index++) {
        remaining_frames = num_frames - (block_index * BLOCK_SIZE);
        for(frame_index = 0; frame_index < BLOCK_SIZE && frame_index < remaining_frames; frame_index++) {
            input = frame_index + (block_index * BLOCK_SIZE);
            generator(&dest[frame_index + (block_index * BLOCK_SIZE)], generator_param, &input);
        }
    }
}

void PerformUnaryFunction(frame* dest, const frame* signal, const unsigned long num_frames, UnaryFunction func) {
    unsigned int num_blocks = ceil(num_frames / BLOCK_SIZE);
    unsigned long block_index;
    unsigned long next_block_size = BLOCK_SIZE;
    unsigned long remaining_frames;
    unsigned long input;
    func(&dest[1], &signal[1], BLOCK_SIZE - 1, &signal[0]);
    for(block_index = 1; block_index < num_blocks+1; block_index++) {
        remaining_frames = num_frames - (block_index * BLOCK_SIZE);
        if(remaining_frames < BLOCK_SIZE) {
            next_block_size = remaining_frames;
        }
        func(&dest[block_index * BLOCK_SIZE], &signal[block_index * BLOCK_SIZE], next_block_size, &signal[(block_index * BLOCK_SIZE) - 1]);
    }
}

void PerformBinaryFunction(frame* dest, const frame* signal_a, const frame* signal_b, const unsigned long num_frames, BinaryFunction func) {
    unsigned int num_blocks = ceil(num_frames / BLOCK_SIZE);
    unsigned long block_index;
    unsigned long next_block_size = BLOCK_SIZE;
    unsigned long remaining_frames;
    unsigned long input;
    for(block_index = 0; block_index < num_blocks+1; block_index++) {
        remaining_frames = num_frames - (block_index * BLOCK_SIZE);
        if(remaining_frames < BLOCK_SIZE) {
            next_block_size = remaining_frames;
        }
        func(&dest[block_index * BLOCK_SIZE], &signal_a[block_index * BLOCK_SIZE], &signal_b[block_index * BLOCK_SIZE], next_block_size);
    }
}
