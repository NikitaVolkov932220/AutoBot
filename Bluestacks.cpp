#include "Bluestacks.h"
bluestacks::bluestacks() {
	mWin.right = 0;
	mWin.left = 0;
	mSize = { 1280 + 34,720 + 34 };
	gSize = { 1280,720 };
}
bluestacks::~bluestacks() {
}
bool bluestacks::FindWin() {
	while (!(mainWin = FindWindowA("Qt5154QWindowIcon", NULL)));
	gameWin = FindWindowExA(mainWin, NULL, NULL, "HD-Player");
	SetForegroundWindow(mainWin);
	ShowWindow(mainWin, SW_RESTORE);
	Sleep(800);
	ShowWindow(mainWin, SW_MINIMIZE);
	Sleep(2000);
	if (!InvalidateRect(mainWin, NULL, TRUE)) {
		cout << "rect win\n";
		return false;
	}
	Sleep(5000);
	if (!InvalidateRect(gameWin, NULL, TRUE)) {
		cout << "rect gwin\n";
		return false;
	}
	mainWin = nullptr;
	return true;
}
bool bluestacks::FindEmu(string Name) {
	HWND wnd = FindWindowA(NULL, Name.c_str());
	if (wnd == NULL) return false;
	return true;
}
bool bluestacks::Initialize() {
	mainWin = FindWindowA("Qt5154QWindowIcon", NULL); // ~= NULL,name
	gameWin = FindWindowExA(mainWin, NULL, NULL, "HD-Player");
	GetWindowTextA(mainWin, nameWin, 200);
	//SetForegroundWindow(mainWin);
	//ShowWindow(mainWin, SW_RESTORE);
	//SetWindowPos((HWND)mainWin, nullptr, 30, 0, mSize.x, mSize.y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	//SetWindowPos((HWND)gameWin, nullptr, 1, 33, gSize.x, gSize.y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	//ShowWindow(mainWin, SW_MINIMIZE);
	MoveWindow(mainWin, 30, 0, mSize.x, mSize.y, true);
	MoveWindow(gameWin, 1, 33, gSize.x, gSize.y, true);
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