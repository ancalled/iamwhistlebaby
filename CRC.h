//
// Created by Andrey Koltochnik on 1/11/17.
//

#include <cstring>
#include "Synthesizer.h"

#ifndef WHISTLE_CRC32_H
#define WHISTLE_CRC32_H

#endif //WHISTLE_CRC32_H

uint16_t crc10(const char *message) {
    uint16_t value = 0, bit, out_bit;
    uint16_t out_bit_mask = 1;
    uint16_t bit_mask = (1 << 10) - 1;

    for (int j = 0; j < strlen(message); j++) {
        uint8_t sym = (uint8_t) message[j];

        for (uint8_t i = 0; i < 8; i++) {
            bit = ((sym & (1 << i)) >> i) & 0x1;
            out_bit = ((value & out_bit_mask) != 0) & (0x1);
            value = value >> 1;
            value = value & bit_mask;

            if (out_bit ^ (bit & 0x1))
                value ^= 0x331;

        };
    }
    return (value ^ 0) & 0xFFF;
}

void generateTail(const char* body, char* tail) {
    uint16_t c = crc10(body);
    int i1 = c & 31;
    int i2 = c / 32;
    tail[0] = wsl::SYMBOLS[i1].symbol;
    tail[1] = wsl::SYMBOLS[i2].symbol;
    tail[2] = '\0';
}