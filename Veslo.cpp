//
// Created by ancalled on 11/14/16.
//

#include <iostream>
#include "Veslo.h"
#include "Coder.h"

Veslo::Veslo(int sr) {
    dec.initialize(sr);
    maxFrames = (int) (sr * BEAK_LASTS / (1000 * WIN_SIZE));
    Segment s = {};
    current = &s;
//    segments[segSize++] = s;
}

void Veslo::listen_old(int *samples, int from) {
//    float freq = dec.getPitch(samples, from);
//    float prob = dec.getProbability();
//
//    if (prob > 0.8) {
//        SymbDetect sd = findSymbol(freq);
//
//        if (sd.error < 0.1 && sd.symbol != '-') {
//            symbs[sd.symbol] += 1;
//            processed++;
//            cout << sd.symbol << "\t" << sd.error << endl;
//        }
//    }
//
//    frames++;
//
//    if (processed >= maxFrames) {
//        char sym = '\0';
//        for (auto const &ent : symbs) {
//            if (ent.second > maxFrames * 2 / 3) {
//                sym = ent.first;
//                break;
//            }
//        }
//
//        if (sym != '\0') {
////            message.append(sym);
//            message += sym;
//        }
//
//        symbs.clear();
//        processed = 0;
//    }
}


void Veslo::listen(uint8_t *samples, const int from, const int frame) {
    Decoder::Pitch res[TOP_PITCHES];
    int found = dec.findTopPitches(samples, from, res);

    for (int i = 0; i < found; i++) {
        Decoder::Pitch pitch = res[i];

        Candidate* cand = nullptr;
        bool overtone = false;
        for (int j = 0; j < current->candSize; j++) {
            Candidate* c = current->candidates[j];
            if (pitch.tau == c->tau) {
                cand = c;
                break;
            }
            if (pitch.tau != c->tau && pitch.tau % c->tau == 0) {
                overtone = true;
                break;
            }
        }


        if (!overtone) {
            if (!cand) {
//                Candidate newone;
//                cand = &newone;
                cand = new Candidate;
                cand->tau = pitch.tau;
                cand->firstFrame = frame;
                cand->prob = pitch.prob;
                current->candidates[current->candSize++] = cand;
            } else {
                cand->prob = cand->prob * cand->frames / (cand->frames + 1) + pitch.prob / (cand->frames + 1);
            }
            cand->frames++;
            cand->lastFrame = frame;

        }
    }

    if (segFrame++ >= maxFrames) {
        Segment s = {};
//        segments[segSize++] = s;
//        current = &s;
        segFrame = 0;
    }

}


void Veslo::printAll() {
//    for (auto sgmt : segments) {
//        int maxFrm = 0;
//        int maxTau;
//        Candidate maxCnd;
//        for (auto cnd : sgmt.candidates) {
//            if (cnd.second.frames > maxFrm) {
//                maxCnd = cnd.second;
//                maxTau = cnd.first;
//                maxFrm = maxCnd.frames;
//            }
//        }
//
//        int symb = 13 - maxTau;
//        cout << symb << "\t"
//             << maxCnd.frames << "\t"
//             << maxCnd.prob << " "
//             //             << maxCnd.firstFrame << " - " << maxCnd.lastFrame
//             //             << " [" << maxCnd.frames << "]"
//             << endl;
//
////        for (auto cnd : sgmt.candidates) {
////            cout << cnd.first << " ["
////                 << 13 - cnd.first << "] "
////                 << cnd.second.prob << " "
////                 << cnd.second.firstFrame << " - " << cnd.second.lastFrame
////                 << " [" << cnd.second.frames << "]"
////                 << endl;
////        }
//
//    }
}

//
//Veslo::SymbDetect Veslo::findSymbol(float freq) {
//    float err = 0;
//    char symb = '-';
//
//    for (int i = 0; i < sizeof(AP::SYMBS) / sizeof(AP::SYMBS[0]) - 1; i++) {
//        float bottom = AP::SYMBS[i].frequence;
//        float top = AP::SYMBS[i + 1].frequence;
//
//        if (freq > bottom && freq < top) {
//            float ln = top - bottom;
//            float diff1 = freq - bottom;
//            float diff2 = top - freq;
//            if (diff1 < diff2) {
//                symb = AP::SYMBS[i].symbol;
//                err = diff1 / ln;
//            } else {
//                symb = AP::SYMBS[i + 1].symbol;
//                err = diff2 / ln;
//            }
//            break;
//        }
//
//    }
//
//    return {symb, err};
//}


