//
// Created by Andrey Koltochnik on 1/11/17.
//

#ifndef WHISTLE_MESSAGE_H
#define WHISTLE_MESSAGE_H

#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;


struct Content {
    char symbol;
    float probability;
};

struct Line {
    std::string mes;
    float prob;

    std::string reversed() {
        std::string copy(mes);
        std::reverse(copy.begin(), copy.end());
        return copy;
    }

    float confidence() {
        return mes.empty() ? 0 : prob / mes.size();
    }
};

class VarianceTree {

public:

    struct Node {
        Content content;
        vector<Node *> leafs;
        int depth;
    };

    VarianceTree();

    ~VarianceTree();


    void addElements(vector<Content> els);

    vector<Line> allVariances();

private:
    void addElement(Node *leaf, vector<Content> items);

    void destroyNode(Node *leaf);

    vector<Line> harvestContent(Node *leaf);

    static bool sortByProb(const Line &l1, const Line &l2) {
        return l1.prob > l2.prob;
    }

    Node *root;
    int depth;

};


#endif //WHISTLE_MESSAGE_H
