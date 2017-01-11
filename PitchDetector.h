//
// Created by Andrey Koltochnik on 11/12/16.
//

#ifndef AUDIOPROTOCOL_DEC2_H
#define AUDIOPROTOCOL_DEC2_H
#define REDUCTION_FACTOR 0.2
#include <vector>


class PitchDetector {

public:

    struct DetectResult {
        float pitch;
        float probability;
    };

    struct PitchCandidate {
        int tau;
        float pitch;
        float probability;
    };

    PitchDetector(uint32_t sr, uint32_t bufSize, float minFreq, float maxFreq);

    PitchDetector::DetectResult getPitch(int16_t *samples, uint32_t from, uint32_t size, float threshold);

    std::vector<PitchDetector::PitchCandidate> getPitchCandidates(int16_t *samples, uint32_t from, uint32_t size, float threshold);

    void printTaus();

private:

    struct ThresholdResult {
        int tau;
        float probability;
    };

    uint32_t bufferSize;
    uint32_t sampleRate;
    float *buf;
    uint16_t minLag;
    uint16_t maxLag;


    void difference(int16_t *samples, uint32_t from, uint32_t size);

    void cumulativeMeanNormalizedDifference();

    PitchDetector::ThresholdResult absoluteThreshold(float threshold);

    float parabolicInterpolation(int tauEstimate);

    std::vector<PitchDetector::PitchCandidate> findLocalMinimums(float threshold);

};


#endif //AUDIOPROTOCOL_DEC2_H
