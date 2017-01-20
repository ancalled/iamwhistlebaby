//
// Created by Andrey Koltochnik on 1/11/17.
//

#include "VarianceTree.h"
#include "CRC.h"
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>


int main() {

//    VarianceTree mes;
//
//    std::vector<Content> lvl1;
//    lvl1.push_back({'h', 0.9});
//    lvl1.push_back({'k', 0.5});
//    mes.nextTier(lvl1);
//
//    std::vector<Content> lvl2;
//    lvl2.push_back({'j', 0.85});
//    mes.nextTier(lvl2);
//
//    std::vector<Content> lvl3;
//    lvl3.push_back({'3', 0.9});
//    lvl3.push_back({'e', 0.7});
//    mes.nextTier(lvl3);
//
//    std::vector<Content> lvl4;
//    lvl4.push_back({'d', 0.75});
//    lvl4.push_back({'a', 0.43});
//    mes.nextTier(lvl4);
//
//    mes.printBranches();
//
//    printf("\n\n");
//
//    mes.clear();
//    mes.nextTier(lvl1);
//    mes.nextTier(lvl2);
//    mes.nextTier(lvl3);
//    mes.printBranches();


//    const unsigned char *message = reinterpret_cast<const unsigned char *> ("hjntdb982ilj6etj6e3k");
//    uint32_t  c = crc32b(message);
//    printf("%d\n", c);//1053339487

//    uint8_t wdth = 12;
//    int len = 6;
//    uint8_t polynomial[] = {12, 11, 3, 2, 1, 0};
//    char bf[wdth];
//    generatePolyMask(bf, wdth, polynomial, len);
//
//    printf("Poly: '%s'\n", bf);
    const char *body = "hjntdb982ilj6etj6e3k";
    const char *message = "hjntdb982ilj6etj6e3k";
    char tail[3];
    generateTail(body, tail);
    printf("%s %s\n", body, tail);
}