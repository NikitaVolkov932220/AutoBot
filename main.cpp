#include <opencv.hpp> // opencv 
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include "controller.h"

using namespace std;
using namespace cv;
using namespace std::filesystem;

int main() {
	controller main;
	main.InitLight();
	/*main.Screenshot();
	main.SaveImg("data/pages/1/sample.png");
	main.createMask(686,328,55,45);
	main.SaveImg("data/pages/1/compare_top.png");*/
	return 0;
}