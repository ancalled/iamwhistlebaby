//
// Created by ancalled on 11/14/16.
//

#ifndef AUDIOPROTOCOL_VESLO_H
#define AUDIOPROTOCOL_VESLO_H


#include <map>
#include "Decoder2.h"
#include "Decoder.h"

class Veslo {


private:

    struct SymbAppearence {
        char symbol;
        int count;
    };

    struct SymbDetect {
        char symbol;
        float error;
    };

    std::map<char, int> symbs;

//    Decoder2 dec;
    Decoder dec;
    std::string message = "";
    int winSize = 0;
    int frames = 0;
    int processed = 0;
    int maxFrames = 0;


public:

    Veslo(int sr);

    void listen(int *samples, int from);

    int hasMessage();

    std::string getMessage();

    void clearState();

private:
    SymbDetect findSymbol(float freq);

};

enum State {

};


#endif //AUDIOPROTOCOL_VESLO_H
