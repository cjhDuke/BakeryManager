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
    BakeryManager(); // 初始化数据路径、数据文件和内存中的商品/销售数据
    void run(); // 启动控制台主菜单循环

private:
    string DATA_DIR;
    string PRODUCT_FILE;
    string SALE_FILE;
    ProductList products;
    AVLTree sales;

    bool fileHasData(string filename); // 判断指定文件是否存在且包含有效数据
    void initDataPath(); // 初始化 data 目录及商品、销售文件路径
    void ensureDataDirectory(); // 确保数据目录存在，不存在时创建
    void seedDataFiles(); // 在数据文件为空时写入默认演示数据
    string trimText(string s); // 去掉字符串首尾空白字符
    string readString(); // 读取一行字符串输入并去掉首尾空白
    int readInt(); // 读取整数输入，非法时提示重新输入
    double readDouble(); // 读取小数输入，非法时提示重新输入
    double getSaleTotal(SaleRecord record); // 计算一条销售记录的销售总额
    int textWidth(string s); // 计算字符串在控制台中的显示宽度
    string formatCell(string s, int width); // 按显示宽度格式化文本单元格
    string formatInt(int value, int width); // 按指定宽度格式化整数
    string formatDouble(double value, int width); // 按指定宽度格式化两位小数

    void printProductHeader(); // 输出商品列表表头
    void printProduct(Product product); // 输出单个商品信息
    void printSaleHeader(); // 输出销售记录列表表头
    void printSale(SaleRecord record); // 输出单条销售记录信息
    void saveAll(); // 将当前商品和销售数据同步保存到文件
    bool updateProduct(Product product); // 按商品号更新商品链表中的商品信息
    string createRecordId(); // 根据现有销售记录生成新的记录号
    void showMenu(); // 显示控制台主菜单

    void displayAllProducts(); // 浏览全部商品信息
    void addProduct(); // 添加商品信息并同步保存
    void removeProduct(); // 删除商品信息并同步保存
    bool inputSaleRecord(string recordId, SaleRecord &record, SaleRecord *oldRecord = nullptr); // 输入并校验一条销售记录
    void addSale(); // 登记销售记录并扣减库存
    void displayAllSales(); // 浏览全部销售记录
    void searchSale(); // 按多种条件查询销售记录
    void searchTodayExact(); // 对指定日期销售记录进行精确查询
    void searchTodayFuzzy(); // 对指定日期销售记录进行模糊查询
    void updateSale(); // 修改销售记录并同步调整库存
    void removeSale(); // 删除销售记录并恢复对应库存
    void sortSales(); // 按日期、总价或数量排序销售记录
    void statisticSales(); // 按商品号或销售日期统计销售数据
    void exportTodaySales(); // 导出指定日期的销售记录和统计信息
};

#include "BakeryManager.cpp"

#endif
