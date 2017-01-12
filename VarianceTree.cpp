//
// Created by Andrey Koltochnik on 1/11/17.
//

#include <algorithm>
#include "VarianceTree.h"

VarianceTree::VarianceTree() {
    depth = 0;
    root = new Node;
    root->depth = depth++;
    root->content.symbol = '\0';
    root->content.probability = 1.0;
}

VarianceTree::~VarianceTree() {
    destroyNode(root);
}


void VarianceTree::destroyNode(VarianceTree::Node *leaf) {
    if (leaf != NULL) {
        for (Node *child: leaf->leafs) {
            destroyNode(child);
        }
        delete leaf;
    }
}


void VarianceTree::addElements(std::vector<Content> els) {
    if (!els.empty()) {
        addElement(root, els);
        depth++;
    }
}


void VarianceTree::addElement(Node *leaf, vector<Content> items) {
    if (leaf->leafs.empty()) {
        for (Content &t: items) {
            Node *newLeaf = new Node;
            newLeaf->content = t;
            newLeaf->depth = depth;
            leaf->leafs.push_back(newLeaf);
        }

    } else {
        for (Node *child: leaf->leafs) {
            addElement(child, items);
        }
    }
}

vector<Line> VarianceTree::allVariances() {
    vector<Line> res = harvestContent(root);
    sort(res.begin(), res.end(), sortByProb);
    return res;
}

vector<Line> VarianceTree::harvestContent(VarianceTree::Node *leaf) {
    if (leaf->leafs.empty()) {
        Line newLine = {"", 0};
        vector<Line> result;
        result.push_back(newLine);
        return result;
    } else {
        vector<Line> allRes;
        for (Node *child: leaf->leafs) {
            vector<Line> result = harvestContent(child);
            for (Line &line: result) {
                line.mes += child->content.symbol;
                line.prob += child->content.probability; //todo avg
                allRes.push_back(line);
            }
        }
        return allRes;
    }
}


