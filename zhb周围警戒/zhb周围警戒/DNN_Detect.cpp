#include "pch.h"
#include "DNN_Detect.h"

extern Mat frame_detect;
extern HIK_CAMERA Camera;

vector<Rect> filtered;
Mat current_frame;

DNN_Detector::DNN_Detector(void)
{
	High_Recognition_Inference_Graph = "High recognition.pb";
	High_Recognition_Graph_pbtxt = "High recognition.pbtxt";
	High_Robustness_Inference_Graph = "High robustness.pb";
	High_Robustness_Graph_pbtxt = "High robustness.pbtxt";
	Ability_Set();
}

void DNN_Detector::Detect_DrawRect()
{
	if (!Detect_Mode)
		return;
	Mat im_tensor = blobFromImage(frame_detect, 1.0, Size(300, 300), Scalar(), true, false);

	net.setInput(im_tensor);

	Mat cvOut = net.forward();

	Mat detectOut(cvOut.size[2], cvOut.size[3], CV_32F, cvOut.ptr<float>());

	for (int row = 0; row <= detectOut.rows; row++)
	{
		float confidence = detectOut.at<float>(row, 2);
		if (confidence > 1 - Sensitivity)
		{
			int tl_x, tl_y, br_x, br_y;
			tl_x = detectOut.at<float>(row, 3) * Camera.Flow_Width;
			tl_y = detectOut.at<float>(row, 4) * Camera.Flow_Height;
			br_x = detectOut.at<float>(row, 5) * Camera.Flow_Width;
			br_y = detectOut.at<float>(row, 6) * Camera.Flow_Height;

			Rect R(tl_x, tl_y, br_x - tl_x, br_y - tl_y);
			if (R.width < R.height*1.75 && R.height < R.width*4
				&& R.width < frame_detect.cols*0.6&& R.height < frame_detect.rows*0.6)
			{
				rectangle(current_frame, R, Scalar(255, 0, 255), 2);
				filtered.push_back(R);
			}
		}
	}
}

void DNN_Detector::Ability_Set()
{
	if(Ability_Mode)
		net = readNetFromTensorflow(High_Recognition_Inference_Graph, High_Recognition_Graph_pbtxt);
	else
		net = readNetFromTensorflow(High_Robustness_Inference_Graph, High_Robustness_Graph_pbtxt);
	return;
}