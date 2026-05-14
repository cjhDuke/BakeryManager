# 面包店销售管理系统

这是一个 C++ 课程设计项目，用于管理面包店商品信息和销售记录。系统使用链表管理商品基础信息，使用 AVL 树管理销售记录，并通过文本文件保存商品和销售信息。

## 一、主要功能

- 浏览全部商品信息
- 添加、删除商品信息
- 登记销售记录
- 浏览全部销售记录
- 按记录号、商品号、商品名称、销售日期、总价区间、数量区间查询销售记录
- 对当日销售记录进行精确查询或模糊查询
- 修改、删除销售记录
- 按销售日期、总价、数量排序
- 按商品号或销售日期统计销售数量和销售总额
- 导出指定日期的销售记录和统计信息
- 商品和销售数据自动同步到文件

## 二、目录结构

```text
C++ CourseDesign
├── src
│   ├── main.cpp              程序入口
│   ├── BakeryManager.h       系统管理类声明
│   ├── BakeryManager.cpp     菜单和主要业务逻辑
│   ├── Product.h             商品信息类
│   ├── ProductNode.h         商品链表节点
│   ├── ProductList.h         商品链表类声明
│   ├── ProductList.cpp       商品链表类实现
│   ├── SaleRecord.h          销售记录类
│   ├── TreeNode.h            AVL 树节点
│   ├── AVLTree.h             AVL 树类声明
│   ├── AVLTree.cpp           AVL 树类实现
│   ├── FileManager.h         文件管理类声明
│   ├── FileManager.cpp       商品和销售文件读写
│   ├── Utils.h               工具函数声明
│   └── Utils.cpp             日期、输入、字符串处理等工具函数
├── products.txt              商品信息数据文件
├── sales.txt                 销售记录数据文件
├── 设计思路.md
└── 课程设计要求与第八任务整理.md
```

## 三、编译运行

本项目采用“头文件包含对应 cpp 实现”的写法，例如 `AVLTree.h` 中包含 `AVLTree.cpp`。因此编译时只需要编译 `src/main.cpp`，不要再把 `src` 下所有 `.cpp` 一起编译，否则会出现重复定义错误。

推荐在 VSCode 终端或 PowerShell 中执行：

```powershell
chcp 65001
g++ -std=c++17 -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8 src\main.cpp -o bakery.exe
.\bakery.exe
```

如果只想快速编译，也可以使用：

```powershell
g++ -std=c++17 src\main.cpp -o bakery.exe
.\bakery.exe
```

## 四、编码说明

源代码文件使用 UTF-8 编码。为了避免中文菜单乱码，建议：

1. VSCode 右下角文件编码选择 `UTF-8`。
2. 运行前在终端执行：

```powershell
chcp 65001
```

3. 编译时使用：

```powershell
-finput-charset=UTF-8 -fexec-charset=UTF-8
```

程序入口中也设置了 Windows 控制台 UTF-8 编码，用于减少中文输出乱码问题。

## 五、数据文件格式

### 1. 商品信息文件 `products.txt`

每行保存一个商品，字段之间用 `|` 分隔：

```text
商品号|商品名称|制作日期|有效期|库存
```

示例：

```text
P001|红豆面包|2026-05-13|2026-05-16|20
```

### 2. 销售记录文件 `sales.txt`

每行保存一条销售记录，字段之间用 `|` 分隔：

```text
记录号|商品号|商品名称|制作日期|有效期|销售日期|售价|折扣|数量
```

示例：

```text
R001|P001|红豆面包|2026-05-13|2026-05-16|2026-05-14|6.50|0.90|2
```

## 六、运行注意事项

- `products.txt` 和 `sales.txt` 会在程序运行目录下读取和保存。
- 如果文件不存在，程序会提示读取失败，并从空数据开始。
- 添加、修改、删除商品或销售记录后，程序会自动保存文件。
- 登记销售记录时会检查商品是否存在、销售日期是否合法、数量是否超过库存。
- 删除销售记录后会把对应销售数量加回商品库存。

## 七、推荐测试流程

1. 运行程序。
2. 选择 `2` 添加商品信息。
3. 选择 `4` 登记销售记录。
4. 选择 `5` 浏览销售记录。
5. 选择 `12` 统计销售记录。
6. 选择 `13` 导出指定日期销售记录。
7. 选择 `0` 保存并退出。

## 八、编译常见问题

如果出现 `redefinition` 或“重复定义”错误，通常是因为你使用了下面这种编译方式：

```powershell
g++ src\*.cpp -o bakery.exe
```

本项目当前不要这样编译。请使用：

```powershell
g++ -std=c++17 src\main.cpp -o bakery.exe
```

