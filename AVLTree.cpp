#include "AVLTree.h"

#include <string>
#include <iostream>
using std::ostream;

size_t AVLTree::AVLNode::numChildren() const {
    size_t childrenCounter = 0;
    if (left) {
        childrenCounter++;
    }
    if (right) {
        childrenCounter++;
    }
    return childrenCounter;
}

bool AVLTree::AVLNode::isLeaf() const {
    bool hasLeftChild = (left != nullptr);
    bool hasRightChild = (right != nullptr);
    if (!hasLeftChild && !hasRightChild) {
        return true;
    } else {
        return false;
    }
}

size_t AVLTree::AVLNode::getHeight() const {
    size_t leftHeight;
    size_t rightHeight;
    if (left != nullptr) {
        leftHeight = left->getHeight();
    } else {
        leftHeight = 0;
    }
    if (right != nullptr) {
        rightHeight = right->getHeight();
    } else {
        rightHeight = 0;
    }
    if (leftHeight > rightHeight) {
        return leftHeight + 1;
    } else {
        return rightHeight + 1;
    }
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

void AVLTree::balanceNode(AVLNode *&node) {
    if (!node) {
        return;
    }
    node->height = node->getHeight();

    int balanceFactor = 0;
    int leftHeight = 0;
    int rightHeight = 0;
    if (node->left) {
        leftHeight = node->left->getHeight();
    } else {
        leftHeight = 0;
    }

    if (node->right) {
        rightHeight = node->right->getHeight();
    } else {
        rightHeight = 0;
    }
    balanceFactor = leftHeight - rightHeight;

    if (balanceFactor > 1) {
        int leftLeftHeight = 0;
        int leftRightHeight = 0;
        if (node->left->left) {
            leftLeftHeight = node->left->left->getHeight();
        } else {
            leftLeftHeight = 0;
        }
        if (node->left->right) {
            leftRightHeight = node->left->right->getHeight();
        } else {
            leftRightHeight = 0;
        }
        int leftBalance = leftLeftHeight - leftRightHeight; 

        if (leftBalance >= 0) {
            rotateRight(node);
        } else {
            rotateLeft(node->left);
            rotateRight(node);
        }
    } else if (balanceFactor < -1) {
        int rightLeftHeight = 0;
        int rightRightHeight = 0;

        if (node->right->left) {
            rightLeftHeight = node->right->left->getHeight();
        } else {
            rightLeftHeight = 0;
        }
        if (node->right->right) {
            rightRightHeight = node->right->right->getHeight();
        } else {
            rightRightHeight = 0;
        }
        int rightBalance = rightLeftHeight - rightRightHeight;

        if (rightBalance <= 0) {
            rotateLeft(node);
        } else {
            rotateRight(node->right);
            rotateLeft(node);
        }
    }

    node->height = node->getHeight();
}

void AVLTree::rotateLeft(AVLNode*& node) {
    if (!node || !node->right) {
        return;
    }
    AVLNode* newRoot = node->right;
    AVLNode* newNewRoot = newRoot->left;
    newRoot->left = node;
    node->right = newNewRoot;
    node = newRoot;
    root->right->height = root->right->getHeight();
    root->height = root->getHeight();
}

void AVLTree::rotateRight(AVLNode*& node) {
    if (!node || !node->left) {
        return;
    }
    AVLNode* newRoot = node->left;
    AVLNode* newNewRoot = newRoot->right;
    newRoot->right = node;
    node->left = newNewRoot;
    node = newRoot;
    root->left->height = root->left->getHeight();
    root->height = root->getHeight();
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
        nodeCounter++;//Add node to nodeCounter
        if (root) {//If root exists, update heightCounter
            heightCounter = root->height;
        } else {
            heightCounter = 0;
        }
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
        nodeCounter--; //Remove node from nodeCounter
        if (root) {//If root exists, update heightCounter
            heightCounter = root->height;
        } else {
            heightCounter = 0;
        }
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

size_t AVLTree::getHeight() const {
    return heightCounter;
}

AVLTree::AVLTree(const AVLTree& other) {
    root = deepCopy(other.root);
    nodeCounter = other.nodeCounter;
    heightCounter = other.heightCounter;
}

AVLTree::AVLNode* AVLTree::deepCopy(AVLNode* current) {
    if (!current) {
        return nullptr;
    }
    AVLNode* newNode = new AVLNode{current->key, current->value, current->height, nullptr, nullptr};
    newNode->left = deepCopy(current->left);
    newNode->right = deepCopy(current->right);
    return newNode;
}

void AVLTree::operator=(const AVLTree& other) {
    if (this == &other) {
        return;
    }
    erase(root);
    root = deepCopy(other.root);
    nodeCounter = other.nodeCounter;
    heightCounter = other.heightCounter;
}

void AVLTree::erase(AVLNode* current) {
    if (!current) {
        return;
    }
    erase(current->left);
    erase(current->right);
    delete current;
}

AVLTree::~AVLTree() {
    erase(root);
}

std::ostream& operator<<(ostream& os, const AVLTree & avlTree) {
    avlTree.printTree(os, avlTree.root, 0);
    return os;
}

void AVLTree::printTree(std::ostream& os, AVLNode* current, int depth) const {
    if (!current) {
        return;
    }
    printTree(os, current->right, depth + 1);
    for (int i = 0; i < depth; i++) {
        os << "   ";
    }
    os << "{" << current->key << ": " << current->value << "}\n";
    printTree(os, current->left, depth + 1);
}