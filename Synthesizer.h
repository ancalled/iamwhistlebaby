//
// Created by ancalled on 12/9/16.
//

#ifndef AUDIOPROTOCOL_SYNTHESIZER_H
#define AUDIOPROTOCOL_SYNTHESIZER_H


#include <cstdint>

#define SYMBS 32

#define RAMP_TIME 12.00
#define TOP_TIME 75.36
#define PI        3.14159265358979323846

namespace wsl {
    struct sound_symbol {
        char symbol;
        float freq;
        uint64_t phaseStep;
    };

    const sound_symbol SYMBOLS[SYMBS] = {
            {'0', 1760.00,  0x146EFDUL},
            {'1', 1864.66,  0x15A60EUL},
            {'2', 1975.53,  0x16EF94UL},
            {'3', 2093.00,  0x184CB7UL},
            {'4', 2217.46,  0x19BEA2UL},
            {'5', 2349.32,  0x1B468AUL},
            {'6', 2489.02,  0x1CE5C0UL},
            {'7', 2637.02,  0x1E9DA1UL},
            {'8', 2793.83,  0x206FB1UL},
            {'9', 2959.96,  0x225D75UL},
            {'a', 3135.96,  0x24688EUL},
            {'b', 3322.44,  0x2692CDUL},
            {'c', 3520.00,  0x28DDFBUL},
            {'d', 3729.31,  0x2B4C15UL},
            {'e', 3951.07,  0x2DDF30UL},
            {'f', 4186.01,  0x309977UL},
            {'g', 4434.92,  0x337D44UL},
            {'h', 4698.64,  0x368D15UL},
            {'i', 4978.03,  0x39CB79UL},
            {'j', 5274.04,  0x3D3B42UL},
            {'k', 5587.65,  0x40DF5BUL},
            {'l', 5919.91,  0x44BAE2UL},
            {'m', 6271.93,  0x48D124UL},
            {'n', 6644.88,  0x4D259BUL},
            {'o', 7040.00,  0x51BBF7UL},
            {'p', 7458.62,  0x56982BUL},
            {'q', 7902.13,  0x5BBE59UL},
            {'r', 8372.02,  0x6132EFUL},
            {'s', 8869.84,  0x66FA88UL},
            {'t', 9397.27,  0x6D1A22UL},
            {'u', 9956.06,  0x7396F2UL},
            {'v', 10548.08, 0x7A7685UL}
    };

}

class Synthesizer {

public:

    Synthesizer(uint32_t sampleRate);

    uint32_t generate(int8_t *samples, uint32_t size, const char *mes);

    wsl::sound_symbol findSymbol(char ch);

    uint32_t expectedSize(size_t symbols);

private:
    const uint32_t sampleRate;
    const uint16_t rampSamples;
    const uint16_t topSamples;
};


#endif //AUDIOPROTOCOL_SYNTHESIZER_H
