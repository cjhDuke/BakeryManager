#ifndef SALERECORD_HPP
#define SALERECORD_HPP

#include <bits/stdc++.h>
#include "Product.h"
using namespace std;

class SaleRecord
{
public:
    SaleRecord() : price(0), discount(0), quantity(0) {} // 构造空销售记录，数值字段默认为 0
    SaleRecord(string id, Product p, string date, double pr, double dis, int qty)
        : recordId(id), product(p), saleDate(date), price(pr), discount(dis), quantity(qty) {} // 使用完整字段构造销售记录

    string getId() const { return recordId; } // 获取销售记录号
    void setId(const string &id) { recordId = id; } // 修改销售记录号

    Product getProduct() const { return product; } // 获取销售记录关联的商品信息
    void setProduct(const Product &p) { product = p; } // 修改销售记录关联的商品信息

    string getSaleDate() const { return saleDate; } // 获取销售日期
    void setSaleDate(const string &date) { saleDate = date; } // 修改销售日期

    double getPrice() const { return price; } // 获取销售单价
    void setPrice(double pr) { price = pr; } // 修改销售单价

    double getDiscount() const { return discount; } // 获取销售折扣
    void setDiscount(double dis) { discount = dis; } // 修改销售折扣

    int getQuantity() const { return quantity; } // 获取销售数量
    void setQuantity(int qty) { quantity = qty; } // 修改销售数量

private:
    string recordId;
    Product product;
    string saleDate;
    double price;
    double discount;
    int quantity;
};

#endif
