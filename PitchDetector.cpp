//
// Created by Andrey Koltochnik on 11/12/16.
//

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include "PitchDetector.h"


PitchDetector::PitchDetector(u_int32_t sr, u_int32_t bufSize, float minFreq, float maxFreq) :
        bufferSize(bufSize), sampleRate(sr) {

    minLag = (uint16_t) (sr / (maxFreq * 2));
    maxLag = (uint16_t) (sr / minFreq);
    buf = (float *) malloc(sizeof(float) * maxLag);

}


PitchDetector::DetectResult PitchDetector::getPitch(int16_t *samples, uint32_t from, uint32_t size, float threshold) {
    float pitchInHertz = -1;

    difference(samples, from, size);

    cumulativeMeanNormalizedDifference();
    ThresholdResult tr = absoluteThreshold(threshold);
    float ftau;
    if (tr.tau != -1) {
        ftau = parabolicInterpolation(tr.tau);
        pitchInHertz = sampleRate / ftau;
    }

    return {pitchInHertz, tr.probability};
}


std::vector<PitchDetector::PitchCandidate>
PitchDetector::getPitchCandidates(int16_t *samples, uint32_t from, uint32_t size, float threshold, int resSize) {
    difference(samples, from, size);
    cumulativeMeanNormalizedDifference();
    return findLocalMinimums(threshold, resSize);
}

void PitchDetector::difference(int16_t *samples, uint32_t from, uint32_t size) {
    int i;
    int tau;
    int16_t delta;
    for (tau = minLag; tau < maxLag; tau++) {
        int ln = size > maxLag ? maxLag : size;
        for (i = from; i < from + ln; i++) {
            delta = samples[i] - samples[i + tau];
            float sqr = delta * delta;
            if (i == from) {
                buf[tau] = sqr;
            } else {
                buf[tau] += sqr;
            }
        }
    }
}

void PitchDetector::cumulativeMeanNormalizedDifference() {
    int tau;
    buf[minLag] = 1;
    float runningSum = 0;
    for (tau = minLag + 1; tau < maxLag; tau++) {
        runningSum += buf[tau];
//        buf[tau] *= tau / runningSum;
        buf[tau] *= (tau - minLag) / runningSum;
    }
}

PitchDetector::ThresholdResult PitchDetector::absoluteThreshold(float threshold) {
    int tau;
    float probability = 0.0;
    for (tau = minLag + 2; tau < maxLag; tau++) {
        float v = buf[tau];
        if (v < threshold) {
            while (tau + 1 < maxLag && buf[tau + 1] < buf[tau]) {
                tau++;
            }

            probability = 1 - buf[tau];
            break;
        }
    }
    // if no pitch found, tau => -1
    if (tau == maxLag || buf[tau] >= threshold) {
        tau = -1;
        probability = 0;
    }
    return {tau, probability};
}

std::vector<PitchDetector::PitchCandidate> PitchDetector::findLocalMinimums(float threshold, int maxSize) {
    std::vector<PitchDetector::PitchCandidate> res;

    int resCnt = 0;
    for (int tau = minLag + 2; tau < maxLag; tau++) {
        float v = buf[tau];
        if (v < threshold) {
            while (tau + 1 < maxLag && buf[tau + 1] < buf[tau]) {
                tau++;
            }

            float ftau = parabolicInterpolation(tau);
            float pitchInHertz = sampleRate / ftau;
            float reduction = (float) (1 - (resCnt++) * REDUCTION_FACTOR);
            float probability = (1 - buf[tau]) * reduction;
            PitchCandidate c = {tau, pitchInHertz, probability};
            res.push_back(c);
            if (res.size() >= maxSize) break;

            while (tau + 1 < maxLag && buf[tau + 1] > buf[tau]) {
                tau++;
            }
        }
    }

    return res;
}


float PitchDetector::parabolicInterpolation(int tauEstimate) {
    float betterTau;
    int x0;
    int x2;

    if (tauEstimate < 1) {
        x0 = tauEstimate;
    } else {
        x0 = tauEstimate - 1;
    }
    if (tauEstimate + 1 < maxLag) {
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
        betterTau = tauEstimate + (s2 - s0) / (2 * (2 * s1 - s2 - s0));
    }
    return betterTau;
}

void PitchDetector::printTaus() {
    printf("Taus:\t");
    for (int i = 0; i < maxLag; i++) {
        printf("%.4f\t", buf[i]);
    }
    printf("\n");
}
