//
// Created by ancalled on 1/23/17.
//

#include "LoopbackWhistleProtocol.h"



LoopbackWhistleProtocol::LoopbackWhistleProtocol(int16_t *inBuffer, int16_t *outBuffer) : inBuffer(inBuffer),
                                                                                          outBuffer(outBuffer) {
    std::fill(inBuffer, inBuffer + BUF_SIZE, 0);
    std::fill(outBuffer, outBuffer + BUF_SIZE, 0);
}



LoopbackWhistleProtocol::~LoopbackWhistleProtocol() {
    delete inBuffer;
    delete outBuffer;
}


bool LoopbackWhistleProtocol::listen(int16_t *samples, size_t size) {
    if (inBufCoursor + size >= BUF_SIZE) {
        return false;
    }
    std::copy(inBuffer + inBufCoursor, inBuffer + inBufCoursor + size, samples);
    inBufCoursor += size;
    return true;
}

bool LoopbackWhistleProtocol::play(int16_t *samples, size_t size) {
    if (outBufCoursor + size >= BUF_SIZE) {
        return false;
    }
    std::copy(samples, samples + size, outBuffer + outBufCoursor);
    outBufCoursor += size;
    return true;
}


