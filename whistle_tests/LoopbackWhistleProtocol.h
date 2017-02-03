//
// Created by ancalled on 1/23/17.
//

#ifndef WHISTLE_LOOPBACKWHISTLEPROTOCOL_H
#define WHISTLE_LOOPBACKWHISTLEPROTOCOL_H
#define BUF_SIZE 441000

#include <WhistleProtocol.h>

class LoopbackWhistleProtocol : public WhistleProtocol {
public:
    LoopbackWhistleProtocol(uint32_t sampleRate, int16_t *inBuffer, int16_t *outBuffer);

    virtual ~LoopbackWhistleProtocol();

    bool play(int16_t *samples, size_t size);

    bool listen(int16_t *samples, size_t size);

private:
    int16_t *inBuffer;
    int16_t *outBuffer;
    size_t inBufCoursor;
    size_t outBufCoursor;
};


#endif //WHISTLE_LOOPBACKWHISTLEPROTOCOL_H
