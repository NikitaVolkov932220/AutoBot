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

	//��� ������(����� �������)
	bool createMask(int x = 0, int y = 0, int width = 1, int height = 1);
	void InitLight();
	// 
	//���������� ������
	void fixErr();
	void getErr();
	//

	//User
	int getUPower();
	int getUID();

	bool getPremiumStatus();

	void Initialize(int instanse = 1); // instance ��������� ����� ����� ���
	void InitUser();
	void InitSquadCount();
	//

	//Squad
	int getBarrackPower();

	void setKit(int pos, int k = 0); // ��������� ������ �����
	//

	//Game
	bool FindObj(Mat finder = Mat()); // ����� ������� � ������� �������� �� ����� (����� ����� ������������ ������� ����� ������ ��� ������)
	bool Compare(Mat Img = Mat(), Mat Sample = Mat(), double rightVal = 0.01); // ������ �������� ����� ���������� ������� � xrect
	bool CompareSample(path pagePath, string samplePath, string maskPath, bool Screen = false, double rightVal = 0.01); //FindObj + CutImg + Compare
	bool checkTime(int hour, int min); //UTC +0
	bool writeMessage(const char* text, string pathPage);

	void setMainPage();//set main page to barracks
	void checkSettings();
	void findBarrack();
	void entryBarrack();//�������� �������� �� ������ �� ���� �� � �� ����� ��������
	//

	//Client

	void isEmpty(); //�������� �� ������� ����
	void isValidSize();
	void setValidSize();
	//

	//Checkers
	///battle
	bool checkEvent();

	void skipEvent();
	///main
	void checkMain();//���������� ��������� + sleep1000 ne zabud`  UPD: ����� ��������, ���� ��������� ���
	///map
	bool checkMap(bool right = false);//right = true => ��������� ������ ����� ����� UPD: ���������� �������� ������ ����� ����� ����� �� ������� � �����
	///load
	void checkLoad();
	void checkLoadMain();
	///arena
	void checkFind();
	bool checkBattle();

	void checkEnd();
	void checkWait();
	///

	//
	
	//Image
	bool setImg(Mat Img);
	bool setImg(string Path);
	bool setMask(Mat Img);
	bool setMask(string Path);
	bool setSample(Mat Img);
	bool setSample(string Path);
	bool SaveImg(string savePath, Mat Img = Mat());

	void Screenshot();

	Mat getImg();
	Mat getSample();
	Mat getMask();
	Mat CutImg();
	Mat changeColour(Mat Img);

	Rect getRect();

	//

	//Buttons
	bool ClickButton(path pagePath, string buttonPath, int sleep = 300);

	void Click(int sleep = 300);
	void Click(int x, int y, int sleep = 300);
	void ClickLong(int x, int y, int x1, int y1); //���� �� ��������, �������� 15 �� 10
	void ClickEsc();
	void ClickReturn();
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
	Rect xrect; //��� ������ � �� �����
	path xPath;
};

