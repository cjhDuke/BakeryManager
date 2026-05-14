#ifndef SALERECORD_HPP
#define SALERECORD_HPP

#include <bits/stdc++.h>
#include "Product.h"
using namespace std;

class SaleRecord
{
public:
    SaleRecord() : price(0), discount(0), quantity(0) {}
    SaleRecord(string id, Product p, string date, double pr, double dis, int qty)
        : recordId(id), product(p), saleDate(date), price(pr), discount(dis), quantity(qty) {}

    string getId() const { return recordId; }
    void setId(const string &id) { recordId = id; }

    Product getProduct() const { return product; }
    void setProduct(const Product &p) { product = p; }

    string getSaleDate() const { return saleDate; }
    void setSaleDate(const string &date) { saleDate = date; }

    double getPrice() const { return price; }
    void setPrice(double pr) { price = pr; }

    double getDiscount() const { return discount; }
    void setDiscount(double dis) { discount = dis; }

    int getQuantity() const { return quantity; }
    void setQuantity(int qty) { quantity = qty; }

private:
    string recordId;
    Product product;
    string saleDate;
    double price;
    double discount;
    int quantity;
};

#endif