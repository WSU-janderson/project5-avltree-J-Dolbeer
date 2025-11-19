/**
 * AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H
#include <string>
#include <optional>
#include <vector>


using namespace std;

class AVLTree {
public:
    using KeyType = std::string;
    using ValueType = size_t;

protected:
    class AVLNode {
    public:
        KeyType key;
        ValueType value;
        size_t height;

        AVLNode* left;
        AVLNode* right;

        // 0, 1 or 2
        size_t numChildren() const;
        // true or false
        bool isLeaf() const;
        // number of hops to deepest leaf node
        size_t getHeight() const;


    };

public:

bool AVLTree::insert(const std::string& key, size_t value);
bool AVLTree::remove(const std::string& key);
bool AVLTree::contains(const std::string& key) const;
std::optional<size_t> AVLTree::get(const std::string& key) const;
size_t& AVLTree::operator[](const std::string& key);
vector<std::string> AVLTree::findRange(const std::string& lowKey, const std::string& highKey) const;
std::vector<std::string> AVLTree::keys() const;
size_t AVLTree::size() const;
size_t AVLTree::getHeight() const;
AVLTree::AVLTree(const AVLTree& other);
void AVLTree::operator=(const AVLTree& other);
AVLTree::~AVLTree();
friend std::ostream& operator<<(ostream& os, const AVLTree & avlTree);

    private:
    AVLNode* root;

    /* Helper methods for remove */
    // this overloaded remove will do the recursion to remove the node
    bool remove(AVLNode*& current, KeyType key);
    // removeNode contains the logic for actually removing a node based on the numebr of children
    bool removeNode(AVLNode*& current);
    // You will implement this, but it is needed for removeNode()
    void balanceNode(AVLNode*& node);

    size_t nodeCounter = 0;
    bool AVLTree::insert(AVLNode*& current, const std::string& key, size_t value);
    bool AVLTree::contains(AVLNode* current, const std::string& key) const;
    std::optional<size_t> AVLTree::get(AVLNode* current, const std::string& key) const;
    size_t& AVLTree::operatorHelper(AVLNode*& current, const std::string& key);
    vector<std::string> AVLTree::findRange(AVLNode* current, const std::string& lowKey, const std::string& highKey) const;
    void AVLTree::keys(AVLNode* current, std::vector<std::string>& keyList) const;
};

#endif //AVLTREE_H
