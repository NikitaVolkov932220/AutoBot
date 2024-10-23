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
	main.Initialize(1);
	Dark task(&main);
	main.InitLight();
	//task.Start();
	do {
		task.Start();
	} while (true);

	//main.Screenshot();
	//main.createMask(985,648,135,18); // 650 40 33 40
	//main.SaveImg("data\\pages\\map\\button_dark.png");
	//////task.findWaypoint();

	//main.setImg("data\\pages\\map\\sample.png");
	//main.setMask("data\\pages\\map\\button_dark.png");
	//main.FindObj();
	//Mat find = main.CutImg();
	//imshow("1", find);
	//waitKey(0);
	//main.setSample("data\\pages\\warnings\\dark\\sample_point.png");
	//main.Compare(find, Mat(),0.1);
	//main.Screenshot();
	//main.SaveImg("data/pages/1/sample.png");
	//main.createMask(715, 320, 20, 15);
	//main.SaveImg("data/pages/1/compare.png");
	//main.createMask(535, 320, 20, 15);
	//main.SaveImg("data/pages/1/compare_1.png");
	return 0;
}
//{ x = 523 y = 202 width = 108 height = 10 } old {970,648,157,19}dark