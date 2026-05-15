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
    string DATA_DIR;
    string PRODUCT_FILE;
    string SALE_FILE;
    ProductList products;
    AVLTree sales;

    bool fileHasData(string filename);
    void initDataPath();
    void ensureDataDirectory();
    void seedDataFiles();
    string trimText(string s);
    string readString();
    int readInt();
    double readDouble();
    double getSaleTotal(SaleRecord record);
    int textWidth(string s);
    string formatCell(string s, int width);
    string formatInt(int value, int width);
    string formatDouble(double value, int width);

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
