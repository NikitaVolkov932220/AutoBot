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

	void takePower(int power);
	void takeID(int ID);
	void takePremiumStatus(bool status);
	void takeSubscribeStatus(bool status);
	void Print();

	uProfile& operator= (const uProfile& user);
private:
	int uPower, uID;
	bool uPremium;
	bool uSubscribe;
};

struct settings {
	bool mode; 
	double mult;
	int power;
};
struct squads {
	static int count;
	vector<int> sets;
	int power;
	bool status = false;//settings battle
	void Init() {
		for (int i = 0, x = 0; i < count; i++) {
			cout << "Боец " << i + 1 << ". Какую вешалку вешать?\n[0]Снять всё\n[1]Первая вешалка\n[2]Вторая вешалка\n[3]Третья вешалка\n";
			cin >> x;
			if (x > 3 || x < 0) x = 0;
			sets.push_back(x);
		}
	}
};