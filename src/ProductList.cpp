#include <bits/stdc++.h>
#include "ProductList.h"
using namespace std;

ProductList::ProductList() : head(nullptr), size(0) {}

// 添加产品
bool ProductList::addProduct(Product p)
{
    ProductNode *newProduct = new ProductNode(p);
    if (!head)
    {
        head = newProduct;
        size++;
    }
    else
    {
        ProductNode *current = head;
        while (current->next)
        {
            if (current->data.getId() == p.getId())
            {
                delete newProduct; // 避免内存泄漏
                return false;      // 产品ID已存在
            }
            current = current->next;
        }
        if (current->data.getId() == p.getId())
        {
            delete newProduct; // 避免内存泄漏
            return false;      // 产品ID已存在
        }
        current->next = newProduct;
        size++;
    }
    return true;
}

// 移除产品
bool ProductList::removeProduct(string id)
{
    if (!head)
        return false;
    if (head->data.getId() == id)
    {
        ProductNode *toDelete = head;
        head = head->next;
        delete toDelete;
        size--;
        return true;
    }
    ProductNode *current = head;
    while (current->next)
    {
        if (current->next->data.getId() == id)
        {
            ProductNode *toDelete = current->next;
            current->next = current->next->next;
            delete toDelete;
            size--;
            return true;
        }
        current = current->next;
    }
    return false; // 未找到产品
}

// 获取所有商品
void ProductList::getAllProducts(vector<Product> &products)
{
    ProductNode *current = head;
    while (current)
    {
        products.push_back(current->data);
        current = current->next;
    }
}

// 查找产品
bool ProductList::findProduct(string id, Product &p)
{
    ProductNode *current = head;
    while (current)
    {
        if (current->data.getId() == id)
        {
            p = current->data;
            return true;
        }
        current = current->next;
    }
    return false; // 未找到产品
}
