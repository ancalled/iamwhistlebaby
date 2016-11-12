//
// Created by Andrey Koltochnik on 11/12/16.
//

#ifndef AUDIOPROTOCOL_DEC2_H
#define AUDIOPROTOCOL_DEC2_H

#define TRESHOLD 0.15
#define MIN_FREQ 1500
#define MAX_FREQ 12000



class Decoder2 {

public:
    Decoder2();

    Decoder2(float sr, int bufSize);

    float getPitch(float *samples, int from, int size);

    float getProbability();

    void initialize(float sr, int bufSize);

private:
    int bufferSize;
    int halfBufferSize;
    float sampleRate;
    float *buf;
    float probability;
    int minLag;
    int maxLag;

    void difference(float *samples, int from, int size);

    void cumulativeMeanNormalizedDifference();

    int absoluteThreshold();

    float parabolicInterpolation(int tauEstimate);


};


#endif //AUDIOPROTOCOL_DEC2_H
