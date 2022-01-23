#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include "HIK_Flow_In.h"

using namespace cv;
using namespace cv::dnn;

extern vector<Rect> filtered;

class DNN_Detector {
public:
	void Ability_Set();
	void Detect_DrawRect();
	DNN_Detector();
	bool Detect_Mode = 1;
	bool Ability_Mode = 0;
	float Sensitivity = 0.7;
private:
	String High_Recognition_Inference_Graph;
	String High_Recognition_Graph_pbtxt;
	String High_Robustness_Inference_Graph;
	String High_Robustness_Graph_pbtxt;
	Net net;
};

