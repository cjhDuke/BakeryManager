#include <bits/stdc++.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif
#include "BakeryManager.h"
using namespace std;

// 构造管理类对象，启动时读取商品和销售文件
BakeryManager::BakeryManager()
{
    initDataPath();
    ensureDataDirectory();
    seedDataFiles();
    if (!FileManager::loadProducts(products, PRODUCT_FILE))
        cout << "商品文件读取失败，将从空商品表开始。" << endl;
    if (!FileManager::loadSales(sales, SALE_FILE))
        cout << "销售文件读取失败，将从空销售记录开始。" << endl;
}

// 判断文件是否存在且至少有一行有效数据
bool BakeryManager::fileHasData(string filename)
{
    ifstream in(filename);
    if (!in.is_open())
        return false;

    string line;
    while (getline(in, line))
    {
        if (!trimText(line).empty())
            return true;
    }
    return false;
}

// 初始化数据文件夹路径，优先使用已有数据的 data 目录
void BakeryManager::initDataPath()
{
    DATA_DIR = FileManager::getDataDirectory();
    PRODUCT_FILE = DATA_DIR + "/products.txt";
    SALE_FILE = DATA_DIR + "/sales.txt";
}

// 确保 data 数据文件夹存在
void BakeryManager::ensureDataDirectory()
{
#ifdef _WIN32
    _mkdir(DATA_DIR.c_str());
#else
    mkdir(DATA_DIR.c_str(), 0755);
#endif
}

// 当数据文件不存在或为空时写入演示数据
void BakeryManager::seedDataFiles()
{
    if (!fileHasData(PRODUCT_FILE))
    {
        ofstream out(PRODUCT_FILE);
        out << "P001|红豆面包|2026-05-10|2026-05-17|48\n";
        out << "P002|奶油吐司|2026-05-11|2026-05-18|35\n";
        out << "P003|肉松小贝|2026-05-12|2026-05-16|26\n";
        out << "P004|全麦欧包|2026-05-09|2026-05-20|40\n";
        out << "P005|巧克力可颂|2026-05-13|2026-05-17|30\n";
        out << "P006|芝士蛋糕|2026-05-12|2026-05-19|18\n";
    }

    if (!fileHasData(SALE_FILE))
    {
        ofstream out(SALE_FILE);
        out << "R001|P001|红豆面包|2026-05-10|2026-05-17|2026-05-14|6.50|0.90|4\n";
        out << "R002|P002|奶油吐司|2026-05-11|2026-05-18|2026-05-14|12.00|1.00|2\n";
        out << "R003|P003|肉松小贝|2026-05-12|2026-05-16|2026-05-14|8.80|0.85|3\n";
        out << "R004|P004|全麦欧包|2026-05-09|2026-05-20|2026-05-15|15.00|0.95|2\n";
        out << "R005|P005|巧克力可颂|2026-05-13|2026-05-17|2026-05-15|9.50|0.90|5\n";
        out << "R006|P006|芝士蛋糕|2026-05-12|2026-05-19|2026-05-15|18.00|0.88|1\n";
    }
}

// 去掉字符串首尾空白字符
string BakeryManager::trimText(string s)
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

// 读取一行字符串输入
string BakeryManager::readString()
{
    string s;
    getline(cin, s);
    while (s.empty() && cin.good())
        getline(cin, s);
    return trimText(s);
}

// 读取整数输入，输入非法时要求重新输入
int BakeryManager::readInt()
{
    int x;
    while (!(cin >> x))
    {
        if (cin.eof())
            return 0;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "输入非法，请重新输入整数: ";
    }
    cin.ignore(10000, '\n');
    return x;
}

// 读取小数输入，输入非法时要求重新输入
double BakeryManager::readDouble()
{
    double x;
    while (!(cin >> x))
    {
        if (cin.eof())
            return 0;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "输入非法，请重新输入数字: ";
    }
    cin.ignore(10000, '\n');
    return x;
}

// 计算销售记录的销售总额
double BakeryManager::getSaleTotal(SaleRecord record)
{
    return record.getPrice() * record.getDiscount() * record.getQuantity();
}

