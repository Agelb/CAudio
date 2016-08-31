#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

typedef struct {
    short value; //16-bit sample
} sample;


typedef struct {
    sample* samples;
} frame;


//frame* AllocateFrames(unsigned long num_frames, unsigned int bit_depth, unsigned int num_channels);
void WriteToFrame(const sample* samp, frame* frm, unsigned int channel, unsigned int depth);
sample* GetSample(const frame* frm, unsigned int channel, unsigned int depth);
void DisposeFrame(frame* frm);
#endif // FRAME_H_INCLUDED
