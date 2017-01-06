//
// Created by ancalled on 11/28/16.
//

#include <chrono>
#include <cstdio>

#include <pulse/simple.h>
#include <pulse/error.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include "command.h"

#define DEFAULT_DECODE_SAMPLE_RATE 44100
#define PULSE_AUDIO_NAME "decoder"

using namespace std::chrono;

int main(int argc, char *argv[]) {

    uint32_t sampleRate;
    if (cmdOptionExists(argv, argv + argc, "-samplerate")) {
        char *srCh = getCmdOption(argv, argv + argc, "-samplerate");
        sampleRate = (uint32_t) atoi(srCh);
    } else {
        sampleRate = DEFAULT_DECODE_SAMPLE_RATE;
    }

    char *fname;
    if (cmdOptionExists(argv, argv + argc, "-file")) {
        fname = getCmdOption(argv, argv + argc, "-file");
    } else {
        printf("Please specify filename with '-file' parameter!\n");
        return 1;
    }

    FILE *pFILE = fopen(fname, "wb");

    //-------------------------------------------

    static const pa_sample_spec ss = {
            .format = PA_SAMPLE_S16LE,
            .rate = sampleRate,
            .channels = 1
    };

    pa_simple *s = NULL;
    int error;
//    PitchDetector dec(sampleRate, bufSize, minFreq, maxFreq);

    if (!(s = pa_simple_new(NULL, PULSE_AUDIO_NAME, PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        return 1;
    }

    uint16_t bufSize = 1024;
    int16_t buf[bufSize];

    for (;;) {
        if (pa_simple_read(s, buf, bufSize, &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            return 2;
        }

        fwrite(buf, bufSize, 1, pFILE);
    }

}





