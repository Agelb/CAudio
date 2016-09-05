#include "filter.h"
#include "frame.h"


void lowpass(frame* dest, const frame* src, const unsigned long num_frames, const frame* state) {
    unsigned long frame_index;
    double beta = .025;
    dest[0].samples[0].value = state[0].samples[0].value;
    for(frame_index = 1; frame_index < num_frames; frame_index++) {
       dest[frame_index].samples[0].value =  (beta * src[frame_index].samples[0].value)+(1-beta)*dest[frame_index-1].samples[0].value;
    }
}
