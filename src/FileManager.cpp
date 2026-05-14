#include <bits/stdc++.h>
#include "FileManager.h"
#include "Utils.h"
using namespace std;

// 将字符串转换为整数，转换失败返回 false
static bool parseInt(string s, int &value)
{
    stringstream ss(s);
    ss >> value;
    return !ss.fail() && ss.eof();
}

// 将字符串转换为小数，转换失败返回 false
static bool parseDouble(string s, double &value)
{
    stringstream ss(s);
    ss >> value;
    return !ss.fail() && ss.eof();
}

// 计算一条销售记录的销售总额
static double getSaleTotal(SaleRecord record)
{
    return record.getPrice() * record.getDiscount() * record.getQuantity();
}

// 从商品文件读取数据并加入商品链表
bool FileManager::loadProducts(ProductList &products, string filename)
{
    ifstream in(filename);
    if (!in.is_open())
        return false;

    string line;
    while (getline(in, line))
    {
        if (line.empty())
            continue;

        vector<string> parts = split(line, '|');
        if (parts.size() != 5)
            continue;

        int stock;
        if (!parseInt(parts[4], stock))
            continue;

        if (!isValidProductId(parts[0]) || !isValidDate(parts[2]) || !isValidDate(parts[3]) || stock < 0)
            continue;

        products.addProduct(Product(parts[0], parts[1], parts[2], parts[3], stock));
    }
    return true;
}

// 将商品链表中的所有商品写入商品文件
bool FileManager::saveProducts(ProductList &products, string filename)
{
    ofstream out(filename);
    if (!out.is_open())
        return false;

    vector<Product> allProducts;
    products.getAllProducts(allProducts);

    for (Product product : allProducts)
    {
        out << product.getId() << "|"
            << product.getName() << "|"
            << product.getMakeDate() << "|"
            << product.getValidDate() << "|"
            << product.getStock() << endl;
    }
    return true;
}

// 从销售文件读取数据并加入销售记录 AVL 树
bool FileManager::loadSales(AVLTree &sales, string filename)
{
    ifstream in(filename);
    if (!in.is_open())
        return false;

    string line;
    while (getline(in, line))
    {
        if (line.empty())
            continue;

        vector<string> parts = split(line, '|');
        if (parts.size() != 9)
            continue;

        double price, discount;
        int quantity;
        if (!parseDouble(parts[6], price) || !parseDouble(parts[7], discount) || !parseInt(parts[8], quantity))
            continue;

        if (parts[0].empty() || !isValidProductId(parts[1]) || !isDateLogical(parts[3], parts[5], parts[4])
            || !isValidPrice(price) || !isValidDiscount(discount) || !isValidQuantity(quantity))
            continue;

        Product product(parts[1], parts[2], parts[3], parts[4], 0);
        SaleRecord record(parts[0], product, parts[5], price, discount, quantity);
        sales.addRecord(record);
    }
    return true;
}

// 将销售记录 AVL 树中的所有记录写入销售文件
bool FileManager::saveSales(AVLTree &sales, string filename)
{
    ofstream out(filename);
    if (!out.is_open())
        return false;

    vector<SaleRecord> records;
    sales.getAllRecords(records);

    for (SaleRecord record : records)
    {
        Product product = record.getProduct();
        out << record.getId() << "|"
            << product.getId() << "|"
            << product.getName() << "|"
            << product.getMakeDate() << "|"
            << product.getValidDate() << "|"
            << record.getSaleDate() << "|"
            << fixed << setprecision(2) << record.getPrice() << "|"
            << fixed << setprecision(2) << record.getDiscount() << "|"
            << record.getQuantity() << endl;
    }
    return true;
}

// 导出指定日期的销售记录和按商品号统计结果
bool FileManager::exportTodaySales(AVLTree &sales, string saleDate, string filename)
{
    if (!isValidDate(saleDate))
        return false;

    ofstream out(filename);
    if (!out.is_open())
        return false;

    vector<SaleRecord> records;
    sales.getAllRecords(records);

    map<string, pair<int, double>> statistics;
    int totalQuantity = 0;
    double totalAmount = 0;

    out << "=== " << saleDate << " 销售记录 ===" << endl;
    out << left << setw(12) << "记录号"
        << setw(12) << "商品号"
        << setw(16) << "商品名称"
        << setw(14) << "销售日期"
        << setw(10) << "售价"
        << setw(10) << "折扣"
        << setw(10) << "数量"
        << setw(12) << "销售总额" << endl;

    for (SaleRecord record : records)
    {
        if (record.getSaleDate() != saleDate)
            continue;

        Product product = record.getProduct();
        double amount = getSaleTotal(record);

        out << left << setw(12) << record.getId()
            << setw(12) << product.getId()
            << setw(16) << product.getName()
            << setw(14) << record.getSaleDate()
            << setw(10) << fixed << setprecision(2) << record.getPrice()
            << setw(10) << fixed << setprecision(2) << record.getDiscount()
            << setw(10) << record.getQuantity()
            << setw(12) << fixed << setprecision(2) << amount << endl;

        statistics[product.getId()].first += record.getQuantity();
        statistics[product.getId()].second += amount;
        totalQuantity += record.getQuantity();
        totalAmount += amount;
    }

    out << endl << "=== 按商品号统计 ===" << endl;
    out << left << setw(12) << "商品号"
        << setw(12) << "销售数量"
        << setw(12) << "销售总额" << endl;

    for (auto item : statistics)
    {
        out << left << setw(12) << item.first
            << setw(12) << item.second.first
            << setw(12) << fixed << setprecision(2) << item.second.second << endl;
    }

    out << endl << "总销售数量: " << totalQuantity << endl;
    out << "总销售额: " << fixed << setprecision(2) << totalAmount << endl;
    return true;
}
