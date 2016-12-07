//
// Created by ancalled on 11/11/16.
//

#ifndef AUDIOPROTOCOL_DECODER_H
#define AUDIOPROTOCOL_DECODER_H

#include "Coder.h"
#include <vector>

#define TRESHOLD 0.15
#define MIN_FREQ 1300
#define MAX_FREQ 5000

#define WIN_SIZE 200
#define HB_SIZE 100
//#define WIN_SIZE 500
//#define HB_SIZE 250

using namespace std;

class Decoder {

public:
    struct Pitch {
        int tau;
        float prob;
    };

    Decoder();

    Decoder(int sr);

    float getPitch(uint8_t *samples, const int from);

//    std::vector<Decoder::Pitch> findTopPitches(const int *samples, const int from);
    int findTopPitches(uint8_t *samples, const int from, Decoder::Pitch *res);

    float getProbability();

    void initialize(int sr);


private:
    int sampleRate;
    int buf[HB_SIZE];
//    float normalized[HB_SIZE];
    float probability;
    int minLag;
    int maxLag;

    int difference(uint8_t *samples, const int from);

    int findFirstMin(const int threshold);

//    vector<Pitch> findLocalMins(const int avg);
    int findLocalMins(const int avg, Decoder::Pitch *res);

//    void cumulativeMeanNormalizedDifference();

//    int absoluteThreshold();

//    float parabolicInterpolation(int tauEstimate);


};


#endif //AUDIOPROTOCOL_DECODER_H
