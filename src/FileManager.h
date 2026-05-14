#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <bits/stdc++.h>
#include "ProductList.h"
#include "AVLTree.h"
using namespace std;

class FileManager
{
public:
    static bool loadProducts(ProductList &products, string filename);
    static bool saveProducts(ProductList &products, string filename);
    static bool loadSales(AVLTree &sales, string filename);
    static bool saveSales(AVLTree &sales, string filename);
    static bool exportTodaySales(AVLTree &sales, string saleDate, string filename);
};

#include "FileManager.cpp"

#endif
