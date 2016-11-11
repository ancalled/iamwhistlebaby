//
// Created by ancalled on 11/11/16.
//

#ifndef AUDIOPROTOCOL_DECODER_H
#define AUDIOPROTOCOL_DECODER_H

#include "Coder.h"
#include <vector>

#define MIN_FREQ 1500
#define MAX_FREQ 12000
#define FRAME_SIZE 512
using namespace std;

class Decoder {
public:
    struct Result {
        char symb;
        double error;
        int frame;
    };

    const int minLag = SAMPLE_RATE / MAX_FREQ;
    const int maxLag = SAMPLE_RATE / MIN_FREQ;

    vector<Decoder::Result> decode(float *samples, int from, int len);

private:
    Result decode_frame(int frameNum, float *samples, int from, int frameSize);

    float atf(float *samples, int from, int size, int lag);

    float quadInterpAdj(float left, float mid, float right);

};


#endif //AUDIOPROTOCOL_DECODER_H
