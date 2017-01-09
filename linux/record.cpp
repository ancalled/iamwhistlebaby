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

//static const uint8_t WAV_HEADER[44] = {0x52, 0x49, 0x46, 0x46, 0x64, 0xf3, 0x07, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6d, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x44, 0xac, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00, 0x02, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 0x40, 0xf3, 0x07, 0x00};

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
//    fwrite(WAV_HEADER, 44, 1, pFILE);
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

        fwrite(buf, 2, bufSize, pFILE);
    }

}