// 计算字符串在控制台中的显示宽度
int BakeryManager::textWidth(string s)
{
    int width = 0;
    for (int i = 0; i < (int)s.size();)
    {
        unsigned char ch = (unsigned char)s[i];
        if (ch < 128)
        {
            width++;
            i++;
        }
        else
        {
            width += 2;
            if ((ch & 0xE0) == 0xC0)
                i += 2;
            else if ((ch & 0xF0) == 0xE0)
                i += 3;
            else if ((ch & 0xF8) == 0xF0)
                i += 4;
            else
                i++;
        }
    }
    return width;
}

// 将字符串按显示宽度补齐到指定列宽
string BakeryManager::formatCell(string s, int width)
{
    int blank = width - textWidth(s);
    if (blank < 1)
        blank = 1;
    return s + string(blank, ' ');
}

// 将整数转换成指定列宽的字符串
string BakeryManager::formatInt(int value, int width)
{
    return formatCell(to_string(value), width);
}

// 将小数转换成保留两位并指定列宽的字符串
string BakeryManager::formatDouble(double value, int width)
{
    stringstream ss;
    ss << fixed << setprecision(2) << value;
    return formatCell(ss.str(), width);
}

// 输出商品表头
void BakeryManager::printProductHeader()
{
    cout << formatCell("商品号", 12)
         << formatCell("商品名称", 18)
         << formatCell("制作日期", 14)
         << formatCell("有效期", 14)
         << formatCell("库存", 8) << endl;
}

// 输出一条商品信息
void BakeryManager::printProduct(Product product)
{
    cout << formatCell(product.getId(), 12)
         << formatCell(product.getName(), 18)
         << formatCell(product.getMakeDate(), 14)
         << formatCell(product.getValidDate(), 14)
         << formatInt(product.getStock(), 8) << endl;
}

// 输出销售记录表头
void BakeryManager::printSaleHeader()
{
    cout << formatCell("记录号", 10)
         << formatCell("商品号", 12)
         << formatCell("商品名称", 18)
         << formatCell("制作日期", 14)
         << formatCell("有效期", 14)
         << formatCell("销售日期", 14)
         << formatCell("售价", 10)
         << formatCell("折扣", 8)
         << formatCell("数量", 8)
         << formatCell("总价", 10) << endl;
}

// 输出一条销售记录
void BakeryManager::printSale(SaleRecord record)
{
    Product product = record.getProduct();
    cout << formatCell(record.getId(), 10)
         << formatCell(product.getId(), 12)
         << formatCell(product.getName(), 18)
         << formatCell(product.getMakeDate(), 14)
         << formatCell(product.getValidDate(), 14)
         << formatCell(record.getSaleDate(), 14)
         << formatDouble(record.getPrice(), 10)
         << formatDouble(record.getDiscount(), 8)
         << formatInt(record.getQuantity(), 8)
         << formatDouble(getSaleTotal(record), 10) << endl;
}

// 保存商品和销售记录到文件
void BakeryManager::saveAll()
{
    ensureDataDirectory();
    if (!FileManager::saveProducts(products, PRODUCT_FILE))
        cout << "商品信息保存失败。" << endl;
    if (!FileManager::saveSales(sales, SALE_FILE))
        cout << "销售记录保存失败。" << endl;
}

// 将修改后的商品对象更新回商品链表
bool BakeryManager::updateProduct(Product product)
{
    if (!products.removeProduct(product.getId()))
        return false;
    return products.addProduct(product);
}

// 生成新的销售记录编号
string BakeryManager::createRecordId()
{
    vector<SaleRecord> records;
    sales.getAllRecords(records);
    int maxId = 0;

    for (SaleRecord record : records)
    {
        string id = record.getId();
        string digits;
        for (char ch : id)
        {
            if (isdigit((unsigned char)ch))
                digits += ch;
        }
        if (!digits.empty())
            maxId = max(maxId, stoi(digits));
    }

    stringstream ss;
    ss << "R" << setw(3) << setfill('0') << maxId + 1;
    return ss.str();
}

