#include "frame.h"

/*frame* AllocateFrames(unsigned long num_frames, unsigned int bit_depth, unsigned int num_channels) {
    unsigned int frame_size = (bit_depth/8) * num_channels;
    unsigned int frame_index;
    frame* frames = (frame*)malloc(frame_size * num_frames);
    for(frame_index = 0; frame_index < num_frames; frame_index++) {
            frames[frame_index].eSamples = 0;
            frames[frame_index].sSamples = 0;
            frames[frame_index].tSamples = 0;
            if(bit_depth == 16){
                frames[frame_index].sSamples = (short*)malloc(num_channels * sizeof(short));
                memset(frames[frame_index].sSamples,0,num_channels * sizeof(short));
            }
    }
    return frames;
}
*/

void AllocateFrames(frame** frames, unsigned long num_frames, unsigned int bit_depth, unsigned int num_channels) {
    unsigned int frame_size = (bit_depth/8) * num_channels;
    unsigned int frame_index;
    unsigned int sample_index;
    *frames = (frame*)malloc(sizeof(frame)*num_frames);
    if(frames == 0) {
        printf("Malloc failed?");
    }
    /*for(frame_index = 0; frame_index < num_frames; frame_index++){
        frames[frame_index] = (frame*)malloc(sizeof(frame));
        /*frames[frame_index]->samples = (sample*)malloc((sizeof(sample) * 10));
        if(frames[frame_index]->samples == 0) {
            printf("malloc failed sample");
        }
        //Try to write to the frame, make sure it's OK
        for(sample_index = 0; sample_index < num_channels; sample_index++) {
            frames[frame_index]->samples[sample_index].value = 0;
        }
    }*/
    //return frames;
}

void AllocateFrame(frame* frm) {
    frm->samples = (sample*)malloc(2);
    frm->samples[0].value = 10;
}

void WriteToFrame(const sample* samp, frame* frm, unsigned int channel, unsigned int depth) {
    //frm->samples = (sample*)malloc(depth/8);
    //sample temp = frm->samples[0];
    frm->samples[channel - 1].value = samp->value;
}

sample* GetSample(const frame* frm, unsigned int channel, unsigned int depth) {
    return (frm->samples + (depth * (channel - 1)));

}

void DisposeFrame(frame* frm) {
    free(frm->samples);
    free(frm);
}
