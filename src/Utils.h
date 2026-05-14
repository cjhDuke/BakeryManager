#ifndef UTILS_HPP
#define UTILS_HPP

#include <bits/stdc++.h>
using namespace std;

bool isValidProductId(string id);
bool isValidDate(string date);
bool isDateLogical(string makeDate, string saleDate, string validDate);
bool isValidPrice(double price);
bool isValidDiscount(double discount);
bool isValidQuantity(int quantity);
vector<string> split(string line, char sep);

#include "Utils.cpp"

#endif
