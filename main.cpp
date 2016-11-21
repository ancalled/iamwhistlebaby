#include <iostream>
#include <cstring>
#include "Coder.h"
#include "Decoder.h"
#include "Decoder2.h"
#include "audioData.h"
#include "Veslo.h"

using namespace std;


//int main() {
//
//    int len = 1024 * 205;
//    float buf[len] = {0};
//
//    std::cout << "Generating beak..." << std::endl;
//
//    const char *message = "0123456789abcdefghijklmnopqrstuv";
//    cout << "Mes length: " << strlen(message) << endl;
//
//    std::cout << "\tgot " << sizeof(buf) << " buf" << std::endl;
//
//    Coder c;
//    c.generate(message, buf, 0, 10);
//
//
//    Decoder d;
//    vector<Decoder::Result> res = d.decode(buf, len);
//    cout << res.size() << endl;
//    int i = 0;
//    for (auto const &r: res) {
//        cout << "[" << i << " | " << r.frame << "] " << r.symb << '\t' << r.error << std::endl;
//        i++;
//    }
//
//    return 0;
//}

/*
int main() {
    Coder coder;
//    Decoder decoder;
    Decoder2 decoder;
    decoder.initialize(SAMPLE_RATE, 6700);


    int len = coder.LEN + 1;
    float buf[6700]  = {0};

    string mes = "0123456789abcdefghijklmnopqrstuv";
    for (char &ch : mes) {
        double expFreq = coder.get_freq_by_sym(ch).frequence;

        coder.encode_symbol(ch, buf, 0, 10);
        //vector<Decoder::Result> res = decoder.decode(buf, 0, len);
        float freq = decoder.getPitch(buf);
        float prob = decoder.getProbability();
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

        cout << ch  << '\t' << freq << "\t" << symb << '\t' << err << '\t' << prob << endl;

    }
}
*/

int main() {
//    const int sampleRate = 38000;
    const int sampleRate = 44100;
    Veslo veslo(sampleRate);
//    Decoder2 decoder;
//    decoder.initialize(SAMPLE_RATE, winSize);

    int frame = 0;
    int from = 0;
    while (from + WIN_SIZE < NUM_SAMPLES) {
        veslo.listen(audio, from);

        from += WIN_SIZE;
        frame++;
    }

    cout << veslo.getMessage() << endl;
}

