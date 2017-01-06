//
// Created by ancalled on 12/15/16.
//

#include "MessageDecoder.h"
#include "Synthesizer.h"

using namespace wsl;

MessageDecoder::MessageDecoder(uint32_t sr, uint16_t frameSize) :
        sampleRate(sr),
        minFreq(DEFAULT_MIN_FREQ),
        maxFreq(DEFAULT_MAX_FREQ),
        frameSize(frameSize),
        detector(sr, DEFAULT_BUF_SIZE, minFreq, maxFreq) {

    transitionFrames = (uint8_t) (2 * sampleRate * RAMP_TIME / 1000 / frameSize);
    sustainedFrames = (uint8_t) (2 * sampleRate * TOP_TIME / 1000 / frameSize);
    frameCnt = 0;
    lastEffectiveFrame = 0;

    detectorState = DS_NONE;
    messageState = MS_NONE;
}

MessageDecoder::ProcessResult MessageDecoder::processFrame(int16_t *samples, uint32_t from, bool print_pitches) {
    PitchDetector::DetectResult pdr;
    pdr = detector.getPitch(samples, from, frameSize, DETECTOR_THRESHOLD);

    if (pdr.pitch > 0) {
        if (detectorState == DS_NONE) detectorState = DS_TRANSITION_FREQ;

        if (candidate.freq > 0 /*detectorState == DS_TRANSITION_FREQ || detectorState == DS_SUSTAINED_FREQ*/) {

            float diff = abs((pdr.pitch - candidate.freq) / (maxFreq - minFreq));

            if (print_pitches) {
                const char* switchSt = diff < FREQ_DIFF ? " " : "@";
                printf("%.2f\t%c\t%.2f\t%.4f %s\t%d\n",
                       candidate.freq,
                       candidate.symbol,
                       candidate.error,
                       diff,
                       switchSt,
                       candidate.sustFrames);
            }

            if (diff < FREQ_DIFF) {
                if (candidate.transFrames < transitionFrames) {
                    candidate.transFrames++;
                } else {
                    if (detectorState != DS_SUSTAINED_FREQ) detectorState = DS_SUSTAINED_FREQ;
                    candidate.sustFrames++;
                }
                lastEffectiveFrame = frameCnt;
            } else {
                initCandidate(pdr.pitch);
            }

            candidate.freq = pdr.pitch;

            if (candidate.sustFrames == sustainedFrames) {
                currMessage += candidate.symbol;
                initCandidate(pdr.pitch);
                if (messageState == MS_NONE) messageState = MS_DETECTING;
            }

        } else {
            initCandidate(pdr.pitch);
        }
    }


    if (currMessage.size() > MIN_MESSAGE_LEN && (frameCnt - lastEffectiveFrame) > MAX_VOID_FRAMES) {
        lastMessage = currMessage;
        currMessage.clear();
        if (messageState == MS_DETECTING) messageState = MS_GOT_RESULT;
    }

    frameCnt++;

    return {messageState, candidate.sustFrames, pdr.pitch, pdr.probability};
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
    SymbMatch m = match(pitch);
    if (m.symbol != '\0') {
        candidate.symbol = m.symbol;
        candidate.error = m.error;
    }
}
