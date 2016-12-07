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
    for (uint16_t i = 0; i < SYMBS; i++) {
        float f = sineFrameSize / (sampleRate / FREQS[i]);
        phaseSteps[i] = toFixedPoint(f, 2);
    }

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
        uint64_t phaseStep = findPhaseStep(ch);
        if (phaseStep) {
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
            smpl = 255 - smpl;
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

uint64_t tablesynth::findPhaseStep(char ch) {
    uint8_t idx;

    if (ch >= 'a' && ch <= 'v') {
        idx = ch - 'a' + 10;
    } else if (ch >= '0' && ch <= '9') {
        idx = ch - '0';
    } else {
        // BAD_CHIRP_CODE_WARNING;
        return 0L;
    }

    return phaseSteps[idx];
}

uint64_t tablesynth::toFixedPoint(double f, int8_t point) {
    uint32_t fp = 0;
    for (int i = 0; i < 4; i++) {
        if (i <= point) {
            int mp = 1 << 8 * (point - i);
            double x = f * mp;
            int i1 = (int) floor(x);
            int b = i1 & 255;
            fp += b << 8 * i;
        } /*else {
            //todo do for higher bytes
        }*/
    }

    return fp;

}
