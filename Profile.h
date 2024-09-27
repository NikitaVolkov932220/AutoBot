#pragma once
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <opencv.hpp>
#include <fstream>
#include <iconv.h>
#include <boost/format.hpp>
using namespace std;
using namespace cv;
class uProfile
{
public:
	uProfile();
	~uProfile();

	int getPower();
	int getID();

	bool getPremiumStatus();
	bool getSubscribeStatus();
	bool Start(int x);
	bool Initialize(int power, int ID, bool statusP, bool statusS = false);

	void setPower(int power);
	void setID(int ID);
	void setPremiumStatus(bool status);
	void setSubscribeStatus(bool status);
	void Print();

	uProfile& operator= (const uProfile& user);
private:
	int uPower, uID;
	bool uPremium;
	bool uSubscribe;
};