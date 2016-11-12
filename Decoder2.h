//
// Created by Andrey Koltochnik on 11/12/16.
//

#ifndef AUDIOPROTOCOL_YIN_H
#define AUDIOPROTOCOL_YIN_H

#define TRESHOLD 0.15



class Decoder2 {

public:
    Decoder2();

    Decoder2(float sr, int bufSize);

    float getPitch(float *buffer);

    float getProbability();

    void initialize(float sr, int bufSize);

private:
    int bufferSize;
    int halfBufferSize;
    float sampleRate;
    float *samples;
    float probability;

    void difference(float *buffer);

    void cumulativeMeanNormalizedDifference();

    int absoluteThreshold();

    float parabolicInterpolation(int tauEstimate);


};


#endif //AUDIOPROTOCOL_YIN_H
