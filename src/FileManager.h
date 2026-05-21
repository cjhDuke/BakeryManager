#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <bits/stdc++.h>
#include "ProductList.h"
#include "AVLTree.h"
using namespace std;

class FileManager
{
public:
    static string getDataDirectory(); // 获取项目根目录下的 data 文件夹路径
    static bool loadProducts(ProductList &products, string filename); // 从商品文件读取数据到商品链表
    static bool saveProducts(ProductList &products, string filename); // 将商品链表数据保存到商品文件
    static bool loadSales(AVLTree &sales, string filename); // 从销售文件读取数据到销售 AVL 树
    static bool saveSales(AVLTree &sales, string filename); // 将销售 AVL 树数据保存到销售文件
    static bool exportTodaySales(AVLTree &sales, string saleDate, string filename); // 导出指定日期销售明细和统计结果
};

#include "FileManager.cpp"

#endif
