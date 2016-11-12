//
// Created by Andrey Koltochnik on 11/12/16.
//

#include <cstdlib>
#include "Decoder2.h"


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
    samples = (float *) malloc(sizeof(float) * halfBufferSize);
    for (int i = 0; i < halfBufferSize; i++) {
        samples[i] = 0;
    }
}

float Decoder2::getProbability() {
    return probability;
}

float Decoder2::getPitch(float *buffer) {
    int tauEstimate = -1;
    float pitchInHertz = -1;

    difference(buffer);

    cumulativeMeanNormalizedDifference();

    tauEstimate = absoluteThreshold();

    if (tauEstimate != -1) {

        pitchInHertz = sampleRate / parabolicInterpolation(tauEstimate);
    }

    return pitchInHertz;
}


void Decoder2::difference(float *buffer) {
    int index;
    int tau;
    float delta;
    for (tau = 0; tau < halfBufferSize; tau++) {
        for (index = 0; index < halfBufferSize; index++) {
            delta = buffer[index] - buffer[index + tau];
            samples[tau] += delta * delta;
        }
    }
}

void Decoder2::cumulativeMeanNormalizedDifference() {
    int tau;
    samples[0] = 1;
    float runningSum = 0;
    for (tau = 1; tau < halfBufferSize; tau++) {
        runningSum += samples[tau];
        samples[tau] *= tau / runningSum;
    }
}

int Decoder2::absoluteThreshold() {
    int tau;
    for (tau = 2; tau < halfBufferSize; tau++) {
        if (samples[tau] < TRESHOLD) {
            while (tau + 1 < halfBufferSize && samples[tau + 1] < samples[tau]) {
                tau++;
            }

            probability = 1 - samples[tau];
            break;
        }
    }
    // if no pitch found, tau => -1
    if (tau == halfBufferSize || samples[tau] >= TRESHOLD) {
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
        if (samples[tauEstimate] <= samples[x2]) {
            betterTau = tauEstimate;
        } else {
            betterTau = x2;
        }
    } else if (x2 == tauEstimate) {
        if (samples[tauEstimate] <= samples[x0]) {
            betterTau = tauEstimate;
        } else {
            betterTau = x0;
        }
    } else {
        float s0, s1, s2;
        s0 = samples[x0];
        s1 = samples[tauEstimate];
        s2 = samples[x2];
        // fixed AUBIO implementation, thanks to Karl Helgason:
        // (2.0f * s1 - s2 - s0) was incorrectly multiplied with -1
        betterTau = tauEstimate + (s2 - s0) / (2 * (2 * s1 - s2 - s0));
    }
    return betterTau;
}