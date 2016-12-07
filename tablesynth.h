//
// Created by ancalled on 12/2/16.
//

#ifndef ADSRTEST_TABLESYNTH_H
#define ADSRTEST_TABLESYNTH_H


#include <cstdint>
//#define SAMPLE_RATE = 62500
#define SYMBS 32
#define SIN_QUADRANT 128
#define RAMP_TIME 12.00
#define TOP_TIME 75.36

class tablesynth {


    union U32 {        // 32 bits of data
        uint32_t u;    // view as one 4-byte value
        uint16_t w[2]; // or two 2-byte values
        uint8_t b[4];     // or four single bytes
    };

    const float FREQS[SYMBS] = {1760.00, 1864.66, 1975.53, 2093.01, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96,
                             3135.96, 3322.44, 3520.00, 3729.31, 3951.07, 4186.01,
                             4434.92, 4698.64, 4978.03, 5274.04, 5587.65, 5919.91, 6271.93, 6644.88, 7040.00, 7458.62,
                             7902.13, 8372.02, 8869.84, 9397.27, 9956.06, 10548.08};


public:

    tablesynth(uint32_t sampleRate);

    uint32_t generate(uint8_t* samples, uint32_t size, const char* mes);

private:

    uint32_t sampleRate;
    U32 phase;
    int frame;

    uint8_t sineQuadrant[SIN_QUADRANT];
    uint64_t phaseSteps[SYMBS];
    uint16_t rampSamples;
    uint16_t topSamples;

    uint32_t generateFrame(uint8_t* samples, uint32_t samples_size, uint32_t samples_from,
                      uint16_t topLen, uint16_t rampLen,
                           uint64_t phaseStep, int32_t phaseGradient,
                           uint8_t amplitude, uint8_t amplStep);

    uint64_t findPhaseStep(char ch);

    uint64_t toFixedPoint(double f, int8_t point);



};


#endif //ADSRTEST_TABLESYNTH_H