// 显示主菜单
void BakeryManager::showMenu()
{
    cout << endl;
    cout << "========== 面包店销售管理系统 ==========" << endl;
    cout << "1. 浏览全部商品信息" << endl;
    cout << "2. 添加商品信息" << endl;
    cout << "3. 删除商品信息" << endl;
    cout << "4. 登记销售记录" << endl;
    cout << "5. 浏览全部销售记录" << endl;
    cout << "6. 查询销售记录" << endl;
    cout << "7. 当日精确查询" << endl;
    cout << "8. 当日模糊查询" << endl;
    cout << "9. 修改销售记录" << endl;
    cout << "10. 删除销售记录" << endl;
    cout << "11. 销售记录排序" << endl;
    cout << "12. 销售记录统计" << endl;
    cout << "13. 导出当日销售记录" << endl;
    cout << "0. 保存并退出" << endl;
    cout << "请选择: ";
}

// 浏览全部商品信息
void BakeryManager::displayAllProducts()
{
    vector<Product> allProducts;
    products.getAllProducts(allProducts);
    if (allProducts.empty())
    {
        cout << "暂无商品信息。" << endl;
        return;
    }

    printProductHeader();
    for (Product product : allProducts)
        printProduct(product);
}

// 添加商品信息并同步保存文件
void BakeryManager::addProduct()
{
    string id, name, makeDate, validDate;
    int stock;

    cout << "商品号: ";
    id = readString();
    if (!isValidProductId(id))
    {
        cout << "商品号不能为空。" << endl;
        return;
    }

    Product oldProduct;
    if (products.findProduct(id, oldProduct))
    {
        cout << "商品号已存在。" << endl;
        return;
    }

    cout << "商品名称: ";
    name = readString();

    cout << "制作日期(YYYY-MM-DD): ";
    makeDate = readString();
    cout << "有效期(YYYY-MM-DD): ";
    validDate = readString();
    if (!isValidDate(makeDate) || !isValidDate(validDate) || makeDate > validDate)
    {
        cout << "日期不合法。" << endl;
        return;
    }

    cout << "库存: ";
    stock = readInt();
    if (stock < 0)
    {
        cout << "库存不能为负数。" << endl;
        return;
    }

    if (products.addProduct(Product(id, name, makeDate, validDate, stock)))
    {
        saveAll();
        cout << "商品添加成功。" << endl;
    }
    else
    {
        cout << "商品添加失败。" << endl;
    }
}

// 删除商品信息并同步保存文件
void BakeryManager::removeProduct()
{
    string id;
    cout << "请输入要删除的商品号: ";
    id = readString();

    if (products.removeProduct(id))
    {
        saveAll();
        cout << "商品删除成功。" << endl;
    }
    else
    {
        cout << "商品不存在，删除失败。" << endl;
    }
}

// 输入并校验一条销售记录
bool BakeryManager::inputSaleRecord(string recordId, SaleRecord &record, SaleRecord *oldRecord)
{
    string productId, saleDate;
    double price, discount;
    int quantity;

    cout << "商品号: ";
    productId = readString();

    Product product;
    if (!products.findProduct(productId, product))
    {
        cout << "商品不存在。" << endl;
        return false;
    }

    cout << "销售日期(YYYY-MM-DD): ";
    saleDate = readString();
    if (!isDateLogical(product.getMakeDate(), saleDate, product.getValidDate()))
    {
        cout << "销售日期不合法，必须满足 制作日期 <= 销售日期 <= 有效期。" << endl;
        return false;
    }

    cout << "售价: ";
    price = readDouble();
    if (!isValidPrice(price))
    {
        cout << "售价不能为负数。" << endl;
        return false;
    }

    cout << "折扣(0~1): ";
    discount = readDouble();
    if (!isValidDiscount(discount))
    {
        cout << "折扣必须在 0 到 1 之间。" << endl;
        return false;
    }

    cout << "数量: ";
    quantity = readInt();
    if (!isValidQuantity(quantity))
    {
        cout << "数量必须为正整数。" << endl;
        return false;
    }

    int available = product.getStock();
    if (oldRecord != nullptr && oldRecord->getProduct().getId() == productId)
        available += oldRecord->getQuantity();

    if (quantity > available)
    {
        cout << "库存不足，当前可用库存为 " << available << "。" << endl;
        return false;
    }

    record = SaleRecord(recordId, product, saleDate, price, discount, quantity);
    return true;
}

