#pragma once
#include <opencv.hpp> // opencv 
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include "Structs.h"
using namespace std;
using namespace filesystem;
using namespace cv;
class bluestacks
{
public:
	bluestacks();
	~bluestacks();

	static void FindEmu(string name);

	void FindWin();
	void Initialize();

	const HWND getmHandle();
	const HWND getgHandle();

	const RECT getmWin();

	const Point getmSize();
	const Point getgSize();
private:
	char nameWin[200];
	HWND mainWin, gameWin;
	RECT mWin, gWin;
	Point mSize, gSize;
};

