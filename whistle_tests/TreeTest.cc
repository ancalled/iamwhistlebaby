//
// Created by ancalled on 12/20/16.
//


#include "gtest/gtest.h"
#include "VarianceTree.h"
#include "CRC.h"
#include "LoopbackWhistleProtocol.h"
#include <WhistleConfig.h>
#include <Base32.h>

using namespace std;


static void generateVariance(VarianceTree &tree, string &mesBody) {

    for (char c: mesBody) {
        uint8_t n = wsl::toNum(c);
        if (n == 128) continue;

        vector<Content> candidates;
        uint8_t cn = n;
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
    char *tail = generateCrc(mesBody.c_str());
    string message = mesBody + tail;
    printf("Mes: %s\n", message.c_str());

    VarianceTree tree(20);
    generateVariance(tree, message);
    tree.printBranches(true);

    string matched = tree.crcMatched();
    ASSERT_EQ(message, matched);
    delete tail;
}


TEST(TreeTest, CRCTest) {
    char *tail = generateCrc("hjntdb982ilj6etj6e3l");
    ASSERT_STREQ("ar", tail);
    delete tail;
}

TEST(TreeTest, Base32Test) {
    string text = "Test text 2134 is here!";
    printf("\nTo encode: %s\n", text.c_str());

    Base32 b32;
    char *wCode = b32.toWhistle(text.c_str(), text.length());
    printf("\nWhistle code: %s\n", wCode);

    char *dec = b32.fromWhistle(wCode, strlen(wCode));
    printf("\nDecoded: %s\n", dec);

    ASSERT_STREQ(text.c_str(), dec);
}


TEST(TreeTest, ProtocolTest) {
    uint32_t sampleRate = 44100;
    int size = sampleRate * 10;
    int16_t buf1[size];
    int16_t buf2[size];
    
    LoopbackWhistleProtocol client1(sampleRate, buf1, buf2);
    LoopbackWhistleProtocol client2(sampleRate, buf2, buf1);

    string sndMes1 = "Hi, how r u?";
    client1.sendMessage(sndMes1);
    string recMes1 = client2.receiveMessage();
    ASSERT_EQ(sndMes1, recMes1);

    string sndMes2 = "I'm fine, so u?";
    client2.sendMessage(sndMes2);
    string recMes2 = client1.receiveMessage();
    ASSERT_EQ(sndMes2, recMes2);
}


int main(int argc, char **argv) {
    cout << "Running tree-tests for libwhistle ver: " << getVersion() << "" << endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}