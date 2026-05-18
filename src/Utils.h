#ifndef UTILS_HPP
#define UTILS_HPP

#include <bits/stdc++.h>
using namespace std;

bool isValidProductId(string id); // 判断商品号是否非空
bool isValidDate(string date); // 判断日期格式和日期值是否合法
bool isDateLogical(string makeDate, string saleDate, string validDate); // 判断制作日期、销售日期、有效期顺序是否合理
bool isValidPrice(double price); // 判断售价是否为非负数
bool isValidDiscount(double discount); // 判断折扣是否在 0 到 1 之间
bool isValidQuantity(int quantity); // 判断数量是否为正整数
vector<string> split(string line, char sep); // 按指定分隔符切分字符串并去掉首尾空白

#include "Utils.cpp"

#endif
