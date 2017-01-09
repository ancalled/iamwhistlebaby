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

#include <algorithm>

template<typename T, size_t size>
int getposition(T const (&array)[size], T const &c) {
    T const *found = std::find(&array[0], &array[size], c);
    return found == &array[size] ? -1 : found - array;
}


long fsize(FILE *pFile) {
    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);
    return lSize;
}

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

//    int pos = getposition(fname, '.');

    printf("File size: %ld bytes, reading by %d\n", fsize(pFile), elSize);

    uint32_t sr = DEFAULT_DECODE_SAMPLE_RATE;
    MessageDecoder dec(sr, bufSize);
    int16_t buf[bufSize];

    bool print_pitches = true;

    uint64_t read = 0;
    while (fread(buf, elSize, bufSize, pFile)) {
        dec.processFrame(buf, 0, print_pitches);
        read += bufSize;
    }
    printf("Read %ld elements (exp %ld sec)\n", read, (read / sr));
//    dec.stopDetection();

    if (dec.hasResult()) {
        std::string mes = dec.popMessage();
        if (mes.length() > 0) {
            printf("%s\n", mes.c_str());
        }
    }

    fclose(pFile);
    return 0;
}





