//
// Created by ancalled on 1/23/17.
//

#ifndef WHISTLE_WHISTLEPROTOCOL_H
#define WHISTLE_WHISTLEPROTOCOL_H

#include <string>
#include "Synthesizer.h"
#include "VarienceMessageDecoder.h"
#include "Base32.h"

using namespace std;

class WhistleProtocol {
public:
    WhistleProtocol();

    void sendMessage(string text);

    string receiveMessage();

    virtual bool play(int16_t* samples, size_t size) = 0;

    virtual bool listen(int16_t* samples, size_t size) = 0;

private:

    Base32 base32;
    VarienceMessageDecoder decoder;
    Synthesizer synth;

    size_t listenBufferSize;
    int16_t* listenBuffer;
};


#endif //WHISTLE_WHISTLEPROTOCOL_H
