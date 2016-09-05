#include "audioParam.h"

void clip_16(short* result, const double src) {
    if(src > MAX_AMPLITUDE_16) {
        *result = MAX_AMPLITUDE_16;
    } else if (src < (MAX_AMPLITUDE_16 * - 1)) {
        *result = MAX_AMPLITUDE_16 * - 1;
    } else {
        *result = (short)src;
    }
}
