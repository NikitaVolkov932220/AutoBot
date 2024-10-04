#include "controller.h"
controller::controller()
    : Ocr(xPath, (path)"ocr\\known\\test", (path)"ocr\\known\\train", (path)"ocr\\known\\digits")
{
    point = { 0,0 };
    rect = { 0,0,0,0 };
    xrect = { 0,0,0,0 };
    count = 1;
    xPath = current_path() / "data\\pages";
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
//////для тестов
void controller::InitLight() {
    Emulator.Initialize();
    if (myError != Warnings::NO_WARNING) {
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
//////

////обработчик ошибок
void controller::fixErr() {
    switch (myError) {
    case Warnings::FAIL_CHECK:
    case Warnings::FAIL_COMPARE:
    case Warnings::WRONG_EMULATOR_NAME:
    case Warnings::WRONG_EMULATOR_SIZE:
    case Warnings::NO_ACTIVE_EMULATOR:
    case Warnings::FAIL_INIT:
    }
}
void controller::getErr() {
    switch (myError) {
    case Warnings::FAIL_CHECK:
    case Warnings::FAIL_COMPARE:
    case Warnings::WRONG_EMULATOR_SIZE:
    case Warnings::NO_ACTIVE_EMULATOR:
    case Warnings::FAIL_INIT:
    }
}
////

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
        myError == Warnings::NO_WARNING;
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
    if (!CompareSample(xPath / "user", "sample", "compare")) {
        myError = Warnings::FAIL_COMPARE;
        return;
    }
    setMask((xPath / "user\\user_id.png").generic_string());
    FindObj();
    Profile.setID(Recognize(CutImg()));
    if (Profile.getID() <= 0)
    {
        myError = Warnings::FAIL_COMPARE;
        return;
    }
    Profile.setSubscribeStatus(true);//вместо тру какая-нибудь функция которая сразу проверит и возвратит тру ор фолс
    setMask((xPath / "user\\user_power.png").generic_string());
    FindObj();
    Profile.setPower(Recognize(CutImg()));
    if (CompareSample(xPath / "user", "sample_prem", "state_prem")) Profile.setPremiumStatus(true);
    else Profile.setPremiumStatus(false);
    ClickButton(xPath / "user", "button_close");
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
    if (myError != Warnings::NO_WARNING)
    {
        getErr();
        return -1;
    }
    entryBarrack();
    if (myError != Warnings::NO_WARNING)
    {
        getErr();
        return -1;
    }
    setMask((xPath / "squad\\main\\power.png").generic_string());
    FindObj();
    if (myError != Warnings::NO_WARNING)
    {
        getErr();
        return -1;
    }
    int x = Recognize(changeColour(CutImg()));
    ClickEsc();
    ClickEsc();
    return x;
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
    switch (squads::count) {
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
    for (int i = start, j = 0; i <= end; ++i, j++) {
        unit[j] = positions[i];
    }
    if (CompareSample(xPath / "squad\\main", "sample", ("unit_" + to_string(unit[pos])))) {
        delete[] unit;
        myError = Warnings::FAIL_SETTING;
        return;//нет бойца
    }
    Click(xrect.x + 3, xrect.y + 6, 2000);
    Sleep(3000);
    if (!CompareSample(xPath / "squad\\main\\unit", "sample", "compare")) goto warning;
    ClickButton(xPath / "squad\\main\\unit", "button_set");
    if (!CompareSample(xPath / "squad\\main\\unit", "sample_set", "compare_set")) goto warning;
    if (k == 0) {
        setImg("C:/bot/data/pages/squad/main/unit/sample_set_0.png");
        setMask("C:/bot/data/pages/squad/main/unit/state_0.png");
        FindObj();
        if (myError != Warnings::NO_WARNING) return;
        Mat find = CutImg();
        Screenshot();
        res = true;
        int x = 0;
        do {
            if (!Compare(find, img, 0.1)) {
                x++;
                Sleep(1000);
                if (x == 5)
                {
                    myError = Warnings::FAIL_COMPARE;
                    return;
                }
            }
            else res = false;
        } while (res);
        Click();
        ClickEsc();
        ClickEsc();
        return;
    }
    res = false;
    for (int i = k; i < 4; i++) {
        if (CompareSample(xPath / "squad\\main\\unit", "set_" + to_string(i), "state_set")) res = true;
    }
    if (!res) {
        ClickEsc();
        ClickEsc();
        return false;
    }
    ClickButton(xPath / "squad\\main\\unit", "button_" + to_string(k));
    if (CompareSample(xPath / "squad\\main\\unit", "sample_confirm", "compare_confirm"))
    {
        ClickButton(xPath / "squad\\main\\unit", "button_confirm");
        ClickEsc();
        return true;
    }
    else {
        ClickEsc();
        ClickEsc();
        return true;
    }
    return false;
warning:
    cout << "где мы" << endl;
    return false;
}
//