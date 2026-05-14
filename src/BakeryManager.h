#ifndef BAKERYMANAGER_HPP
#define BAKERYMANAGER_HPP

#include <bits/stdc++.h>
#include "ProductList.h"
#include "AVLTree.h"
#include "FileManager.h"
#include "Utils.h"
using namespace std;

class BakeryManager
{
public:
    BakeryManager();
    void run();

private:
    const string PRODUCT_FILE = "products.txt";
    const string SALE_FILE = "sales.txt";
    ProductList products;
    AVLTree sales;

    string trimText(string s);
    string readString();
    int readInt();
    double readDouble();
    double getSaleTotal(SaleRecord record);

    void printProductHeader();
    void printProduct(Product product);
    void printSaleHeader();
    void printSale(SaleRecord record);
    void saveAll();
    bool updateProduct(Product product);
    string createRecordId();
    void showMenu();

    void displayAllProducts();
    void addProduct();
    void removeProduct();
    bool inputSaleRecord(string recordId, SaleRecord &record, SaleRecord *oldRecord = nullptr);
    void addSale();
    void displayAllSales();
    void searchSale();
    void searchTodayExact();
    void searchTodayFuzzy();
    void updateSale();
    void removeSale();
    void sortSales();
    void statisticSales();
    void exportTodaySales();
};

#include "BakeryManager.cpp"

#endif
