#include "bluestacks.h"
bluestacks::bluestacks() {
	mWin.right = 0;
	mWin.left = 0;
	mSize = { 1280 + 34,720 + 34 };
	gSize = { 1280,720 };
}
bluestacks::~bluestacks() {
}
void bluestacks::FindWin(int sleep) {
	Sleep(500);
	int x = 0;
	while (!(mainWin = FindWindowA("Qt5154QWindowIcon", NULL))) {
		Sleep(sleep);
		x++;
		if (x >= 100) {
			myError = Warnings::NO_ACTIVE_EMULATOR;
			break;
		}
	}
	if (myError != Warnings::NO_WARNING) return;
	GetWindowText(mainWin, nameWin, 200);
	gameWin = FindWindowExA(mainWin, NULL, NULL, "HD-Player");
	SetForegroundWindow(mainWin);
	SetActiveWindow(mainWin);
	// Установка параметров окна
	LONG_PTR style = GetWindowLongPtr(mainWin, GWL_EXSTYLE);
	style |= WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	SetWindowLongPtr(mainWin, GWL_STYLE, style);
	ShowWindow(mainWin, SW_MINIMIZE);
	if (!SetWindowPos(mainWin, HWND_BOTTOM, 1, 1, 1, 1, /*SWP_NOZORDER | SWP_NOACTIVATE |*/ SWP_FRAMECHANGED)) myError = Warnings::WRONG_EMULATOR_SIZE;
	Sleep(500);
	ShowWindow(mainWin, SW_SHOWNOACTIVATE);
	if (!SetWindowPos(mainWin, HWND_BOTTOM, 1, 1, mSize.x, mSize.y, /*SWP_NOZORDER | SWP_NOACTIVATE |*/ SWP_FRAMECHANGED)) myError = Warnings::WRONG_EMULATOR_SIZE;
	Sleep(500);
	return;
}
void bluestacks::setValidSize() {
	SetForegroundWindow(mainWin);
	SetActiveWindow(mainWin);
	// Установка параметров окна
	LONG_PTR style = GetWindowLongPtr(mainWin, GWL_EXSTYLE);
	style |= WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	SetWindowLongPtr(mainWin, GWL_STYLE, style);
	ShowWindow(mainWin, SW_MINIMIZE);
	if (!SetWindowPos(mainWin, HWND_BOTTOM, 1, 1, 1, 1, /*SWP_NOZORDER | SWP_NOACTIVATE |*/ SWP_FRAMECHANGED)) myError = Warnings::WRONG_EMULATOR_SIZE;
	Sleep(500);
	ShowWindow(mainWin, SW_SHOWNOACTIVATE);
	if (!SetWindowPos(mainWin, HWND_BOTTOM, 1, 1, mSize.x, mSize.y, /*SWP_NOZORDER | SWP_NOACTIVATE |*/ SWP_FRAMECHANGED)) myError = Warnings::WRONG_EMULATOR_SIZE;
	Sleep(500);
}
void bluestacks::FindEmu(string Name) {
	HWND wnd = FindWindowA(NULL, Name.c_str());
	if (wnd == NULL) myError = Warnings::WRONG_EMULATOR_NAME;
	return;
}
void bluestacks::Initialize(int sleep) {
	if (mainWin == (HWND)0) 
	{
		//mainWin = FindWindowA("Qt5154QWindowIcon", NULL);
		//gameWin = FindWindowExA(mainWin, NULL, NULL, "HD-Player");
		FindWin(sleep);
	}

	//// Установка параметров окна
	//LONG_PTR style = GetWindowLongPtr(mainWin, GWL_EXSTYLE);
	//style |= WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	//SetWindowLongPtr(mainWin, GWL_STYLE, style);

	//ShowWindow(mainWin, SW_MINIMIZE);
	//SetWindowPos(mainWin, HWND_BOTTOM, 0, 0, 1, 1, SWP_NOZORDER | SWP_NOACTIVATE);

	//ShowWindow(mainWin, SW_SHOWNOACTIVATE);
	//SetWindowPos(mainWin, HWND_BOTTOM, 1, 1, mSize.x, mSize.y, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	if (myError != Warnings::NO_WARNING) return;
	// Получение размеров окна
	GetClientRect(mainWin, &mWin);
	GetClientRect(gameWin, &gWin);

	if (mWin.bottom == 0 && mWin.right == 0 && mWin.left == 0 && mWin.top == 0) myError = Warnings::FAIL_INIT;
	if (mWin.right - mWin.left != mSize.x) myError = Warnings::WRONG_EMULATOR_SIZE;
	if (mWin.bottom - mWin.top != mSize.y) myError = Warnings::WRONG_EMULATOR_SIZE;
	return;
}
const HWND bluestacks::getmHandle() {
	return mainWin;
}
const HWND bluestacks::getgHandle() {
	return gameWin;
}
const RECT bluestacks::getmWin() {
	return mWin;
}
const Point bluestacks::getmSize() {
	return mSize;
}
const Point bluestacks::getgSize() {
	return gSize;
}