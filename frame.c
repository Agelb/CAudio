#include "frame.h"

void AllocateFrames(frame** frames, unsigned long num_frames, unsigned int bit_depth, unsigned int num_channels) {
    //unsigned int frame_size = (bit_depth/8) * num_channels;
    unsigned int frame_index;
    unsigned int sample_index;
    *frames = (frame*)malloc(sizeof(frame)*num_frames);
    /*for(frame_index = 0; frame_index < num_frames; frame_index++) {
        AllocateFrame(&frames[frame_index]);
    }*/
    if(frames == 0) {
        printf("Malloc failed?");
    }
}

void AllocateFrame(frame* frm) {
    frm->samples = (sample*)malloc(sizeof(sample));
    frm->samples[0].value = 10;
}

void WriteToFrame(const sample* samp, frame* frm, unsigned int channel, unsigned int depth) {
    frm->samples[channel - 1].value = samp->value;
}

sample* GetSample(const frame* frm, unsigned int channel, unsigned int depth) {
    return (frm->samples + (depth * (channel - 1)));

}

void DisposeFrame(frame* frm) {
    free(frm->samples);
    free(frm);
}
