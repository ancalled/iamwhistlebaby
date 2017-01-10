//
// Created by ancalled on 12/15/16.
//

#include <cmath>
#include "MessageDecoder.h"
#include "Synthesizer.h"

using namespace wsl;

MessageDecoder::MessageDecoder(uint32_t sr, uint16_t frameSize) :
        sampleRate(sr),
        minFreq(DEFAULT_MIN_FREQ),
        maxFreq(DEFAULT_MAX_FREQ),
        frameSize(frameSize),
        detector(sr, DEFAULT_BUF_SIZE, minFreq, maxFreq) {

    transitionFrames = (uint8_t) round(sampleRate * RAMP_TIME / 1000 / frameSize);
    sustainedFrames = (uint8_t) round(sampleRate * TOP_TIME / 1000 / frameSize);
    frameCnt = 0;
    lastEffectiveFrame = 0;

    symbolState = SS_NO_SYMBOL;
    messageState = MS_NONE;
}

MessageDecoder::ProcessResult MessageDecoder::processFrame(int16_t *samples, uint32_t from, bool print_pitches) {
    PitchDetector::DetectResult pitch;
    pitch = detector.getPitch(samples, from, frameSize, DETECTOR_THRESHOLD);

    if (pitch.pitch > 0) {
        if (symbolState == SS_NO_SYMBOL) symbolState = SS_SYMBOL_CANDIDATE;

        if (candidate.freq > 0 /*symbolState == SS_SYMBOL_CANDIDATE || symbolState == SS_CONFIRMED_SYMBOL*/) {

            float diff = abs((pitch.pitch - candidate.freq) / (maxFreq - minFreq));

            if (print_pitches) {
                printf("%.2f\t%c\t%.2f\t%.4f\t%d",
                       candidate.freq,
                       candidate.symbol,
                       candidate.error,
                       diff,
                       candidate.sustFrames
                );

                if (candidate.sustFrames == sustainedFrames) {
                    printf("\t'%c'", candidate.symbol);
                }
            }

            if (diff < FREQ_DIFF) {
                if (candidate.transFrames < transitionFrames) {
                    candidate.transFrames++;
                } else {
                    if (symbolState != SS_CONFIRMED_SYMBOL) symbolState = SS_CONFIRMED_SYMBOL;
                    candidate.sustFrames++;
                }
                lastEffectiveFrame = frameCnt;
            } else {
                initCandidate(pitch.pitch);
            }

            candidate.freq = pitch.pitch;

            if (candidate.sustFrames == sustainedFrames) {
                candidate.trusted = true;
                currMessage += candidate.symbol;
                symbolState = SS_SYMBOL_CANDIDATE;
                candidate.freq = 0;
                if (print_pitches) {
                    printf("\t'%c'", candidate.symbol);
                }
            }

            if (candidate.sustFrames >= 2) {
                if (messageState == MS_NONE) messageState = MS_DETECTING;
            }

            if (print_pitches) {
                printf("\n");
            }

        } else {
            initCandidate(pitch.pitch);
        }
    } else {
        if (print_pitches) {
            printf("- no pitch -\n");
        }
    }


    if (currMessage.size() > MIN_MESSAGE_LEN && (frameCnt - lastEffectiveFrame) > MAX_VOID_FRAMES) {
        lastMessage = currMessage;
        currMessage.clear();
        if (messageState == MS_DETECTING) messageState = MS_GOT_RESULT;
    }

    frameCnt++;

    return {messageState, candidate, pitch};
}


float MessageDecoder::abs(float val) {
    return val > 0 ? val : -val;
}

MessageDecoder::SymbMatch MessageDecoder::match(float pitch) {
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

bool MessageDecoder::hasResult() {
    return messageState == MS_GOT_RESULT;
}

const std::string MessageDecoder::popMessage() {
    if (messageState == MS_GOT_RESULT && lastMessage.size() > 0) {
        std::string res = lastMessage;
        lastMessage.clear();
        return res;
    } else {
        return "";
    }
}

void MessageDecoder::stopDetection() {
    lastMessage = currMessage;
    currMessage.clear();
    if (messageState == MS_DETECTING) messageState = MS_GOT_RESULT;
}

void MessageDecoder::clearState() {
    currMessage.clear();
    lastMessage.clear();
    candidate.freq = 0;
    candidate.sustFrames = 0;
    candidate.symbol = '\0';
    candidate.error = 0;
}

void MessageDecoder::initCandidate(float pitch) {
    candidate.freq = pitch;
    candidate.sustFrames = 1;
    candidate.transFrames = 1;
    candidate.trusted = false;
    SymbMatch m = match(pitch);
    if (m.symbol != '\0') {
        candidate.symbol = m.symbol;
        candidate.error = m.error;
    }
}

void MessageDecoder::printTaus() {
    detector.printTaus();
}
