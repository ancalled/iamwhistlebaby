//
// Created by ancalled on 12/15/16.
//

#ifndef WHISTLE_DECODER_H
#define WHISTLE_DECODER_H


#include <cstdint>
#include <vector>
#include <string>
#include "PitchDetector.h"

#define DEFAULT_MIN_FREQ 1000
#define DEFAULT_MAX_FREQ 20000
#define DEFAULT_BUF_SIZE 512

//#define DETECTOR_THRESHOLD 0.15
#define DETECTOR_THRESHOLD 0.3
//#define DETECTOR_THRESHOLD 0.4
//#define FREQ_DIFF 0.01
#define FREQ_DIFF 0.005
//#define FREQ_DIFF 0.001
#define MIN_MESSAGE_LEN 3
#define MAX_VOID_FRAMES 4

class MessageDecoder {

public:
    struct Candidate {
        float freq;
        int transFrames;
        int sustFrames;
        char symbol;
        float error;
        bool trusted;
    };

    enum MessageState {
        MS_NONE = 0,
        MS_DETECTING = 1,
        MS_GOT_RESULT = 2
    };

    struct ProcessResult {
        MessageState messageState;
        Candidate candidate;
        PitchDetector::DetectResult detector;
    };


    MessageDecoder(uint32_t sr, uint16_t frameSize);

    MessageDecoder::ProcessResult processFrame(int16_t *samples, uint32_t from = 0, bool print_pitches = false);

    bool hasResult();

    const std::string popMessage();

    void stopDetection();

    void clearState();

    void printTaus();

private:

    enum SymbolState {
        SS_NO_SYMBOL = 1,
        SS_SYMBOL_CANDIDATE = 2,
        SS_CONFIRMED_SYMBOL = 3,
    };

    struct SymbMatch {
        char symbol;
        float error;
    };

    const uint32_t sampleRate;
    const float minFreq;
    const float maxFreq;
    uint16_t frameSize;
    uint8_t transitionFrames;
    uint8_t sustainedFrames;
    PitchDetector detector;
    SymbolState symbolState = SS_NO_SYMBOL;
    MessageState messageState = MS_NONE;
    Candidate candidate = {0, 0, '\0', 0, false};
    std::string lastMessage;
    std::string currMessage;
    uint32_t frameCnt;
    uint32_t lastEffectiveFrame;

    SymbMatch match(float pitch);

    float abs(float val);

    void initCandidate(float pitch);
};


#endif //WHISTLE_DECODER_H
