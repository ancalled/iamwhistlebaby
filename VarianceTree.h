//
// Created by Andrey Koltochnik on 1/11/17.
//

#ifndef WHISTLE_MESSAGE_H
#define WHISTLE_MESSAGE_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;


struct Content {
    char symbol;
    float probability;
    float cumulProbability;

    void calcCumulProbability(float parentProb, int parentDepth) {
        cumulProbability = (parentProb * parentDepth + probability) / (parentDepth + 1);
    }
};


class VarianceTree {

public:

    struct Node {
        Content content;
        vector<Node *> children;
        Node* parent;
        int depth;
    };

    struct Branch {
        std::string body;
        float probability;

        std::string reversed() {
            std::string copy(body);
            std::reverse(copy.begin(), copy.end());
            return copy;
        }
    };

    VarianceTree(int capacity = 0);

    ~VarianceTree();

    void nextTier(vector<Content> items);

    void addTiers(vector<vector<Content>> tiers);

    string crcMatched();

    Branch getTopBranch();

    vector<Branch> getTopBranches(int size);


    void printBranches(bool withCrc = false);


    void clear();

    int size();

private:

    vector<Branch> getBranches();

    void addItems(Node *node, vector<Content> items);

    void destroyNode(Node *node);

    void destroyLeaf(Node *leaf);

    vector<Branch> getBranches(Node *node);

    vector<Node *> getLeafs(Node *node);

    void cleanUp(int restSize);

    static bool compareContents(const Content &c1, const Content &c2) {
        return c1.probability > c2.probability;
    }

    static bool compareBranches(const Branch &l1, const Branch &l2) {
        return l1.probability > l2.probability;
    }

    static bool compareNodes(const Node *n1, const Node *n2) {
        return n1->content.cumulProbability > n2->content.cumulProbability;
    }



    Node *root;
    int depth;
    int capacity;

};


#endif //WHISTLE_MESSAGE_H
