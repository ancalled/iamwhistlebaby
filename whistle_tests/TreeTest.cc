//
// Created by ancalled on 12/20/16.
//


#include "gtest/gtest.h"
#include "VarianceTree.h"
#include "CRC.h"
#include <stdlib.h>
#include <vector>
#include <WhistleConfig.h>

using namespace std;


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


int main(int argc, char **argv) {
    cout << "Running tree-tests for libwhistle ver: " << getVersion() << "" << endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}