#include "controller.h"
controller::controller()
    : Ocr(xPath, (path)"ocr\\known\\test", (path)"ocr\\known\\train", (path)"ocr\\known\\digits")
{
    xrect = { 0,0,0,0 };
    count = 1;
    xPath = current_path() / "data\\pages\\";
    Ocr::Initialize();
    positions.push_back(7);
    positions.push_back(5);
    positions.push_back(3);
    positions.push_back(1);
    positions.push_back(2);
    positions.push_back(4);
    positions.push_back(6);
    positions.push_back(8);
}
controller::~controller() {

}
//для тестов
void controller::InitLight() {
    Emulator.Initialize();
    if (myError != Warnings::NO_WARNING)
    {
        fixErr();
    }
    Profile.setPremiumStatus(true);
    Profile.setPower(9000000);
    Profile.setID(69334893);
    squads::count = 6;
    return;
}
bool controller::createMask(int x, int y, int width, int height) {
    Mat Mask(720, 1280, CV_8UC3, Scalar(255, 255, 255));
    Rect square(x, y, width, height);
    rectangle(Mask, square, Scalar(0, 0, 0), -1);
    img = Mask;
    if (img.empty()) return false;
    return true;
}
//

//обработчик ошибок
void controller::fixErr() {
    //switch (myError)
    //case Warnings::FAIL_CHECK:
    //case Warnings::FAIL_COMPARE:
    //case Warnings::WRONG_EMULATOR_NAME:
    //case Warnings::WRONG_EMULATOR_SIZE:
    //case Warnings::NO_ACTIVE_EMULATOR:
    //case Warnings::FAIL_INIT:
    
}
void controller::getErr() {
    //switch (myError)
    //case Warnings::FAIL_CHECK:
    //case Warnings::FAIL_COMPARE:
    //case Warnings::WRONG_EMULATOR_SIZE:
    //case Warnings::NO_ACTIVE_EMULATOR:
    //case Warnings::FAIL_INIT:
}
//

//User
int controller::getUPower() {
    return Profile.getPower();
}
int controller::getUID() {
    return Profile.getID();
}

void controller::Initialize(int instance) {
    cout << "Запускаю бота\n";
    Emulator.Initialize();
    if (myError == Warnings::FAIL_INIT)
    {
        myError = Warnings::NO_WARNING;
        if (Profile.Start(instance))
        {
            cout << "Запускаю и нахожу лаунчер\n";
            Emulator.FindWin();
            if (myError != Warnings::NO_WARNING) 
            {
                getErr();
                return;
            }
            cout << "Нашёлся. Запускаю игру\n";
            while (!checkLoad());
            Emulator.Initialize();
            if (myError != Warnings::NO_WARNING)
            {
                fixErr();
            }
        }
        else 
        {
            myError = Warnings::NO_ACTIVE_EMULATOR;
            getErr();
            return;
        }
    }
    else if (myError == Warnings::WRONG_EMULATOR_SIZE)
    {
        fixErr();
    }
    else cout << "Нашёл лаунчер.";
    cout << "Жду пока загрузится\n";

    checkLoadMain();
    cout << "Загрузился, проверяю главную страницу\n";
    checkMain();
    if (myError != Warnings::NO_WARNING)
    {
        myError = Warnings::NO_WARNING;
        if (checkEvent()) 
        {
            skipEvent();
            checkMain();
            if (myError != Warnings::NO_WARNING) 
            {
                getErr();
                return;
            }
        }
        else
        {
            myError = Warnings::FAIL_CHECK;
            getErr();
            return;
        }
    }
    cout << "Инициализирую игрока\n";
    InitUser();
    if (myError != Warnings::NO_WARNING)
    {
        cout << "Игрок не найден по каким-либо причинам\n";
        getErr();
        return;
    }
    else Profile.Print();
    cout << "Проверяю настройки и подписку\n";
    if (!Profile.getSubscribeStatus()) goto status;
    checkSettings();

    if (myError != Warnings::NO_WARNING) 
    {
        getErr();
        return;
    }
    cout << "Настройки правильные, начинаю работу\n";
    checkLoadMain();
    checkMain();
    if(myError!=Warnings::NO_WARNING)
    {
        getErr();
        return;
    }
    setMainPage();
    findBarrack();
    if (myError == Warnings::NO_WARNING)
    {
        entryBarrack();
        if (myError == Warnings::NO_WARNING)
        {
            InitSquadCount();
            if (myError != Warnings::NO_WARNING)
            {
                getErr();
                return;
            }
            else
            {
                cout << "Ваша казарма вмещает: " << squads::count << " бойцов" << endl;
                ClickEsc();
                ClickEsc();
            }
        }
        else
        {
            getErr();
            return;
        }
    }
    else
    {
        getErr();
        return;
    }
    return;
status:
    cout << "Без подписки бан!" << endl;
    return;
}
void controller::InitUser() {
    ClickButton(xPath / "main", "button_user");
    Sleep(3000);
    if (!CompareSample(xPath / "user", "sample", "compare")) goto warning;
    setMask((xPath / "user\\user_id.png").generic_string());
    FindObj();
    Profile.setID(Recognize(CutImg()));
    if (Profile.getID() <= 0) goto warning;
    Profile.setSubscribeStatus(true);//вместо тру какая-нибудь функция которая сразу проверит и возвратит тру ор фолс
    setMask((xPath / "user\\user_power.png").generic_string());
    FindObj();
    Profile.setPower(Recognize(CutImg()));
    if (CompareSample(xPath / "user", "sample_prem", "state_prem")) Profile.setPremiumStatus(true);
    else Profile.setPremiumStatus(false);
    ClickButton(xPath / "user", "button_close");
    return;
warning:
    myError = Warnings::FAIL_COMPARE;
    return;
}
void controller::InitSquadCount() {
    setSample("C:/bot/data/pages/squad/main/sample_barrack.png");
    setMask("C:/bot/data/pages/squad/main/count.png");
    Screenshot();
    FindObj();
    squads::count = Recognize(CutImg());
    if (count == 0) myError = Warnings::FAIL_COMPARE;
    return;
}

