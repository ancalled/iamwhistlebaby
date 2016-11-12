//
// Created by ancalled on 11/11/16.
//

#include <cstddef>
#include <cstring>
#include "Coder.h"

AP::Symb Coder::get_freq_by_sym(char c) const {
    for (unsigned int i = 0; i < sizeof(AP::SYMBS) / sizeof(AP::SYMBS[0]); i++) {
        if (c == AP::SYMBS[i].symbol)
            return AP::SYMBS[i];
    };
}


void Coder::encode(const char *message, float *samples, int from, float ampl) {
    int idx = from;

    float prevFreq = get_freq_by_sym(message[0]).frequence;
    float currFreq = prevFreq;
    float nextFreq = 0;
    size_t len = strlen(message);
    for (int i = 0; i < len; i++) {
        if (i < len - 1) {
            nextFreq = get_freq_by_sym(message[i + 1]).frequence;
        }

        if (currFreq != 0 && prevFreq != 0) {
            portamento_beak(currFreq, prevFreq, nextFreq, samples, idx, ampl);
            idx += LEN;
        }
        prevFreq = currFreq;
        currFreq = nextFreq;
    }
}

void Coder::encode_symbol(char symb, float *samples, int from, float ampl) {
    float freq = get_freq_by_sym(symb).frequence;
    portamento_beak(freq, freq, freq, samples, from, ampl);
}


void Coder::portamento_beak(float currFreq, float prevFreq, float nextFreq, float *values, int from, float ampl) {
    float freq = prevFreq + (currFreq - prevFreq) / 2;
    float step = (currFreq - freq) / PORT_RAMP_LEN;
    for (int i = 0; i < PORT_RAMP_LEN; i++) {
        float angle = (float) (i / (SAMPLE_RATE / freq) * 2.0 * PI);
        values[from++] = (float) sin(angle) * ampl;
        freq += step;
    }

    for (int i = 0; i < PORT_LEN; i++) {
        float angle = (float) (i / (SAMPLE_RATE / freq) * 2.0 * PI);
        values[from++] = (float) sin(angle) * ampl;
    }

    step = ((nextFreq - freq) / 2) / PORT_RAMP_LEN;
    for (int i = 0; i < PORT_RAMP_LEN; i++) {
        float angle = (float) (i / (SAMPLE_RATE / freq) * 2.0 * PI);
        values[from++] = (float) sin(angle) * ampl;

        freq += step;
    }
}