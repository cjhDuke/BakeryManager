#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <bits/stdc++.h>
using namespace std;

class Product
{
public:
    Product() : stock(0) {}
    Product(string id, string name, string make, string valid, int s)
        : productId(id), productName(name), makeDate(make), validDate(valid), stock(s) {}

    string getId() const { return productId; }
    void setId(const string &id) { productId = id; }

    string getName() const { return productName; }
    void setName(const string &name) { productName = name; }

    string getMakeDate() const { return makeDate; }
    void setMakeDate(const string &date) { makeDate = date; }

    string getValidDate() const { return validDate; }
    void setValidDate(const string &date) { validDate = date; }

    int getStock() const { return stock; }
    void setStock(int s) { stock = s; }

private:
    string productId;
    string productName;
    string makeDate;
    string validDate;
    int stock;
};

#endif