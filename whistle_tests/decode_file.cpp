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
#include "media_tests/command.h"

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

    char *fname;
    if (cmdOptionExists(argv, argv + argc, "-file")) {
        fname = getCmdOption(argv, argv + argc, "-file");
    } else {
        printf("Please specify filename with '-file' parameter!\n");
        return 1;
    }

    uint32_t sampleRate;
    if (cmdOptionExists(argv, argv + argc, "-samplerate")) {
        char *srCh = getCmdOption(argv, argv + argc, "-samplerate");
        sampleRate = (uint32_t) atoi(srCh);
    } else {
        sampleRate = DEFAULT_DECODE_SAMPLE_RATE;
    }

    uint16_t bufSize = DEFAULT_BUFFER_SIZE;

    FILE *pFILE = fopen(fname, "rb");

    MessageDecoder dec(sampleRate, bufSize);
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




