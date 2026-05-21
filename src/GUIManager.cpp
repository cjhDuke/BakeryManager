#include "GUIManager.h"
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

GUIManager::GUIManager() : wheelDelta(0)
{
    m.uMsg = 0;
    m.x = 0;
    m.y = 0;
    initDataPath();
    ensureDataDirectory();
    seedDataFiles();
    if (!FileManager::loadProducts(products, PRODUCT_FILE))
        showMessage(L"商品文件读取失败，将从空商品表开始。", L"提示");
    if (!FileManager::loadSales(sales, SALE_FILE))
        showMessage(L"销售文件读取失败，将从空销售记录开始。", L"提示");
}

bool GUIManager::isWindowAlive()
{
    return IsWindow(GetHWnd());
}

void GUIManager::processMsg()
{
    m.uMsg = 0;
    wheelDelta = 0;
    while (MouseHit())
    {
        MOUSEMSG temp = GetMouseMsg();
        if (temp.uMsg == WM_MOUSEWHEEL)
            wheelDelta += temp.wheel;
        m = temp;
    }
}

string GUIManager::trimText(string s)
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

bool GUIManager::fileHasData(string filename)
{
    ifstream in(filename);
    if (!in.is_open())
        return false;

    string line;
    while (getline(in, line))
        if (!trimText(line).empty())
            return true;
    return false;
}

void GUIManager::initDataPath()
{
    DATA_DIR = FileManager::getDataDirectory();
    PRODUCT_FILE = DATA_DIR + "/products.txt";
    SALE_FILE = DATA_DIR + "/sales.txt";
}

void GUIManager::ensureDataDirectory()
{
#ifdef _WIN32
    _mkdir(DATA_DIR.c_str());
#else
    mkdir(DATA_DIR.c_str(), 0755);
#endif
}

void GUIManager::seedDataFiles()
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

void GUIManager::saveAll()
{
    ensureDataDirectory();
    if (!FileManager::saveProducts(products, PRODUCT_FILE))
        showMessage(L"商品信息保存失败。", L"错误", MB_OK | MB_ICONERROR);
    if (!FileManager::saveSales(sales, SALE_FILE))
        showMessage(L"销售记录保存失败。", L"错误", MB_OK | MB_ICONERROR);
}

wstring GUIManager::toWide(string s)
{
    if (s.empty())
        return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    if (len <= 0)
        len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, nullptr, 0);
    if (len <= 0)
        return wstring(s.begin(), s.end());

    wstring result(len - 1, L'\0');
    if (!result.empty())
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &result[0], len);
    return result;
}

string GUIManager::toUtf8(wstring s)
{
    if (s.empty())
        return "";
    int len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (len <= 0)
        return "";

    string result(len - 1, '\0');
    if (!result.empty())
        WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, &result[0], len, nullptr, nullptr);
    return result;
}

string GUIManager::getIn(const wstring &message)
{
    wchar_t buf[256] = {0};
    InputBox(buf, 256, message.c_str(), L"输入");
    return trimText(toUtf8(buf));
}

void GUIManager::showMessage(const wstring &message, const wstring &title, UINT type)
{
    MessageBoxW(GetHWnd(), message.c_str(), title.c_str(), type);
}

bool GUIManager::parseIntValue(string s, int &value)
{
    s = trimText(s);
    if (s.empty())
        return false;
    stringstream ss(s);
    ss >> value;
    return !ss.fail() && ss.eof();
}

bool GUIManager::parseDoubleValue(string s, double &value)
{
    s = trimText(s);
    if (s.empty())
        return false;
    stringstream ss(s);
    ss >> value;
    return !ss.fail() && ss.eof();
}

string GUIManager::formatDouble(double value)
{
    stringstream ss;
    ss << fixed << setprecision(2) << value;
    return ss.str();
}

double GUIManager::getSaleTotal(SaleRecord record)
{
    return record.getPrice() * record.getDiscount() * record.getQuantity();
}

vector<Product> GUIManager::getProducts()
{
    vector<Product> result;
    products.getAllProducts(result);
    return result;
}

