#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#ifdef UNICODE
#undef UNICODE
#endif
#ifdef _UNICODE
#undef _UNICODE
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _CRT_SECURE_NO_WARNINGS

#include <bits/stdc++.h>
#include <windows.h>
#include <graphics.h>
#include <conio.h>
#include "FileManager.h"
#include "Utils.h"
using namespace std;

inline string toEasyXText(const wchar_t *text)
{
    if (text == nullptr)
        return "";
    int len = WideCharToMultiByte(CP_ACP, 0, text, -1, nullptr, 0, nullptr, nullptr);
    if (len <= 0)
        return "";
    string result(len - 1, '\0');
    if (!result.empty())
        WideCharToMultiByte(CP_ACP, 0, text, -1, &result[0], len, nullptr, nullptr);
    return result;
}

inline void outtextxy(int x, int y, const wchar_t *text)
{
    string temp = toEasyXText(text);
    ::outtextxy(x, y, temp.c_str());
}

inline int textwidth(const wchar_t *text)
{
    string temp = toEasyXText(text);
    return ::textwidth(temp.c_str());
}

inline int textheight(const wchar_t *text)
{
    string temp = toEasyXText(text);
    return ::textheight(temp.c_str());
}

inline void settextstyle(int height, int width, const wchar_t *face)
{
    string temp = toEasyXText(face);
    ::settextstyle(height, width, temp.c_str());
}

inline bool InputBox(wchar_t *buffer, int maxCount, const wchar_t *prompt = nullptr, const wchar_t *title = nullptr,
                     const wchar_t *defaultText = nullptr, int width = 0, int height = 0, bool onlyOk = true)
{
    if (buffer == nullptr || maxCount <= 0)
        return false;

    vector<char> temp(maxCount * 4 + 1, 0);
    string promptText = toEasyXText(prompt);
    string titleText = toEasyXText(title);
    string defaultValue = toEasyXText(defaultText);

    bool ok = ::InputBox(temp.data(), (int)temp.size(), promptText.empty() ? nullptr : promptText.c_str(),
                         titleText.empty() ? nullptr : titleText.c_str(),
                         defaultValue.empty() ? nullptr : defaultValue.c_str(), width, height, onlyOk);

    MultiByteToWideChar(CP_ACP, 0, temp.data(), -1, buffer, maxCount);
    buffer[maxCount - 1] = L'\0';
    return ok;
}

class GUIManager
{
public:
    GUIManager(); // 初始化图形界面的数据路径、文件和内存数据
    void run(); // 启动 EasyX 图形界面主循环

private:
    const int W = 1100;
    const int H = 720;
    MOUSEMSG m;
    int wheelDelta;

    string DATA_DIR;
    string PRODUCT_FILE;
    string SALE_FILE;
    ProductList products;
    AVLTree sales;

    bool isWindowAlive(); // 判断图形窗口是否仍然有效
    void processMsg(); // 处理鼠标消息和滚轮消息
    void initDataPath(); // 初始化图形界面使用的数据文件路径
    void ensureDataDirectory(); // 确保 data 数据目录存在
    void seedDataFiles(); // 数据文件为空时生成默认演示数据
    bool fileHasData(string filename); // 判断指定文件是否存在且有内容
    string trimText(string s); // 去掉字符串首尾空白字符
    void saveAll(); // 保存当前商品和销售数据

    wstring toWide(string s); // 将 UTF-8 字符串转换为宽字符串用于 Windows 界面显示
    string toUtf8(wstring s); // 将宽字符串转换为 UTF-8 字符串用于数据保存
    string getIn(const wstring &message); // 弹出输入框并读取用户输入
    void showMessage(const wstring &message, const wstring &title = L"提示", UINT type = MB_OK); // 弹出提示消息框

    bool parseIntValue(string s, int &value); // 将字符串解析为整数，失败时返回 false
    bool parseDoubleValue(string s, double &value); // 将字符串解析为小数，失败时返回 false
    string formatDouble(double value); // 将小数格式化为两位小数字符串
    double getSaleTotal(SaleRecord record); // 计算销售记录总额
    vector<Product> getProducts(); // 获取当前全部商品数据
    vector<SaleRecord> getSales(); // 获取当前全部销售记录
    bool updateProduct(Product product); // 按商品号更新商品信息
    string createRecordId(); // 根据现有销售记录生成新记录号

    void drawHeader(const wstring &title); // 绘制窗口顶部标题栏
    void drawHint(const wstring &hint); // 绘制底部提示文字
    void drawMetric(int x, int y, int w, const wstring &title, const wstring &value, COLORREF color); // 绘制统计信息卡片
    bool button(int x, int y, int w, int h, const wstring &text, COLORREF bg = RGB(70, 130, 180)); // 绘制普通按钮并判断是否点击
    bool smallButton(int x, int y, int w, int h, const wstring &text, COLORREF bg); // 绘制小按钮并判断是否点击
    void outTextFit(int x, int y, int maxWidth, const wstring &text); // 在限定宽度内输出文字
    void adjustScroll(int &scrollIndex, int total, int pageSize); // 根据滚轮调整列表滚动位置

    void showProductWindow(); // 显示商品管理窗口
    void addProductDialog(); // 弹出添加商品对话流程
    void removeProductDialog(string id); // 删除指定商品并提示结果

    void showSaleList(vector<SaleRecord> records, const wstring &title, bool editable); // 显示销售记录列表窗口
    bool inputSaleDialog(string recordId, SaleRecord &record, SaleRecord *oldRecord = nullptr); // 输入并校验销售记录
    void addSaleDialog(); // 弹出新增销售记录流程
    void updateSaleDialog(string id); // 修改指定销售记录并同步库存
    void removeSaleDialog(string id); // 删除指定销售记录并恢复库存

    void showSearchWindow(); // 显示销售查询窗口
    void showSortWindow(); // 显示销售排序窗口
    void showStatisticsWindow(); // 显示销售统计窗口
    void exportByDateDialog(); // 按日期导出销售记录

    static bool cmpByDate(SaleRecord a, SaleRecord b); // 按销售日期比较两条销售记录
    static bool cmpByTotal(SaleRecord a, SaleRecord b); // 按销售总额比较两条销售记录
    static bool cmpByQuantity(SaleRecord a, SaleRecord b); // 按销售数量比较两条销售记录
};

#include "GUIManager.cpp"

#endif
