#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <bits/stdc++.h>
using namespace std;

class Product
{
public:
    Product() : stock(0) {} // 构造空商品，默认库存为 0
    Product(string id, string name, string make, string valid, int s)
        : productId(id), productName(name), makeDate(make), validDate(valid), stock(s) {} // 使用完整字段构造商品对象

    string getId() const { return productId; } // 获取商品号
    void setId(const string &id) { productId = id; } // 修改商品号

    string getName() const { return productName; } // 获取商品名称
    void setName(const string &name) { productName = name; } // 修改商品名称

    string getMakeDate() const { return makeDate; } // 获取制作日期
    void setMakeDate(const string &date) { makeDate = date; } // 修改制作日期

    string getValidDate() const { return validDate; } // 获取有效期
    void setValidDate(const string &date) { validDate = date; } // 修改有效期

    int getStock() const { return stock; } // 获取当前库存
    void setStock(int s) { stock = s; } // 修改当前库存

private:
    string productId;
    string productName;
    string makeDate;
    string validDate;
    int stock;
};

#endif
