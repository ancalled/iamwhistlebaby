#include <iostream>
#include <cstring>
#include "Coder.h"
#include "Decoder.h"
#include "Veslo.h"
#include "tablesynth.h"

using namespace std;


int main() {

//    uint32_t sampleRate = 19200;  //atmega adc prescale = 64
//    uint32_t sampleRate = 38400;  //atmega adc prescale = 32
    uint32_t sampleRate = 44100;
//    uint32_t sampleRate = 62500;

//    const char *mes = "ntdb982ilj6etj6e3l\0";
//    const char *mes = "ntdb982il\0";
    const char *mes = "hjntdb982ilj6etj6e3l\0";
    tablesynth synth(sampleRate);

    uint32_t size = (uint32_t) (strlen(mes) * (sampleRate * (TOP_TIME + RAMP_TIME) / 1000)) + 1;
    uint8_t samples[size];

    uint32_t gen = synth.generate(samples, size, mes);
    printf("Generated %d samples\n", gen);

    Veslo veslo(sampleRate);
    veslo.listen(samples, 0, 0);
}



//int main1() {
//    const int sampleRate = 19000;
//    Veslo veslo(sampleRate);
//
//    int frame = 0;
//    int from = 0;
//    while (from + WIN_SIZE < NUM_SAMPLES) {
//        veslo.listen(audio, from, frame);
//
//        from += WIN_SIZE;
//        frame++;
//    }
//
//    veslo.printAll();
//}

