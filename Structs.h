#pragma once
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <opencv.hpp>
#include <fstream>
#include <iconv.h>
enum class Warnings : int {//�������� 100%
	NO_WARNING,
	UNKNOWN,
	//setting
	FAIL_SETTING,
	//game
	FAIL_CHECK,
	FAIL_COMPARE,
	//Emulators
	WRONG_EMULATOR_NAME,
	WRONG_EMULATOR_SIZE,
	NO_ACTIVE_EMULATOR,
	FAIL_INIT,
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
			cout << "���� " << i + 1 << ". ����� ������� ������?\n[0]����� ��\n[1]������ �������\n[2]������ �������\n[3]������ �������\n";
			cin >> x;
			if (x > 3 || x < 0) x = 0;
			sets.push_back(x);
		}
	}
};


Warnings myError = Warnings::NO_WARNING;
int squads::count = 0;