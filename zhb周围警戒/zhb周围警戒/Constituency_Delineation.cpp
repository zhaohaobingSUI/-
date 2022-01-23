#include "pch.h"
#include "Constituency_Delineation.h"

extern Mat current_frame;//cpp文件中的参数，.h文件不可
extern Mat mask;
extern HIK_CAMERA Camera;

vector<vector<Point>> Polys;										//用户所画检测区的点集
bool flag_created = 1;												//检测区是否创建完成标志
bool Paint_Flag = 1;

void Left_Down(int x, int y);

void Right_Down();

void Middle_Down();

void mouseHandler(int event, int x, int y, int, void*)
{
	if (Paint_Flag)
	{
		if (event == EVENT_LBUTTONDOWN)// 左键按下，绘出选区
		{
			Left_Down(x, y);
		}

		if (event == EVENT_RBUTTONDOWN)// 右键按下，生成选区
		{
			Right_Down();
		}

		if (event == EVENT_MBUTTONDOWN)//中键按下，撤销操作
		{
			Middle_Down();
		}
	}
}

void Left_Down(int x, int y)
{
	if (flag_created)
	{
		vector<Point> Empty_Poly;
		Polys.push_back(Empty_Poly);	//进入下一选区
		flag_created = 0;
	}
	vector<Point> Temp_Points;
	Point point_current, point_last;
	int var;							//创建数据缓存变量

	Temp_Points = Polys.back();
	var = Temp_Points.size();
	if (var > 0)
		point_last = Temp_Points.back();
	point_current = Point((float)x / Camera.Scale_Width, (float)y / Camera.Scale_Height);//拉取并解析数据

	Temp_Points.push_back(point_current);
	var++;								//更新数据：点数加1

	cv::circle(mask, point_current, 2, Scalar(0), -1, 8, 0);
	if (var > 1)
	{
		cv::line(mask, point_last, point_current, Scalar(0), 2, 8, 0);
	}									/*更新图像：如果当前点数大于1，
										则在当前点与上一点间画线*/

	Polys.pop_back();
	Polys.push_back(Temp_Points);		//删除旧数据并将新数据压回
}

void Right_Down()
{
	if (Polys.size() > 0 && !flag_created)
	{
		vector<Point> Temp_Points;
		Point point_start, point_last;
		int var;							//创建数据缓存变量

		Temp_Points = Polys.back();
		var = Temp_Points.size();			//拉取并解析数据

		if (var > 2)
		{
			point_last = Temp_Points.back();
			point_start = Temp_Points.front();//拉取并解析数据

			cv::line(mask, point_last, point_start, Scalar(0), 2, 8, 0);
			/*更新图像：如果当前点数大于2，则
			判定为多边形，自动将点集首尾相连*/
		}

		if (var > 1)
		{
			flag_created = 1;
		}
		else if (var == 0 && Polys.size() > 1)
		{
			Polys.pop_back();
			flag_created = 1;
		}
	}
}

void Middle_Down()
{
	//if (Polys.size() > 0)
	//{
	//	vector<Point> Temp_Points;
	//	int var;
	//	Temp_Points = Polys.back();
	//	var = Temp_Points.size();//预读取选区点数，以分类处理

	//	switch (var)
	//	{
	//	case 0:					 //本选区已不存在点
	//	{
	//		switch (Polys.size())
	//		{
	//		case 0: break;
	//		case 1:
	//		{
	//			Polys.pop_back();//回退至上一选区
	//			flag_created = 1;
	//			break;
	//		}
	//		default:
	//		{
	//			Polys.pop_back();//回退至上一选区
	//			flag_created = 0;//设置选区为创建中状态

	//			Temp_Points = Polys.back();
	//			//读取上一选区点集

	//			Point point_current, point_last;

	//			point_current = Temp_Points.front();
	//			point_last = Temp_Points.back();
	//			var = Temp_Points.size();		//读取上一选区末尾接线点信息

	//			cv::circle(mask, point_current, 2, Scalar(255), -1, 8, 0);
	//			cv::line(mask, point_current, point_last, Scalar(255), 2, 8, 0);
	//			cv::circle(mask, point_last, 2, Scalar(255), -1, 8, 0);
	//			//消除最近一次画线和点痕
	//			if (var == 2)
	//			{
	//				if (Polys.size() == 1)
	//				{
	//					Temp_Points.clear();				//清空当前点集

	//					Polys.pop_back();
	//					flag_created = 1;
	//				}
	//				else
	//				{
	//					Temp_Points.clear();				//清空当前点集

	//					Polys.pop_back();
	//					Polys.push_back(Temp_Points);		//删除旧数据并将新数据压回
	//					flag_created = 1;
	//				}
	//			}
	//		}
	//		}
	//		break;
	//	}
	//	case 1:
	//	{										//本选区仅有1个点
	//		Point point_current;				//创建数据缓存变量

	//		point_current = Temp_Points.back();	//拉取并解析数据

	//		cv::circle(mask, point_current, 2, Scalar(255), -1, 8, 0);
	//		//消除点痕

	//		Temp_Points.clear();				//清空当前点集

	//		Polys.pop_back();
	//		Polys.push_back(Temp_Points);		//删除旧数据并将新数据压回

	//		break;
	//	}
	//	case 2:
	//	{										//点集为直线
	//		Point point_current, point_last;	//创建数据缓存变量

	//		point_current = Temp_Points.back();
	//		point_last = Temp_Points.front();	//拉取并解析数据

	//		cv::circle(mask, point_current, 2, Scalar(255), -1, 8, 0);
	//		cv::line(mask, point_current, point_last, Scalar(255), 2, 8, 0);
	//		cv::circle(mask, point_last, 2, Scalar(255), -1, 8, 0);
	//		//消除连线和所有点痕

	//		Temp_Points.clear();				//清空当前点集

	//		Polys.pop_back();
	//		Polys.push_back(Temp_Points);		//删除旧数据并将新数据压回

	//		break;
	//	}
	//	default:
	//	{
	//		if (flag_created)
	//		{
	//			Point point_start, point_last;	//创建数据缓存变量

	//			point_start = Temp_Points.front();
	//			point_last = Temp_Points.back();	//读取选区末尾接线点信息

	//			cv::circle(mask, point_start, 2, Scalar(255), -1, 8, 0);
	//			cv::line(mask, point_start, point_last, Scalar(255), 2, 8, 0);
	//			//消除最近一次画线和点痕

	//			flag_created = 0;					//设置选区为创建中状态
	//		}
	//		else
	//		{
	//			Point point_current, point_last;	//创建数据缓存变量

	//			point_current = Temp_Points.back();
	//			Temp_Points.pop_back();				//更新当前点集
	//			point_last = Temp_Points.back();	//拉取并解析数据

	//			cv::circle(mask, point_current, 2, Scalar(255), -1, 8, 0);
	//			cv::line(mask, point_current, point_last, Scalar(255), 2, 8, 0);
	//			//消除连线和最新点痕

	//			Polys.pop_back();
	//			Polys.push_back(Temp_Points);		//删除旧数据并将新数据压回
	//		}
	//		break;
	//	}
	//	}
	//}

	Polys.clear();
	mask = Mat(Size(Camera.Flow_Width, Camera.Flow_Height), CV_8UC1, Scalar(1));
	flag_created = 1;
}