vector<SaleRecord> GUIManager::getSales()
{
    vector<SaleRecord> result;
    sales.getAllRecords(result);
    return result;
}

bool GUIManager::updateProduct(Product product)
{
    if (!products.removeProduct(product.getId()))
        return false;
    return products.addProduct(product);
}

string GUIManager::createRecordId()
{
    vector<SaleRecord> records = getSales();
    int maxId = 0;

    for (SaleRecord record : records)
    {
        string id = record.getId();
        string digits;
        for (char ch : id)
            if (isdigit((unsigned char)ch))
                digits += ch;
        if (!digits.empty())
            maxId = max(maxId, stoi(digits));
    }

    stringstream ss;
    ss << "R" << setw(3) << setfill('0') << maxId + 1;
    return ss.str();
}

void GUIManager::drawHeader(const wstring &title)
{
    setbkcolor(RGB(248, 250, 252));
    cleardevice();
    setbkmode(TRANSPARENT);
    setlinecolor(RGB(215, 220, 228));
    setfillcolor(WHITE);
    solidrectangle(0, 0, W, 72);
    line(0, 72, W, 72);

    settextcolor(RGB(28, 37, 54));
    settextstyle(30, 0, L"黑体");
    outtextxy(34, 22, title.c_str());
}

void GUIManager::drawHint(const wstring &hint)
{
    settextcolor(RGB(100, 112, 130));
    settextstyle(16, 0, L"微软雅黑");
    outtextxy(34, H - 34, hint.c_str());
}

void GUIManager::drawMetric(int x, int y, int w, const wstring &title, const wstring &value, COLORREF color)
{
    setlinecolor(RGB(220, 226, 236));
    setfillcolor(WHITE);
    fillroundrect(x, y, x + w, y + 92, 8, 8);

    settextcolor(RGB(95, 108, 128));
    settextstyle(17, 0, L"微软雅黑");
    outtextxy(x + 18, y + 16, title.c_str());

    settextcolor(color);
    settextstyle(28, 0, L"黑体");
    outtextxy(x + 18, y + 48, value.c_str());
}

bool GUIManager::button(int x, int y, int w, int h, const wstring &text, COLORREF bg)
{
    bool hover = (m.x >= x && m.x <= x + w && m.y >= y && m.y <= y + h);
    COLORREF fill = hover ? RGB(88, 150, 235) : bg;
    setlinecolor(hover ? RGB(40, 95, 170) : RGB(47, 82, 130));
    setfillcolor(fill);
    fillroundrect(x, y, x + w, y + h, 8, 8);

    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    settextstyle(21, 0, L"微软雅黑");
    outtextxy(x + (w - textwidth(text.c_str())) / 2, y + (h - textheight(text.c_str())) / 2, text.c_str());
    return hover && m.uMsg == WM_LBUTTONUP;
}

bool GUIManager::smallButton(int x, int y, int w, int h, const wstring &text, COLORREF bg)
{
    bool hover = (m.x >= x && m.x <= x + w && m.y >= y && m.y <= y + h);
    setfillcolor(hover ? WHITE : bg);
    setlinecolor(bg);
    fillroundrect(x, y, x + w, y + h, 5, 5);

    setbkmode(TRANSPARENT);
    settextcolor(hover ? bg : WHITE);
    settextstyle(15, 0, L"微软雅黑");
    outtextxy(x + (w - textwidth(text.c_str())) / 2, y + (h - textheight(text.c_str())) / 2, text.c_str());
    return hover && m.uMsg == WM_LBUTTONUP;
}

void GUIManager::outTextFit(int x, int y, int maxWidth, const wstring &text)
{
    wstring show = text;
    if (textwidth(show.c_str()) > maxWidth)
    {
        while (!show.empty() && textwidth((show + L"...").c_str()) > maxWidth)
            show.pop_back();
        show += L"...";
    }
    outtextxy(x, y, show.c_str());
}

void GUIManager::adjustScroll(int &scrollIndex, int total, int pageSize)
{
    if (wheelDelta > 0)
        scrollIndex--;
    else if (wheelDelta < 0)
        scrollIndex++;

    if (total <= pageSize)
    {
        scrollIndex = 0;
        return;
    }
    if (scrollIndex < 0)
        scrollIndex = 0;
    if (scrollIndex > total - pageSize)
        scrollIndex = total - pageSize;
}

