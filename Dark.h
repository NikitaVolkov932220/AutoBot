#pragma once
#include "controller.h"
class Dark
{
public:
	Dark(controller* Emu);
	~Dark();
	bool Stop(); // ?
	bool Settings();
	bool setSquad(); // ������� �����
	bool findWaypoint();

	int checkWaypoint();

	void checkStage();
	void Begin();
	void Warnings();//������� ������ ����� � ��������
	void checkPower();
	void Start();//�������� 
	void attackWaypoints();//��������
	void safePower();
private:
	controller* check;
	settings set;
	string name[4];
	path localPath;
	int count;
	int stage;
	bool setting;
	int dispersion;//��� "���� ����" ��� ������(�� ��������� ����� 0)
};

