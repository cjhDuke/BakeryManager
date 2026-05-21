#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

class AVLTree
{
public:
    AVLTree(); // 初始化一棵空的销售记录 AVL 树
    bool addRecord(SaleRecord record); // 添加一条销售记录，记录号重复时返回 false
    bool removeRecord(string id); // 按记录号删除销售记录，未找到时返回 false
    TreeNode *searchRecord(string id); // 按记录号查找销售记录节点
    void getAllRecords(vector<SaleRecord> &records); // 中序遍历获取全部销售记录

private:
    TreeNode *root;
    int getHeight(TreeNode *root); // 获取指定节点高度，空节点高度为 0
    int getBalance(TreeNode *root); // 计算指定节点的平衡因子
    void updateHeight(TreeNode *root); // 根据左右子树高度更新当前节点高度
    TreeNode *rotateRight(TreeNode *root); // 对 LL 型失衡执行右旋
    TreeNode *rotateLeft(TreeNode *root); // 对 RR 型失衡执行左旋
    TreeNode *rotateLeftRight(TreeNode *root); // 对 LR 型失衡执行先左旋后右旋
    TreeNode *rotateRightLeft(TreeNode *root); // 对 RL 型失衡执行先右旋后左旋
    TreeNode *insert(TreeNode *root, SaleRecord record); // 递归插入销售记录并维护 AVL 平衡
    TreeNode *remove(TreeNode *root, string id); // 递归删除销售记录并维护 AVL 平衡
    TreeNode *findMin(TreeNode *root); // 查找子树中记录号最小的节点
    void inOrder(TreeNode *root, vector<SaleRecord> &records); // 中序遍历 AVL 树并收集销售记录
};

#include "AVLTree.cpp"

#endif