void GUIManager::addProductDialog()
{
    string id = getIn(L"商品号:");
    if (!isValidProductId(id))
    {
        showMessage(L"商品号不能为空。", L"错误", MB_OK | MB_ICONERROR);
        return;
    }

    Product oldProduct;
    if (products.findProduct(id, oldProduct))
    {
        showMessage(L"商品号已存在。", L"错误", MB_OK | MB_ICONERROR);
        return;
    }

    string name = getIn(L"商品名称:");
    string makeDate = getIn(L"制作日期(YYYY-MM-DD):");
    string validDate = getIn(L"有效期(YYYY-MM-DD):");
    if (!isValidDate(makeDate) || !isValidDate(validDate) || makeDate > validDate)
    {
        showMessage(L"日期不合法。", L"错误", MB_OK | MB_ICONERROR);
        return;
    }

    int stock;
    if (!parseIntValue(getIn(L"库存:"), stock) || stock < 0)
    {
        showMessage(L"库存必须是非负整数。", L"错误", MB_OK | MB_ICONERROR);
        return;
    }

    if (products.addProduct(Product(id, name, makeDate, validDate, stock)))
    {
        saveAll();
        showMessage(L"商品添加成功。");
    }
    else
    {
        showMessage(L"商品添加失败。", L"错误", MB_OK | MB_ICONERROR);
    }
}

void GUIManager::removeProductDialog(string id)
{
    if (MessageBoxW(GetHWnd(), L"确认删除这个商品吗？", L"确认", MB_YESNO | MB_ICONQUESTION) != IDYES)
        return;

    if (products.removeProduct(id))
    {
        saveAll();
        showMessage(L"商品删除成功。");
    }
    else
    {
        showMessage(L"商品不存在，删除失败。", L"错误", MB_OK | MB_ICONERROR);
    }
}

void GUIManager::showProductWindow()
{
    int scrollIndex = 0;
    const int PAGE_SIZE = 11;

    while (true)
    {
        if (!isWindowAlive())
            return;
        processMsg();
        drawHeader(L"商品信息管理");

        vector<Product> allProducts = getProducts();
        adjustScroll(scrollIndex, (int)allProducts.size(), PAGE_SIZE);

        if (smallButton(34, 88, 116, 32, L"+ 添加商品", RGB(20, 155, 95)))
            addProductDialog();

        settextcolor(RGB(55, 65, 81));
        settextstyle(17, 0, L"微软雅黑");
        int y = 138;
        outtextxy(48, y, L"商品号");
        outtextxy(158, y, L"商品名称");
        outtextxy(382, y, L"制作日期");
        outtextxy(542, y, L"有效期");
        outtextxy(700, y, L"库存");
        outtextxy(840, y, L"操作");
        setlinecolor(RGB(205, 213, 224));
        line(34, y + 28, W - 34, y + 28);

        if (allProducts.empty())
        {
            settextcolor(RGB(148, 163, 184));
            settextstyle(26, 0, L"黑体");
            outtextxy(W / 2 - 60, 310, L"暂无商品");
        }
        else
        {
            for (int i = scrollIndex, count = 0; i < (int)allProducts.size() && count < PAGE_SIZE; i++, count++)
            {
                int rowY = y + 44 + count * 43;
                Product product = allProducts[i];

                setfillcolor(count % 2 == 0 ? RGB(255, 255, 255) : RGB(242, 246, 252));
                solidrectangle(34, rowY - 8, W - 34, rowY + 30);

                settextcolor(RGB(31, 41, 55));
                settextstyle(17, 0, L"微软雅黑");
                outTextFit(48, rowY, 88, toWide(product.getId()));
                outTextFit(158, rowY, 190, toWide(product.getName()));
                outTextFit(382, rowY, 130, toWide(product.getMakeDate()));
                outTextFit(542, rowY, 130, toWide(product.getValidDate()));
                outTextFit(700, rowY, 70, toWide(to_string(product.getStock())));

                if (smallButton(836, rowY - 4, 70, 28, L"删除", RGB(220, 70, 70)))
                    removeProductDialog(product.getId());
            }
        }

        if (button(W / 2 - 55, H - 76, 110, 42, L"返回"))
            return;
        // drawHint(L"提示：鼠标滚轮可翻页，添加或删除后会自动保存到 data 目录。");
        FlushBatchDraw();
        Sleep(10);
    }
}

