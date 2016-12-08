//
// Created by ancalled on 12/2/16.
//

#include <cstring>
#include <cmath>
#include "tablesynth.h"

tablesynth::tablesynth(uint32_t sampleRate) : sampleRate(sampleRate) {
    phase.u = 384UL << 16;
    frame = 0;

    uint16_t sineFrameSize = SIN_QUADRANT * 4;

    rampSamples = (uint16_t) ((sampleRate * RAMP_TIME) / 1000);
    topSamples = (uint16_t) ((sampleRate * TOP_TIME) / 1000);
//    for (uint16_t i = 0; i < SYMBS; i++) {
//        float f = sineFrameSize / (sampleRate / SYMBOLS[i].freq);
//        phaseSteps[i] = toFixedPoint(f, 2);
//    }

    for (uint16_t i = 0; i < SIN_QUADRANT; i++) {
        sineQuadrant[i] = (uint8_t) (128 * (1 + sin((2 * M_PI * i) / sineFrameSize)));
    }

}


uint32_t tablesynth::generate(uint8_t *samples, uint32_t size, const char *mes) {
    uint32_t proc = 0;
    size_t len = strlen(mes);
    uint64_t prevPhaseStep = 0;
    uint8_t halfampl = 1 << 6;
    uint8_t amplStep = (uint8_t) (rampSamples / halfampl);
    for (int i = 0; i < len; i++) {
        char ch = mes[i];
        sound_symbol ss = findSymbol(ch);
        if (ss.symbol != '\0') {
            uint64_t phaseStep = ss.phaseStep;

            if (!prevPhaseStep) {
                prevPhaseStep = phaseStep;
            }
            int32_t phaseGradient = ((int32_t) phaseStep - (int32_t) prevPhaseStep) / rampSamples;
            proc += generateFrame(samples, size, proc, topSamples, rampSamples,
                                  prevPhaseStep, phaseGradient, halfampl, amplStep);
            prevPhaseStep = phaseStep;
        }
    }
    return proc;
}

uint32_t tablesynth::generateFrame(uint8_t *samples, uint32_t samples_size, uint32_t samples_from,
                                   uint16_t topLen, uint16_t rampLen,
                                   uint64_t phaseStep, int32_t phaseGradient,
                                   uint8_t amplitude, uint8_t amplStep) {

    uint32_t i = 0;

    for (i = 0; i < rampLen + topLen; i++) {
        uint8_t p = phase.b[2];
        int idx = (p & 0x80 ? 255 - p : p);
        uint8_t smpl = sineQuadrant[idx];
        if (phase.b[3] & 1) {
//            smpl = 255 - smpl;
            smpl = - smpl;
        }

        smpl = (smpl * amplitude) >> 8;
        samples[samples_from + i] = smpl;
        phase.u += phaseStep;
        if (i < rampLen) {
            phaseStep += phaseGradient;
            if (!(i % amplStep) && !(amplitude & 0x80)) {
                amplitude++;
            }
        }
        if (i >= samples_size) break;
    }

    frame++;

    return i;
}

tablesynth::sound_symbol tablesynth::findSymbol(char ch) {
    for (int i = 0; i < SYMBS; i++) {
        sound_symbol s = SYMBOLS[i];
        if (s.symbol == ch) {
            return s;
        }
    }
    return {'\0', -1, 0};
}

/*
 * f - float value,
 * p - points in bytes
 */
uint64_t tablesynth::toFixedPoint(double f, int8_t p) {
    uint32_t fp = 0;
    for (int8_t i = 0; i <= p; i++)
        fp += ((uint32_t) floor(f * (1 << 8 * (p - i))) & 255) << 8 * i;
    return fp;
}
