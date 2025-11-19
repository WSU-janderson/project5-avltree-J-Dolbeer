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

bool AVLTree::insert(AVLNode*& current, const std::string& key, size_t value) {
    //If node does not exist, create it
    if (!current) {
        current = new AVLNode{key, value, 1, nullptr, nullptr};
        return true;
    }

    bool inserted = false;
    if (key < current->key) { //Create left child
        inserted = insert(current->left, key, value);
    } else if (key > current->key) { //Create right child
        inserted = insert(current->right, key, value);
    } else { //Node already exists
        return false;
    }

    if (inserted) {//Get height and balance tree
        current->height = current->getHeight();
        balanceNode(current);
        nodeCounter++;
    }

    return inserted;
}

bool AVLTree::remove(const std::string& key) {
    return remove(root, key);
}

bool AVLTree::remove(AVLNode*& current, KeyType key) {
    //If node does not exist, return false
    if (!current) {
        return false;
    }

    bool removed = false;
    if (key < current->key) {//Go to left
        removed = remove(current->left, key);
    } else if (key > current->key) {//Go to right
        removed = remove(current->right, key);
    } else {//Remove current node
        removed = removeNode(current);
    }

    if (removed) {//Get height and balance tree
        current->height = current->getHeight();
        balanceNode(current);
        nodeCounter--;
    }

    return removed;
}

bool AVLTree::contains(const std::string& key) const {
    return contains(root, key);
}

bool AVLTree::contains(AVLNode* current, const std::string& key) const {
    if (!current) {//If not found, return false
        return false;
    }
    if (key == current->key) {//Return current node
        return true;
    } else if (key < current->key) {//Go left
        return contains(current->left, key);
    } else if (key > current-> key){//Go right
        return contains(current->right, key);
    }
}

std::optional<size_t> AVLTree::get(const std::string& key) const {
    return get(root, key);
}

std::optional<size_t> AVLTree::get(AVLNode* current, const std::string& key) const {
    if (!current) {//If not found, return std::nullopt
        return std::nullopt;
    }

    if (key == current->key) {//Return value of current node
        return current->value;
    } else if (key < current->key) {//Go left
        return get(current->left, key);
    } else if (key > current-> key) {//Go right
        return get(current->right, key);
    }
}

size_t& AVLTree::operator[](const std::string& key) {
    return operatorHelper(root, key);
}

size_t& AVLTree::operatorHelper(AVLNode*& current, const std::string& key) {
    if (key == current->key) {//Return value of current node
        return current->value;
    } else if (key < current->key) {//Go left
        return operatorHelper(current->left, key);
    } else if (key > current->key) {//Go right
        return operatorHelper(current->right, key);
    }
}

vector<std::string> AVLTree::findRange(const std::string& lowKey, const std::string& highKey) const {
    return findRange(root, lowKey, highKey);
}

vector<std::string> AVLTree::findRange(AVLNode* current, const std::string& lowKey, const std::string& highKey) const {
    vector<std::string> keysRange;
    if (!current) {//If node does not exist, return empty vector
        return keysRange;
    }

    if (current->key > lowKey) {//Go left
        vector<std::string> keysToLeft = findRange(current->left, lowKey, highKey);
        keysRange.insert(keysRange.end(), keysToLeft.begin(), keysToLeft.end());
    }

    if (current->key >= lowKey && current->key <= highKey) {//Add current key
        keysRange.push_back(current->key);
    }

    if (current->key < highKey) {//Go right
        vector<std::string> keysToRight = findRange(current->right, lowKey, highKey);
        keysRange.insert(keysRange.end(), keysToRight.begin(), keysToRight.end());
    }

    return keysRange;
}

std::vector<std::string> AVLTree::keys() const {
    std::vector<std::string> keyList;
    keys(root, keyList);
    return keyList;
}

void AVLTree::keys(AVLNode* current, std::vector<std::string>& keyList) const {
    if (!current) {
        return;
    }

    keys(current->left, keyList);
    keyList.push_back(current->key);
    keys(current->right, keyList);
}

size_t AVLTree::size() const {
    return nodeCounter;
}