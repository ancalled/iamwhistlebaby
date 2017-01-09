//
// Created by ancalled on 11/28/16.
//

#include <chrono>
#include <cstdio>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <MessageDecoder.h>

#define PLOT_SAMPLES 1
#define PLOT_PITCHES 2
#define PLOT_TAUS 3

#define DEFAULT_DECODE_SAMPLE_RATE 44100
#define DEFAULT_BUFFER_SIZE 512
#define DEFAULT_MIN_FREQ 1500
#define DEFAULT_MAX_FREQ 20000

#define PULSE_AUDIO_NAME "decoder"

using namespace std::chrono;


int main(int argc, char *argv[]) {

    if (argc < 1) {
        printf("Please specify filename as program parameter!\n");
        return -1;
    }
    char *fname = argv[1];

    uint16_t bufSize = DEFAULT_BUFFER_SIZE;

    FILE *pFILE = fopen(fname, "rb");

    MessageDecoder dec(DEFAULT_DECODE_SAMPLE_RATE, bufSize);
    int16_t buf[bufSize];

    bool print_pitches = true;

    while (fread(buf, bufSize, 1, pFILE)) {
        dec.processFrame(buf, 0, print_pitches);
    }

    if (dec.hasResult()) {
        std::string mes = dec.popMessage();
        if (mes.length() > 0) {
            printf("%s\n", mes.c_str());
        }
    }

}





