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

    Node *root;
    int depth;

};


#endif //WHISTLE_MESSAGE_H
