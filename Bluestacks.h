#pragma once
#include <opencv.hpp> // opencv 
#include <iostream>
#include <Windows.h>
#include <filesystem>
using namespace std;
using namespace filesystem;
using namespace cv;
class bluestacks
{
public:
	bluestacks();
	~bluestacks();
	bool FindWin();//доработать
	static bool FindEmu(string Name);
	bool Initialize();//доработать
	const HWND getmHandle();
	const HWND getgHandle();
	const RECT getmWin();
	const Point getmSize();
	const Point getgSize();
private:
	HWND mainWin, gameWin;
	char nameWin[200];
	RECT mWin, gWin;
	Point mSize, gSize;
};

