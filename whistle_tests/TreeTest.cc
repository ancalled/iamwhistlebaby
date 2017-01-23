//
// Created by ancalled on 12/20/16.
//


#include "gtest/gtest.h"
#include "VarianceTree.h"
#include "CRC.h"
#include <stdlib.h>
#include <vector>
#include <WhistleConfig.h>
#include <Base32.h>

using namespace std;

static char *copy(const char *orig) {
    char *res = new char[strlen(orig) + 1];
    strcpy(res, orig);
    return res;
}

static void generateVariance(VarianceTree &tree, string &mesBody) {

    for (char c: mesBody) {
        int16_t n = wsl::toNum(c);
        if (n < 0) continue;

        vector<Content> candidates;
        int16_t cn = n;
        float prob = 1.0;
        while (cn < 32) {
            prob -= (rand() % 15) / 100.0;
            char cc = wsl::toChar(cn);
            candidates.push_back({cc, prob});
            cn += 12;
        }
        tree.nextTier(candidates);
    }
}


static char *toWhistle(unsigned char *in, int len) {
    char *res = new char[len];
    for (int i = 0; i < len; i++) {
        res[i] = wsl::SYMBOLS[in[i]].symbol;
    }
    return res;
}

static unsigned char *fromWhistle(char *in, int len) {
    unsigned char *res = new unsigned char[len];
    for (int i = 0; i < len; i++) {
        res[i] = (unsigned char) wsl::toNum(in[i]);
    }
    return res;
}

TEST(TreeTest, InitTree) {
    VarianceTree mes;

    mes.addTiers({
                         {{'h', 0.9}, {'h' + 12, 0.5}},
                         {{'j', 0.85}},
                         {{'3', 0.9}},
                 });

    mes.printBranches();
}

TEST(TreeTest, CRCCheck) {
    printf("\n");

    string mesBody = "hjntdb982ilj6etj6e3l";
//    string mesBody = "hjntdb";
    char tail[3];
    generateTail(mesBody.c_str(), tail);
    string message = mesBody + tail;
    printf("Mes: %s\n", message.c_str());

    VarianceTree tree(20);
    generateVariance(tree, message);
    tree.printBranches(true);

    string matched = tree.getMessage();
    ASSERT_EQ(message, matched);
}


TEST(TreeTest, CRCTest) {
    char tail[3];
    generateTail("hjntdb982ilj6etj6e3l", tail);
    ASSERT_STREQ("ar", tail);
}

TEST(TreeTest, Base32Test) {
    string text = "Testy text here!";
    printf("\nTo encode: %s\n", text.c_str());

    unsigned char *in = reinterpret_cast<unsigned char *>(copy(text.c_str()));
    unsigned char out[500];

    Base32 b32;
    b32.Encode32(in, text.size(), out);
    size_t len = strlen((char *) out);
    char *whistle = toWhistle(out, len);
    printf("Encoded: %s\n", whistle);

    unsigned char rev[500];
    b32.Decode32(fromWhistle(whistle, len), len, rev);
    printf("Decoded: %s\n", rev);
//    ASSERT_EQ(in, rev);
}


int main(int argc, char **argv) {
    cout << "Running tree-tests for libwhistle ver: " << getVersion() << "" << endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}