bool GUIManager::inputSaleDialog(string recordId, SaleRecord &record, SaleRecord *oldRecord)
{
    string productId = getIn(L"商品号:");
    Product product;
    if (!products.findProduct(productId, product))
    {
        showMessage(L"商品不存在。", L"错误", MB_OK | MB_ICONERROR);
        return false;
    }

    string saleDate = getIn(L"销售日期(YYYY-MM-DD):");
    if (!isDateLogical(product.getMakeDate(), saleDate, product.getValidDate()))
    {
        showMessage(L"销售日期不合法，必须满足 制作日期 <= 销售日期 <= 有效期。", L"错误", MB_OK | MB_ICONERROR);
        return false;
    }

    double price, discount;
    int quantity;
    if (!parseDoubleValue(getIn(L"售价:"), price) || !isValidPrice(price))
    {
        showMessage(L"售价不能为负数。", L"错误", MB_OK | MB_ICONERROR);
        return false;
    }
    if (!parseDoubleValue(getIn(L"折扣(0~1):"), discount) || !isValidDiscount(discount))
    {
        showMessage(L"折扣必须在 0 到 1 之间。", L"错误", MB_OK | MB_ICONERROR);
        return false;
    }
    if (!parseIntValue(getIn(L"数量:"), quantity) || !isValidQuantity(quantity))
    {
        showMessage(L"数量必须为正整数。", L"错误", MB_OK | MB_ICONERROR);
        return false;
    }

    int available = product.getStock();
    if (oldRecord != nullptr && oldRecord->getProduct().getId() == productId)
        available += oldRecord->getQuantity();
    if (quantity > available)
    {
        showMessage(L"库存不足，当前可用库存为 " + toWide(to_string(available)) + L"。", L"错误", MB_OK | MB_ICONERROR);
        return false;
    }

    record = SaleRecord(recordId, product, saleDate, price, discount, quantity);
    return true;
}

void GUIManager::addSaleDialog()
{
    string recordId = createRecordId();
    SaleRecord record;
    if (!inputSaleDialog(recordId, record))
        return;

    Product product = record.getProduct();
    product.setStock(product.getStock() - record.getQuantity());
    if (!updateProduct(product))
    {
        showMessage(L"库存更新失败，登记取消。", L"错误", MB_OK | MB_ICONERROR);
        return;
    }

    if (sales.addRecord(record))
    {
        saveAll();
        showMessage(L"销售登记成功，记录号为 " + toWide(recordId) + L"。");
    }
    else
    {
        product.setStock(product.getStock() + record.getQuantity());
        updateProduct(product);
        showMessage(L"销售登记失败，记录号可能重复。", L"错误", MB_OK | MB_ICONERROR);
    }
}

void GUIManager::updateSaleDialog(string id)
{
    TreeNode *node = sales.searchRecord(id);
    if (node == nullptr)
    {
        showMessage(L"销售记录不存在。", L"错误", MB_OK | MB_ICONERROR);
        return;
    }

    SaleRecord oldRecord = node->data;
    SaleRecord newRecord;
    if (!inputSaleDialog(id, newRecord, &oldRecord))
        return;

    Product oldProduct;
    Product newProduct;
    if (!products.findProduct(oldRecord.getProduct().getId(), oldProduct) || !products.findProduct(newRecord.getProduct().getId(), newProduct))
    {
        showMessage(L"商品信息异常，修改失败。", L"错误", MB_OK | MB_ICONERROR);
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
        showMessage(L"销售记录修改成功。");
    }
    else
    {
        sales.addRecord(oldRecord);
        updateProduct(oldProduct);
        showMessage(L"销售记录修改失败，已恢复原记录。", L"错误", MB_OK | MB_ICONERROR);
    }
}

