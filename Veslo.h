//
// Created by ancalled on 11/14/16.
//

#ifndef AUDIOPROTOCOL_VESLO_H
#define AUDIOPROTOCOL_VESLO_H


#include <map>
#include "Decoder2.h"
#include "Decoder.h"

class Veslo {
    static const int TOP_PITCHES = 5;

public:

    struct Candidate {
        int tau = 0;
        float prob = 0;
        int frames = 0;
        int firstFrame = 0;
        int lastFrame = 0;
    };

    struct Segment {
        Candidate* candidates[TOP_PITCHES];
        int candSize = 0;
    };


private:

//    struct SymbDetect {
//        char symbol;
//        float error;
//    };

//    std::vector<Veslo::Segment> segments;

    Veslo::Segment* segments[20];
    Veslo::Segment* current;
    int segSize = 0;

    Decoder dec;
    int frames = 0;
    int processed = 0;
    int maxFrames = 0;
    int segFrame = 0;

public:

    Veslo(int sr);

    void listen_old(int *samples, int from);

    void listen(uint8_t *samples, const int from, const int frame);


    void printAll();

private:
//    SymbDetect findSymbol(float freq);
};

enum State {

};


#endif //AUDIOPROTOCOL_VESLO_H
