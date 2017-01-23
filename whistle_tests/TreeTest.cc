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


TEST(TreeTest, InitTree) {
    VarianceTree mes;

    mes.addTiers({
                         {{'h', 0.9}, {'k', 0.5}},
                         {{'j', 0.85}},
                         {{'3', 0.9}, {'e', 0.7}},
                 });

    mes.printBranches();
}


TEST(TreeTest, CRCTest) {
    const char *body = "hjntdb982ilj6etj6e3k";
    char tail[3];
    generateTail(body, tail);
    printf("%s %s\n", body, tail);
}


int main(int argc, char **argv) {
    cout << "Running tree-tests for libwhistle ver: " << getVersion() << "" << endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}