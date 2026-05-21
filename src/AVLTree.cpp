#include <bits/stdc++.h>
#include "AVLTree.h"
using namespace std;

AVLTree::AVLTree() : root(nullptr) {}

// 获取树的高度
int AVLTree::getHeight(TreeNode *node)
{
    return node ? node->height : 0;
}

// 获取当前节点的平衡值
int AVLTree::getBalance(TreeNode *node)
{
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// 更新树的高度
void AVLTree::updateHeight(TreeNode *node)
{
    if (node)
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
}

// 右旋，LL型
TreeNode *AVLTree::rotateRight(TreeNode *root)
{
    TreeNode *temp = root->left;
    root->left = temp->right;
    temp->right = root;

    updateHeight(root);
    updateHeight(temp);

    return temp;
}

// 左旋，RR型
TreeNode *AVLTree::rotateLeft(TreeNode *root)
{
    TreeNode *temp = root->right;
    root->right = temp->left;
    temp->left = root;

    updateHeight(root);
    updateHeight(temp);

    return temp;
}

// 左右旋，LR型
TreeNode *AVLTree::rotateLeftRight(TreeNode *root)
{
    root->left = rotateLeft(root->left);
    return rotateRight(root);
}

// 右左旋，RL型
TreeNode *AVLTree::rotateRightLeft(TreeNode *root)
{
    root->right = rotateRight(root->right);
    return rotateLeft(root);
}

// 插入节点
TreeNode *AVLTree::insert(TreeNode *root, SaleRecord record)
{
    if (!root)
        return new TreeNode(record);
    if (record.getId() < root->data.getId())
    {
        root->left = insert(root->left, record);
    }
    else if (record.getId() > root->data.getId())
    {
        root->right = insert(root->right, record);
    }
    else
    {
        return root;
    }

    updateHeight(root);
    int balance = getBalance(root);

    if (balance > 1 && record.getId() < root->left->data.getId())
        return rotateRight(root);
    if (balance < -1 && record.getId() > root->right->data.getId())
        return rotateLeft(root);
    if (balance > 1 && record.getId() > root->left->data.getId())
        return rotateLeftRight(root);
    if (balance < -1 && record.getId() < root->right->data.getId())
        return rotateRightLeft(root);

    return root;
}

// 移除节点
TreeNode *AVLTree::remove(TreeNode *root, string id)
{
    if (root == nullptr)
    {
        return nullptr;
    }
    else if (id < root->data.getId())
    {
        root->left = remove(root->left, id);
    }
    else if (id > root->data.getId())
    {
        root->right = remove(root->right, id);
    }
    else
    {
        // 如果非同时拥有两个子节点则直接删除
        if ((root->left == nullptr) || (root->right == nullptr))
        {
            TreeNode *temp = root->left ? root->left : root->right;
            TreeNode *toDelete = root;
            root = temp;
            delete toDelete;
        }
        // 若有两个子节点需要将此节点的值更新为以右孩子节点为根的最小值，然后删除那个节点
        else
        {
            TreeNode *temp = findMin(root->right);
            root->data = temp->data;
            root->right = remove(root->right, temp->data.getId());
        }
    }

    if (root == nullptr)
        return nullptr;

    // 删除节点后需要更新树的信息，可能存在旋转操作
    updateHeight(root);

    int balance = getBalance(root);

    if (balance > 1)
    {
        if (getBalance(root->left) >= 0)
        {
            root = rotateRight(root);
        }
        else
        {
            root = rotateLeftRight(root);
        }
    }
    else if (balance < -1)
    {
        if (getBalance(root->right) <= 0)
        {
            root = rotateLeft(root);
        }
        else
        {
            root = rotateRightLeft(root);
        }
    }
    return root;
}

// 查找最小节点
TreeNode *AVLTree::findMin(TreeNode *root)
{
    while (root->left)
    {
        root = root->left;
    }
    return root;
}

// 中序遍历
void AVLTree::inOrder(TreeNode *root, vector<SaleRecord> &records)
{
    if (root)
    {
        inOrder(root->left, records);
        records.push_back(root->data);
        inOrder(root->right, records);
    }
}

// 添加记录
bool AVLTree::addRecord(SaleRecord record)
{
    if (searchRecord(record.getId()) != nullptr)
        return false;
    root = insert(root, record);
    return true;
}

// 移除记录
bool AVLTree::removeRecord(string id)
{
    if (searchRecord(id) == nullptr)
        return false;
    root = remove(root, id);
    return true;
}

// 查找记录
TreeNode *AVLTree::searchRecord(string id)
{
    TreeNode *current = root;
    while (current)
    {
        if (id == current->data.getId())
        {
            return current;
        }
        else if (id < current->data.getId())
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return nullptr; // 未找到
}

// 获取所有记录
void AVLTree::getAllRecords(vector<SaleRecord> &records)
{
    inOrder(root, records);
}
