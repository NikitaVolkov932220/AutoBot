#include "bluestacks.h"
bluestacks::bluestacks() {
	mWin.right = 0;
	mWin.left = 0;
	mSize = { 1280 + 34,720 + 34 };
	gSize = { 1280,720 };
}
bluestacks::~bluestacks() {
}
bool bluestacks::FindWin() {
	Sleep(3000);
	while (!(mainWin = FindWindowA("Qt5154QWindowIcon", NULL)));
	mainWin = nullptr;
	return true;
}
bool bluestacks::FindEmu(string Name) {
	HWND wnd = FindWindowA(NULL, Name.c_str());
	if (wnd == NULL) return false;
	return true;
}
bool bluestacks::Initialize() {
	mainWin = FindWindowA("Qt5154QWindowIcon", NULL);
	gameWin = FindWindowExA(mainWin, NULL, NULL, "HD-Player");

	// Установка параметров окна
	LONG_PTR style = GetWindowLongPtr(mainWin, GWL_EXSTYLE);
	style |= WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	SetWindowLongPtr(mainWin, GWL_STYLE, style);

	ShowWindow(mainWin, SW_MINIMIZE);
	MoveWindow(mainWin, 0, 0, 1, 1, false);

	ShowWindow(mainWin, SW_SHOWNOACTIVATE);
	SetWindowPos(mainWin, HWND_BOTTOM, 1, 1, 1280 + 34, 720 + 34, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	// Получение размеров окна
	GetClientRect(mainWin, &mWin);
	GetClientRect(gameWin, &gWin);

	if (mWin.bottom == 0 && mWin.right == 0 && mWin.left == 0 && mWin.top == 0) return false;
	return true;
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