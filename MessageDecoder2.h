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
//#define DEFAULT_MAX_FREQ 20000
#define DEFAULT_MAX_FREQ 25000
#define DEFAULT_BUF_SIZE 512


//#define PROB_THRESHOLD 0.79
//#define PROB_THRESHOLD 0.65
#define PROB_THRESHOLD 0.6
//#define PROB_THRESHOLD 0.5

// pow(2, 1/12) - 1
#define DIFF_COEF 0.0595

#define VAR_TREE_CAP 16
//#define VAR_TREE_CAP 0 // no limit


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
        uint64_t cnt;
        char symbol;
        float pitch;
        float probability;
    };

    struct SymbMatch {
        char symbol;
        float error;
    };

    struct SymbolCandidate {
        std::vector<Frame> frames;
        char symbol;
        int framesCnt;
        float probability;
        long lastFrame;

        SymbolCandidate(Frame &frame) {
            framesCnt = 0;
            probability = 0;
            symbol = frame.symbol;
            attach(frame);
        }

        void attach(Frame &frame) {
            frames.push_back(frame);
//            probability = (probability * framesCnt + frame.probability) / (framesCnt + 1);
            probability += frame.probability;
            framesCnt++;
            lastFrame = frame.cnt;
        }

        void voidFrame() {
//            probability = (probability * framesCnt) / (framesCnt + 1);
            framesCnt++;
        }

        void removeFirst() {
            const vector<Frame>::iterator &first = frames.begin();
            Frame frame = *first;
            framesCnt--;
//            probability = (probability * framesCnt - frame.probability) / (framesCnt + 1);
            probability -= frame.probability;
            frames.erase(first);
        }

        bool matched(Frame &frame) {
            return symbol == frame.symbol;
            //todo also check pitch diff
        }

        bool trusted(uint16_t expectedFrames) {
            return probability > PROB_THRESHOLD * expectedFrames;
        }
    };

    MessageDecoder2(uint32_t sr, uint16_t frameSize, bool debugPrint = false);

    void processFrame(int16_t *samples, uint32_t from);

    const std::string popMessage();

    const vector<string> popMessages(int size);

    void clearState();


private:
    bool debugPrint;
    DecState state;
    const uint32_t sampleRate;
    const float minFreq;
    const float maxFreq;
    uint16_t frameSize;
    uint8_t transitionFrames;
    uint8_t sustainedFrames;
    uint64_t frameCnt;
    uint32_t candidateFrames;
    PitchDetector detector;
    std::vector<SymbolCandidate> candidates;

    VarianceTree mesTree;

    bool attachFrame(Frame &frame);

    void initCandidate(Frame &frame);

    MessageDecoder2::SymbMatch matchSymbol(float pitch);

    void changeState(DecState newState);

    float abs(float val) { return val > 0 ? val : -val; };


};


#endif //WHISTLE_MESSAGEDECODER2_H
