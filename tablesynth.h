//
// Created by ancalled on 12/2/16.
//

#ifndef ADSRTEST_TABLESYNTH_H
#define ADSRTEST_TABLESYNTH_H


#include <cstdint>

//#define SYMBS 32
#define SYMBS 10
#define SIN_QUADRANT 128
#define RAMP_TIME 12.00
#define TOP_TIME 75.36

class tablesynth {

    struct sound_symbol {
        char symbol;
        float freq;
        uint64_t phaseStep;
    };

    union U32 {        // 32 bits of data
        uint32_t u;    // view as one 4-byte value
        uint16_t w[2]; // or two 2-byte values
        uint8_t b[4];     // or four single bytes
    };

//    const float FREQS[SYMBS] = {1760.00, 1864.66, 1975.53, 2093.01, 2217.46, 2349.32,
//                                2489.02, 2637.02, 2793.83, 2959.96,
//                                3135.96, 3322.44, 3520.00, 3729.31, 3951.07, 4186.01,
//                                4434.92, 4698.64, 4978.03, 5274.04, 5587.65, 5919.91,
//                                6271.93, 6644.88, 7040.00, 7458.62,
//                                7902.13, 8372.02, 8869.84, 9397.27, 9956.06, 10548.08};

//    const sound_symbol SYMBOLS[SYMBS] = {
//            {'0', 1760.00,  0xE6AFCUL},
//            {'1', 1864.66,  0xF4679UL},
//            {'2', 1975.53,  0x102EFCUL},
//            {'3', 2093.00,  0x112556UL},
//            {'4', 2217.46,  0x122A59UL},
//            {'5', 2349.32,  0x133EE1UL},
//            {'6', 2489.02,  0x1463DAUL},
//            {'7', 2637.02,  0x159A3BUL},
//            {'8', 2793.83,  0x16E316UL},
//            {'9', 2959.96,  0x183F7CUL},
//            {'a', 3135.96,  0x19B095UL},
//            {'b', 3322.44,  0x1B37A9UL},
//            {'c', 3520.00,  0x1CD5F9UL},
//            {'d', 3729.31,  0x1E8CEEUL},
//            {'e', 3951.07,  0x205DFEUL},
//            {'f', 4186.01,  0x224AB3UL},
//            {'g', 4434.92,  0x2454B3UL},
//            {'h', 4698.64,  0x267DC3UL},
//            {'i', 4978.03,  0x28C7AFUL},
//            {'j', 5274.04,  0x2B3476UL},
//            {'k', 5587.65,  0x2DC626UL},
//            {'l', 5919.91,  0x307EF3UL},
//            {'m', 6271.93,  0x336130UL},
//            {'n', 6644.88,  0x366F52UL},
//            {'o', 7040.00,  0x39ABF3UL},
//            {'p', 7458.62,  0x3D19DCUL},
//            {'q', 7902.13,  0x40BBF7UL},
//            {'r', 8372.02,  0x449566UL},
//            {'s', 8869.84,  0x48A967UL},
//            {'t', 9397.27,  0x4CFB80UL},
//            {'u', 9956.06,  0x518F5FUL},
//            {'v', 10548.08, 0x5668EDUL}
//    };

    const sound_symbol SYMBOLS[SYMBS] = {
            {'0', 1462, 0x2765A5UL},
            {'1', 1583, 0x2AA85DUL},
            {'2', 1727, 0x2E89C1UL},
            {'3', 1900, 0x333333UL},
            {'4', 2111, 0x38E2C9UL},
            {'5', 2375, 0x400000UL},
            {'6', 2714, 0x492299UL},
            {'7', 3167, 0x5557A2UL},
            {'8', 3800, 0x666666UL},
            {'9', 4750, 0x800000UL}
    };


public:

    tablesynth(uint32_t sampleRate);

    uint32_t generate(uint8_t *samples, uint32_t size, const char *mes);

private:

    uint32_t sampleRate;
    U32 phase;
    int frame;

    uint8_t sineQuadrant[SIN_QUADRANT];
    uint16_t rampSamples;
    uint16_t topSamples;

    uint32_t generateFrame(uint8_t *samples, uint32_t samples_size, uint32_t samples_from,
                           uint16_t topLen, uint16_t rampLen,
                           uint64_t phaseStep, int32_t phaseGradient,
                           uint8_t amplitude, uint8_t amplStep);

    tablesynth::sound_symbol findSymbol(char ch);

    uint64_t toFixedPoint(double f, int8_t p);


};


#endif //ADSRTEST_TABLESYNTH_H
