//
// Created by Andrey Koltochnik on 1/11/17.
//

#include "VarianceTree.h"
#include "CRC.h"

VarianceTree::VarianceTree(int capacity) : capacity(capacity) {
    depth = 0;
    root = new Node;
    root->depth = depth++;
    root->content.symbol = '\0';
    root->content.probability = 1.0;
}

VarianceTree::~VarianceTree() {
    destroyNode(root);
}


void VarianceTree::destroyNode(VarianceTree::Node *node) {
    if (node != NULL) {
        vector<Node *>::iterator it = node->children.begin();
        while (it != node->children.end()) {
            Node *child = *it;
            destroyNode(child);
            it = node->children.erase(it);
        }
//        for (Node *child: node->children) {
//            destroyNode(child);
//        }
        depth = node->depth;
        delete node;
    }
}


void VarianceTree::nextTier(std::vector<Content> items) {
    if (!items.empty()) {
//        sort(items.begin(), items.end(), sortContentByProb);
        addItems(root, items);
        depth++;
        if (capacity > 0) {
//            cleanUp(capacity);
        }
    }
}


void VarianceTree::addItems(Node *node, vector<Content> items) {
    if (node->children.empty()) {
        if (node->depth == depth - 1) {
            for (Content &t: items) {
                Node *newLeaf = new Node;
                newLeaf->content = t;
                newLeaf->depth = depth;
                newLeaf->parent = node;
                if (node != root) {
                    newLeaf->content.calcCumulProbability(node->content.cumulProbability, node->depth);
                } else {
                    newLeaf->content.cumulProbability = t.probability;
                }
                node->children.push_back(newLeaf);
            }
        }
    } else {
        try {
            for (Node *child: node->children) {
                addItems(child, items);

            }
        } catch (std::exception& e) {
            printf("Exception in VarianceTree!\n");
        };
    }
}


vector<VarianceTree::Branch> VarianceTree::getBranches() {
    vector<Branch> res = getBranches(root);
    sort(res.begin(), res.end(), compareBranches);
    return res;
}


vector<VarianceTree::Branch> VarianceTree::getBranches(VarianceTree::Node *node) {
    if (node->children.empty()) {
        if (node->depth == depth - 1) {
            Branch newLine = {"", node->content.cumulProbability};
            vector<Branch> result;
            result.push_back(newLine);
            return result;
        } else {
            return vector<Branch>();
        }
    } else {
        vector<Branch> allRes;
        for (Node *child: node->children) {
            vector<Branch> result = getBranches(child);
            for (Branch &line: result) {
                line.body += child->content.symbol;
                allRes.push_back(line);
            }
        }
        return allRes;
    }
}

void VarianceTree::printBranches() {
    vector<Branch> result = getBranches();
    int num = 1;
    for (Branch &branch: result) {
        const string &str = branch.reversed();
        printf("%d\t%s\t%.4f\n", num++, str.c_str(), branch.probability);
    }
    printf("\n");

}

vector<VarianceTree::Node *> VarianceTree::getLeafs(VarianceTree::Node *node) {
    if (node->children.empty()) {
        vector<Node *> result;
        result.push_back(node);
        return result;
    } else {
        vector<Node *> allRes;
        for (Node *child: node->children) {
            vector<Node *> result = getLeafs(child);
            allRes.insert(allRes.end(), result.begin(), result.end());
        }
        return allRes;
    }
}


void VarianceTree::cleanUp(int restSize) {
    vector<Node *> leafs = getLeafs(root);
    sort(leafs.begin(), leafs.end(), compareNodes);

    for (long i = leafs.size() - 1; i >= restSize; i--) {
        destroyLeaf(leafs[i]);
    }
}

void VarianceTree::destroyLeaf(VarianceTree::Node *leaf) {
    if (leaf->parent != root) {
        vector<Node *> &vc = leaf->parent->children;
        vc.erase(std::remove(vc.begin(), vc.end(), leaf), vc.end());
        if (vc.empty()) {
            destroyLeaf(leaf->parent);
        }
    }
    delete leaf;
}

VarianceTree::Branch VarianceTree::getTopBranch() {
    const vector<Branch> &branches = getBranches();
    return branches.front();
}

vector<VarianceTree::Branch> VarianceTree::getTopBranches(int size) {
    const vector<Branch> &branches = getBranches();
    int len = size < branches.size() ? size : branches.size();
    vector<Branch> filter(branches.begin(), branches.begin() + len);
    return filter;
}

void VarianceTree::clear() {
    vector<Node *>::iterator it = root->children.begin();
    while (it != root->children.end()) {
        Node *child = *it;
        destroyNode(child);
        it = root->children.erase(it);
    }
//    for (Node *nd: root->children) {
//        destroyNode(nd);
//    }
//    depth = 0;
}

string VarianceTree::getMessage() {
    int tailSize = 2;
    const vector<Branch> &branches = getBranches();
    for (Branch b : branches) {
        string candidate = b.reversed();
        string body = candidate.substr(0, candidate.size() - tailSize);
        string tail = candidate.substr(candidate.size() - tailSize, tailSize);
        char gen[tailSize];
        generateTail(body.c_str(), gen, tailSize);
        if (strcmp(gen, tail.c_str()) == 0) {
            return candidate;
        }
    }

    return std::__cxx11::string();
}

void VarianceTree::addTiers(vector<vector<Content>> tiers) {
    for (vector<Content> &items: tiers) {
        nextTier(items);
    }
}



