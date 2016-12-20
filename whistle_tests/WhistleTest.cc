//
// Created by ancalled on 12/20/16.
//


#include <Decoder.h>
#include "gtest/gtest.h"
#include "Synthesizer.h"
#define SR_SIZE 4

static const uint32_t SAMPLE_RATES[SR_SIZE] = {
        19000,
        38000,
        44100,
        62500
};

TEST(WhistleTest, Generate) {
    const char *mes = "hjntdb982ilj6etj6e3l\0";
    size_t mesLen = strlen(mes);

    for (int i = 0; i < SR_SIZE; i++) {
        uint32_t sampleRate = SAMPLE_RATES[i];

        Synthesizer synth(sampleRate);

        uint32_t size = (uint32_t) (mesLen * sampleRate * (TOP_TIME + RAMP_TIME) / 1000);
        int8_t samples[size];

        uint32_t gen = synth.generate(samples, size, mes);
        EXPECT_EQ(size, gen);
    }
};


TEST(WhistleTest, CodeAndDecode) {
    uint32_t sampleRate = 62500;
    const char *mes = "hjntdb982ilj6etj6e3l\0";

    Synthesizer synth(sampleRate);

    uint32_t size = (uint32_t) (strlen(mes) * (sampleRate * (TOP_TIME + RAMP_TIME) / 1000)) + 1;
    int8_t samples[size];

    uint32_t gen = synth.generate(samples, size, mes);
    printf("Generated %d samples\n", gen);

    // Decoding

    int8_t framesPerSound = 10;
    uint16_t frameSize = (uint16_t) (sampleRate * (RAMP_TIME + TOP_TIME) / 1000 / framesPerSound);
    float threshold = 0.35;
    Decoder decoder(sampleRate, frameSize);
    int frame = 0;
    uint32_t n = 0;
    int16_t buf[frameSize];

    while (n < gen) {
        uint32_t sz = n + frameSize < gen ? frameSize : gen - n;
        for (int i = 0; i < sz; i++) {
            buf[i] = samples[n + i] - 127;
        }

        decoder.processFrame(buf, 0);
//        float pitch = dec.getPitch(buf, 0, sz, threshold);
//        float prob = dec.getProbability();
//        printf("%.2f\t%.2f\n", frame, pitch, prob);

        n += frameSize;
        frame++;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}