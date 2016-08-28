#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#pragma pack(1)
typedef struct
{
    // 3 4-byte fields. Must be written to file in this order
    char sGroupID[4]; //This will be the string with "RIFF"
    unsigned long dwFileLength; // This is the file length, with an 8 byte offset
    char sRiffType[4]; //This will be the string "WAVE"
} RIFF_HEADER;

typedef struct
{
    char sChunkID[4]; //Will identify the chunk type
    unsigned long dwChunkSize;
} CHUNK_HEADER;

typedef struct
{
    CHUNK_HEADER header;
    unsigned short wFormatTag;
    unsigned short wChannels;
    unsigned long dwSamplesPerSec; //Sample Rate
    unsigned long dwAvgBytesPerSec;
    unsigned short wBlockAlign; //Sample frame size in bites
    unsigned short dwBitsPerSample;
} FORMAT_CHUNK;

typedef struct
{
    CHUNK_HEADER header;
    //uint8_t* byteArray; // for 8 bit sound
    short* shortArray; // for 16 bit sound
} DATA_CHUNK;

void populate_samples(short* dest, const FORMAT_CHUNK* format, unsigned long seconds, short* check_bytes){
    //printf("PI: %f", M_PI);
    short amplitude = 32760 / 2; // half of the max amplitude for an 8-bit depth sample
    double frequency = 440.0;
    double angle = (M_PI * 2 * frequency) / (format->dwSamplesPerSec * format->wChannels);
    long tick;
    double value;
    for(tick = 0; tick < seconds * format->dwSamplesPerSec - 1; tick += 1){
        value = amplitude * sin(angle * tick);
        check_bytes[tick] = value;
        //value = amplitude * sin(((M_PI*2) * tick * frequency)/format->dwSamplesPerSec);
        //value = amplitude * sin((2 * M_PI * tick * frequency) / 44100);
        //value = amplitude * sine(tick))

        dest[tick] = (short)value;
        if(check_bytes[tick] != dest[tick]){
            printf("Integrity error at tick %d", tick);
        }
    }
}

int main(int argc, char *argv[])
{
    char* FILENAME = 0;
    const unsigned short MAX_FILENAME_SIZE;
    unsigned int seconds = 1;
    int i;
    for(i=0; i < argc; ++i){
        printf("Argument %d : %s\n",i, argv[i]);
        if(strcmp("-f", argv[i]) == 0) {
            unsigned short filename_argument_size;
            filename_argument_size = strlen(argv[i+1]);
            if(filename_argument_size >= MAX_FILENAME_SIZE - 1) {
                printf("Filenames must be at most %d characters.", MAX_FILENAME_SIZE - 1);
                return 1;
            }
            FILENAME = (char*)malloc(strlen(argv[i+1]+1));
            strncpy(FILENAME, argv[i+1], strlen(argv[i+1]));
            i++;
        } else if (strcmp("-s",argv[i]) == 0) {
            printf("Seconds are specified: %d\n", argv[i+1]);
            //seconds = 5;//atoi(argv[i+1]);
            i++;
        }
    }

    if(FILENAME == 0)
        FILENAME = "Defaultb.WAV";

    short check_bytes[44100];
    //Initialize the header memory
    RIFF_HEADER main_header;
    FORMAT_CHUNK format;
    DATA_CHUNK data;

    strncpy(main_header.sGroupID, "RIFF", 4);
    strncpy(main_header.sRiffType, "WAVE", 4);
    strncpy(format.header.sChunkID, "fmt ", 4);
    strncpy(data.header.sChunkID, "data", 4);

    format.header.dwChunkSize = 16;
    format.wFormatTag = 1;
    format.wChannels = 1;
    format.dwSamplesPerSec = 44100;
    format.dwBitsPerSample = 16;
    format.wBlockAlign = format.wChannels * (format.dwBitsPerSample / 8);
    format.dwAvgBytesPerSec = format.wBlockAlign * format.dwSamplesPerSec;

    data.shortArray = (short*)malloc((seconds * format.dwSamplesPerSec) * sizeof(short));
    memset(data.shortArray, '0', seconds * format.dwSamplesPerSec * sizeof(short));
    data.header.dwChunkSize = seconds * format.dwAvgBytesPerSec;
    populate_samples(data.shortArray, &format, seconds, check_bytes);
    main_header.dwFileLength = 4 + 16 + (format.dwAvgBytesPerSec * seconds);

    FILE *wav_file = fopen(FILENAME, "wb+");
    FILE *check_file;
    fwrite(&main_header, 1, sizeof(main_header), wav_file);
    fwrite(&format, 1, sizeof(format), wav_file);
    fwrite(&data.header, 1, 8, wav_file);
    fwrite(data.shortArray, 2, (format.dwSamplesPerSec * seconds), wav_file);
    fclose(wav_file);
    free(data.shortArray);
    free(FILENAME);
    return 0;
}
