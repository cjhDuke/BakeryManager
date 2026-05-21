#ifndef PRODUCTLIST_HPP
#define PRODUCTLIST_HPP

#include <bits/stdc++.h>
#include "ProductNode.h"
using namespace std;

class ProductList
{
public:
    ProductList(); // 初始化空商品链表
    bool addProduct(Product p); // 添加商品，商品号重复时返回 false
    bool removeProduct(string id); // 按商品号删除商品，未找到时返回 false
    bool findProduct(string id, Product &p); // 按商品号查找商品并通过参数返回
    void getAllProducts(vector<Product> &products); // 遍历链表获取全部商品
    // void displayProducts();

private:
    ProductNode *head;
    int size;
};

#include "ProductList.cpp"

#endif