void GUIManager::removeSaleDialog(string id)
{
    TreeNode *node = sales.searchRecord(id);
    if (node == nullptr)
    {
        showMessage(L"销售记录不存在。", L"错误", MB_OK | MB_ICONERROR);
        return;
    }
    if (MessageBoxW(GetHWnd(), L"确认删除这条销售记录吗？", L"确认", MB_YESNO | MB_ICONQUESTION) != IDYES)
        return;

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
        showMessage(L"销售记录删除成功。");
    }
    else
    {
        showMessage(L"销售记录删除失败。", L"错误", MB_OK | MB_ICONERROR);
    }
}

void GUIManager::showSaleList(vector<SaleRecord> records, const wstring &title, bool editable)
{
    int scrollIndex = 0;
    const int PAGE_SIZE = 10;

    while (true)
    {
        if (!isWindowAlive())
            return;
        processMsg();
        drawHeader(title);

        if (editable)
            records = getSales();
        adjustScroll(scrollIndex, (int)records.size(), PAGE_SIZE);

        if (editable && smallButton(34, 88, 116, 32, L"+ 登记销售", RGB(20, 155, 95)))
            addSaleDialog();

        settextcolor(RGB(55, 65, 81));
        settextstyle(16, 0, L"微软雅黑");
        int y = 138;
        outtextxy(40, y, L"记录号");
        outtextxy(120, y, L"商品号");
        outtextxy(220, y, L"商品名称");
        outtextxy(390, y, L"销售日期");
        outtextxy(530, y, L"售价");
        outtextxy(620, y, L"折扣");
        outtextxy(700, y, L"数量");
        outtextxy(780, y, L"总价");
        if (editable)
            outtextxy(900, y, L"操作");
        setlinecolor(RGB(205, 213, 224));
        line(34, y + 28, W - 34, y + 28);

        if (records.empty())
        {
            settextcolor(RGB(148, 163, 184));
            settextstyle(26, 0, L"黑体");
            outtextxy(W / 2 - 70, 310, L"暂无销售记录");
        }
        else
        {
            for (int i = scrollIndex, count = 0; i < (int)records.size() && count < PAGE_SIZE; i++, count++)
            {
                int rowY = y + 44 + count * 45;
                SaleRecord record = records[i];
                Product product = record.getProduct();

                setfillcolor(count % 2 == 0 ? RGB(255, 255, 255) : RGB(242, 246, 252));
                solidrectangle(34, rowY - 8, W - 34, rowY + 31);

                settextcolor(RGB(31, 41, 55));
                settextstyle(16, 0, L"微软雅黑");
                outTextFit(40, rowY, 72, toWide(record.getId()));
                outTextFit(120, rowY, 88, toWide(product.getId()));
                outTextFit(220, rowY, 150, toWide(product.getName()));
                outTextFit(390, rowY, 120, toWide(record.getSaleDate()));
                outTextFit(530, rowY, 72, toWide(formatDouble(record.getPrice())));
                outTextFit(620, rowY, 60, toWide(formatDouble(record.getDiscount())));
                outTextFit(700, rowY, 58, toWide(to_string(record.getQuantity())));
                outTextFit(780, rowY, 88, toWide(formatDouble(getSaleTotal(record))));

                if (editable)
                {
                    if (smallButton(898, rowY - 4, 62, 28, L"修改", RGB(70, 130, 180)))
                        updateSaleDialog(record.getId());
                    if (smallButton(970, rowY - 4, 62, 28, L"删除", RGB(220, 70, 70)))
                        removeSaleDialog(record.getId());
                }
            }
        }

        if (button(W / 2 - 55, H - 76, 110, 42, L"返回"))
            return;
        // drawHint(editable ? L"提示：登记、修改、删除销售记录时会同步更新商品库存。" : L"提示：鼠标滚轮可翻页。");
        FlushBatchDraw();
        Sleep(10);
    }
}

