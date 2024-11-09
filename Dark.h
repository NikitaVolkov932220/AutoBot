#pragma once
#include "controller.h"
class Dark
{
public:
	Dark(controller* Emu);
	~Dark();
	bool Stop(); // ?
	bool Settings();
	bool setSquad(); // сделать умную
	bool findWaypoint();
	bool Warnings();//удалить лишние фотки и комменты

	int checkWaypoint();

	void checkStage();
	void Begin();
	void checkPower();
	void Start();
	void attackWaypoints();
	void safePower();
private:
	controller* check;
	settings set;
	string name[4];
	path localPath;
	int count;
	int stage;
	bool setting;
	int dispersion;//доп "окно мощи" при атаках(по умолчанию будет 0)
};

