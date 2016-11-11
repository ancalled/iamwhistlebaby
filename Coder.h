//
// Created by ancalled on 11/11/16.
//

#ifndef AUDIOPROTOCOL_CODER_H
#define AUDIOPROTOCOL_CODER_H

#define PI  3.141592
#define SAMPLE_RATE  76000
//#define SAMPLE_RATE  44100
#define BEAK_LASTS  87.2
#define PORT_RAMP_LASTS  12
#define PORT_LASTS  63.2



#include <cmath>

namespace AP {
    struct Symb {
        char symbol;
        float frequence;
    };

    static Symb const SYMBS[32] = {{'0', 1760.00}, {'1', 1864.66}, {'2', 1975.53}, {'3', 2093.01}, {'4', 2217.46}, {'5', 2349.32},
                                   {'6', 2489.02}, {'7', 2637.02}, {'8', 2793.83}, {'9', 2959.96}, {'a', 3135.96}, {'b', 3322.44},
                                   {'c', 3520.00}, {'d', 3729.31}, {'e', 3951.07}, {'f', 4186.01}, {'g', 4434.92}, {'h', 4698.64},
                                   {'i', 4978.03}, {'j', 5274.04}, {'k', 5587.65}, {'l', 5919.91}, {'m', 6271.93}, {'n', 6644.88},
                                   {'o', 7040.00}, {'p', 7458.62}, {'q', 7902.13}, {'r', 8372.02}, {'s', 8869.84}, {'t', 9397.27},
                                   {'u', 9956.06}, {'v', 10548.08}};
}


class Coder {

public:

    int const LEN = (int) round(BEAK_LASTS * SAMPLE_RATE / 1000);
    float const PORT_RAMP_LEN = (int) round(PORT_RAMP_LASTS * SAMPLE_RATE / 1000);
    float const PORT_LEN = (int) round(PORT_LASTS * SAMPLE_RATE / 1000);

    AP::Symb get_freq_by_sym(char c) const;

    void portamento_beak(float currFreq, float prevFreq, float nextFreq, float *values, int from, float ampl);

    void encode(const char *message, float *samples, int from, float ampl);

    void encode_symbol(char symb, float *samples, int from, float ampl);

private:
};


#endif //AUDIOPROTOCOL_CODER_H
