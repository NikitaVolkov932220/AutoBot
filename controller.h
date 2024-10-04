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

<<<<<<< HEAD
	//��� ������(����� �������)
	void InitLight();
=======
	//Äëÿ òåñòîâ(ïîòîì óäàëèòü)
	bool InitLight();
>>>>>>> 2b89b1e9107a479a7a0be707d3cf92d4377f873a
	bool createMask(int x = 0, int y = 0, int width = 1, int height = 1);
	// 
	//���������� ������
	void fixErr();
	void getErr();
	//

	//User
	int getUPower();
	int getUID();

<<<<<<< HEAD
	void Initialize(int instanse = 1); // instance ��������� ����� ����� ���
	void InitUser();
	void InitSquadCount();

=======
	bool Initialize(int instanse = 1); // instance âû÷èñëÿòü ïîòîì ÷åðåç êìä
	bool InitUser();
	bool InitSquadCount();
>>>>>>> 2b89b1e9107a479a7a0be707d3cf92d4377f873a
	bool getPremiumStatus();

	//

	//Squad
	int getBarrackPower();

<<<<<<< HEAD
	void setKit(int pos, int k = 0); // ��������� ������ �����
=======
	bool setKit(); // óñòàíîâêà íàáîðà áîéöà
>>>>>>> 2b89b1e9107a479a7a0be707d3cf92d4377f873a
	//

	//Game
	bool FindObj(Mat finder = Mat());
	bool Compare(Mat Img = Mat(), Mat Sample = Mat(), double rightVal = 0.01);
	bool CompareSample(path pagePath, string SamplePath, string MaskPath, double rightVal = 0.01, bool Screen = false);


	bool writeMessage(const char* text, string pathPage);

	void setMainPage();
	void checkSettings();
	void findBarrack();
	void entryBarrack();
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
<<<<<<< HEAD
	void checkMain();//���������� ���������
=======
	bool checkMain();//ïåðåäåëàòü ñðàâíåíèå
>>>>>>> 2b89b1e9107a479a7a0be707d3cf92d4377f873a
	///map
	bool checkMap(bool right = false);//right = true => ïðîâåðÿòü ïðàâóþ ÷àñòü êàðòû
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

