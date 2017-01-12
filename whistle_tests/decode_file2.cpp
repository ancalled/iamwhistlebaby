//
// Created by ancalled on 11/28/16.
//

#include <chrono>
#include <cstdio>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <MessageDecoder2.h>
#include <Synthesizer.h>
#include "stringutil.h"

#define PLOT_SAMPLES 1
#define PLOT_PITCHES 2
#define PLOT_TAUS 3


#define PULSE_AUDIO_NAME "decoder"

using namespace std;
using namespace std::chrono;


int main(int argc, char *argv[]) {

    if (argc < 1) {
        printf("Please specify filename as program parameter!\n");
        return -1;
    }
    const char *fname = argv[1];

    uint16_t bufSize = 500;

    FILE *pFile = fopen(fname, "rb");
    size_t elSize = 2;

    string mes = find_mes(fname);

    printf("File size: %ld bytes, reading by %ld\n", fsize(pFile), elSize);

    uint32_t sr = 44100;
    MessageDecoder2 dec(sr, bufSize);
    int16_t buf[bufSize];
    uint64_t read = 0;

    while (fread(buf, elSize, bufSize, pFile)) {
        dec.processFrame(buf, 0);
        read += bufSize;
    }
    fclose(pFile);
    return 0;
}