// 登记销售记录并同步更新库存和文件
void BakeryManager::addSale()
{
    string recordId = createRecordId();
    SaleRecord record;

    if (!inputSaleRecord(recordId, record))
        return;

    Product product = record.getProduct();
    product.setStock(product.getStock() - record.getQuantity());

    if (!updateProduct(product))
    {
        cout << "库存更新失败，登记取消。" << endl;
        return;
    }

    if (sales.addRecord(record))
    {
        saveAll();
        cout << "销售登记成功，记录号为 " << recordId << "。" << endl;
    }
    else
    {
        product.setStock(product.getStock() + record.getQuantity());
        updateProduct(product);
        cout << "销售登记失败，记录号可能重复。" << endl;
    }
}

// 浏览全部销售记录
void BakeryManager::displayAllSales()
{
    vector<SaleRecord> records;
    sales.getAllRecords(records);
    if (records.empty())
    {
        cout << "暂无销售记录。" << endl;
        return;
    }

    printSaleHeader();
    for (SaleRecord record : records)
        printSale(record);
}

// 按多种条件查询销售记录
void BakeryManager::searchSale()
{
    cout << "1. 按记录号查询" << endl;
    cout << "2. 按商品号查询" << endl;
    cout << "3. 按商品名称模糊查询" << endl;
    cout << "4. 按销售日期查询" << endl;
    cout << "5. 按总价区间查询" << endl;
    cout << "6. 按数量区间查询" << endl;
    cout << "请选择: ";

    int op = readInt();
    vector<SaleRecord> records;
    sales.getAllRecords(records);
    vector<SaleRecord> result;
    string key;

    if (op == 1)
    {
        cout << "记录号: ";
        key = readString();
        TreeNode *node = sales.searchRecord(key);
        if (node != nullptr)
            result.push_back(node->data);
    }
    else if (op == 2)
    {
        cout << "商品号: ";
        key = readString();
        for (SaleRecord record : records)
            if (record.getProduct().getId() == key)
                result.push_back(record);
    }
    else if (op == 3)
    {
        cout << "商品名称关键词: ";
        key = readString();
        for (SaleRecord record : records)
            if (record.getProduct().getName().find(key) != string::npos)
                result.push_back(record);
    }
    else if (op == 4)
    {
        cout << "销售日期(YYYY-MM-DD): ";
        key = readString();
        if (!isValidDate(key))
        {
            cout << "日期不合法。" << endl;
            return;
        }
        for (SaleRecord record : records)
            if (record.getSaleDate() == key)
                result.push_back(record);
    }
    else if (op == 5)
    {
        double low, high;
        cout << "最低总价: ";
        low = readDouble();
        cout << "最高总价: ";
        high = readDouble();
        if (low > high)
            swap(low, high);
        for (SaleRecord record : records)
            if (getSaleTotal(record) >= low && getSaleTotal(record) <= high)
                result.push_back(record);
    }
    else if (op == 6)
    {
        int low, high;
        cout << "最低数量: ";
        low = readInt();
        cout << "最高数量: ";
        high = readInt();
        if (low > high)
            swap(low, high);
        for (SaleRecord record : records)
            if (record.getQuantity() >= low && record.getQuantity() <= high)
                result.push_back(record);
    }
    else
    {
        cout << "无效选择。" << endl;
        return;
    }

    if (result.empty())
    {
        cout << "没有找到符合条件的销售记录。" << endl;
        return;
    }

    printSaleHeader();
    for (SaleRecord record : result)
        printSale(record);
}

