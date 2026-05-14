#ifndef PRODUCTLIST_HPP
#define PRODUCTLIST_HPP

#include <bits/stdc++.h>
#include "ProductNode.h"
using namespace std;

class ProductList
{
public:
    ProductList();
    bool addProduct(Product p);
    bool removeProduct(string id);
    bool findProduct(string id, Product &p);
    void getAllProducts(vector<Product> &products);
    // void displayProducts();

private:
    ProductNode *head;
    int size;
};

#include "ProductList.cpp"

#endif
