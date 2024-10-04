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
#include "Structs.h"
class controller:
	public Ocr
{
public:
	controller();
	~controller();

	//��� ������(����� �������)
	void InitLight();
	bool createMask(int x = 0, int y = 0, int width = 1, int height = 1);
	// 
	//���������� ������
	void fixErr();
	void getErr();
	//

	//User
	int getUPower();
	int getUID();

	void Initialize(int instanse = 1); // instance ��������� ����� ����� ���
	void InitUser();
	void InitSquadCount();

	bool getPremiumStatus();

	//

	//Squad
	int getBarrackPower();

	void setKit(int pos, int k = 0); // ��������� ������ �����
	//

	//Game
	bool FindObj(Mat finder = Mat());
	bool Compare(Mat Img = Mat(), Mat Sample = Mat(), double rightVal = 0.01);
	bool CompareSample(path pagePath, string SamplePath, string MaskPath, double rightVal = 0.01, bool Screen = false);


	bool writeMessage(const char* text, string pathPage);
	bool checkTime(int hour, int min); //UTC +0

	void setMainPage();
	void checkSettings();
	void findBarrack();
	void entryBarrack();
	//

	//Client

	bool isEmpty();//�������� �� ������� ����
	bool isValidSize();
	bool setValidSize();
	//

	//Checkers
	///general
	bool checkEvent();

	void skipEvent();
	///main
	void checkMain();//���������� ���������
	///map
	bool checkMap(bool right = false);//right = true => ��������� ������ ����� �����
	///load
	void checkLoadMain();

	bool checkLoad();
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
	int count;//���-�� ������ �������
	vector<int> positions;
	bluestacks Emulator;
	uProfile Profile;
	Mat img, sample, mask;
	RECT rect;//?
	Rect xrect;//?
	Point point;//?
	path xPath;
};

