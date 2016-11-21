#include <cstdlib>
#include "Decoder.h"
// Created by ancalled on 11/11/16.
//


Decoder::Decoder() {
}

Decoder::Decoder(int sr) {
    initialize(sr);
}

void Decoder::initialize(int sr) {
    sampleRate = sr;
    probability = 0.0;

    //initialize array and set it to zero
    for (int i = 0; i < HB_SIZE; i++) {
        buf[i] = 0;
    }

    for (int i = 0; i < HB_SIZE; i++) {
        normalized[i] = 0;
    }

    minLag = SAMPLE_RATE / MAX_FREQ;
    maxLag = SAMPLE_RATE / MIN_FREQ;

}


float Decoder::getProbability() {
    return probability;
}

float Decoder::getPitch(int *samples, int from) {
    int tauEstimate = -1;
    float pitchInHertz = -1;


    difference(samples, from);

    cumulativeMeanNormalizedDifference();

    tauEstimate = absoluteThreshold();

    if (tauEstimate != -1) {

        pitchInHertz = sampleRate / parabolicInterpolation(tauEstimate);
//        pitchInHertz = sampleRate / tauEstimate;
    }


    return pitchInHertz;
}


void Decoder::difference(int *samples, int from) {
    int idx;
    int tau;
    int delta;
    for (tau = 1; tau < HB_SIZE; tau++) {

        for (idx = from; idx < from + HB_SIZE; idx++) {
            delta = samples[idx] - samples[idx + tau];
            int v = delta * delta;
            if (idx == from) {
                buf[tau] = v;
            } else {
                buf[tau] += v;
            }
        }
    }
}

void Decoder::cumulativeMeanNormalizedDifference() {
    int tau;
    buf[0] = 1;
    int runningSum = 0;
    for (tau = 1; tau < HB_SIZE; tau++) {
        runningSum += buf[tau];
        float sm = tau / (float)runningSum;
        normalized[tau] =  buf[tau] * sm;
    }
}

int Decoder::absoluteThreshold() {
    int tau;
    for (tau = minLag + 2; tau < HB_SIZE; tau++) {
        if (normalized[tau] < TRESHOLD) {
            while (tau + 1 < HB_SIZE && normalized[tau + 1] < normalized[tau]) {
                tau++;
            }

            probability = 1 - normalized[tau];
            break;
        }
    }
    // if no pitch found, tau => -1
    if (tau == HB_SIZE || normalized[tau] >= TRESHOLD) {
        tau = -1;
        probability = 0;
    }
    return tau;
}


float Decoder::parabolicInterpolation(int tauEstimate) {
    float betterTau;
    int x0;
    int x2;

    if (tauEstimate < 1) {
        x0 = tauEstimate;
    } else {
        x0 = tauEstimate - 1;
    }
    if (tauEstimate + 1 < HB_SIZE) {
        x2 = tauEstimate + 1;
    } else {
        x2 = tauEstimate;
    }
    if (x0 == tauEstimate) {
        if (normalized[tauEstimate] <= normalized[x2]) {
            betterTau = tauEstimate;
        } else {
            betterTau = x2;
        }
    } else if (x2 == tauEstimate) {
        if (normalized[tauEstimate] <= normalized[x0]) {
            betterTau = tauEstimate;
        } else {
            betterTau = x0;
        }
    } else {
        float s0, s1, s2;
        s0 = normalized[x0];
        s1 = normalized[tauEstimate];
        s2 = normalized[x2];
        betterTau = tauEstimate + (s2 - s0) / (2 * (2 * s1 - s2 - s0));
    }
    return betterTau;
}