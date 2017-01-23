//
// Created by ancalled on 1/23/17.
//

#include <cstdint>
#include "Base32.h"
#include "Synthesizer.h"


size_t Base32::getEncode32Length(size_t bytes) {
    size_t bits = bytes * 8;
    size_t length = bits / 5;
    if ((bits % 5) > 0) {
        length++;
    }
    return length;
}

size_t Base32::getDecode32Length(size_t bytes) {
    size_t bits = bytes * 5;
    size_t length = bits / 8;
    return length;
}


static bool encode32Block(const char *in5, uint8_t *out8) {
    // pack 5 bytes
    uint64_t buffer = 0;
    for (int i = 0; i < 5; i++) {
        if (i != 0) buffer <<= 8;
        buffer |= in5[i];
    }
    // output 8 bytes
    for (int j = 7; j >= 0; j--) {
        buffer <<= (24 + (7 - j) * 5);
        buffer >>= (24 + (7 - j) * 5);
        uint8_t c = (uint8_t) (buffer >> (j * 5));
        // self check
        if (c >= 32) return false;
        out8[7 - j] = c;
    }
    return true;
}


bool Base32::encode32(const char *in, size_t inLen, uint8_t *out) {
    if ((in == 0) || (inLen <= 0) || (out == 0)) return false;

    size_t d = inLen / 5;
    size_t r = inLen % 5;

    uint8_t outBuff[8];

    for (int j = 0; j < d; j++) {
        if (!encode32Block(&in[j * 5], &outBuff[0])) return false;
        memmove(&out[j * 8], &outBuff[0], sizeof(uint8_t) * 8);
    }

    if (r > 0) {
        char padd[5];
        memset(padd, 0, sizeof(uint8_t) * 5);
        for (int i = 0; i < r; i++) {
            padd[i] = in[inLen - r + i];
        }
        if (!encode32Block(&padd[0], &outBuff[0])) return false;
        memmove(&out[d * 8], &outBuff[0], sizeof(uint8_t) * getEncode32Length(r));
    }

    return true;
}


static bool decode32Block(const uint8_t *in8, char *out5) {
    // pack 8 bytes
    uint64_t buffer = 0;
    for (int i = 0; i < 8; i++) {
        // input check
        if (in8[i] >= 32) return false;
        if (i != 0) {
            buffer = (buffer << 5);
        }
        buffer = buffer | in8[i];
    }
    // output 5 bytes
    for (int j = 4; j >= 0; j--) {
        out5[4 - j] = (unsigned char) (buffer >> (j * 8));
    }
    return true;
}


bool Base32::decode32(const uint8_t *in, size_t inLen, char *out) {
    if ((in == 0) || (inLen <= 0) || (out == 0)) return false;

    size_t d = inLen / 8;
    size_t r = inLen % 8;

    char outBuff[5];

    for (int j = 0; j < d; j++) {
        if (!decode32Block(&in[j * 8], &outBuff[0])) return false;
        memmove(&out[j * 5], &outBuff[0], sizeof(unsigned char) * 5);
    }

    unsigned char padd[8];
    memset(padd, 0, sizeof(unsigned char) * 8);
    for (int i = 0; i < r; i++) {
        padd[i] = in[inLen - r + i];
    }
    if (!decode32Block(&padd[0], &outBuff[0])) return false;
    memmove(&out[d * 5], &outBuff[0], sizeof(unsigned char) * getDecode32Length(r));

    return true;
}


char *Base32::toWhistle(const char *text, size_t len) {
    size_t quints = len % 5 != 0 ? len / 5 + 1 : len / 5;
    size_t outSize = quints * 8;
    uint8_t out[outSize];
    encode32(text, len, out);

    char *res = new char[outSize];
    for (int i = 0; i < outSize; i++) {
        res[i] = wsl::SYMBOLS[out[i]].symbol;
    }
    return res;
}

char *Base32::fromWhistle(char *text, size_t len) {
    uint8_t indecies[len];
    for (int i = 0; i < len; i++) {
        indecies[i] = wsl::toNum(text[i]);
    }
    char *out = new char[100];
    decode32(indecies, len, out);
    return out;
}


