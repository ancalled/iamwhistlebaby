//
// Created by ancalled on 1/11/17.
//
#include <cmath>
#include <cmath>
#include "MessageDecoder2.h"
#include "Synthesizer.h"

using namespace wsl;
using namespace std;

MessageDecoder2::MessageDecoder2(uint32_t sr, uint16_t frameSize) :
        sampleRate(sr),
        minFreq(DEFAULT_MIN_FREQ),
        maxFreq(DEFAULT_MAX_FREQ),
        frameSize(frameSize),
        detector(sr, DEFAULT_BUF_SIZE, minFreq, maxFreq) {
    transitionFrames = (uint8_t) round(sampleRate * RAMP_TIME / 1000 / frameSize);
    frameCnt = 0;
}

void MessageDecoder2::processFrame(int16_t *samples, uint32_t from) {
    vector<PitchDetector::PitchCandidate> pcs =
            detector.getPitchCandidates(samples, from, frameSize, 0.5);
    for (PitchDetector::PitchCandidate &pc: pcs) {
        SymbMatch sm = matchSymbol(pc.pitch);

        bool found = matchCandidate(pc, sm);
        if (!found) {
            initCandidate(pc, sm);
        }
    }

    for (SymbolCandidate &sc: candidates) {
        if (sc.cumulativeProbability > CUMULATIVE_PROBABILITY_THRESHOLD) {

        }
    }

    frameCnt++;
}

bool MessageDecoder2::matchCandidate(PitchDetector::PitchCandidate &pc, SymbMatch &sm) {

    bool found = false;
    for (SymbolCandidate &sc: candidates) {
        if (sc.symbol == sm.symbol) {
            sc.addPitch(pc, sm);
            sc.lastFrame = frameCnt;
            found = true;
            break;
        }
    }
    return found;


}

void MessageDecoder2::initCandidate(PitchDetector::PitchCandidate &pc, SymbMatch &sm) {
    SymbolCandidate sc;
    sc.addPitch(pc, sm);
    sc.frames = 0;
    sc.symbol = sm.symbol;
    sc.lastFrame = frameCnt;
    candidates.push_back(sc);
}

MessageDecoder2::SymbMatch MessageDecoder2::matchSymbol(float pitch) {
    const sound_symbol &first = SYMBOLS[0];
    if (first.freq - 40 < pitch && pitch < first.freq) {
        float er = abs(first.freq - pitch) / 100;
        return {first.symbol, er};
    }

    for (int i = 0; i < SYMBS - 1; i++) {
        sound_symbol s1 = SYMBOLS[i];
        sound_symbol s2 = SYMBOLS[i + 1];
        if (s1.freq < pitch && pitch <= s2.freq) {
            sound_symbol found = s2.freq - pitch > pitch - s1.freq ? s1 : s2;
            float er = abs(found.freq - pitch) / (s2.freq - s1.freq);
            return {found.symbol, er};
        }
    }

    const sound_symbol &last = SYMBOLS[SYMBS - 1];
    if (last.freq < pitch && pitch < last.freq + 300) {
        float er = abs(last.freq - pitch) / 500;
        return {last.symbol, er};
    }

    return {0, 0};
}

