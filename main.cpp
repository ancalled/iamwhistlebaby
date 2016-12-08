#include <iostream>
#include <cstring>
#include "Coder.h"
#include "Decoder.h"
#include "Veslo.h"
#include "tablesynth.h"

using namespace std;

int main() {

    uint32_t sampleRate = 19200;  //atmega adc prescale = 64
//    uint32_t sampleRate = 38400;  //atmega adc prescale = 32
//    uint32_t sampleRate = 44100;
//    uint32_t sampleRate = 62500;

    const char *mes = "hjntdb982ilj6etj6e3l\0";
//    const char *mes = "hjntdb9\0";
    tablesynth synth(sampleRate);

    uint32_t size = (uint32_t) (strlen(mes) * (sampleRate * (TOP_TIME + RAMP_TIME) / 1000)) + 1;
    uint8_t samples[size];
//    uint8_t* samples;

    uint32_t gen = synth.generate(samples, size, mes);
    printf("Generated %d samples\n", gen);


    // Decoding

    int8_t framesPerSound = 10;
//    uint32_t frameSize = 512;
    uint32_t frameSize = (uint32_t) (sampleRate * (RAMP_TIME + TOP_TIME) / 1000 / framesPerSound);
    float threshold = 0.35;
    YinDecoder dec(sampleRate, frameSize, 1500.0, 15000.0);
    int frame = 0;
    uint32_t n = 0;
    int16_t buf[frameSize];

    while (n < gen) {
        uint32_t sz = n + frameSize < gen ? frameSize : gen - n;
        for (int i = 0; i < sz; i++) {
            buf[i] = samples[n + i] - 127;
        }

        float pitch = dec.getPitch(buf, 0, sz, threshold);
        float prob = dec.getProbability();
        printf("%.2f\t%.2f\n", frame, pitch, prob);

        n += frameSize;
        frame++;
    }

}




