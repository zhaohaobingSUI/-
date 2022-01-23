#pragma once//¸´ÖÆÒ»±é

#include "HCNetSDK.h"
#include "plaympeg4.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui_c.h>

#define Video_ShowSize Size(1620,1080)

using namespace cv;
using namespace std;

class HIK_CAMERA {
public:
	int Flow_Width;
	int Flow_Height;
	float Scale_Width;
	float Scale_Height;
	char* UserIP;
	char* UserID;
	char* UserKey;
	bool Play_Flag = 0;
	LONG nPort = -1;
	int Camera_Init();
	int Camera_Login();
	void Camera_Clear();
	int Get_Flow(int Channel, int StreamType);
	void Camera_Stop();
	void Camera_Logout();

private:
	LONG lUserID;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	LONG lRealPlayHandle;
	NET_DVR_PREVIEWINFO struPlayInfo;
};

void CALLBACK DecCBFun(long nPort, char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1, long nReserved2);

void CALLBACK fRealDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser);

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);