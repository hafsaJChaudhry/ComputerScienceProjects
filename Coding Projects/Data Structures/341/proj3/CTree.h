//CTree.h file
//Hafsa Chaudhry

#ifndef CTREE_H
#define CTREE_H
#include <iostream>
#include <cstring>
#include <string>
#include <math.h>
using namespace std;

struct Node{
    int m_data;
    Node* m_left;
    Node* m_right;
    int m_height; //subtree height
    int m_size; //subtree size/number of nodes
    Node(int dataN, Node *leftN, Node *rightN, int height = 0, int size = 1): m_data(dataN), m_left(leftN), m_right(rightN), m_height(0), m_size(1){};
};

class CTree{
public:
    Node* m_root;
    CTree();
    CTree(const CTree& other);
    Node* copyNewTree(const Node* newTree);
    ~CTree();
    const CTree& operator=(const CTree& rhs);
    int updateHeight();
    int updateHeight(Node* node);
    int IncreaseHeight(Node* node);
    int getSize();
    int getSize(Node* tree);
    int IncreaseSize(Node* subtree);
    Node* maxNode(Node* node);
    void insert(int key);
    Node* insert(int key, Node* node);
    int numbOfDescendants(Node *root);
    bool remove(int key);
    Node* remove(int key, Node* node);
    bool find(int key);
    bool find(int key, Node* node);
    void rebalance();
    void rebalance(Node* node, Node* parent);
    Node* rebalance(int* rebalTree, int count, int size);
    void fillArray(Node* node, int inorderArr[]);
    void inorder();
    void inorder(Node* node);
    bool locate(const char *position, int& key);
    bool locate(const char *position, int&key, Node* node);
    void emptyOut();
    void emptyOut(Node* &tree);
private:
    int *rebalanceTree;
    int m_count = 0;
};

#endif

