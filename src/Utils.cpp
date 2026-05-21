#include <bits/stdc++.h>
#include "Utils.h"
using namespace std;

// 去掉字符串首尾空白字符
static string trim(string s)
{
    int l = 0, r = (int)s.size() - 1;
    while (l <= r && isspace((unsigned char)s[l]))
        l++;
    while (r >= l && isspace((unsigned char)s[r]))
        r--;
    if (l > r)
        return "";
    return s.substr(l, r - l + 1);
}

// 判断年份是否为闰年
static bool isLeapYear(int year)
{
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

// 判断商品号是否非空
bool isValidProductId(string id)
{
    id = trim(id);
    return !id.empty();
}

// 判断日期是否符合 YYYY-MM-DD 格式且日期真实存在
bool isValidDate(string date)
{
    date = trim(date);
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;

    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (!isdigit((unsigned char)date[i]))
            return false;
    }

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (month < 1 || month > 12 || day < 1)
        return false;

    int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year))
        days[2] = 29;

    return day <= days[month];
}

// 判断制作日期、销售日期、有效期的先后关系是否合理
bool isDateLogical(string makeDate, string saleDate, string validDate)
{
    if (!isValidDate(makeDate) || !isValidDate(saleDate) || !isValidDate(validDate))
        return false;
    return makeDate <= saleDate && saleDate <= validDate;
}

// 判断售价是否为非负数
bool isValidPrice(double price)
{
    return price >= 0;
}

// 判断折扣是否在 0 到 1 之间
bool isValidDiscount(double discount)
{
    return discount >= 0 && discount <= 1;
}

// 判断数量是否为正整数
bool isValidQuantity(int quantity)
{
    return quantity > 0;
}

// 按指定分隔符切分字符串，并去掉每一段首尾空白
vector<string> split(string line, char sep)
{
    vector<string> parts;
    string item;
    stringstream ss(line);
    while (getline(ss, item, sep))
    {
        parts.push_back(trim(item));
    }
    return parts;
}
