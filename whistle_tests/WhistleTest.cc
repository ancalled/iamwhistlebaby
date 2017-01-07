//
// Created by ancalled on 12/20/16.
//


#include <MessageDecoder.h>
#include "gtest/gtest.h"
#include "Synthesizer.h"
#include <cstring>
#include <WhistleConfig.h>

using namespace std;

#define SR_SIZE 4

static const uint32_t SAMPLE_RATES[SR_SIZE] = {
        19000,
        38000,
        44100,
        62500
};

size_t levDist(const char *s, size_t n, const char *t, size_t m) {
    ++n;
    ++m;
    size_t *d = new size_t[n * m];

    memset(d, 0, sizeof(size_t) * n * m);

    for (size_t i = 1, im = 0; i < m; ++i, ++im) {
        for (size_t j = 1, jn = 0; j < n; ++j, ++jn) {
            if (s[jn] == t[im]) {
                d[(i * n) + j] = d[((i - 1) * n) + (j - 1)];
            } else {
                d[(i * n) + j] = min(d[(i - 1) * n + j] + 1, /* A deletion. */
                                     min(d[i * n + (j - 1)] + 1, /* An insertion. */
                                         d[(i - 1) * n + (j - 1)] + 1)); /* A substitution. */
            }
        }
    }

    size_t r = d[n * m - 1];
    delete[] d;
    return r;
}

string randomMes(int maxLength) {
    int len = rand() % maxLength;
    string mes = "";
    for (int i = 0; i < len; i++) {
        char symbol = wsl::SYMBOLS[rand() % SYMBS].symbol;
        mes += symbol;
    }
    return mes;
}

void fill(int16_t *source, uint32_t from, int srcSize, int16_t *dest, uint16_t destSize) {
    uint32_t sz = from + destSize < srcSize ? destSize : srcSize - from;
    for (uint32_t i = 0; i < sz; i++) {
        dest[i] = (int16_t) (source[from + i] - 127);
    }
}

TEST(WhistleTest, Generate) {
    const char *mes = "hjntdb982ilj6etj6e3l\0";
    size_t mesLen = strlen(mes);

    for (int i = 0; i < SR_SIZE; i++) {
        uint32_t sampleRate = SAMPLE_RATES[i];

        Synthesizer synth(sampleRate);

        uint32_t size = (uint32_t) (mesLen * sampleRate * (TOP_TIME + RAMP_TIME) / 1000);
        int16_t samples[size];

        uint32_t gen = synth.generate(samples, size, mes);
//        EXPECT_EQ(size, gen);
    }
};


TEST(WhistleTest, CodeAndDecode) {
//    uint32_t sampleRate = 19000;
//    uint32_t sampleRate = 38000;
    uint32_t sampleRate = 44100;
//    uint32_t sampleRate = 62500;

    int8_t framesPerSound = 10;
    int16_t samplesPerSoud = (int16_t) (sampleRate * (RAMP_TIME + TOP_TIME) / 1000);
    uint16_t frameSize = (uint16_t) (samplesPerSoud / framesPerSound);
    Synthesizer synth(sampleRate);
    MessageDecoder decoder(sampleRate, frameSize);

    string toEncode = "hjntdb982ilj6etj6e3l\0";
//    string toEncode = "hjpk93soli6k530de9\0";

    uint32_t size = (uint32_t) (toEncode.size() * samplesPerSoud) + 1;
    int16_t samples[size];

    uint32_t gen = synth.generate(samples, size, toEncode.c_str());

    // Decoding

    int frame = 0;
    uint32_t n = 0;
//    int16_t buf[frameSize];

    while (n < gen) {
//        fill(samples, n, gen, buf, frameSize);
        decoder.processFrame(samples, n);

        n += frameSize;
        frame++;
    }

    decoder.stopDetection();

    string decoded = decoder.popMessage();
    EXPECT_EQ(toEncode, decoded);
    size_t dist = levDist(toEncode.c_str(), toEncode.size(), decoded.c_str(), decoded.size());
    cout << "Dist: " << dist << endl;
    decoder.clearState();
}


TEST(WhistleTest, CodeAndDecodeMultiple) {
    uint32_t sampleRate = 62500;

    int8_t framesPerSound = 10;
    int16_t samplesPerSoud = (int16_t) (sampleRate * (RAMP_TIME + TOP_TIME) / 1000);
    uint16_t frameSize = (uint16_t) (samplesPerSoud / framesPerSound);
    Synthesizer synth(sampleRate);
    MessageDecoder decoder(sampleRate, frameSize);

    for (int i = 0; i < 100; i++) {

        string toEncode = randomMes(31);

        uint32_t size = (uint32_t) (toEncode.size() * samplesPerSoud) + 1;
        int16_t samples[size];
        uint32_t gen = synth.generate(samples, size, toEncode.c_str());

        int frame = 0;
        uint32_t n = 0;
//        int16_t buf[frameSize];
        while (n < gen) {
//            fill(samples, n, gen, buf, frameSize);
            decoder.processFrame(samples, n);

            n += frameSize;
            frame++;
        }
        decoder.stopDetection();

        string decoded = decoder.popMessage();
//        cout << "Decoded: " << decoded << endl;
        EXPECT_EQ(toEncode, decoded);
        size_t dist = levDist(toEncode.c_str(), toEncode.size(), decoded.c_str(), decoded.size());
//        cout << "Dist: " << dist << endl;
        decoder.clearState();
    }
}



int main(int argc, char **argv) {

    cout << "Running tests for libwhistle ver: " << getVersion() << "" << endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}