void GUIManager::showSearchWindow()
{
    while (true)
    {
        if (!isWindowAlive())
            return;
        processMsg();
        drawHeader(L"销售记录查询");

        int x = 380;
        if (button(x, 120, 260, 42, L"按记录号查询"))
        {
            string key = getIn(L"记录号:");
            vector<SaleRecord> result;
            TreeNode *node = sales.searchRecord(key);
            if (node != nullptr)
                result.push_back(node->data);
            showSaleList(result, L"查询结果", false);
        }
        else if (button(x, 178, 260, 42, L"按商品号查询"))
        {
            string key = getIn(L"商品号:");
            vector<SaleRecord> result;
            for (SaleRecord record : getSales())
                if (record.getProduct().getId() == key)
                    result.push_back(record);
            showSaleList(result, L"查询结果", false);
        }
        else if (button(x, 236, 260, 42, L"按商品名称模糊查询"))
        {
            string key = getIn(L"商品名称关键词:");
            vector<SaleRecord> result;
            for (SaleRecord record : getSales())
                if (record.getProduct().getName().find(key) != string::npos)
                    result.push_back(record);
            showSaleList(result, L"查询结果", false);
        }
        else if (button(x, 294, 260, 42, L"按销售日期查询"))
        {
            string key = getIn(L"销售日期(YYYY-MM-DD):");
            if (!isValidDate(key))
            {
                showMessage(L"日期不合法。", L"错误", MB_OK | MB_ICONERROR);
                continue;
            }
            vector<SaleRecord> result;
            for (SaleRecord record : getSales())
                if (record.getSaleDate() == key)
                    result.push_back(record);
            showSaleList(result, L"查询结果", false);
        }
        else if (button(x, 352, 260, 42, L"按总价区间查询"))
        {
            double low, high;
            if (!parseDoubleValue(getIn(L"最低总价:"), low) || !parseDoubleValue(getIn(L"最高总价:"), high))
            {
                showMessage(L"请输入合法数字。", L"错误", MB_OK | MB_ICONERROR);
                continue;
            }
            if (low > high)
                swap(low, high);
            vector<SaleRecord> result;
            for (SaleRecord record : getSales())
                if (getSaleTotal(record) >= low && getSaleTotal(record) <= high)
                    result.push_back(record);
            showSaleList(result, L"查询结果", false);
        }
        else if (button(x, 410, 260, 42, L"按数量区间查询"))
        {
            int low, high;
            if (!parseIntValue(getIn(L"最低数量:"), low) || !parseIntValue(getIn(L"最高数量:"), high))
            {
                showMessage(L"请输入合法整数。", L"错误", MB_OK | MB_ICONERROR);
                continue;
            }
            if (low > high)
                swap(low, high);
            vector<SaleRecord> result;
            for (SaleRecord record : getSales())
                if (record.getQuantity() >= low && record.getQuantity() <= high)
                    result.push_back(record);
            showSaleList(result, L"查询结果", false);
        }
        else if (button(x, 468, 260, 42, L"当日精确查询"))
        {
            string saleDate = getIn(L"销售日期(YYYY-MM-DD):");
            string key = getIn(L"商品号或完整商品名称:");
            vector<SaleRecord> result;
            for (SaleRecord record : getSales())
            {
                Product product = record.getProduct();
                if (record.getSaleDate() == saleDate && (product.getId() == key || product.getName() == key))
                    result.push_back(record);
            }
            showSaleList(result, L"查询结果", false);
        }
        else if (button(x, 526, 260, 42, L"当日模糊查询"))
        {
            string saleDate = getIn(L"销售日期(YYYY-MM-DD):");
            string key = getIn(L"商品号或商品名称关键词:");
            vector<SaleRecord> result;
            for (SaleRecord record : getSales())
            {
                Product product = record.getProduct();
                bool matchProduct = product.getId().find(key) != string::npos || product.getName().find(key) != string::npos;
                if (record.getSaleDate() == saleDate && matchProduct)
                    result.push_back(record);
            }
            showSaleList(result, L"查询结果", false);
        }
        else if (button(x, 592, 260, 42, L"返回"))
        {
            return;
        }

        // drawHint(L"查询结果会在独立窗口中显示，返回后可继续选择其他查询条件。");
        FlushBatchDraw();
        Sleep(10);
    }
}

