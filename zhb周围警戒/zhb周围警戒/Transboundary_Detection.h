#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <io.h>
#include <direct.h>
#include <iostream>
#include <windows.h>
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#include "HIK_Flow_In.h"

#define Video_ShowSize Size(1620,1080)

using namespace cv;
using namespace std;

int createDirectory(std::string path);

void Working_Area_Colored();

void Transboundary_Alert();
