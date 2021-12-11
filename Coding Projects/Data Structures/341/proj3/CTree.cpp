//CTree.cpp file
//Hafsa Chaudhry

#include "CTree.h"
#include <iostream>
#include <cstring>
#include <string>
#include <math.h>
#include <algorithm>
using namespace std;

//default constructor
CTree::CTree(){
    m_root = NULL;
}

//copy constructor
CTree::CTree(const CTree& other){
    m_root = copyNewTree(other.m_root);
}

//copy constructor helper
Node* CTree::copyNewTree(const Node* newTree){
    if(newTree == NULL){
        return NULL;

    }
    else{
        Node* tree;
        tree = new Node(newTree->m_data, NULL, NULL);
        tree->m_height = newTree->m_height;
        tree->m_size = newTree->m_size;
        tree->m_right = copyNewTree(newTree->m_right);
        tree->m_left = copyNewTree(newTree->m_left);
        return tree;
    }
}

//destructor
CTree::~CTree(){
    emptyOut();
}

//gets height
int CTree::updateHeight(){
    return updateHeight(m_root);
}

//gets height helper
int CTree::updateHeight(Node* node){
    if(node != NULL){
        return node->m_height;
    }
    else
        return -1;
}

//increases height
int CTree::IncreaseHeight(Node* node){
    return node->m_height = max(updateHeight(node->m_left), updateHeight(node->m_right))+1;
}

//get size
int CTree::getSize(){
    return getSize(m_root);
}

//get size helper
int CTree::getSize(Node* tree){
    if(tree != NULL){
        return tree->m_size;
    }
    else
        return 0;
}

//increases size
int CTree::IncreaseSize(Node* subtree){
    return subtree->m_size = (getSize(subtree->m_left) + getSize(subtree->m_right))+1;
}

//get's largest node (could either replace with this or replace with min node and right)
Node* CTree::maxNode(Node* node){
    if (node == NULL){
        return NULL;
    }
    else if(node->m_left == NULL){
        return node;
    }
    else{
        return maxNode(node->m_left);
    }
}

//makes the tree empty
void CTree::emptyOut(){
    emptyOut(m_root);
    m_root = NULL;
}

//empty helper function
void CTree::emptyOut(Node* &tree){
    if(tree != NULL){
        emptyOut(tree->m_left);
        emptyOut(tree->m_right);
        delete tree;
    }
}

//overloaded assignment operator
const CTree& CTree::operator=(const CTree& rhs){
    if(this != &rhs){
        emptyOut();
        m_root = rhs.m_root;
    }
    return *this;
}

//insert function
void CTree::insert(int key){
    m_root = insert(key, m_root);
}

//insert helper function
Node* CTree::insert(int key, Node* node){
    //(value:height:size)
    if(find(key) == false){
        if(node == NULL){
            node = new Node(key, NULL, NULL);
            return node;
        }
        else{
            rebalance();
        }
        if(key < node->m_data){
            node->m_left = insert(key, node->m_left);
            rebalance();
        }
        if(key > node->m_data){
            node->m_right = insert(key, node->m_right);
            rebalance();
        }
        IncreaseHeight(node);
        IncreaseSize(node);
    }
    return node;
}

//number of descendants
int CTree::numbOfDescendants(Node * root){
    int sum = 0;
    if(root== NULL) return 0;
    sum = sum+numbOfDescendants(root->m_left);
    sum = sum+numbOfDescendants(root->m_right);
    return ++sum;
}

//finds and removes item from given key value
bool CTree::remove(int key){
    if(remove(key, m_root)){
        return true;
    }
    return false;
}

//remove helper function
Node* CTree::remove(int key, Node* node){
    Node *removeN;
    if(node != NULL){
        if(key < node->m_data){
            node->m_left = remove(key, node->m_left);
            node->m_size = node->m_size-1;
        }
        else if(key > node->m_data){
            node->m_right = remove(key, node->m_right);
            node->m_size = node->m_size-1;
        }
        else if(node->m_left != NULL and node->m_right != NULL){
            //replace node with max left or min right
            removeN = maxNode(node->m_right);
            node->m_data = removeN->m_data;
            node->m_right= remove(node->m_data, node->m_right);
            node->m_size = node->m_size-1;
        }

        else{
            Node* previous = node;
            if(node->m_left != NULL)
                node = node->m_left;
            else
                node = node->m_right;
            delete previous;
        }
    }
    return node;
}

