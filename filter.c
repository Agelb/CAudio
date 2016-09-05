#include "filter.h"
#include "frame.h"

/*
void lowpass(frame* dest, const frame* src, const unsigned long num_frames) {
    unsigned long frame_index;
    dest[0].samples[0].value = 0;
    short value;
    for(frame_index = 1; frame_index < num_frames; frame_index++) {
        clip_16(&value, (double)(src[frame_index].samples[0].value + src[frame_index-1].samples[0].value));
        dest[frame_index].samples[0].value = value;
    }
}
*/
