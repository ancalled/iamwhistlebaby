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
#include "unistd.h"

#define PLOT_SAMPLES 1
#define PLOT_PITCHES 2
#define PLOT_TAUS 3

#define DEFAULT_DECODE_SAMPLE_RATE 44100
#define DEFAULT_BUFFER_SIZE 512

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

//    if (!access(fname, F_OK) != -1) {
//        printf("File %s doesn't exists!\n", fname);
//        return 1;
//    }

    FILE *pFILE = fopen(fname, "rb");

    //-------------------------------------------

    static const pa_sample_spec ss = {
            .format = PA_SAMPLE_S16LE,
            .rate = sampleRate,
            .channels = 1
    };

    pa_simple *s = NULL;
    int error;
//    PitchDetector dec(sampleRate, bufSize, minFreq, maxFreq);

    if (!(s = pa_simple_new(NULL, PULSE_AUDIO_NAME, PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        return 1;
    }

    uint16_t bufSize = DEFAULT_BUFFER_SIZE;
    int16_t buf[bufSize];

    for (;;) {
        size_t r;

        r = fread(buf, bufSize, 1, pFILE);
        if (r > 0) {
            if (pa_simple_write(s, buf, bufSize, &error) < 0) {
                fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
                return 2;
            }
        } else {
            break;
        }
    }

    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        return 3;
    }


}





