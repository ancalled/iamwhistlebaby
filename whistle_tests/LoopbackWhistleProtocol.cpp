//
// Created by ancalled on 1/23/17.
//

#include "LoopbackWhistleProtocol.h"


LoopbackWhistleProtocol::LoopbackWhistleProtocol(uint32_t sampleRate, std::vector<int16_t> &inBuffer,
                                                 std::vector<int16_t> &outBuffer)
        : WhistleProtocol(sampleRate),
          inBuffer(inBuffer),
          outBuffer(outBuffer),
          inBufCoursor(0),
          outBufCoursor(0) {
//    std::fill(inBuffer, inBuffer + BUF_SIZE, 0);
//    std::fill(outBuffer, outBuffer + BUF_SIZE, 0);
}


LoopbackWhistleProtocol::~LoopbackWhistleProtocol() {
//    delete inBuffer;
//    delete outBuffer;
}


size_t LoopbackWhistleProtocol::listen(int16_t *samples, size_t size) {
//    if (inBufCoursor + size >= BUF_SIZE) {
//        return false;
//    }
    size_t sz = inBufCoursor + size;
    if (sz > inBuffer.size()) {
        sz = inBuffer.size();
    }
    std::copy(inBuffer.begin() + inBufCoursor, inBuffer.begin() + sz, samples);
    size_t read = sz - inBufCoursor;
    inBufCoursor = sz;
    return read;
}

bool LoopbackWhistleProtocol::play(int16_t *samples, size_t size) {
    if (outBufCoursor + size >= BUF_SIZE) {
        return false;
    }
//    outBuffer.assign(samples, samples + size);
    for (int i = 0; i < size; i++) {
        outBuffer.push_back(samples[i]);
    }
//    std::copy(samples, samples + size, outBuffer + outBufCoursor);
    outBufCoursor += size;
    return true;
}


