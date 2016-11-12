//
// Created by Andrey Koltochnik on 11/12/16.
//

#include <cstdlib>
#include "Decoder2.h"
#include "Coder.h"


Decoder2::Decoder2() {
}

Decoder2::Decoder2(float sr, int bufSize) {
    initialize(sr, bufSize);
}

void Decoder2::initialize(float sr, int bufSize) {
    bufferSize = bufSize;
    sampleRate = sr;
    halfBufferSize = bufferSize / 2;
    probability = 0.0;

    //initialize array and set it to zero
    buf = (float *) malloc(sizeof(float) * halfBufferSize);
    for (int i = 0; i < halfBufferSize; i++) {
        buf[i] = 0;
    }

    minLag = SAMPLE_RATE / MAX_FREQ;
    maxLag = SAMPLE_RATE / MIN_FREQ;

}


float Decoder2::getProbability() {
    return probability;
}

float Decoder2::getPitch(float *samples, int from, int size) {
    int tauEstimate = -1;
    float pitchInHertz = -1;

    difference(samples, from, size);

    cumulativeMeanNormalizedDifference();

    tauEstimate = absoluteThreshold();

    if (tauEstimate != -1) {

        pitchInHertz = sampleRate / parabolicInterpolation(tauEstimate);
    }

    return pitchInHertz;
}


void Decoder2::difference(float *samples, int from, int size) {
    int index;
    int tau;
    float delta;
    for (tau = minLag; tau < halfBufferSize; tau++) {
        int ln = size > halfBufferSize ? halfBufferSize : size;

        for (index = from; index < from + ln; index++) {
            delta = samples[index] - samples[index + tau];
            buf[tau] += delta * delta;
        }
    }
}

void Decoder2::cumulativeMeanNormalizedDifference() {
    int tau;
    buf[0] = 1;
    float runningSum = 0;
    for (tau = minLag + 1; tau < halfBufferSize; tau++) {
        runningSum += buf[tau];
        buf[tau] *= tau / runningSum;
    }
}

int Decoder2::absoluteThreshold() {
    int tau;
    for (tau = minLag + 2; tau < halfBufferSize; tau++) {
        if (buf[tau] < TRESHOLD) {
            while (tau + 1 < halfBufferSize && buf[tau + 1] < buf[tau]) {
                tau++;
            }

            probability = 1 - buf[tau];
            break;
        }
    }
    // if no pitch found, tau => -1
    if (tau == halfBufferSize || buf[tau] >= TRESHOLD) {
        tau = -1;
        probability = 0;
    }
    return tau;
}


float Decoder2::parabolicInterpolation(int tauEstimate) {
    float betterTau;
    int x0;
    int x2;

    if (tauEstimate < 1) {
        x0 = tauEstimate;
    } else {
        x0 = tauEstimate - 1;
    }
    if (tauEstimate + 1 < halfBufferSize) {
        x2 = tauEstimate + 1;
    } else {
        x2 = tauEstimate;
    }
    if (x0 == tauEstimate) {
        if (buf[tauEstimate] <= buf[x2]) {
            betterTau = tauEstimate;
        } else {
            betterTau = x2;
        }
    } else if (x2 == tauEstimate) {
        if (buf[tauEstimate] <= buf[x0]) {
            betterTau = tauEstimate;
        } else {
            betterTau = x0;
        }
    } else {
        float s0, s1, s2;
        s0 = buf[x0];
        s1 = buf[tauEstimate];
        s2 = buf[x2];
        // fixed AUBIO implementation, thanks to Karl Helgason:
        // (2.0f * s1 - s2 - s0) was incorrectly multiplied with -1
        betterTau = tauEstimate + (s2 - s0) / (2 * (2 * s1 - s2 - s0));
    }
    return betterTau;
}