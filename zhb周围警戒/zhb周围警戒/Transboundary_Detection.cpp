#include "pch.h"
#include "Transboundary_Detection.h"

extern Mat current_frame;
extern Mat frame_detect;
extern Mat mask;
extern vector<vector<Point>> Polys;
extern bool flag_created;
extern vector<Rect> filtered;
extern HIK_CAMERA Camera;
extern String WindowName;
extern const char* ConfigFile;
extern unsigned _stdcall Warning_Play_Thread(void*);

unsigned _stdcall Warning_Play_Thread(void*);
String Record_Path;
bool Warning_Flag = 0;

void Warning_Record(Size VideoSize)
{
	String RecordName, Date, Time;
	VideoWriter videoWriter;
	int codec = VideoWriter::fourcc('D', 'I', 'V', 'X');
	double fps = 25;											//设置视频储存格式
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	Date = to_string(sys.wYear) + "年" + to_string(sys.wMonth) + "月" + to_string(sys.wDay) + "日";
	Time = to_string(sys.wHour) + "-" + to_string(sys.wMinute) + "-" + to_string(sys.wSecond);

	fstream Pathio(ConfigFile);
	getline(Pathio, Record_Path);

	if (_access((Record_Path + '/' + Date).c_str(), 0))
		int flag = _mkdir((Record_Path + '/' + Date).c_str());

	RecordName = Record_Path + '/' + Date + '/' + Time;
	videoWriter.open(RecordName + ".avi", codec, fps, VideoSize, true);

	Mat show_frame;

	resize(current_frame, show_frame, VideoSize);

	imwrite(RecordName + ".jpg", show_frame);			//保存越界截图

	HANDLE handle1 = (HANDLE)_beginthreadex(NULL, 0, Warning_Play_Thread, NULL, NULL, NULL);

	while (Warning_Flag)					//加载实时监控
	{
		Mat bottom_frame;
		bottom_frame = Mat(VideoSize, CV_8UC3, Scalar(0, 0, 255));
		frame_detect.copyTo(bottom_frame, mask);
		videoWriter << bottom_frame;		//写入越界记录视频
		resize(bottom_frame, bottom_frame, Video_ShowSize);
		cv::imshow(WindowName, bottom_frame);
		cv::waitKey(40);
	}

	cv::imshow(RecordName + ".jpg", show_frame);

	videoWriter.release();
}

bool intersection(const vector<Point> & line1, const vector<Point> &line2)
{
	CV_Assert(line1.size() == line2.size());
	CV_Assert(line1.size() == 2);
	Point point1_11, point1_12, point1_21, point1_22;

	point1_11 = line2[0] - line1[0];
	point1_12 = line2[1] - line1[0];

	point1_21 = line2[0] - line1[1];
	point1_22 = line2[1] - line1[1];

	if (point1_11.cross(point1_12)*point1_21.cross(point1_22) > 0)
	{
		return 0;
	}

	Point point2_11, point2_12, point2_21, point2_22;

	point2_11 = line1[0] - line2[0];
	point2_12 = line1[1] - line2[0];

	point2_21 = line1[0] - line2[1];
	point2_22 = line1[1] - line2[1];

	if (point2_11.cross(point2_12)*point2_21.cross(point2_22) > 0)
	{
		return 0;
	}
	else
	{
		cv::line(current_frame, line1[0], line1[1], Scalar(0, 0, 255), 2, 8, 0);
		cv::line(current_frame, line2[0], line2[1], Scalar(0, 0, 255), 2, 8, 0);
		return 1;
	}
}

