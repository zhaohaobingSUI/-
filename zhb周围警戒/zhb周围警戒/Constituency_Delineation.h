#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "HIK_Flow_In.h"

using namespace cv;
using namespace std;

void mouseHandler(int event, int x, int y, int, void*);				//鼠标回调函数