bool GUIManager::cmpByDate(SaleRecord a, SaleRecord b)
{
    return a.getSaleDate() < b.getSaleDate();
}

bool GUIManager::cmpByTotal(SaleRecord a, SaleRecord b)
{
    return a.getPrice() * a.getDiscount() * a.getQuantity() < b.getPrice() * b.getDiscount() * b.getQuantity();
}

bool GUIManager::cmpByQuantity(SaleRecord a, SaleRecord b)
{
    return a.getQuantity() < b.getQuantity();
}

void GUIManager::showSortWindow()
{
    while (true)
    {
        if (!isWindowAlive())
            return;
        processMsg();
        drawHeader(L"销售记录排序");

        int x = 400;
        if (button(x, 180, 240, 46, L"按销售日期排序"))
        {
            vector<SaleRecord> records = getSales();
            sort(records.begin(), records.end(), cmpByDate);
            showSaleList(records, L"按销售日期排序", false);
        }
        else if (button(x, 250, 240, 46, L"按总价排序"))
        {
            vector<SaleRecord> records = getSales();
            sort(records.begin(), records.end(), cmpByTotal);
            showSaleList(records, L"按总价排序", false);
        }
        else if (button(x, 320, 240, 46, L"按数量排序"))
        {
            vector<SaleRecord> records = getSales();
            sort(records.begin(), records.end(), cmpByQuantity);
            showSaleList(records, L"按数量排序", false);
        }
        else if (button(x, 410, 240, 46, L"返回"))
        {
            return;
        }

        // drawHint(L"排序结果只用于查看，不改变文件中销售记录的存储顺序。");
        FlushBatchDraw();
        Sleep(10);
    }
}

void GUIManager::showStatisticsWindow()
{
    int mode = 1;
    while (true)
    {
        if (!isWindowAlive())
            return;
        processMsg();
        drawHeader(L"销售统计");

        if (smallButton(34, 88, 116, 32, L"按商品号", mode == 1 ? RGB(35, 120, 210) : RGB(120, 132, 150)))
            mode = 1;
        if (smallButton(164, 88, 116, 32, L"按销售日期", mode == 2 ? RGB(35, 120, 210) : RGB(120, 132, 150)))
            mode = 2;

        struct StatisticItem
        {
            string productName;
            int quantity;
            double amount;
            StatisticItem() : quantity(0), amount(0) {}
        };

        map<string, StatisticItem> statistics;
        int totalQuantity = 0;
        double totalAmount = 0;
        for (SaleRecord record : getSales())
        {
            Product product = record.getProduct();
            string key = (mode == 1) ? product.getId() : record.getSaleDate();
            if (mode == 1)
                statistics[key].productName = product.getName();
            statistics[key].quantity += record.getQuantity();
            statistics[key].amount += getSaleTotal(record);
            totalQuantity += record.getQuantity();
            totalAmount += getSaleTotal(record);
        }

        drawMetric(34, 142, 220, L"总销售数量", toWide(to_string(totalQuantity)), RGB(20, 120, 90));
        drawMetric(276, 142, 220, L"总销售额", toWide(formatDouble(totalAmount)), RGB(40, 100, 190));

        int y = 280;
        settextcolor(RGB(55, 65, 81));
        settextstyle(18, 0, L"微软雅黑");
        outtextxy(70, y, mode == 1 ? L"商品号" : L"销售日期");
        if (mode == 1)
        {
            outtextxy(250, y, L"商品名称");
            outtextxy(500, y, L"销售数量");
            outtextxy(700, y, L"销售总额");
        }
        else
        {
            outtextxy(300, y, L"销售数量");
            outtextxy(520, y, L"销售总额");
        }
        setlinecolor(RGB(205, 213, 224));
        line(34, y + 30, W - 34, y + 30);

        int count = 0;
        for (auto item : statistics)
        {
            int rowY = y + 48 + count * 42;
            setfillcolor(count % 2 == 0 ? RGB(255, 255, 255) : RGB(242, 246, 252));
            solidrectangle(34, rowY - 8, W - 34, rowY + 30);
            settextcolor(RGB(31, 41, 55));
            settextstyle(17, 0, L"微软雅黑");
            outTextFit(70, rowY, 190, toWide(item.first));
            if (mode == 1)
            {
                outTextFit(250, rowY, 210, toWide(item.second.productName));
                outTextFit(500, rowY, 150, toWide(to_string(item.second.quantity)));
                outTextFit(700, rowY, 170, toWide(formatDouble(item.second.amount)));
            }
            else
            {
                outTextFit(300, rowY, 150, toWide(to_string(item.second.quantity)));
                outTextFit(520, rowY, 170, toWide(formatDouble(item.second.amount)));
            }
            count++;
            if (count >= 8)
                break;
        }

        if (statistics.empty())
        {
            settextcolor(RGB(148, 163, 184));
            settextstyle(26, 0, L"黑体");
            outtextxy(W / 2 - 70, 420, L"暂无销售记录");
        }

        if (button(W / 2 - 55, H - 76, 110, 42, L"返回"))
            return;
        // drawHint(L"统计项较多时显示前 8 条；完整明细可通过导出功能查看。");
        FlushBatchDraw();
        Sleep(10);
    }
}

