//
// Created by ancalled on 12/15/16.
//

#include "Decoder.h"
#include "Synthesizer.h"

using namespace wsl;

Decoder::Decoder(uint32_t sr, uint16_t frame) :
        sampleRate(sr),
        minFreq(1500),
        maxFreq(12000),
        frameSize(frame),
        detector(sr, 512, minFreq, maxFreq) {

    framesToDetect = (uint8_t) (sampleRate * TOP_TIME / 1000 / frameSize);
}

void Decoder::processFrame(int16_t *samples, uint32_t from) {
    float pitch = detector.getPitch(samples, from, frameSize, 0.15);
    if (candidate.freq > 0) {
        float diff = abs((pitch - candidate.freq) / (maxFreq - minFreq));
        if (diff < 0.01) {
            candidate.frames++;
        } else {
            if (candidate.frames >= framesToDetect) {
                printf("%.2f\t%d\t%.2f\t%d\n", candidate.freq, candidate.symbol, candidate.error, candidate.frames);
            }

            candidate.frames = 1;
            SymbMatch m = match(pitch);
            candidate.symbol = m.symbol;
            candidate.error = m.error;
        }

        candidate.freq = pitch;
    } else {
        candidate.freq = pitch;
        candidate.frames = 1;
        SymbMatch m = match(pitch);
        candidate.symbol = m.symbol;
        candidate.error = m.error;
    }


}

float Decoder::abs(float val) {
    return val > 0 ? val : -val;
}

Decoder::SymbMatch Decoder::match(float pitch) {
    for (int i = 0; i < SYMBS - 1; i++) {
        sound_symbol s1 = SYMBOLS[i];
        sound_symbol s2 = SYMBOLS[i + 1];
        if (s1.freq < pitch && pitch < s2.freq) {
            sound_symbol found = s2.freq - pitch > pitch - s1.freq ? s1 : s2;
            float er = abs(found.freq - pitch) / (s2.freq - s1.freq);
            return {found.symbol, er};
        }
    }

    return {0, 0};
}
