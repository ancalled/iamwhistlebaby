//
// Created by ancalled on 1/11/17.
//
#include <cmath>
#include <algorithm>
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
    sustainedFrames = (uint8_t) round(sampleRate * TOP_TIME / 1000 / frameSize);
    frameCnt = 0;
    state = SEEK;
}


void MessageDecoder2::processFrame(int16_t *samples, uint32_t from) {
    vector<PitchDetector::PitchCandidate> pcs =
            detector.getPitchCandidates(samples, from, frameSize, 0.5, 2);

    for (PitchDetector::PitchCandidate &pc: pcs) {
        SymbMatch sm = matchSymbol(pc.pitch);
        if (sm.symbol != '#') {
            float symProb = 1 - sm.error;
            float relProb = pc.probability * symProb;
            Frame frame = {frameCnt, sm.symbol, pc.pitch, relProb};

            bool found = matchCandidate(frame);
            if (!found) {
                initCandidate(frame);
            }
        }
    }

    if (candidates.empty()) {
        if (state == STEP) {
            state = SEEK;
            vector<Line> result = mesTree.allVariances();
            printf("------------------------------------------------\n");
            printf("[%ld] Message candidates:\n", frameCnt);
            int num = 1;
            for (Line &line: result) {
                const string &str = line.reversed();
                printf("%d\t%s\t%.4f\n", num++, str.c_str(), line.confidence());
            }
            printf("\n");
        }
    } else {

        vector<Content> trusted;
        DecState newState = NONE;
        vector<SymbolCandidate>::iterator it = candidates.begin();
        while (it != candidates.end()) {
            SymbolCandidate &sc = *it;
            bool removed = false;
            if (frameCnt - sc.lastFrame > sustainedFrames * 1.5) {
                //remove candidates which were not updating last sustFrames
                it = candidates.erase(it);
                removed = true;
            } else {
                if (sc.lastFrame != frameCnt) {
                    //touch candidates which were not matched on this iteration
                    sc.voidFrame();
                }

                if (state == SEEK) {
                    if (sc.frames == sustainedFrames) {
                        if (sc.cumulativeProbability >= PROB_THRESHOLD) {
                            //got probable candidate, try to align it during next steps
                            newState = ALIGN;

                            trusted.push_back({sc.symbol, sc.cumulativeProbability});
                            it = candidates.erase(it);
                            removed = true;
                        } else {
                            it = candidates.erase(it);
                            removed = true;
                        }

                    }
                } else if (state == ALIGN) {
                    if (sc.frames < sustainedFrames && frameCnt - sc.lastFrame > 0) {
                        //this candidate is no more relevant, remove it
                        it = candidates.erase(it);
                        removed = true;
                    } else {
                        //todo implement frame alignment
//                    sc.removeFirst();
//                    newState = STEP;
                    }

                    newState = STEP;
                } else if (state == STEP) {
                    if (sc.frames == sustainedFrames) {
                        if (sc.cumulativeProbability >= PROB_THRESHOLD) {
                            trusted.push_back({sc.symbol, sc.cumulativeProbability});
                        }
                        it = candidates.erase(it);
                        removed = true;
                    }
                }
            }

            if (!removed) it++;
        }

        if (newState != NONE) {
            state = newState;
        }

        if (!trusted.empty()) {
            mesTree.addElements(trusted);
//            printf("------------------------------------------------\n");
//            printf("[%ld] Message candidates:\n", frameCnt);
//            vector<Line> result = mesTree.allVariances();
//            for (Line &line: result) {
//                printf("%s\t%.2f\n", line.reversed().c_str(), line.prob);
//            }
//            printf("\n");
        }
    }

    frameCnt++;
}


bool MessageDecoder2::matchCandidate(Frame &frame) {
    for (SymbolCandidate &sc: candidates) {
        if (sc.matched(frame)) {
            sc.attach(frame);
            return true;
        }
    }
    return false;
}


void MessageDecoder2::initCandidate(Frame &frame) {
    SymbolCandidate sc(frame);
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

    return {'#', 1.0};
}