void GUIManager::exportByDateDialog()
{
    string saleDate = getIn(L"导出日期(YYYY-MM-DD):");
    if (!isValidDate(saleDate))
    {
        showMessage(L"日期不合法。", L"错误", MB_OK | MB_ICONERROR);
        return;
    }

    ensureDataDirectory();
    string filename = DATA_DIR + "/export_" + saleDate + ".txt";
    if (FileManager::exportTodaySales(sales, saleDate, filename))
        showMessage(L"导出成功，文件名: " + toWide(filename));
    else
        showMessage(L"导出失败。", L"错误", MB_OK | MB_ICONERROR);
}

void GUIManager::run()
{
    initgraph(W, H);
    BeginBatchDraw();

    while (true)
    {
        if (!isWindowAlive())
            break;
        processMsg();
        drawHeader(L"面包店销售管理系统");

        vector<Product> allProducts = getProducts();
        vector<SaleRecord> allSales = getSales();
        int totalQuantity = 0;
        double totalAmount = 0;
        for (SaleRecord record : allSales)
        {
            totalQuantity += record.getQuantity();
            totalAmount += getSaleTotal(record);
        }

        drawMetric(34, 100, 220, L"商品种类", toWide(to_string((int)allProducts.size())), RGB(36, 120, 210));
        drawMetric(276, 100, 220, L"销售记录", toWide(to_string((int)allSales.size())), RGB(20, 145, 100));
        drawMetric(518, 100, 220, L"销售总数", toWide(to_string(totalQuantity)), RGB(160, 100, 40));
        drawMetric(760, 100, 270, L"销售总额", toWide(formatDouble(totalAmount)), RGB(170, 80, 120));

        int x = 132, y = 245, w = 220, h = 54;
        if (button(x, y, w, h, L"商品管理"))
            showProductWindow();
        else if (button(x + 300, y, w, h, L"销售管理"))
            showSaleList(allSales, L"销售记录管理", true);
        else if (button(x + 600, y, w, h, L"销售查询"))
            showSearchWindow();
        else if (button(x, y + 86, w, h, L"销售排序"))
            showSortWindow();
        else if (button(x + 300, y + 86, w, h, L"销售统计"))
            showStatisticsWindow();
        else if (button(x + 600, y + 86, w, h, L"导出销售"))
            exportByDateDialog();
        else if (button(x + 150, y + 172, w, h, L"保存数据", RGB(45, 145, 95)))
        {
            saveAll();
            showMessage(L"数据已保存。");
        }
        else if (button(x + 450, y + 172, w, h, L"退出系统", RGB(180, 70, 70)))
        {
            saveAll();
            break;
        }

        // drawHint(L"可视化版本复用原项目链表、AVL 树和文件读写；编译时只需要编译 main_visualization.cpp。");
        FlushBatchDraw();
        Sleep(10);
    }

    EndBatchDraw();
    closegraph();
}
