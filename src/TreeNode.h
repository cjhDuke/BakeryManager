#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <bits/stdc++.h>
#include "SaleRecord.h"
using namespace std;

struct TreeNode
{
    SaleRecord data;
    int height;
    TreeNode *left;
    TreeNode *right;
    TreeNode(SaleRecord record) : data(record), height(1), left(nullptr), right(nullptr) {}
};

#endif