//
// Created by Andrey Koltochnik on 11/12/16.
//

#ifndef AUDIOPROTOCOL_DEC2_H
#define AUDIOPROTOCOL_DEC2_H




class YinDecoder {

public:

    YinDecoder(uint32_t sr, uint32_t bufSize, float minFreq, float maxFreq);

    float getPitch(int16_t *samples, uint32_t from, uint32_t size, float threshold);

    float getProbability();

    float* getBuf();

private:
    uint32_t bufferSize;
    uint32_t halfBufferSize;
    uint32_t sampleRate;
    float *buf;
    float probability;
    uint16_t minLag;
    uint16_t maxLag;

#ifdef GNUPLOT_DEBUG
    std::FILE *asdfFile;
    std::FILE *pitchesFile;
#endif

    void difference(int16_t *samples, uint32_t from, uint32_t size);

    void cumulativeMeanNormalizedDifference();

    int absoluteThreshold(float threshold);

    float parabolicInterpolation(int tauEstimate);


};


#endif //AUDIOPROTOCOL_DEC2_H
