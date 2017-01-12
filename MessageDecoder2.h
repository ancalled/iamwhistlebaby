//
// Created by ancalled on 1/11/17.
//

#ifndef WHISTLE_MESSAGEDECODER2_H
#define WHISTLE_MESSAGEDECODER2_H

#include <cstdint>
#include <vector>
#include <string>
#include "PitchDetector.h"
#include "VarianceTree.h"

#define DEFAULT_MIN_FREQ 1000
#define DEFAULT_MAX_FREQ 20000
#define DEFAULT_BUF_SIZE 512

//#define PROB_THRESHOLD 0.79
#define PROB_THRESHOLD 0.65

// pow(2, 1/12) - 1
#define DIFF_COEF 0.0595

class MessageDecoder2 {

public:
    enum DecState {
        NONE = 0,
        SEEK = 1,
        ALIGN = 2,
        STEP = 3,
        CHECK = 4
    };

    struct Frame {
        long cnt;
        char symbol;
        float pitch;
        float probability;
    };

    struct SymbMatch {
        char symbol;
        float error;
    };

    struct SymbolCandidate {
        std::vector<Frame> candidates;
        char symbol;
        int frames;
        float cumulativeProbability;
        long lastFrame;

        SymbolCandidate(Frame &frame) {
            frames = 0;
            symbol = frame.symbol;
            attach(frame);
        }

        void attach(Frame &frame) {
            candidates.push_back(frame);
            cumulativeProbability = (cumulativeProbability * frames + frame.probability) / (frames + 1);
            frames++;
            lastFrame = frame.cnt;
        }

        void voidFrame() {
            cumulativeProbability = (cumulativeProbability * frames) / (frames + 1);
            frames++;
        }

        void removeFirst() {
            const vector<Frame>::iterator &first = candidates.begin();
            Frame frame = *first;
            frames--;
            cumulativeProbability = (cumulativeProbability * frames - frame.probability) / (frames + 1);
            candidates.erase(first);
        }

        bool matched(Frame &frame) {
            return symbol == frame.symbol;
            //todo also check pitch diff
        }
    };

    MessageDecoder2(uint32_t sr, uint16_t frameSize);

    void processFrame(int16_t *samples, uint32_t from);



private:
    DecState state;
    const uint32_t sampleRate;
    const float minFreq;
    const float maxFreq;
    uint16_t frameSize;
    uint8_t transitionFrames;
    uint8_t sustainedFrames;
    uint64_t frameCnt;
    PitchDetector detector;
    std::vector<SymbolCandidate> candidates;

    VarianceTree mesTree;

    bool matchCandidate(Frame &frame);

    void initCandidate(Frame &frame);

    MessageDecoder2::SymbMatch matchSymbol(float pitch);

    float abs(float val) { return val > 0 ? val : -val; };

};


#endif //WHISTLE_MESSAGEDECODER2_H