bool poly_rect_intersection(vector<Point> & Lines, Rect & targetRect)
{
	bool if_inside_tl = 0, if_inside_br = 0;			//检验目标矩形是否在多边形区域内
	Point tl = targetRect.tl(), br = targetRect.br();
	for (size_t i = 0, j = Lines.size() - 1; i < Lines.size(); j = i++)
	{
		if (((Lines[i].y > tl.y) != (Lines[j].y > tl.y)) &&
			(tl.x < (Lines[j].x - Lines[i].x) * (tl.y - Lines[i].y) /
			(Lines[j].y - Lines[i].y) + Lines[i].x))
			if_inside_tl = !if_inside_tl;
	}
	for (size_t i = 0, j = Lines.size() - 1; i < Lines.size(); j = i++)
	{
		if (((Lines[i].y > br.y) != (Lines[j].y > br.y)) &&
			(br.x < (Lines[j].x - Lines[i].x) * (br.y - Lines[i].y) /
			(Lines[j].y - Lines[i].y) + Lines[i].x))
			if_inside_br = !if_inside_br;
	}
	if (if_inside_tl || if_inside_br)
	{
		cv::rectangle(current_frame, targetRect.tl(), targetRect.br(), cv::Scalar(0, 0, 255), 2);
		return true;
	}	

	for (size_t i = 0; i < Lines.size(); i++)
	{
		Point p1 = Lines[i], p2;
		if (i == Lines.size() - 1)
			p2 = Lines[0];
		else
			p2 = Lines[i + 1];
		vector<Point> line;
		line.push_back(p1);
		line.push_back(p2);

		vector<Point> line1;
		line1.clear();
		line1.push_back(Point(targetRect.x, targetRect.y));
		line1.push_back(Point(targetRect.x, targetRect.y + targetRect.height));

		vector<Point> line2;
		line2.clear();
		line2.push_back(Point(targetRect.x + targetRect.width, targetRect.y));
		line2.push_back(Point(targetRect.x + targetRect.width, targetRect.y + targetRect.height));

		vector<Point> line3;
		line3.clear();
		line3.push_back(Point(targetRect.x, targetRect.y));
		line3.push_back(Point(targetRect.x + targetRect.width, targetRect.y));

		vector<Point> line4;
		line4.clear();
		line4.push_back(Point(targetRect.x, targetRect.y + targetRect.height));
		line4.push_back(Point(targetRect.x + targetRect.width, targetRect.y + targetRect.height));

		if (intersection(line1, line) || intersection(line2, line) ||
			intersection(line3, line) || intersection(line4, line))
			return true;
	}

	return false;
}

int createDirectory(std::string path)
{
	int len = path.length();
	char tmpDirPath[256] = { 0 };
	for (int i = 0; i < len; i++)
	{
		tmpDirPath[i] = path[i];
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/')
		{
			if (_access(tmpDirPath, 0) == -1)
			{
				int ret = _mkdir(tmpDirPath);
				if (ret == -1) return ret;
			}
		}
	}
	return 0;
}

void Working_Area_Colored()
{
	if (flag_created)
	{
		for (size_t i = 0; i < Polys.size(); i++)
		{
			vector<Point> Area = Polys[i];
			for (size_t j = 0; j < Area.size(); j++)
			{
				Point pointf, pointl;
				if (j == Area.size() - 1)
				{
					pointf = Area[j];
					pointl = Area[0];
				}
				else
				{
					pointf = Area[j];
					pointl = Area[j + 1];
				}
				cv::circle(current_frame, pointf, 2, Scalar(0, 255, 0), -1, 8, 0);
				cv::line(current_frame, pointf, pointl, Scalar(0, 255, 0), 2, 8, 0);
				cv::circle(current_frame, pointf, 2, Scalar(0, 255, 0), -1, 8, 0);
			}
		}
	}
}

void Transboundary_Alert()
{
	if (flag_created)
	{
		for (size_t i = 0; i < Polys.size(); i++)
		{
			for (size_t j = 0; j < filtered.size(); j++)
			{
				if (poly_rect_intersection(Polys[i], filtered[j]))
				{
					Warning_Flag = 1;
					Warning_Record(Size(Camera.Flow_Width, Camera.Flow_Height));
					break;
				}
			}
		}
	}
}