// 对指定日期的销售记录进行精确查询
void BakeryManager::searchTodayExact()
{
    string saleDate, key;
    cout << "销售日期(YYYY-MM-DD): ";
    saleDate = readString();
    if (!isValidDate(saleDate))
    {
        cout << "日期不合法。" << endl;
        return;
    }

    cout << "商品号或完整商品名称: ";
    key = readString();

    vector<SaleRecord> records;
    sales.getAllRecords(records);
    vector<SaleRecord> result;

    for (SaleRecord record : records)
    {
        Product product = record.getProduct();
        if (record.getSaleDate() == saleDate && (product.getId() == key || product.getName() == key))
            result.push_back(record);
    }

    if (result.empty())
    {
        cout << "没有找到符合条件的销售记录。" << endl;
        return;
    }

    printSaleHeader();
    for (SaleRecord record : result)
        printSale(record);
}

// 对指定日期的销售记录进行模糊查询
void BakeryManager::searchTodayFuzzy()
{
    string saleDate, key;
    cout << "销售日期(YYYY-MM-DD): ";
    saleDate = readString();
    if (!isValidDate(saleDate))
    {
        cout << "日期不合法。" << endl;
        return;
    }

    cout << "商品号或商品名称关键词: ";
    key = readString();

    vector<SaleRecord> records;
    sales.getAllRecords(records);
    vector<SaleRecord> result;

    for (SaleRecord record : records)
    {
        Product product = record.getProduct();
        bool matchProduct = product.getId().find(key) != string::npos || product.getName().find(key) != string::npos;
        if (record.getSaleDate() == saleDate && matchProduct)
            result.push_back(record);
    }

    if (result.empty())
    {
        cout << "没有找到符合条件的销售记录。" << endl;
        return;
    }

    printSaleHeader();
    for (SaleRecord record : result)
        printSale(record);
}

// 修改销售记录并同步更新库存和文件
void BakeryManager::updateSale()
{
    string id;
    cout << "请输入要修改的销售记录号: ";
    id = readString();

    TreeNode *node = sales.searchRecord(id);
    if (node == nullptr)
    {
        cout << "销售记录不存在。" << endl;
        return;
    }

    SaleRecord oldRecord = node->data;
    SaleRecord newRecord;
    if (!inputSaleRecord(id, newRecord, &oldRecord))
        return;

    Product oldProduct;
    Product newProduct;
    if (!products.findProduct(oldRecord.getProduct().getId(), oldProduct)
        || !products.findProduct(newRecord.getProduct().getId(), newProduct))
    {
        cout << "商品信息异常，修改失败。" << endl;
        return;
    }

    oldProduct.setStock(oldProduct.getStock() + oldRecord.getQuantity());
    if (oldProduct.getId() == newProduct.getId())
        newProduct = oldProduct;
    newProduct.setStock(newProduct.getStock() - newRecord.getQuantity());

    sales.removeRecord(id);
    if (oldProduct.getId() == newProduct.getId())
    {
        updateProduct(newProduct);
    }
    else
    {
        updateProduct(oldProduct);
        updateProduct(newProduct);
    }

    if (sales.addRecord(newRecord))
    {
        saveAll();
        cout << "销售记录修改成功。" << endl;
    }
    else
    {
        sales.addRecord(oldRecord);
        updateProduct(oldProduct);
        cout << "销售记录修改失败，已恢复原记录。" << endl;
    }
}

// 删除销售记录并同步恢复库存和文件
void BakeryManager::removeSale()
{
    string id;
    cout << "请输入要删除的销售记录号: ";
    id = readString();

    TreeNode *node = sales.searchRecord(id);
    if (node == nullptr)
    {
        cout << "销售记录不存在。" << endl;
        return;
    }

    cout << "确认删除? 输入 Y 确认: ";
    string confirm = readString();
    if (confirm != "Y" && confirm != "y")
    {
        cout << "已取消删除。" << endl;
        return;
    }

    SaleRecord record = node->data;
    Product product;
    if (products.findProduct(record.getProduct().getId(), product))
    {
        product.setStock(product.getStock() + record.getQuantity());
        updateProduct(product);
    }

    if (sales.removeRecord(id))
    {
        saveAll();
        cout << "销售记录删除成功。" << endl;
    }
    else
    {
        cout << "销售记录删除失败。" << endl;
    }
}

