#include "AVLTree.h"

#include <string>

size_t AVLTree::AVLNode::numChildren() const {
    return 0;
}

bool AVLTree::AVLNode::isLeaf() const {
    return false;
}

size_t AVLTree::AVLNode::getHeight() const {
    return 0;
}

bool AVLTree::removeNode(AVLNode*& current){
    if (!current) {
        return false;
    }

    AVLNode* toDelete = current;
    auto nChildren = current->numChildren();
    if (current->isLeaf()) {
        // case 1 we can delete the node
        current = nullptr;
    } else if (current->numChildren() == 1) {
        // case 2 - replace current with its only child
        if (current->right) {
            current = current->right;
        } else {
            current = current->left;
        }
    } else {
        // case 3 - we have two children,
        // get smallest key in right subtree by
        // getting right child and go left until left is null
        AVLNode* smallestInRight = current->right;
        // I could check if smallestInRight is null,
        // but it shouldn't be since the node has two children
        while (smallestInRight->left) {
            smallestInRight = smallestInRight->left;
        }
        std::string newKey = smallestInRight->key;
        int newValue = smallestInRight->value;
        remove(root, smallestInRight->key); // delete this one

        current->key = newKey;
        current->value = newValue;

        current->height = current->getHeight();
        balanceNode(current);

        return true; // we already deleted the one we needed to so return
    }
    delete toDelete;

    return true;
}

bool AVLTree::remove(AVLNode *&current, KeyType key) {
    return false;
}

void AVLTree::balanceNode(AVLNode *&node) {
}

bool AVLTree::insert(const std::string& key, size_t value) {
    return insert(root, key, value);
}

bool AVLTree::insert(AVLNode*& node, const std::string& key, size_t value) {
    //If node does not exist, create it
    if (!node) {
        node = new AVLNode{key, value, 1, nullptr, nullptr};
        return true;
    }

    bool inserted = false;
    if (key < node->key) { //Create left child
        inserted = insert(node->left, key, value);
    } else if (key > node->key) { //Create right child
        inserted = insert(node->right, key, value);
    } else { //Node already exists
        return false;
    }

    if (inserted) {//Get height and balance tree
        node->height = node->getHeight();
        balanceNode(node);
    }

    return inserted;
}