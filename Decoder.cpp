#include <cstdlib>
#include <cstdint>
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

//    for (int i = 0; i < HB_SIZE; i++) {
//        normalized[i] = 0;
//    }

    minLag = sampleRate / MAX_FREQ;
    maxLag = sampleRate / MIN_FREQ;
}


float Decoder::getProbability() {
    return probability;
}

float Decoder::getPitch(uint8_t *samples, const int from) {
    int tauEstimate;
    float pitchInHertz = -1;


    int avg = difference(samples, from);
    int threshold = avg / 6;
    buf[0] = avg;


    tauEstimate = findFirstMin(threshold);
    if (tauEstimate != -1) {
        pitchInHertz = sampleRate / tauEstimate;
    }

//    cumulativeMeanNormalizedDifference();
//
//    tauEstimate = absoluteThreshold();
//
//    if (tauEstimate != -1) {
//        pitchInHertz = sampleRate / parabolicInterpolation(tauEstimate);
//    }


    return pitchInHertz;
}


//vector<Decoder::Pitch> Decoder::findTopPitches(const int *samples, const int from) {
int Decoder::findTopPitches(uint8_t *samples, const int from, Decoder::Pitch *res) {
    int avg = difference(samples, from);
    buf[minLag] = avg;

    return findLocalMins(avg, res);
}


int Decoder::difference(uint8_t *samples, const int from) {
    int idx;
    int tau;
    int delta;
    int sm = 0;

    for (tau = minLag; tau < maxLag; tau++) {

        for (idx = from; idx < from + HB_SIZE; idx++) {
            delta = samples[idx] - samples[idx + tau];
            int v = delta * delta;
            if (idx == from) {
                buf[tau] = v;
            } else {
                buf[tau] += v;
            }
            if (idx == from + HB_SIZE - 1) {
                sm += buf[tau];
            }
        }
    }

    return sm / (maxLag - minLag);
}


int Decoder::findLocalMins(const int avg, Decoder::Pitch *res) {
    int i = 0;
    for (int tau = minLag; tau < maxLag; tau++) {
        if (buf[tau] < avg) {
            while (tau + 1 < maxLag && buf[tau + 1] < buf[tau]) {
                tau++;
            }

            res[i++] = {tau, 1 - (float) buf[tau] / avg};

            while (tau + 1 < maxLag && buf[tau + 1] > buf[tau]) {
                tau++;
            }

            if (i >= sizeof(res) / sizeof(res[0])) break;
        }
    }
    return i;
}


int Decoder::findFirstMin(const int threshold) {
    for (int tau = minLag + 2; tau < maxLag; tau++) {
        if (buf[tau] < threshold) {
            while (tau + 1 < maxLag && buf[tau + 1] < buf[tau]) {
                tau++;
            }

            probability = 1; //todo this is cheat!
            return tau;
        }
    }
    return -1;
}




//void Decoder::cumulativeMeanNormalizedDifference() {
//    int tau;
//    buf[0] = 1;
//    int runningSum = 0;
//    for (tau = minLag; tau < maxLag; tau++) {
//        runningSum += buf[tau];
//        float sm = tau / (float) runningSum;
//        normalized[tau] = buf[tau] * sm;
//    }
//}
//
//
//int Decoder::absoluteThreshold() {
//    int tau;
//    for (tau = minLag + 2; tau < maxLag; tau++) {
//        if (normalized[tau] < TRESHOLD) {
//            while (tau + 1 < maxLag && normalized[tau + 1] < normalized[tau]) {
//                tau++;
//            }
//
//            probability = 1 - normalized[tau];
//            break;
//        }
//    }
//    // if no pitch found, tau => -1
//    if (tau == maxLag || normalized[tau] >= TRESHOLD) {
//        tau = -1;
//        probability = 0;
//    }
//    return tau;
//}
//
//float Decoder::parabolicInterpolation(int tauEstimate) {
//    float betterTau;
//    int x0;
//    int x2;
//
//    if (tauEstimate < 1) {
//        x0 = tauEstimate;
//    } else {
//        x0 = tauEstimate - 1;
//    }
//    if (tauEstimate + 1 < maxLag) {
//        x2 = tauEstimate + 1;
//    } else {
//        x2 = tauEstimate;
//    }
//    if (x0 == tauEstimate) {
//        if (normalized[tauEstimate] <= normalized[x2]) {
//            betterTau = tauEstimate;
//        } else {
//            betterTau = x2;
//        }
//    } else if (x2 == tauEstimate) {
//        if (normalized[tauEstimate] <= normalized[x0]) {
//            betterTau = tauEstimate;
//        } else {
//            betterTau = x0;
//        }
//    } else {
//        float s0, s1, s2;
//        s0 = normalized[x0];
//        s1 = normalized[tauEstimate];
//        s2 = normalized[x2];
//        betterTau = tauEstimate + (s2 - s0) / (2 * (2 * s1 - s2 - s0));
//    }
//    return betterTau;
//}
