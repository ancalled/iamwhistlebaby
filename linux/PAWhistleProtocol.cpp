//
// Created by ancalled on 1/24/17.
//


#include "PAWhistleProtocol.h"


PAWhistleProtocol::PAWhistleProtocol(uint32_t sampleRate) : WhistleProtocol(sampleRate) {

    static const pa_sample_spec ss = {
            .format = PA_SAMPLE_S16LE,
            .rate = sampleRate,
            .channels = 1
    };

    int error;
    if (!(recs = pa_simple_new(NULL, PULSE_AUDIO_NAME, PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
    }


    if (!(plys = pa_simple_new(NULL, PULSE_AUDIO_NAME, PA_STREAM_PLAYBACK, NULL, "play", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
    }

}

bool PAWhistleProtocol::play(int16_t *samples, size_t size) {
    int error;
    if (pa_simple_write(plys, samples, size, &error)) {
        fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
        return false;
    }

    if (pa_simple_drain(plys, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        return 4;
    }
    return true;
}

bool PAWhistleProtocol::listen(int16_t *samples, size_t size) {
    int error;
    if (pa_simple_read(recs, samples, size, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
        return false;
    }
    return true;
}

