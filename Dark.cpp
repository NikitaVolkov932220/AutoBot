#include "Dark.h"
Dark::Dark(controller* Emu) {
	check = Emu;
	set.power = 0;
	set.mode = false;
	name[0] = "altar";
	name[1] = "camp";
	name[2] = "game";
	name[3] = "reflection";
	localPath = Emu->getPath();
	setting = false;
	count = 1;
	stage = 0;
	dispersion = 0;
}
Dark::~Dark() {

}
bool Dark::Settings() {
	int x = 0;
	cout << "Выберите режим:\n[1]Яблоки\n[2]Ключи\n";
	//cin >> x;
	x = 2;
	if (x != 1 && x != 2) return false;
	if (x == 1) set.mode = false;
	if (x == 2) set.mode = true;
	cout << "Выберите сложность:\n[1]Normal\n[2]Hard\n[3]Insane\n";
	//cin >> set.mult;
	set.mult = 3;
	if (set.mult < 1 || set.mult >3) return false;
	cout << "Введите количество прохождений:\n";
	//cin >> count;
	count = 5;
	if (count < 1) return false;
	setting = true;
	return true;
}
bool Dark::setSquad() {
	int x = 0;
	bool res = false;
	do
	{
		if (check->CompareSample(localPath / "squad\\dark", "sample", "compare", true))
		{
			res = true;
		}
		else 
		{
			x++;
			if (x == 40) 
			{
				myError = Warnings::FAIL_COMPARE;
				return false;
			}
			Sleep(500);
		}
	} while (!res);
	
	//пока что сделаю по уродски
	check->ClickButton(localPath / "squad\\dark", "button_previous");
	check->ClickButton(localPath / "squad\\dark", "button_start");
	Warnings();
	Warnings();
	return true;
}
bool Dark::findWaypoint() {
	int x = 0;
	do 
	{
		check->Screenshot();
		check->setSample(check->getImg());
		for (int i = 0; i < 7; i++)
		{
			cout << "Check find_" << to_string(i) << " ";
			check->setImg((localPath / "dark\\map\\sample_").generic_string() + to_string(i) + ".png");
			check->setMask((localPath / "dark\\map\\find_").generic_string() + to_string(i) + ".png");
			check->FindObj();
			if (check->Compare(check->CutImg(), Mat(), 0.1)) return true;
		}
		Sleep(1000);
		x++;
	} while (x < 4);
	if (check->CompareSample(localPath / "dark\\waypoints", "sample_next", "state_next", true))
	{
		check->ClickButton(localPath / "dark\\waypoints", "button_next");
		Warnings();
		return false;
	}
	if (check->CompareSample(localPath / "dark\\waypoints", "sample_end", "state_end"))
	{
		check->ClickButton(localPath / "dark\\waypoints", "button_end");
		stage = 4;
		Warnings();
		return false;
	}
	if (x >= 5) myError = Warnings::FAIL_COMPARE;
	return false;
}

int Dark::checkWaypoint() {
	int x = 0;
	do
	{
		check->Screenshot();
		for (int i = 0; i < 4; i++)
		{
			if (check->CompareSample(localPath / ("dark\\waypoints\\" + name[i]), "sample", "compare")) return i;
		}
		if (check->CompareSample(localPath / "battle\\dark", "sample", "compare")) return 4;
		Sleep(500);
		x++;
	} while (x < 10);
	return -1;
}

