//
// Created by Andrey Koltochnik on 1/11/17.
//

#include "VarianceTree.h"
#include "CRC32.h"
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>


int main() {

    VarianceTree mes;

    std::vector<Content> lvl1;
    lvl1.push_back({'h', 0.9});
    lvl1.push_back({'k', 0.5});
    mes.nextTier(lvl1);

    std::vector<Content> lvl2;
    lvl2.push_back({'j', 0.85});
    mes.nextTier(lvl2);

    std::vector<Content> lvl3;
    lvl3.push_back({'3', 0.9});
    lvl3.push_back({'e', 0.7});
    mes.nextTier(lvl3);

    std::vector<Content> lvl4;
    lvl4.push_back({'d', 0.75});
    lvl4.push_back({'a', 0.43});
    mes.nextTier(lvl4);

    mes.printBranches();

    printf("");
//    const unsigned char *message = reinterpret_cast<const unsigned char *> ("hjntdb982ilj6etj6e3k");
//    uint32_t  c = crc32b(message);
//    printf("%d\n", c);//1053339487

}