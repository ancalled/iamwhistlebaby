//
// Created by ancalled on 1/11/17.
//
#include <cmath>
#include <algorithm>
#include "MessageDecoder2.h"
#include "Synthesizer.h"

using namespace wsl;
using namespace std;

static char *stateNames[] = {"NONE", "SEEK", "ALIGN", "STEP", "CHECK"};


MessageDecoder2::MessageDecoder2(uint32_t sr, uint16_t frameSize, bool debugPrint) :
        sampleRate(sr),
        minFreq(DEFAULT_MIN_FREQ),
        maxFreq(DEFAULT_MAX_FREQ),
        frameSize(frameSize),
        detector(sr, DEFAULT_BUF_SIZE, minFreq, maxFreq),
        mesTree(VAR_TREE_CAP),
        debugPrint(debugPrint) {
    transitionFrames = (uint8_t) round(sampleRate * RAMP_TIME / 1000 / frameSize);
    sustainedFrames = (uint8_t) round(sampleRate * TOP_TIME / 1000 / frameSize);
    frameCnt = 0;
    candidateFrames = 0;
    state = SEEK;
    if (debugPrint)
        printf("Frames %d + %d\n", transitionFrames, sustainedFrames);
}


static void printContent(vector<Content> &vec, uint64_t frameCnt) {
    printf("[%ld] ", frameCnt);
    int i = 0;
    for (Content c: vec) {
        printf("%c (%.2f)", c.symbol, c.probability);
        if (i++ < vec.size() - 1) printf(", ");
    }
    printf("\n");
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

            if (!attachFrame(frame)) {
                initCandidate(frame);
            }
        }
    }

    if (candidates.empty()) {
        if (state == STEP) {
            changeState(SEEK) ;
        }
    } else {

        vector<Content> confirmed;
        DecState newState = NONE;

        if (state == SEEK) {
            vector<SymbolCandidate>::iterator it = candidates.begin();
            while (it != candidates.end()) {
                SymbolCandidate &c = *it;
                bool rem = false;
                if (frameCnt - c.lastFrame > sustainedFrames) {
                    //remove candidates which were not updating last sustFrames
                    rem = true;
                } else {
                    if (c.lastFrame != frameCnt) {
                        //touch candidates which were not matched on this iteration
                        c.voidFrame();
                    }

                    if (c.framesCnt == sustainedFrames) {
//                            if (c.probability >= PROB_THRESHOLD) {
                        if (c.trusted(sustainedFrames)) {
                            //got probable candidate, try to align it during next few steps
                            newState = ALIGN;
                            confirmed.push_back({c.symbol, c.probability});
                        }
                        rem = true;
                    }
                }
                if (rem) {
                    it = candidates.erase(it);
                } else {
                    it++;
                }
            }

        } else if (state == ALIGN) {
            newState = STEP;

        } else if (state == STEP) {
            if (++candidateFrames == sustainedFrames) {
                for (SymbolCandidate &c: candidates) {
//                    if (c.probability >= PROB_THRESHOLD) {
                    if (c.trusted(sustainedFrames)) {
                        confirmed.push_back({c.symbol, c.probability});
                    }
                }

                candidateFrames = 0;
                candidates.clear();

                if (confirmed.empty()) {
                    newState = SEEK;
                } else {
                    newState = ALIGN;
                }
            }

        }

        if (!confirmed.empty()) {
            if (debugPrint)
                printContent(confirmed, frameCnt);
            mesTree.nextTier(confirmed);
        }

        if (newState != NONE) {
            changeState(newState);
        }
    }

    frameCnt++;
}


bool MessageDecoder2::attachFrame(Frame &frame) {
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
    if (last.freq < pitch && pitch < last.freq + 390) {
        float er = abs(last.freq - pitch) / 500;
        return {last.symbol, er};
    }

    return {'#', 1.0};
}

const std::string MessageDecoder2::popMessage() {
    return mesTree.getTopBranch().reversed();
}

void MessageDecoder2::changeState(MessageDecoder2::DecState newState) {
    if (debugPrint)
        printf("[%ld]      %s -> %s\n", frameCnt, stateNames[state], stateNames[newState]);
    state = newState;
}

const vector<string> MessageDecoder2::popMessages(int size) {
    vector<VarianceTree::Branch> branches = mesTree.getTopBranches(size);
    vector<string> res(size);
    for (int i = 0; i < size; i++) {
        res[i] = branches[i].reversed();
    }
    return res;
}

void MessageDecoder2::clearState() {
    changeState(SEEK);
    candidates.clear();
    candidateFrames = 0;
    mesTree.clear();
}
