//
// Created by ancalled on 11/11/16.
//

#ifndef AUDIOPROTOCOL_DECODER_H
#define AUDIOPROTOCOL_DECODER_H

#include "Coder.h"
#include <vector>

#define TRESHOLD 0.15
#define MIN_FREQ 1500
#define MAX_FREQ 12000

#define WIN_SIZE 300
#define HB_SIZE 150
//#define WIN_SIZE 500
//#define HB_SIZE 250

using namespace std;

class Decoder {

public:
    Decoder();

    Decoder(int sr);

    float getPitch(int *samples, int from);

    float getProbability();

    void initialize(int sr);

private:
    int sampleRate;
    int buf[HB_SIZE];
//    float normalized[HB_SIZE];
    float probability;
    int minLag;
    int maxLag;

    int difference(int *samples, int from);

    int findFirstMin(int threshold);

//    void cumulativeMeanNormalizedDifference();

//    int absoluteThreshold();

//    float parabolicInterpolation(int tauEstimate);


};


#endif //AUDIOPROTOCOL_DECODER_H