// 按销售日期、总价或数量对销售记录排序后输出
void BakeryManager::sortSales()
{
    vector<SaleRecord> records;
    sales.getAllRecords(records);
    if (records.empty())
    {
        cout << "暂无销售记录。" << endl;
        return;
    }

    cout << "1. 按销售日期排序" << endl;
    cout << "2. 按总价排序" << endl;
    cout << "3. 按数量排序" << endl;
    cout << "请选择: ";

    int op = readInt();
    if (op == 1)
    {
        sort(records.begin(), records.end(), [](SaleRecord a, SaleRecord b) {
            return a.getSaleDate() < b.getSaleDate();
        });
    }
    else if (op == 2)
    {
        sort(records.begin(), records.end(), [](SaleRecord a, SaleRecord b) {
            return a.getPrice() * a.getDiscount() * a.getQuantity()
                 < b.getPrice() * b.getDiscount() * b.getQuantity();
        });
    }
    else if (op == 3)
    {
        sort(records.begin(), records.end(), [](SaleRecord a, SaleRecord b) {
            return a.getQuantity() < b.getQuantity();
        });
    }
    else
    {
        cout << "无效选择。" << endl;
        return;
    }

    printSaleHeader();
    for (SaleRecord record : records)
        printSale(record);
}

// 按商品号或销售日期统计销售数量和销售总额
void BakeryManager::statisticSales()
{
    vector<SaleRecord> records;
    sales.getAllRecords(records);
    if (records.empty())
    {
        cout << "暂无销售记录。" << endl;
        return;
    }

    cout << "1. 按商品号统计" << endl;
    cout << "2. 按销售日期统计" << endl;
    cout << "请选择: ";
    int op = readInt();

    map<string, pair<int, double>> statistics;
    for (SaleRecord record : records)
    {
        string key;
        if (op == 1)
            key = record.getProduct().getId();
        else if (op == 2)
            key = record.getSaleDate();
        else
        {
            cout << "无效选择。" << endl;
            return;
        }
        statistics[key].first += record.getQuantity();
        statistics[key].second += getSaleTotal(record);
    }

    cout << formatCell(op == 1 ? "商品号" : "销售日期", 18)
         << formatCell("销售数量", 12)
         << formatCell("销售总额", 12) << endl;

    for (auto item : statistics)
    {
        cout << formatCell(item.first, 18)
             << formatInt(item.second.first, 12)
             << formatDouble(item.second.second, 12) << endl;
    }
}

// 导出指定日期的销售记录和统计信息
void BakeryManager::exportTodaySales()
{
    string saleDate;
    cout << "请输入导出日期(YYYY-MM-DD): ";
    saleDate = readString();
    if (!isValidDate(saleDate))
    {
        cout << "日期不合法。" << endl;
        return;
    }

    ensureDataDirectory();
    string filename = DATA_DIR + "/export_" + saleDate + ".txt";
    if (FileManager::exportTodaySales(sales, saleDate, filename))
        cout << "导出成功，文件名: " << filename << endl;
    else
        cout << "导出失败。" << endl;
}

// 启动菜单循环并根据用户选择调用对应功能
void BakeryManager::run()
{
    while (true)
    {
        showMenu();
        int op = readInt();

        switch (op)
        {
        case 1:
            displayAllProducts();
            break;
        case 2:
            addProduct();
            break;
        case 3:
            removeProduct();
            break;
        case 4:
            addSale();
            break;
        case 5:
            displayAllSales();
            break;
        case 6:
            searchSale();
            break;
        case 7:
            searchTodayExact();
            break;
        case 8:
            searchTodayFuzzy();
            break;
        case 9:
            updateSale();
            break;
        case 10:
            removeSale();
            break;
        case 11:
            sortSales();
            break;
        case 12:
            statisticSales();
            break;
        case 13:
            exportTodaySales();
            break;
        case 0:
            saveAll();
            cout << "数据已保存，程序退出。" << endl;
            return;
        default:
            cout << "无效选择，请重新输入。" << endl;
        }
    }
}
