#pragma once
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <opencv.hpp> // opencv 
#include <fstream>
#include "Structs.h"
#include "Ocr.h"
#include "Profile.h"
#include "bluestacks.h"
class controller:
	public Ocr
{
public:
	controller();
	~controller();

	//Äëÿ òåñòîâ(ïîòîì óäàëèòü)
	bool InitLight();
	bool createMask(int x = 0, int y = 0, int width = 1, int height = 1);
	// 
	
	//User
	int getUPower();
	int getUID();

	bool Initialize(int instanse = 1); // instance âû÷èñëÿòü ïîòîì ÷åðåç êìä
	bool InitUser();
	bool InitSquadCount();
	bool getPremiumStatus();

	//

	//Squad
	int getBarrackPower();

	bool setKit(); // óñòàíîâêà íàáîðà áîéöà
	//

	//Game

	bool FindObj(Mat finder = Mat());
	bool Compare(Mat Img = Mat(), Mat Sample = Mat(), double rightVal = 0.01);
	bool CompareSample(path pagePath, string SamplePath, string MaskPath, double rightVal = 0.01, bool Screen = false);
	bool checkSettings();
	bool findBarrack();
	bool entryBarrack();
	bool writeMessage(const char* text, string pathPage);

	void setMainPage();
	//

	//Client

	bool isEmpty();//ïðîâåðêà íà ïóñòîòó îêíà
	bool isValidSize();
	bool setValidSize();
	//

	//Checkers
	///general
	bool checkTime(int hour, int min); //UTC +0
	bool checkEvent();

	void skipEvent();
	///main
	bool checkMain();//ïåðåäåëàòü ñðàâíåíèå
	///map
	bool checkMap(bool right = false);//right = true => ïðîâåðÿòü ïðàâóþ ÷àñòü êàðòû
	///load
	bool checkLoadMain();

	void checkLoad();
	///arena
	bool checkFind();
	bool checkBattle();

	void checkEnd();
	void checkWait();
	///

	//
	
	//Image
	void Screenshot();

	bool setImg(Mat Img);
	bool setImg(string Path);
	bool setMask(Mat Img);
	bool setMask(string Path);
	bool setSample(string Path);
	bool setSample(Mat Img);
	bool SaveImg(string savePath, Mat Img = Mat());

	Mat getImg();
	Mat getSample();
	Mat getMask();
	Mat CutImg();
	Mat changeColour(Mat Img);

	Rect getRect();

	//

	//Buttons
	void Click(int sleep = 300);
	void Click(int x, int y, int sleep = 300);
	void ClickLong(int x, int y, int x1, int y1);
	void ClickEsc();
	void ClickReturn();

	bool ClickButton(path pagePath, string buttonPath, int sleep = 100);
	//

	//Filesystem

	path getPath();
	//
private:
	int count;//êîë-âî áîéöîâ êàçàðìû
	vector<int> positions;
	bluestacks Emulator;
	uProfile Profile;
	Mat img, sample, mask;
	RECT rect;//?
	Rect xrect;//?
	Point point;//?
	path xPath;
};

