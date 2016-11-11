#include <iostream>
#include <cstring>
#include "Coder.h"
#include "Decoder.h"

using namespace std;


//int main() {
//
//    int len = 1024 * 205;
//    float samples[len] = {0};
//
//    std::cout << "Generating beak..." << std::endl;
//
//    const char *message = "0123456789abcdefghijklmnopqrstuv";
//    cout << "Mes length: " << strlen(message) << endl;
//
//    std::cout << "\tgot " << sizeof(samples) << " samples" << std::endl;
//
//    Coder c;
//    c.generate(message, samples, 0, 10);
//
//
//    Decoder d;
//    vector<Decoder::Result> res = d.decode(samples, len);
//    cout << res.size() << endl;
//    int i = 0;
//    for (auto const &r: res) {
//        cout << "[" << i << " | " << r.frame << "] " << r.symb << '\t' << r.error << std::endl;
//        i++;
//    }
//
//    return 0;
//}

int main() {
    Coder coder;
    Decoder decoder;

    int len = coder.LEN + 1;
    float samples[len]  = {0};

    string mes = "0123456789abcdefghijklmnopqrstuv";
    for (char &ch : mes) {
        double expFreq = coder.get_freq_by_sym(ch).frequence;

        coder.encode_symbol(ch, samples, 0, 10);
        vector<Decoder::Result> res = decoder.decode(samples, 0, len);

        int matches = 0;
        float sumEr = 0;
        for (auto const &r: res) {
            if (r.symb == ch) {
                matches++;
                sumEr += r.error;
            }
        }
        float e = 100 * sumEr / matches;

        string additional = "";
//        if (matches> 0 && 1.0 * matches / res.size() < 0.8) {
//            TreeMap<Character, Integer> chart = new TreeMap<>();
//            for (Decoder::Result r: res) {
//                Integer v = chart.get(r.symb);
//                chart.put(r.symb, v != null ? v + 1 : 1);
//            }
//
//            int j = 0;
//            for (char actCh: chart.keySet()) {
//                if (actCh == c) continue;
//                double actFreq = FREQS[getIdxOf(actCh)];
//
//                additional += "'" + actCh + "' [" + actFreq + "] " + chart.get(actCh) + "\t";
//            }
//        }
        cout << ch << "\t" << matches << "\t" << e << endl;
//        printf("'%s' [%4.2f]\t%3d/%d\t%8.2f\t%s\n", ch, expFreq, matches, res.size(), e, additional);
    }

}