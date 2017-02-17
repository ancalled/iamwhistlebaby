//
// Created by ancalled on 1/24/17.
//

#ifndef WHISTLE_PAWHISTLEPROTOCOL_H
#define WHISTLE_PAWHISTLEPROTOCOL_H


#include <WhistleProtocol.h>
#include <pulse/simple.h>
#include <pulse/error.h>

#define PULSE_AUDIO_NAME "whistle"


class PAWhistleProtocol : public WhistleProtocol {

public:
    PAWhistleProtocol(uint32_t sampleRate);

    bool play(int16_t *samples, size_t size);

    size_t listen(int16_t *samples, size_t size);

//    virtual ~PAWhistleProtocol();

private:

    pa_simple *recs;
    pa_simple *plys;
};


#endif //WHISTLE_PAWHISTLEPROTOCOL_H
