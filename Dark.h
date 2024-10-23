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

	int checkWaypoint();

	void checkStage();
	void Begin();
	void Warnings();//удалить лишние фотки и комменты
	void checkPower();
	void Start();//доделать 
	void attackWaypoints();//доделать
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

