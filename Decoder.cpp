#include "Decoder.h"//
// Created by ancalled on 11/11/16.
//


vector<Decoder::Result> Decoder::decode(float *samples, int from, int len) {
    int n = 0;
    vector<Result> vec;
    do {
        Result rs = decode_frame(n++, samples, from, FRAME_SIZE);
        from += FRAME_SIZE;
        vec.push_back(rs);
    } while (from + FRAME_SIZE < len);

    return vec;
}


Decoder::Result Decoder::decode_frame(int frameNum, float *samples, int from, int frameSize) {
//    if (samples.length != frameSize) return null;

    float max = 0;
    int maxIdx = -1;

    for (int l = minLag; l < maxLag; l++) {
        float cor = atf(samples, from, frameSize, l);
        if (cor > max) {
            max = cor;
            maxIdx = l;
        }
    }

    if (maxIdx > 0) {
//            float period = maxIdx;
        float period = maxIdx + quadInterpAdj(
                atf(samples, from, frameSize, maxIdx - 1), max,
                atf(samples, from, frameSize, maxIdx + 1));


        float freq = SAMPLE_RATE / period;

        float err = 0;
        char symb = '-';
        const int &len = sizeof(AP::SYMBS) / sizeof(AP::SYMBS[0]);
        for (int i = 0; i < len - 1; i++) {
            float bottom = AP::SYMBS[i].frequence;
            float top = AP::SYMBS[i + 1].frequence;

            if (freq > bottom && freq < top) {
                float len = top - bottom;
                float diff1 = freq - bottom;
                float diff2 = top - freq;
                if (diff1 < diff2) {
                    symb = AP::SYMBS[i].symbol;
                    err = diff1 / len;
                } else {
                    symb = AP::SYMBS[i + 1].symbol;
                    err = diff2 / len;
                }
                break;
            }

        }
        return {symb, err, frameNum};
    }
}


float Decoder::atf(float *samples, int from, int size, int lag) {
    float sm = 0;
    if (lag >= 0) {
        for (int i = from + lag; i < from + size; i++) {
            sm += samples[i] * samples[i - lag];
        }
    } else {
        for (int i = from; i < from + size + lag; i++) {
            sm += samples[i] * samples[i - lag];
        }
    }

    return sm;

}


float Decoder::quadInterpAdj(float left,   // sample value left of the peak
                             float mid,    // sample value at the peak
                             float right) {// sample value right of the peak
    return (float) 0.5 * (right - left) / (2 * mid - left - right);
}


