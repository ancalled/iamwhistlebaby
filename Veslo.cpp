//
// Created by ancalled on 11/14/16.
//

#include <iostream>
#include "Veslo.h"
#include "Coder.h"

Veslo::Veslo(int sr) : winSize(winSize) {
    dec.initialize(sr);
    maxFrames = (int) round(sr * BEAK_LASTS / (1000 * winSize)) - 1;
}

void Veslo::listen(int *samples, int from) {
    float freq = dec.getPitch(samples, from);
    float prob = dec.getProbability();

    if (prob > 0.8) {
       SymbDetect sd = findSymbol(freq);

        if (sd.error < 0.1 && sd.symbol != '-') {
            symbs[sd.symbol] += 1;
            processed++;
            cout << sd.symbol << "\t" << sd.error << endl;
        }
    }

    frames++;

    if (processed >= maxFrames) {
        char sym ='\0';
        for(auto const& ent : symbs) {
            if (ent.second > maxFrames * 2 / 3) {
                sym = ent.first;
                break;
            }
        }

        if (sym != '\0') {
//            message.append(sym);
            message += sym;
        }

        symbs.clear();
        processed = 0;
    }

}

int Veslo::hasMessage() {
    return 0;
}

std::string Veslo::getMessage() {
    return message;
}

void Veslo::clearState() {
//    message
}

Veslo::SymbDetect Veslo::findSymbol(float freq) {
    float err = 0;
    char symb = '-';

    for (int i = 0; i < sizeof(AP::SYMBS) / sizeof(AP::SYMBS[0]) - 1; i++) {
        float bottom = AP::SYMBS[i].frequence;
        float top = AP::SYMBS[i + 1].frequence;

        if (freq > bottom && freq < top) {
            float ln = top - bottom;
            float diff1 = freq - bottom;
            float diff2 = top - freq;
            if (diff1 < diff2) {
                symb = AP::SYMBS[i].symbol;
                err = diff1 / ln;
            } else {
                symb = AP::SYMBS[i + 1].symbol;
                err = diff2 / ln;
            }
            break;
        }

    }

    return {symb, err};
}