bool controller::getPremiumStatus() {
    return Profile.getPremiumStatus();
}
//

//Squad
int controller::getBarrackPower() {
    findBarrack();
    int x = 0;
    if (myError != Warnings::NO_WARNING) goto warning;
    entryBarrack();
    if (myError != Warnings::NO_WARNING) goto warning;
    setMask((xPath / "squad\\main\\power.png").generic_string());
    FindObj();
    if (myError != Warnings::NO_WARNING) goto warning;
    x = Recognize(changeColour(CutImg()));
    ClickEsc();
    ClickEsc();
    return x;
warning:
    getErr();
    return -1;
}

void controller::setKit(int pos,int k) {
    bool res = false;
    if (pos > squads::count)
    {
        myError = Warnings::FAIL_SETTING;//типа если нет такой позиции при кол-ве казармы
        return;
    }
    int start, end, * unit;
    unit = new int[squads::count];
    switch (squads::count) 
    {
    case 8:
        start = 0;
        end = 7;
        break;
    case 7:
        start = 0;
        end = 6;
        break;
    case 6:
        start = 1;
        end = 6;
        break;
    case 5:
        start = 1;
        end = 5;
        break;
    case 4:
        start = 2;
        end = 5;
        break;
    case 3:
        start = 2;
        end = 4;
        break;
    case 2:
        start = 3;
        end = 4;
        break;
    }
    for (int i = start, j = 0; i <= end; ++i, j++) 
    {
        unit[j] = positions[i];
    }
    if (CompareSample(xPath / "squad\\main", "sample", ("unit_" + to_string(unit[pos])))) 
    {
        delete[] unit;
        myError = Warnings::FAIL_SETTING;
        return;//нет бойца
    }
    Click(xrect.x + 3, xrect.y + 6, 2000);
    Sleep(3000);
    if (!CompareSample(xPath / "squad\\main\\unit", "sample", "compare")) goto warning;
    ClickButton(xPath / "squad\\main\\unit", "button_set");
    if (!CompareSample(xPath / "squad\\main\\unit", "sample_set", "compare_set")) goto warning;
    if (k == 0) 
    {
        setImg("C:/bot/data/pages/squad/main/unit/sample_set_0.png");
        setMask("C:/bot/data/pages/squad/main/unit/state_0.png");
        FindObj();
        if (myError != Warnings::NO_WARNING) return;
        Mat find = CutImg();
        Screenshot();
        res = true;
        int x = 0;
        do 
        {
            if (!Compare(find, img, 0.1)) 
            {
                x++;
                Sleep(1000);
                if (x == 5) goto warning;
            }
            else res = false;
        } while (res);
        Click();
        ClickEsc();
        ClickEsc();
        return;
    }
    res = false;
    for (int i = k; i < 4; i++) 
    {
        if (CompareSample(xPath / "squad\\main\\unit", "set_" + to_string(i), "state_set")) res = true;
    }
    if (!res) 
    {
        ClickEsc();
        ClickEsc();
        return;
    }
    ClickButton(xPath / "squad\\main\\unit", "button_" + to_string(k));
    if (CompareSample(xPath / "squad\\main\\unit", "sample_confirm", "compare_confirm"))
    {
        ClickButton(xPath / "squad\\main\\unit", "button_confirm");
        ClickEsc();
        return;
    }
    else
    {
        ClickEsc();
        ClickEsc();
        return;
    }
    return;
warning:
    myError = Warnings::FAIL_COMPARE;
    return;
}
//

