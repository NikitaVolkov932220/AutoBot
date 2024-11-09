#include <opencv.hpp> // opencv 
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include "controller.h"
#include "Dark.h"

using namespace std;
using namespace cv;
using namespace std::filesystem;

int main() {
	setlocale(0, "");
	controller main;
	//main.Initialize(1);
	Dark task(&main);
	main.InitLight();
	//task.Start();
	do {
		task.Start();
	} while (true);
	// 

	//main.Screenshot();
	//main.createMask(636,280,60,32);
	//main.SaveImg("data\\pages\\dark\\waypoints\\compare_stage_0.png");
	//main.createMask(636, 315, 60, 32);
	//main.SaveImg("data\\pages\\dark\\waypoints\\compare_stage_1.png");
	//main.createMask(636, 350, 60, 32);
	//main.SaveImg("data\\pages\\dark\\waypoints\\compare_stage_2.png");
	//////task.findWaypoint();

	//main.setImg("data\\pages\\dark\\waypoints\\stage_1.png");
	//main.setMask("data\\pages\\dark\\waypoints\\compare_stage_0.png");
	//main.FindObj();
	//Mat find = main.CutImg();
	//imshow("1", find);
	//waitKey(0);
	//main.setMask("data\\pages\\dark\\waypoints\\compare_stage_1.png");
	//main.FindObj();
	//Mat find1 = main.CutImg();
	//imshow("1", find1);
	//waitKey(0);
	//main.setMask("data\\pages\\dark\\waypoints\\compare_stage_2.png");
	//main.FindObj();
	//Mat find2 = main.CutImg();
	//imshow("1", find2);
	//waitKey(0);
	//main.Compare(find, Mat(), 0.01);
	return 0;
}
//{ x = 523 y = 202 width = 108 height = 10 } old {970,648,157,19}dark {640,565,40,80}find_2