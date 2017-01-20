//
// Created by ancalled on 11/28/16.
//

#include <chrono>
#include <cstdio>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <SimpleMessageDecoder.h>
#include <Synthesizer.h>
#include "stringutil.h"

#define PLOT_SAMPLES 1
#define PLOT_PITCHES 2
#define PLOT_TAUS 3

#define DEFAULT_DECODE_SAMPLE_RATE 44100
#define DEFAULT_BUFFER_SIZE 512
#define DEFAULT_MIN_FREQ 1500
#define DEFAULT_MAX_FREQ 20000

#define PULSE_AUDIO_NAME "decoder"

using namespace std;
using namespace std::chrono;


int main(int argc, char *argv[]) {

    if (argc < 1) {
        printf("Please specify filename as program parameter!\n");
        return -1;
    }
    const char *fname = argv[1];

//    uint16_t bufSize = DEFAULT_BUFFER_SIZE;
    uint16_t bufSize = 500;

    FILE *pFile = fopen(fname, "rb");
    size_t elSize = 2;

    string mes = find_mes(fname);

    printf("File size: %ld bytes, reading by %ld\n", fsize(pFile), elSize);

    uint32_t sr = DEFAULT_DECODE_SAMPLE_RATE;
    SimpleMessageDecoder dec(sr, bufSize);
    int16_t buf[bufSize];
    vector<SimpleMessageDecoder::ProcessResult> results;
    uint64_t read = 0;
    bool debug = /*false*/true;

    while (fread(buf, elSize, bufSize, pFile)) {
        if (debug) printf("%ld\t", read);
        const SimpleMessageDecoder::ProcessResult &r = dec.processFrame(buf, 0, debug);
        results.push_back(r);
        read += bufSize;
//        if (debug && r.detector.pitch < 0) printf("\n");
    }
    fclose(pFile);
    //    dec.stopDetection();
    printf("Read %ld elements (exp %ld sec)\n", read, (read / sr));
    if (dec.hasResult()) {
        std::string decMes = dec.popMessage();
        if (decMes.length() > 0) {
            printf("Detected: %s\n", decMes.c_str());
        }
    }

    printf("\n\n----------------------------------------------------\n\n");

    int frameCnt = 0;
    int symIdx = 0;
    bool processing = false;
    int samplesPerSymbol = (int) round((RAMP_TIME + TOP_TIME) * sr / 1000);
    int framesPerSymbol = samplesPerSymbol / bufSize;
    int procSamples = 0;
    int eqCnt = 0;
    int corDetected = 0;
    bool nextSymb = true;


    for (SimpleMessageDecoder::ProcessResult &res: results) {
        if (res.messageState == SimpleMessageDecoder::MessageState::MS_GOT_RESULT) {
            processing = false;
        }

//        if (frameCnt >= 9 && frameCnt < 20) {
//            dec.printTaus();
//        }


        if (res.detector.pitch > 0 || processing) {
            if (nextSymb) {
                printf("--------------------\n");
                printf("#%d\n", frameCnt);
                nextSymb = false;
            }


            if (!processing) processing = true;
            char &expected = mes[symIdx];
            char actual = res.detector.pitch > 0 ? res.candidate.symbol : '-';
            if (actual == expected) eqCnt++;

            printf("%c\t%c\n", expected, actual);

            procSamples += bufSize;
            if (procSamples >= samplesPerSymbol) {
//                printf("\t\t");
                if (eqCnt >= framesPerSymbol - 2) {
                    corDetected++;
                    printf("[ok]");
                } else {
                    printf("[wrong]");
                }

                symIdx++;
                procSamples %= samplesPerSymbol;
                eqCnt = 0;
                nextSymb = true;

                printf("\n");
            }


        }


        frameCnt++;
    }



    if (corDetected != mes.length()) {
        printf("%d of %ld\n", corDetected, mes.length());

    } else {
        printf("Successfully detected!\n");
    }

    return 0;
}