void Dark::checkStage() {
	int x = 0;
	bool res = false;
	Sleep(3000);
	if (check->CompareSample(localPath / "load", "sample", "compare",true)) check->checkLoad();
	do 
	{
		check->Screenshot();
		for (int i = 0; i < 3; i++)
		{
			if (check->CompareSample(localPath / "dark\\waypoints", ("stage_" + to_string(i + 1)), "compare_stage"))
			{
				res = true;
				stage = i + 1;
				return;
			}
		}
		x++;
		if (x == 20) goto warning;
		Sleep(500);
	} while (!res);
	return;
warning:return;
}
void Dark::attackWaypoints() {
	Rect point;
	bool res = false;
	int x = 0;
	int risk = 0;
	while (true) 
	{
	start:
		Sleep(500);
		res = false;
		if (!findWaypoint()) return;
		point = check->getRect();
		check->Click(point.x, point.y);
		switch (x = checkWaypoint()) 
		{
		case -1://warning fail compare waypoint 
		{
			myError = Warnings::FAIL_COMPARE;
			return;
		}
		case 0://altar
		{
			check->ClickButton(localPath / ("dark\\waypoints\\" + name[x]), "button_move");
			check->ClickButton(localPath / ("dark\\waypoints\\" + name[x]), "button_confirm");
			goto def;
		}
		case 1://camp
		{
			check->ClickButton(localPath / ("dark\\waypoints\\" + name[x]), "button_reward");
			check->ClickButton(localPath / ("dark\\waypoints\\" + name[x]), "button_confirm");
			Warnings();
			goto def;
		}
		case 2://game 
		{
			if (risk < 3) 
			{
				check->ClickButton(localPath / ("dark\\waypoints\\" + name[x]), "button_risk");
				risk++;
			}
			else
			{
				check->ClickButton(localPath / ("dark\\waypoints\\" + name[x]), "button_skip");
				res = true;
			}
			check->ClickButton(localPath / ("dark\\waypoints\\" + name[x]), "button_confirm");
			Sleep(5000);//???
			goto def;
		}
		case 3://reflection
		{
			check->ClickButton(localPath / ("dark\\waypoints\\" + name[x]), "button_confirm");
			goto def;
		}
		case 4://battle
		{
			check->ClickButton(localPath / "battle\\dark", "button_best");
			int i = 0;
			do 
			{
				checkPower();
				if (set.power > check->getUPower() + dispersion)
				{
					safePower();
					i++;
				}
				else i = 5;
			} while (i < 3);
			if (set.power > check->getUPower() + dispersion)
			{
				//не знаю ещё че тут делать буду, подумать ещё 1001904718842019 раз когда-нибудь
				//WARNING
			}
			x = 0;
			if (check->getPremiumStatus())
			{
				check->ClickButton(localPath / "battle\\dark", "button_qstart");
				Warnings();
				int h = 0;
				do
				{
					if (check->CompareSample(localPath / "battle\\end\\quick", "sample", "compare",true)) 
					{
						Sleep(500);
						check->ClickEsc();
						res = true;
					}
					else if (check->checkEvent())
					{
						check->skipEvent();
					}
					else
					{
						x++;
						if (x == 100) 
						{
							myError = Warnings::FAIL_COMPARE;
							return;
						}
						Sleep(500);
					}
				} while (!res);
			}
			else 
			{
				check->ClickButton(localPath / "battle\\dark", "button_start");
				Warnings();
				//написать на обычный старт после боя
				//
				//
			}
			x = 0;
			res = false;
			do 
			{
				if (check->CompareSample(localPath / "dark\\waypoints\\blessing", "sample", "compare",true)) 
				{
					res = true;
				}
				else 
				{
					x++;
					if (x == 120)
					{
						myError = Warnings::FAIL_COMPARE;
						return;
					}
					Sleep(500);
				}
			} while (!res);
			check->ClickButton(localPath / "dark\\waypoints\\blessing", ("button_" + to_string(1 + rand() % 3)));
			Sleep(500);
			check->ClickButton(localPath / "dark\\waypoints\\blessing", "button_confirm");
			goto start;
		}
		}
	
	def:
		x = 0;
		res = false;
		while (!res) 
		{
			if (check->CompareSample(localPath / "dark\\waypoints", "sample_complete", "compare_complete",true))
			{
				check->ClickButton(localPath / "dark\\waypoints", "button_close");
				res = true;
			}
			else 
			{
				x++;
				if (x == 100) 
				{
					myError = Warnings::FAIL_COMPARE;
					return;
				}
				Sleep(500);
			}
		}
	}
	return;
}
void Dark::checkPower() {
	Sleep(500);
	check->Screenshot();
	check->setMask((localPath / "battle\\dark\\state_power.png").generic_string());
	check->FindObj();
	Mat img = check->CutImg();
	set.power = check->Recognize(check->changeColour(img));
	if (set.power == 0) myError = Warnings::FAIL_COMPARE;
	return;
}
void Dark::Warnings() {
	Sleep(1000);
	if (check->CompareSample(localPath / "warnings\\dark", "sample_1", "compare", true,0.001)) goto warning;
	if (check->CompareSample(localPath / "warnings\\dark", "sample_2", "compare",false,0.001)) goto warning;
	/*if (check->CompareSample(localPath / "warnings\\dark", "sample_floor", "compare_floor",true)) goto warning;
	if (check->CompareSample(localPath / "warnings\\dark", "sample_units", "compare_units")) goto warning;
	if (check->CompareSample(localPath / "warnings\\dark", "sample_point", "compare_point")) goto warning;
	if (check->CompareSample(localPath / "warnings\\dark", "sample_select", "compare_select")) goto warning;
	if (check->CompareSample(localPath / "warnings\\dark", "sample_spells", "compare_spells")) goto warning;*/
	return;
warning:
	check->ClickButton(localPath / "warnings\\dark", "button_yes");
	return;
}
void Dark::Begin() {
	localPath = check->getPath();
	bool res = false;
	int x = 0;
	do {
		if (check->CompareSample(localPath / "dark", "sample", "compare", true))
		{
			res = true;
		}
		else
		{
			x++;
			if (x == 40)
			{
				cout << "check stage" << endl;
				checkStage();
				if (stage > 0||stage<4) return;
				else { stage = 5; goto warning; }
			}
			Sleep(500);
		}
	} while (!res);

	if (!set.mode) check->ClickButton(localPath / "dark", "button_apples");
	else check->ClickButton(localPath / "dark", "button_keys");
	switch ((int)set.mult)
	{
	case 1:
	{
		check->ClickButton(localPath / "dark", "button_normal");
		break;
	}
	case 2:
	{
		check->ClickButton(localPath / "dark", "button_hard");
		break;
	}
	case 3:
	{
		check->ClickButton(localPath / "dark", "button_insane");
		break;
	}
	}
	check->ClickButton(localPath / "dark", "button_start");
	return;
warning:
	myError = Warnings::FAIL_CHECK;
	return;
}
void Dark::Start() {
	int k = 0;
	int x = 0;
	bool res = true;
	if (!setting) 
	{
		cout << "Настройки собора:" << endl;
		if (!Settings())
		{
			myError = Warnings::FAIL_SETTING;
			return;
		}
	}
	check->checkMain();
	if (hasWarn(myError)) goto warning;
	check->ClickButton((localPath / "main"), "button_map");
	///////
	while (!check->checkMap()) {
		//if (check->checkMap(true))
		//{
		//	check->ClickLong(683 - 287, 504, 683, 504);
		//	break;
		//}
		//else {
			x++;
			if (x == 50) goto warning;
			Sleep(500);
		//}
	}
	////////
	if (!check->checkMap()) 
	{
		if (check->checkMap(true)) 
		{
			check->ClickLong(683 - 287, 504, 683, 504);
		}
		else goto warning;
	}
	check->ClickButton((localPath / "map"), "button_dark");
	while (k < count) 
	{
		stage = 0;
		Sleep(500);
		cout << "Dark cathedral " << k + 1 << endl;
		cout << "Check Dark\n";
		Begin();
		if (stage == 0)
		{
			res = setSquad();
			checkStage();
		}
		else if(stage >4) goto warning;
		if (res)
		{
			while(stage !=4)
			{
				cout << "Attacks\n";
				attackWaypoints();
				if (hasWarn(myError)) goto warning;
				if(stage != 4) checkStage();
			}
			if (stage == 4) res = false;
			while (!res) 
			{
				if (check->CompareSample(localPath / "dark", "sample_end", "compare_end", true)) 
				{
					if (check->CompareSample(localPath / "dark", "sample_end", "state_end")) 
					{
						check->ClickButton(localPath / "dark", "button_end");
						Sleep(3000);
						if (check->CompareSample(localPath / "load", "sample", "compare", true))
						{
							check->checkLoad();
							res = true;
						}
						else {
							x++;
							if (x == 100) {
								myError = Warnings::FAIL_COMPARE;
								return;
							}
							Sleep(500);
						}
					}
				}
				else {
					x++;
					if (x == 100) {
						myError = Warnings::FAIL_COMPARE;
						return;
					}
					Sleep(500);
				}
			}
			
			if (check->CompareSample(localPath / "dark", "sample", "compare", true))
			{
				res = true;
			}
			else if (check->checkEvent())
			{
				check->skipEvent();
			}
			else
			{
				myError = Warnings::FAIL_PAGE;
				goto warning;
			}
		}
		k++;
	}
	check->checkLoad();
	localPath = check->getPath();
	check->ClickButton(localPath / "dark", "button_close");
	check->checkMain();
	if (myError == Warnings::FAIL_CHECK) {
		check->ClickButton(localPath / "map", "button_close");
		myError = Warnings::NO_WARNING;
	}
	return;
warning:
	check->getErr();
	return;
}
void Dark::safePower() {
	check->ClickLong(660, 215, 660, 430);
}