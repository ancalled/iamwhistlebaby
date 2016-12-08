//
// Created by Andrey Koltochnik on 11/12/16.
//

#ifndef AUDIOPROTOCOL_DEC2_H
#define AUDIOPROTOCOL_DEC2_H

#define TRESHOLD 0.15
//#define MIN_FREQ 1500
//#define MAX_FREQ 12000



class YinDecoder {

public:

    YinDecoder(u_int32_t sr, u_int32_t bufSize);

    float getPitch(uint8_t *samples, uint32_t from, uint32_t size);

    float getProbability();

private:
    u_int32_t bufferSize;
    u_int32_t halfBufferSize;
    u_int32_t sampleRate;
    u_int32_t *buf;
    float probability;
    int minLag;
    int maxLag;

    void difference(uint8_t *samples, uint32_t from, uint32_t size);

    void cumulativeMeanNormalizedDifference();

    int absoluteThreshold();

    float parabolicInterpolation(int tauEstimate);


};


#endif //AUDIOPROTOCOL_DEC2_H
