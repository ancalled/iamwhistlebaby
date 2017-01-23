//
// Created by Andrey Koltochnik on 1/11/17.
//

#include <cstring>
#include "Synthesizer.h"

#ifndef WHISTLE_CRC32_H
#define WHISTLE_CRC32_H

#endif //WHISTLE_CRC32_H

//https://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks
static uint16_t crc10(const char *message) {
    uint16_t value = 0, bit, out_bit;
    uint16_t out_bit_mask = 1;
    uint16_t bit_mask = (1 << 10) - 1;

    for (int j = 0; j < strlen(message); j++) {
        uint8_t sym = (uint8_t) message[j];

        for (uint8_t i = 0; i < 8; i++) {
            bit = ((sym & (1 << i)) >> i) & 0x1;
            out_bit = ((value & out_bit_mask) != 0) & 0x1;
            value >>= 1;
            value &= bit_mask;

            if (out_bit ^ (bit & 0x1))
                value ^= 0x331; //polynom

        };
    }
    return (value ^ 0) & 0xFFF;
}

static void generateTail(const char* body, char* tail, int size = 2) {
    uint16_t c = crc10(body);
    int pow = 5;

    int mask = (1 << pow) - 1;
    int div = 0;
    for (int i = 0; i < size; i++) {
        int idx = (c >> div) & mask;
        tail[i] = wsl::SYMBOLS[idx].symbol;
        div += pow;
    }
    tail[size] = '\0';
}