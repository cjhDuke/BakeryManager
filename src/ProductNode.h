#ifndef PRODUCTNODE_HPP
#define PRODUCTNODE_HPP

#include <bits/stdc++.h>
#include "Product.h"
using namespace std;

struct ProductNode
{
    Product data;
    ProductNode *next;
    ProductNode(Product p) : data(p), next(nullptr) {} // 用商品对象初始化链表节点
};

#endif
