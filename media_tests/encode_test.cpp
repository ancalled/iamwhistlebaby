//
// Created by ancalled on 11/28/16.
//

#include <chrono>
#include <cstdio>

#include <pulse/simple.h>
#include <pulse/error.h>
#include <iostream>
#include <cmath>
#include <Synthesizer.h>
#include <stdio.h>
#include <algorithm>
#include <cstring>
#include "command.h"

#define PLOT_SAMPLES 1
#define PLOT_PITCHES 2
#define PLOT_TAUS 3

#define DEFAULT_ENCODE_SAMPLE_RATE 44100


using namespace std::chrono;

int play(int16_t* samples, size_t size, uint32_t sampleRate) {
    static const pa_sample_spec ss = {
            .format = PA_SAMPLE_S16LE,
            .rate = sampleRate,
            .channels = 1
    };

    pa_simple *s = NULL;
    int error;
    const char *name = "synth";
    if (!(s = pa_simple_new(NULL, name, PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        return 1;
    }

    if (pa_simple_write(s, samples, size, &error)) {
        fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
        return 3;
    }

//    int from = 0;
//    uint32_t bufSize = 1024;
//    int16_t buf[bufSize];
//    while (from < size) {
//
//        ssize_t r = bufSize;
//        for (int i = 0; i < bufSize; i += 2) {
//            buf[i] = 0;
//            buf[i + 1] = samples[from++];
//        }
//
//        printf("Play next %d, from %d\n", (int) r, from);
//        /* ... and play it */
//        if (pa_simple_write(s, buf, (size_t) r, &error) < 0) {
//            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
//            return 3;
//        }
//    }

    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        return 4;
    }

    return 0;
}


int main(int argc, char *argv[]) {

    uint32_t sampleRate;
    if (cmdOptionExists(argv, argv + argc, "-samplerate")) {
        char *srCh = getCmdOption(argv, argv + argc, "-samplerate");
        sampleRate = (uint32_t) atoi(srCh);
    } else {
        sampleRate = DEFAULT_ENCODE_SAMPLE_RATE;
    }

    char *mes;
    if (cmdOptionExists(argv, argv + argc, "-message")) {
        mes = getCmdOption(argv, argv + argc, "-message");
    } else {
        printf("Please specify message to encode with '-message' parameter!");
        return 1;
    }

    size_t len = strlen(mes);
    Synthesizer synth(sampleRate);
    uint32_t size = synth.expectedSize(len);

    int8_t samples[size];
    uint32_t gen = synth.generate(samples, size, mes);

    printf("Generated %d samples\n", gen);
    for (int i = 0; i < gen; i++) {
        printf("%d\n", samples[i]);
    }

    return play(samples, gen, sampleRate);
}



