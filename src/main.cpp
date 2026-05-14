#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif
#include <bits/stdc++.h>
#include "BakeryManager.h"
using namespace std;

// 设置 Windows 控制台为 UTF-8 编码，避免中文输出乱码
void setConsoleUtf8()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

// 程序入口，创建管理类对象并启动菜单系统
int main()
{
    setConsoleUtf8();
    BakeryManager manager;
    manager.run();
    return 0;
}
