#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

class AVLTree
{
public:
    AVLTree();
    bool addRecord(SaleRecord record);
    bool removeRecord(string id);
    TreeNode *searchRecord(string id);
    void getAllRecords(vector<SaleRecord> &records);

private:
    TreeNode *root;
    int getHeight(TreeNode *root);
    int getBalance(TreeNode *root);
    void updateHeight(TreeNode *root);
    TreeNode *rotateRight(TreeNode *root);
    TreeNode *rotateLeft(TreeNode *root);
    TreeNode *rotateLeftRight(TreeNode *root);
    TreeNode *rotateRightLeft(TreeNode *root);
    TreeNode *insert(TreeNode *root, SaleRecord record);
    TreeNode *remove(TreeNode *root, string id);
    TreeNode *findMin(TreeNode *root);
    void inOrder(TreeNode *root, vector<SaleRecord> &records);
};

#include "AVLTree.cpp"

#endif
