#include "pch.h"
#include "Constituency_Delineation.h"

extern Mat current_frame;//cpp�ļ��еĲ�����.h�ļ�����
extern Mat mask;
extern HIK_CAMERA Camera;

vector<vector<Point>> Polys;										//�û�����������ĵ㼯
bool flag_created = 1;												//������Ƿ񴴽���ɱ�־
bool Paint_Flag = 1;

void Left_Down(int x, int y);

void Right_Down();

void Middle_Down();

void mouseHandler(int event, int x, int y, int, void*)
{
	if (Paint_Flag)
	{
		if (event == EVENT_LBUTTONDOWN)// ������£����ѡ��
		{
			Left_Down(x, y);
		}

		if (event == EVENT_RBUTTONDOWN)// �Ҽ����£�����ѡ��
		{
			Right_Down();
		}

		if (event == EVENT_MBUTTONDOWN)//�м����£���������
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
		Polys.push_back(Empty_Poly);	//������һѡ��
		flag_created = 0;
	}
	vector<Point> Temp_Points;
	Point point_current, point_last;
	int var;							//�������ݻ������

	Temp_Points = Polys.back();
	var = Temp_Points.size();
	if (var > 0)
		point_last = Temp_Points.back();
	point_current = Point((float)x / Camera.Scale_Width, (float)y / Camera.Scale_Height);//��ȡ����������

	Temp_Points.push_back(point_current);
	var++;								//�������ݣ�������1

	cv::circle(mask, point_current, 2, Scalar(0), -1, 8, 0);
	if (var > 1)
	{
		cv::line(mask, point_last, point_current, Scalar(0), 2, 8, 0);
	}									/*����ͼ�������ǰ��������1��
										���ڵ�ǰ������һ��仭��*/

	Polys.pop_back();
	Polys.push_back(Temp_Points);		//ɾ�������ݲ���������ѹ��
}

void Right_Down()
{
	if (Polys.size() > 0 && !flag_created)
	{
		vector<Point> Temp_Points;
		Point point_start, point_last;
		int var;							//�������ݻ������

		Temp_Points = Polys.back();
		var = Temp_Points.size();			//��ȡ����������

		if (var > 2)
		{
			point_last = Temp_Points.back();
			point_start = Temp_Points.front();//��ȡ����������

			cv::line(mask, point_last, point_start, Scalar(0), 2, 8, 0);
			/*����ͼ�������ǰ��������2����
			�ж�Ϊ����Σ��Զ����㼯��β����*/
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
	//	var = Temp_Points.size();//Ԥ��ȡѡ���������Է��ദ��

	//	switch (var)
	//	{
	//	case 0:					 //��ѡ���Ѳ����ڵ�
	//	{
	//		switch (Polys.size())
	//		{
	//		case 0: break;
	//		case 1:
	//		{
	//			Polys.pop_back();//��������һѡ��
	//			flag_created = 1;
	//			break;
	//		}
	//		default:
	//		{
	//			Polys.pop_back();//��������һѡ��
	//			flag_created = 0;//����ѡ��Ϊ������״̬

	//			Temp_Points = Polys.back();
	//			//��ȡ��һѡ���㼯

	//			Point point_current, point_last;

	//			point_current = Temp_Points.front();
	//			point_last = Temp_Points.back();
	//			var = Temp_Points.size();		//��ȡ��һѡ��ĩβ���ߵ���Ϣ

	//			cv::circle(mask, point_current, 2, Scalar(255), -1, 8, 0);
	//			cv::line(mask, point_current, point_last, Scalar(255), 2, 8, 0);
	//			cv::circle(mask, point_last, 2, Scalar(255), -1, 8, 0);
	//			//�������һ�λ��ߺ͵��
	//			if (var == 2)
	//			{
	//				if (Polys.size() == 1)
	//				{
	//					Temp_Points.clear();				//��յ�ǰ�㼯

	//					Polys.pop_back();
	//					flag_created = 1;
	//				}
	//				else
	//				{
	//					Temp_Points.clear();				//��յ�ǰ�㼯

	//					Polys.pop_back();
	//					Polys.push_back(Temp_Points);		//ɾ�������ݲ���������ѹ��
	//					flag_created = 1;
	//				}
	//			}
	//		}
	//		}
	//		break;
	//	}
	//	case 1:
	//	{										//��ѡ������1����
	//		Point point_current;				//�������ݻ������

	//		point_current = Temp_Points.back();	//��ȡ����������

	//		cv::circle(mask, point_current, 2, Scalar(255), -1, 8, 0);
	//		//�������

	//		Temp_Points.clear();				//��յ�ǰ�㼯

	//		Polys.pop_back();
	//		Polys.push_back(Temp_Points);		//ɾ�������ݲ���������ѹ��

	//		break;
	//	}
	//	case 2:
	//	{										//�㼯Ϊֱ��
	//		Point point_current, point_last;	//�������ݻ������

	//		point_current = Temp_Points.back();
	//		point_last = Temp_Points.front();	//��ȡ����������

	//		cv::circle(mask, point_current, 2, Scalar(255), -1, 8, 0);
	//		cv::line(mask, point_current, point_last, Scalar(255), 2, 8, 0);
	//		cv::circle(mask, point_last, 2, Scalar(255), -1, 8, 0);
	//		//�������ߺ����е��

	//		Temp_Points.clear();				//��յ�ǰ�㼯

	//		Polys.pop_back();
	//		Polys.push_back(Temp_Points);		//ɾ�������ݲ���������ѹ��

	//		break;
	//	}
	//	default:
	//	{
	//		if (flag_created)
	//		{
	//			Point point_start, point_last;	//�������ݻ������

	//			point_start = Temp_Points.front();
	//			point_last = Temp_Points.back();	//��ȡѡ��ĩβ���ߵ���Ϣ

	//			cv::circle(mask, point_start, 2, Scalar(255), -1, 8, 0);
	//			cv::line(mask, point_start, point_last, Scalar(255), 2, 8, 0);
	//			//�������һ�λ��ߺ͵��

	//			flag_created = 0;					//����ѡ��Ϊ������״̬
	//		}
	//		else
	//		{
	//			Point point_current, point_last;	//�������ݻ������

	//			point_current = Temp_Points.back();
	//			Temp_Points.pop_back();				//���µ�ǰ�㼯
	//			point_last = Temp_Points.back();	//��ȡ����������

	//			cv::circle(mask, point_current, 2, Scalar(255), -1, 8, 0);
	//			cv::line(mask, point_current, point_last, Scalar(255), 2, 8, 0);
	//			//�������ߺ����µ��

	//			Polys.pop_back();
	//			Polys.push_back(Temp_Points);		//ɾ�������ݲ���������ѹ��
	//		}
	//		break;
	//	}
	//	}
	//}

	Polys.clear();
	mask = Mat(Size(Camera.Flow_Width, Camera.Flow_Height), CV_8UC1, Scalar(1));
	flag_created = 1;
}