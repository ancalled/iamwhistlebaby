//
// Created by ancalled on 1/11/17.
//

#ifndef WHISTLE_MESSAGEDECODER2_H
#define WHISTLE_MESSAGEDECODER2_H

#include <cstdint>
#include <vector>
#include <string>
#include "PitchDetector.h"

#define DEFAULT_MIN_FREQ 1000
#define DEFAULT_MAX_FREQ 20000
#define DEFAULT_BUF_SIZE 512

#define CUMULATIVE_PROBABILITY_THRESHOLD 0.7

class MessageDecoder2 {

public:
    struct SymbMatch {
        char symbol;
        float error;
    };

    struct SymbolCandidate {
        std::vector<PitchDetector::PitchCandidate> candidates;
        char symbol;
        int frames;
        float cumulativeProbability;
        uint64_t lastFrame;

        void addPitch(PitchDetector::PitchCandidate &pc, SymbMatch &sm) {
            candidates.push_back(pc);
            float symProb = 1 - sm.error;
            float prob = pc.probability * symProb;
            cumulativeProbability = (cumulativeProbability * frames + prob) / (frames + 1);
            frames++;

        }
    };

    MessageDecoder2(uint32_t sr, uint16_t frameSize);

    void processFrame(int16_t *samples, uint32_t from);

private:
    const uint32_t sampleRate;
    const float minFreq;
    const float maxFreq;
    uint16_t frameSize;
    uint8_t transitionFrames;
    uint8_t sustainedFrames;
    PitchDetector detector;
    std::vector<SymbolCandidate> candidates;
    uint64_t frameCnt;
    
    bool matchCandidate(PitchDetector::PitchCandidate &pc, SymbMatch &sm);
    void initCandidate(PitchDetector::PitchCandidate &pc, SymbMatch &sm);

    MessageDecoder2::SymbMatch matchSymbol(float pitch);

    float abs(float val) {return val > 0 ? val : -val;};

};


#endif //WHISTLE_MESSAGEDECODER2_H
