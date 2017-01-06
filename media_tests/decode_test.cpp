//
// Created by ancalled on 11/28/16.
//

#include <chrono>
#include <cstdio>

#include <pulse/simple.h>
#include <pulse/error.h>
#include <iostream>
#include <cmath>
#include <PitchDetector.h>
#include <algorithm>
#include <cstring>
#include <Decoder.h>
#include "command.h"

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

    uint32_t sampleRate;
    if (cmdOptionExists(argv, argv + argc, "-samplerate")) {
        char *srCh = getCmdOption(argv, argv + argc, "-samplerate");
        sampleRate = (uint32_t) atoi(srCh);
    } else {
        sampleRate = DEFAULT_DECODE_SAMPLE_RATE;
    }

    uint16_t bufSize;
    if (cmdOptionExists(argv, argv + argc, "-bufsize")) {
        char *bsCh = getCmdOption(argv, argv + argc, "-bufsize");
        bufSize = (uint16_t) atoi(bsCh);
    } else {
        bufSize = DEFAULT_BUFFER_SIZE;
    }

    uint16_t minFreq;
    if (cmdOptionExists(argv, argv + argc, "-minFreq")) {
        char *mfCh = getCmdOption(argv, argv + argc, "-minFreq");
        minFreq = (uint16_t) atoi(mfCh);
    } else {
        minFreq = DEFAULT_MIN_FREQ;
    }

    uint16_t maxFreq;
    if (cmdOptionExists(argv, argv + argc, "-maxFreq")) {
        char *mfCh = getCmdOption(argv, argv + argc, "-maxFreq");
        maxFreq = (uint16_t) atoi(mfCh);
    } else {
        maxFreq = DEFAULT_MAX_FREQ;
    }

    const uint16_t minTau = (uint16_t) sampleRate / maxFreq;
    const uint16_t maxTau = (uint16_t) sampleRate / minFreq;

    int replotMilis = 0;
    bool gnuplot = false;
    int plotType = 0;
    if (cmdOptionExists(argv, argv + argc, "-gnuplot")) {
        gnuplot = true;
        if (cmdOptionExists(argv, argv + argc, "-replotmilis")) {
            char *refCh = getCmdOption(argv, argv + argc, "-replotmilis");
            replotMilis = atoi(refCh);
        } else {
            replotMilis = 1000;
        }

        if (cmdOptionExists(argv, argv + argc, "-type")) {
            char *tpCh = getCmdOption(argv, argv + argc, "-type");
            if (strcmp(tpCh, "samples") == 0) {
                plotType = PLOT_SAMPLES;
            } else if (strcmp(tpCh, "pitches") == 0) {
                plotType = PLOT_PITCHES;
            } else if (strcmp(tpCh, "taus") == 0) {
                plotType = PLOT_TAUS;
            }
        }
    }

    //-------------------------------------------

    static const pa_sample_spec ss = {
            .format = PA_SAMPLE_S16LE,
            .rate = sampleRate,
            .channels = 1
    };

    pa_simple *s = NULL;
    int error;
    Decoder dec(sampleRate, bufSize);
//    PitchDetector dec(sampleRate, bufSize, minFreq, maxFreq);

    if (!(s = pa_simple_new(NULL, PULSE_AUDIO_NAME, PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        return 1;
    }

    int16_t buf[bufSize];

    milliseconds lastRepaint;
    if (gnuplot) {
        lastRepaint = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    }

    for (;;) {
        if (pa_simple_read(s, buf, bufSize, &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            return 2;
        }

        PitchDetector::DetectResult r = dec.processFrame(buf);

        std::string mes = dec.getMessage();
        if (mes.length() > 0) {
            printf("Detected: %s\n", mes.c_str());
        }
        if (gnuplot) {
            if (plotType == PLOT_SAMPLES) {
                for (int i = 0; i < bufSize; i++) {
                    printf("%d\n", buf[i]);
                }

            } else if (plotType == PLOT_PITCHES) {
                printf("%.2f\t%.2f\n", r.pitch, r.probability);

            }
//            else if (plotType == PLOT_TAUS) {
//                for (int tau = 0; tau < maxTau; tau++) {
//                    printf("%.2f\t", tau < minTau ? 1.0 : dec.getBuf()[tau]);
//                }
//                printf("\n");
//            }

            milliseconds now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
            if ((now.count() - lastRepaint.count()) > replotMilis) {
                printf("replot");
                lastRepaint = now;
            }
        }

    }

}





