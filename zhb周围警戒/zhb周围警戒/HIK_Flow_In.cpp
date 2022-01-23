#include "pch.h"
#include "HIK_Flow_In.h"

Mat frame_detect;
Mat mask;
HIK_CAMERA Camera;
String WindowName = "People detector";

int HIK_CAMERA::Camera_Init(void)
{
	NET_DVR_Init();
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);
	NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);
	return NET_DVR_GetLastError();
}

int HIK_CAMERA::Camera_Login()
{
	lUserID = NET_DVR_Login_V30(UserIP, 8000, UserID, UserKey, &struDeviceInfo);
	return NET_DVR_GetLastError();
}

void HIK_CAMERA::Camera_Clear()
{
	Flow_Width = 0;
	Flow_Height = 0;
	Scale_Width = 0;
	Scale_Height = 0;
	UserIP = "";
	UserID = "";
	UserKey = "";
	Play_Flag = 0;
	nPort = -1;
	lUserID = 0;
	memset(&struDeviceInfo, 0, sizeof(struDeviceInfo));
	lRealPlayHandle = 0;
	memset(&struPlayInfo, 0, sizeof(struPlayInfo));
}

int HIK_CAMERA::Get_Flow(int Channel, int StreamType)
{
	lRealPlayHandle = 0;

	struPlayInfo.hPlayWnd = 0;
	struPlayInfo.lChannel = Channel;
	struPlayInfo.dwStreamType = StreamType;
	struPlayInfo.dwLinkMode = 0;

	lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, fRealDataCallBack, NULL);

	if (lRealPlayHandle < 0)
	{
		NET_DVR_Logout(lUserID);
		return NET_DVR_GetLastError();
	}

	while (frame_detect.empty()) waitKey(50);
	Flow_Width = frame_detect.cols;
	Flow_Height = frame_detect.rows;
	Scale_Width = (float)Video_ShowSize.width / Flow_Width;
	Scale_Height = (float)Video_ShowSize.height / Flow_Height;

	Play_Flag = 1;

	return 0;
}

void HIK_CAMERA::Camera_Stop()
{
	NET_DVR_StopRealPlay(lRealPlayHandle);
	Play_Flag = 0;
}

void HIK_CAMERA::Camera_Logout()
{
	NET_DVR_StopRealPlay(lRealPlayHandle);
	NET_DVR_Logout(lUserID);
	NET_DVR_Cleanup();
}

void CALLBACK DecCBFun(long nPort, char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1, long nReserved2)
{
	long lFrameType = pFrameInfo->nType;
	if (lFrameType == T_YV12)
	{
		Mat src(pFrameInfo->nHeight + pFrameInfo->nHeight / 2, pFrameInfo->nWidth, CV_8UC1, pBuf);
		cvtColor(src, frame_detect, COLOR_YUV2BGR_YV12);
	}
}

void CALLBACK fRealDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{
	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD: //系统头

		if (!PlayM4_GetPort(&(Camera.nPort)))  //获取播放库未使用的通道号
		{
			break;
		}

		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(Camera.nPort, STREAME_REALTIME))  //设置实时流播放模式
			{
				break;
			}

			if (!PlayM4_OpenStream(Camera.nPort, pBuffer, dwBufSize, 10 * 1024 * 1024)) //打开流接口
			{
				break;
			}

			if (!PlayM4_Play(Camera.nPort, NULL)) //播放开始
			{
				break;
			}
			if (!PlayM4_SetDecCallBack(Camera.nPort, DecCBFun))
			{
				break;
			}
		}
		break;
	case NET_DVR_STREAMDATA:   //码流数据
		if (dwBufSize > 0 && Camera.nPort != -1)
		{
			if (!PlayM4_InputData(Camera.nPort, pBuffer, dwBufSize))
			{
				break;
			}
		}
		break;
	default: //其他数据
		if (dwBufSize > 0 && Camera.nPort != -1)
		{
			if (!PlayM4_InputData(Camera.nPort, pBuffer, dwBufSize))
			{
				break;
			}
		}
		break;
	}
}

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	char tempbuf[256] = { 0 };
	switch (dwType)
	{
	case EXCEPTION_RECONNECT:
		break;
	default:
		break;
	}
}