//find that figures out if given key is in the tree
bool CTree::find(int key){
    return find(key, m_root);
}

//find helper function
bool CTree::find(int key, Node* node){
    if(node == NULL){
        return false;
    }
    if(key == node->m_data){
        return true;
    }else if(key< node->m_data){
        return find(key, node->m_left);
    }else if(key > node->m_data){
        return find(key, node->m_right);
    }
    return true;
}

//rebalance function
void CTree::rebalance(){
    rebalance(m_root, m_root);
}

//rebalance helper function
void CTree::rebalance(Node* node, Node* parent){
    if(node != NULL){
        int diff = numbOfDescendants(node->m_left) - numbOfDescendants(node->m_right);
        if(diff < -2){
            rebalanceTree = new int[node->m_size];
            fillArray(node, rebalanceTree);
            int size = node->m_size;
            if(node == m_root){
                m_root = rebalance(rebalanceTree, 0, size-1);
            }
            else if(parent->m_right == node){
                parent->m_right = rebalance(rebalanceTree, 0 , size-1);
                m_count = 0;
                delete [] rebalanceTree;
                rebalanceTree = NULL;
            }
        }
        else if(diff > 2){
            rebalanceTree = new int[node->m_size];
            fillArray(node, rebalanceTree);
            int size = node->m_size;
            if(node == m_root){
                m_root = rebalance(rebalanceTree, 0, size-1);
            }
            else if(parent->m_left == node){
                parent->m_left = rebalance(rebalanceTree, 0, size-1);
                m_count = 0;
                delete [] rebalanceTree;
                rebalanceTree = NULL;
            }
        }
    }

    if(node == NULL)
        return;
    rebalance(node->m_left, node);
    rebalance(node->m_right, node);
}

//does the internal rebalancing algorithm here
Node* CTree::rebalance(int* rebalTree, int count, int size){
    int middle = (size + count) /2;
    if(count < size){
        int tempTree = rebalTree[middle];

        Node* newTree;
        newTree = new Node(tempTree, NULL, NULL);
        int End = middle - 1;
        int	Start = middle + 1;
        newTree->m_left = rebalance(rebalTree, count, End);
        newTree->m_right = rebalance(rebalTree, Start, size);
        newTree->m_size = getSize();//IncreaseSize(newTemp);
        newTree->m_height = IncreaseHeight(newTree);
        return newTree;
    }
    return NULL;
}

//creates an in ordered sorted array
void CTree::fillArray(Node* node, int inorderArr[]){
    if(node != NULL){
        fillArray(node->m_left, inorderArr);
        inorderArr[m_count] = node->m_data;
        m_count++;
        fillArray(node->m_right, inorderArr);
    }
    return;
}

//inorder array
void CTree::inorder(){
    inorder(m_root);
}

//inorder helper function, prints out inorder tree
void CTree::inorder(Node* node){
    if(node != NULL){
        cout << "(";
        inorder(node->m_left);
        cout << (node->m_data) << ":" << (node->m_height) << ":" << (node->m_size);
        inorder(node->m_right);
        cout << ")";
    }
}

//figures out if there is a node in the position given
bool CTree::locate(const char *position, int& key){
    if(locate(position, key, m_root)){
        return true;
    }else{
        return false;
    }
}

//locate helper function
bool CTree::locate(const char *position, int&key, Node* node){
    string here = position;
    if(node != NULL){
        if(here.length() == 0){
            key = node->m_data;
            return true;
        }else if(here.at(0) == 'L'){
            here = here.substr(1, here.length()-1);
            position = here.c_str();
            return locate(position, key, node->m_left);
        }else if(here.at(0) == 'R'){
            here = here.substr(1, here.length()-1);
            position = here.c_str();
            return locate(position, key, node->m_right);
        }else
            return false;
    }
    return true;
}