//Game
bool controller::FindObj(Mat finder) {
    if(!finder.empty())
    {
        mask = finder;
    }
    if (mask.empty()) 
    { 
        myError = Warnings::EMPTY_IMG;
        return false; 
    }
    // Преобразуем изображение в оттенки серого
    Mat grayImage;
    cvtColor(mask, grayImage, COLOR_BGR2GRAY);
    // Бинаризуем изображение
    Mat binaryImage;
    threshold(grayImage, binaryImage, 127, 255, THRESH_BINARY_INV);
    // Находим контуры
    vector<vector<Point>> contours;
    findContours(binaryImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    // Находим прямоугольник, описывающие контур
    xrect = boundingRect(contours[0]);
    if (xrect.width == 0) return false;
    return true;
}
bool controller::Compare(Mat Img, Mat Sample, double rightVal) {
    if (!Img.empty())
    {
        img = Img;
    }
    if (!Sample.empty())
    {
        sample = Sample;
    }
    if (img.empty() || sample.empty())
    {
        myError = Warnings::EMPTY_IMG;
        return false;
    }
    Mat img1(img), sample1(sample);
    xrect.x = 0;
    xrect.y = 0;

    //для отслежки
    /* imshow("1", img1);
     imshow("2", sample1);
     waitKey(0);*/
    //

    cvtColor(img1, img1, COLOR_BGR2GRAY);
    cvtColor(sample1, sample1, COLOR_BGR2GRAY);
    // Создание матрицы результата
    int res_col = sample1.cols - img1.cols + 1;
    int res_row = sample1.rows - img1.rows + 1;
    Mat result(res_row, res_col, CV_32FC1);

    // Сравнение шаблона с изображением
    matchTemplate(img1, sample1, result, TM_SQDIFF_NORMED);

    // Нахождение наилучшего совпадения
    double minVal = 0;
    Point minLoc;
    minMaxLoc(result, &minVal, nullptr, &minLoc, nullptr);
    xrect.x = minLoc.x;
    xrect.y = minLoc.y;

    //cout << "x:" << xrect.x << " y:" << xrect.y << " minval " << minVal << endl;
    
    // Проверка наилучшего совпадения
    if (minVal <= rightVal) 
    {
        return true;
    }
    return false;
}
bool controller::CompareSample(path pagePath, string samplePath, string maskPath, bool Screen, double rightVal) {
    if(Screen)
    {
        Screenshot();
    }
    //Нахождение координат обрезаемой области(FindObj)
    Mat img1 = imread((pagePath / maskPath).generic_string() + ".png");
    Mat sample1(720, 1280, CV_8UC4);
    cvtColor(img1, img1, COLOR_BGR2GRAY);
    threshold(img1, sample1, 127, 255, THRESH_BINARY_INV);
    vector<vector<Point>> contours;
    findContours(sample1, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    xrect = boundingRect(contours[0]);
    if (img.empty())
    {
        myError = Warnings::EMPTY_IMG;
        return false;
    }
    //Обрезка нужного объекта
    img1 = img(xrect);
    sample1 = imread((pagePath / samplePath).generic_string() + ".png");
    if (sample1.empty())
    {
        myError = Warnings::EMPTY_IMG;
        return false;
    }

    //для тестов
    /*imshow("1", img);
    imshow("5", img1);
    imshow("2", sample1);
    waitKey(0);*/

    //Поиск объекта
    cvtColor(img1, img1, COLOR_BGR2GRAY);
    cvtColor(sample1, sample1, COLOR_BGR2GRAY);
    int res_col = sample1.cols - img1.cols + 1;
    int res_row = sample1.rows - img1.rows + 1;
    Mat result(res_row, res_col, CV_32FC1);
    matchTemplate(img1, sample1, result, TM_SQDIFF_NORMED);
    double minVal = 0;
    Point minLoc;
    minMaxLoc(result, &minVal, nullptr, &minLoc, nullptr);
    if (minVal < rightVal) 
    {
        return true;
    }
    return false;
}
bool controller::checkTime(int hour, int min) {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm utc_time;
    gmtime_s(&utc_time, &now_time);
    if (utc_time.tm_hour >= hour && utc_time.tm_min >= min) 
    {
        return true;
    }
    return false;
}
bool controller::writeMessage(const char* text, string pathPage) {}

void controller::setMainPage() {
    checkMain();
    if (myError != Warnings::NO_WARNING) 
    {
        //later
    }
    ClickButton(xPath / "main", "button_friends");
    int x = 0;
    while (!CompareSample(xPath / "top_players", "sample", "compare_top1", true))
    {
        x++;
        if (x == 100) myError = Warnings::FAIL_CHECK;
        return;
    }
    //if (!CompareSample(xPath / "top_players", "sample", "compare")) return;
    Click(618, 239);
    Click(618, 239);
    while (!CompareSample(xPath / "top_players", "sample_top", "compare_top", true)) Sleep(500);
    ClickEsc();
    checkMain();
    if (myError != Warnings::NO_WARNING)
    {
        //later
    }
    Sleep(500);
}
void controller::checkSettings() {
    ClickButton(xPath / "main", "button_settings");
    if (!CompareSample(xPath / "settings", "sample", "compare",true)) goto warning;
    if (!CompareSample(xPath / "settings", "sample", "state_fps")) ClickButton(xPath / "settings", "button_fps");
    if (!CompareSample(xPath / "settings", "sample", "state_HD")) ClickButton(xPath / "settings", "button_HD"); // не уверен что работает, хз как переключается HD
    if (!CompareSample(xPath / "settings", "sample", "state_lang")) {
        ClickButton(xPath / "settings", "button_lang");
        if (!CompareSample(xPath / "settings", "sample_lang", "compare_lang",true)) goto warning;
        ClickButton(xPath / "settings", "button_en");
        if (!CompareSample(xPath / "settings", "sample_confirm", "compare_confirm",true)) goto warning;
        ClickButton(xPath / "settings", "button_yes");
    }
    else ClickEsc();
    return;
warning:
    myError = Warnings::FAIL_COMPARE;
    return;
}
//

//Client
void controller::isEmpty() {
    if (Emulator.getmHandle() == (HWND)0) myError = Warnings::NO_ACTIVE_EMULATOR;
}
void controller::isValidSize() {
    RECT rect = Emulator.getmWin();
    Point point = Emulator.getgSize();
    if ((rect.right - rect.left) != point.x || (rect.bottom - rect.top) != point.y) myError = Warnings::WRONG_EMULATOR_SIZE;
}
void controller::setValidSize() {
    Emulator.setValidSize();
}

//

//Checker

//

//Image
void controller::Screenshot() {
    Point point = Emulator.getgSize();
    HWND hwnd = Emulator.getgHandle();
    int xWidth = point.x; // 1280
    int xHeight = point.y; // 720
    HDC hdcWindow = GetDC(hwnd);
    Mat res(xHeight, xWidth, CV_8UC4);
    RECT rc;
    GetClientRect(hwnd, &rc);
    HDC hdcMem = CreateCompatibleDC(hdcWindow);
    HBITMAP bitmap = CreateCompatibleBitmap(hdcWindow, rc.right - rc.left, rc.bottom - rc.top);
    HGDIOBJ OBJ = SelectObject(hdcMem, bitmap);
    SelectObject(hdcMem, bitmap);
    BitBlt(hdcMem, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcWindow, 0, 0, SRCCOPY);
    SelectObject(hdcMem, OBJ);
    HDC hdcRar = GetDC(NULL);
    SelectObject(hdcRar, bitmap);
    StretchBlt(hdcRar, 0, 0, xWidth, xHeight, hdcMem, 0, 0, xWidth, xHeight, SRCCOPY);
    BITMAPINFOHEADER bi = { sizeof(BITMAPINFOHEADER), xWidth, xHeight, 1, 32 };
    GetDIBits(hdcMem, bitmap, 0, xHeight, res.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    DeleteObject(bitmap);
    DeleteDC(hdcMem);
    DeleteDC(hdcRar);
    ReleaseDC(hwnd, hdcWindow);
    flip(res, img, 0);
}
bool controller::setImg(Mat Img) {
    if (Img.empty())
    {
        myError = Warnings::EMPTY_IMG;
        return false;
    }
    img = Img;
    return true;
}
bool controller::setImg(string path) {
    img = imread(path.c_str());
    if (img.empty())
    {
        myError = Warnings::WRONG_IMG_PATH;
        return false;
    }
    return true;
}
bool controller::setMask(Mat Mask) {
    if (Mask.empty())
    {
        myError = Warnings::EMPTY_IMG;
        return false;
    }
    mask = Mask;
    return true;
}
bool controller::setMask(string path) {
    mask = imread(path.c_str());
    if (mask.empty())
    {
        myError = Warnings::WRONG_IMG_PATH;
        return false;
    }
    return true;
}
bool controller::setSample(Mat Sample) {
    if (Sample.empty())
    {
        myError = Warnings::EMPTY_IMG;
        return false;
    }
    sample = Sample;
    return true;
}
bool controller::setSample(string path) {
    sample = imread(path.c_str());
    if (sample.empty())
    {
        myError = Warnings::WRONG_IMG_PATH;
        return false;
    }
    return true;
}
bool controller::SaveImg(string savePath, Mat Img) {
    if (Img.empty()) 
    {
        Img = img;
    }
    if (Img.empty())
    {
        myError = Warnings::EMPTY_IMG;
        return false;
    }
    if (imwrite(savePath.c_str(), Img)) return true;
    else
    {
        myError = Warnings::WRONG_IMG_PATH;
    }
    return false;
}
Mat controller::getImg() { return img; }
Mat controller::getMask() { return mask; }
Mat controller::getSample() { return sample; }
Mat controller::CutImg() { return img(xrect); }
Mat controller::changeColour(Mat Img) {
    // Создаем выходное изображение
    Mat image = Img;

    //почему? не знаю
    cvtColor(image, image, COLOR_RGB2BGR);
    cvtColor(image, image, COLOR_BGR2RGB);
    //

    // Определяем диапазоны цветов
    Scalar lowerWhite(200, 200, 200); // Нижний предел для белого
    Scalar upperWhite(255, 255, 255); // Верхний предел для белого

    Scalar lowerBlackBlue(0, 0, 0); // Нижний предел для черного
    Scalar upperBlackBlue(100, 100, 100); // Верхний предел для черного

    Scalar lowerBlue(100, 150, 0); // Нижний предел для голубого
    Scalar upperBlue(255, 255, 100); // Верхний предел для голубого

    // Цвета для замены
    Vec3b dullGreen(144, 238, 144); // светло-зеленый
    Vec3b white(255, 255, 255); // белый

    // Проходим по каждому пикселю изображения
    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++) 
        {
            Vec3b pixel = image.at<Vec3b>(y, x);
            if (pixel[0] >= lowerWhite[0] && pixel[0] <= upperWhite[0] &&
                pixel[1] >= lowerWhite[1] && pixel[1] <= upperWhite[1] &&
                pixel[2] >= lowerWhite[2] && pixel[2] <= upperWhite[2]) 
            {
                // Заменяем белый на зеленый
                image.at<Vec3b>(y, x) = dullGreen;
            }
            else if ((pixel[0] >= lowerBlackBlue[0] && pixel[0] <= upperBlackBlue[0] &&
                pixel[1] >= lowerBlackBlue[1] && pixel[1] <= upperBlackBlue[1] &&
                pixel[2] >= lowerBlackBlue[2] && pixel[2] <= upperBlackBlue[2]) ||
                (pixel[0] >= lowerBlue[0] && pixel[0] <= upperBlue[0] &&
                    pixel[1] >= lowerBlue[1] && pixel[1] <= upperBlue[1] &&
                    pixel[2] >= lowerBlue[2] && pixel[2] <= upperBlue[2]))
            {
                // Заменяем черный или голубой на белый
                image.at<Vec3b>(y, x) = white;
            }
        }
    }
    return image;
}

Rect controller::getRect() { return xrect; }
//

//Buttons
void controller::Click(int sleep) {
    HWND hwnd = Emulator.getgHandle();
    HWND wnd = Emulator.getmHandle();
    SendMessage(wnd, WM_SETFOCUS, 0, 0);
    PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(xrect.x + 5, xrect.y + 5));
    Sleep(sleep);
    PostMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(xrect.x + 5, xrect.y + 5));
    Sleep(1000);
}
void controller::Click(int x, int y, int sleep) {
    HWND hwnd = Emulator.getgHandle();
    HWND wnd = Emulator.getmHandle();
    SendMessage(wnd, WM_SETFOCUS, 0, 0);
    PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
    Sleep(sleep);
    PostMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
    Sleep(1000);
}
void controller::ClickLong(int x, int y, int x1, int y1) {
    HWND hwnd = Emulator.getgHandle();
    HWND wnd = Emulator.getmHandle();
    auto start = Vec2i(x, y);
    auto stop = Vec2i(x1, y1);
    auto path = stop - start;
    Vec2f path_dir = path;
    path_dir = normalize(path_dir);
    SendMessage(wnd, WM_SETFOCUS, 0, 0);
    BYTE arr[256];
    memset(arr, 0, sizeof(256));
    GetKeyboardState(arr);
    auto old = arr[VK_LBUTTON];
    arr[VK_LBUTTON] = 128;
    SetKeyboardState(arr);

    SendMessage(hwnd, WM_MOUSEACTIVATE, (WPARAM)wnd, MAKELPARAM(HTCLIENT, WM_LBUTTONDOWN));
    Sleep(15);
    SendMessage(hwnd, WM_SETCURSOR, (WPARAM)hwnd, MAKELPARAM(HTCLIENT, WM_LBUTTONDOWN));
    Sleep(15);
    PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(start[0], start[1]));
    Sleep(15);

    const int count = int(norm(path));
    Vec2i last_pos;

    for (int i = 0; i < count; ++i)
    {
        auto pos = Vec2i(path_dir * float(i));
        last_pos = start + pos;

        SetKeyboardState(arr);
        PostMessage(hwnd, WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(last_pos[0], last_pos[1]));

        const int pause = 50;
        const int offset = 15;
        if (i < offset)
        {
            auto p = 10 + ((offset - (i + 1)) * pause) / offset;
            Sleep(p);
        }
        else if (i > count - offset)
        {
            auto p = 30 + ((offset - (count - i)) * pause) / offset;
            Sleep(p);
        }
        else if (i % 5 == 0)
            Sleep(1);
    }

    PostMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(last_pos[0], last_pos[1]));
    Sleep(15);
    SendMessage(hwnd, WM_SETCURSOR, (WPARAM)hwnd, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
    Sleep(15);

    arr[VK_LBUTTON] = old;
    SetKeyboardState(arr);
}
void controller::ClickEsc() {
    HWND hwnd = Emulator.getgHandle();
    PostMessage(hwnd, WM_KEYDOWN, VK_ESCAPE, MapVirtualKey(VK_ESCAPE, MAPVK_VK_TO_VSC));
    Sleep(300);
    PostMessage(hwnd, WM_KEYUP, VK_ESCAPE, MapVirtualKey(VK_ESCAPE, MAPVK_VK_TO_VSC));
    Sleep(1000);
}
void controller::ClickReturn() {
    HWND hwnd = Emulator.getgHandle();
    PostMessage(hwnd, WM_KEYDOWN, VK_RETURN, 0);
    Sleep(300);
    PostMessage(hwnd, WM_KEYUP, VK_RETURN, 0);
    Sleep(1000);
}

bool controller::ClickButton(path pagePath, string buttonPath, int sleep) {
    mask.release();//аккуратно с этим
    mask = imread((pagePath / buttonPath).generic_string() + ".png"); // buttonPath = button_****
    if (mask.empty())
    {
        myError = Warnings::WRONG_IMG_PATH;
        return false;
    }
    if (FindObj()) 
    {
        Click(sleep);
        return true;
    }
    return false;
}
//

//Filesystem
path controller::getPath() { return xPath; }
//